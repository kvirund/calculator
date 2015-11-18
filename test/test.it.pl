#!/usr/bin/perl -w

use strict;

use if "MSWin32" eq $^O, "Win32::Console::ANSI";
use if "MSWin32" eq $^O, "Win32::Process";
use Term::ANSIColor;
use POSIX ":sys_wait_h";
use IPC::Open2;
use Time::HiRes qw(usleep);
use File::Basename;

my $DIR = dirname($0);
my $APPLICATION = "./test.it -w";
my $INPUT_FILENAME = "$DIR/input";

my %tests = ();

sub read_tests($$);

sub read_tests($$)
{
    my $filename = shift;
    my $result = shift;
    open TESTS, "<$filename";
    my @input = <TESTS>;
    close TESTS;
    my $state = "outside";
    my @states = [];
    my $line = 0;
    my $name = "";
    my $input_lines = 0;
    my $output_lines = 0;
    my $input = "";
    my $output = "";
    my $tid = "";
    my @dependencies = [];
    for (@input)
    {
        $_ =~ s/[\x0d\x0a]//g;
        ++$line;
        if ("outside" eq $state)
        {
            next if /^#/;
            next if /^\s*$/;
            if (/^\s*include\s*"(.*)"$/)
            {
                my $subtest = $1;
                $subtest = $DIR.'/'.$subtest unless $subtest =~ /^[.\/]/;
                read_tests($subtest, $result);
                next;
            }
            if (/^(.+):\s*<(.+)>\s*(\d+)\s+(\d+)\s*<(.*)>$/)
            {
                $name = $1;
                $tid = $2;
                $input_lines = $3;
                $output_lines = $4;
                @dependencies = split/,\s*/,$5;
                $input = $output = "";
                push @states, "test_processing";
                push @states, "output" if 0 < $output_lines;
                push @states, "input" if 0 < $input_lines;
                $state = pop @states;
            }
            else
            {
                print STDERR "Wrong input in file '$filename' at line #$line. Skip it.\n";
            }
        }
        elsif ("input" eq $state)
        {
            $input .= $_.$/;
            $state = pop @states if 0==--$input_lines;
        }
        elsif ("output" eq $state)
        {
            $output .= $_.$/;
            $state = pop @states if 0==--$output_lines;
        }
        else
        {
            die "Unexpected state '$state' at line #$line";
        }

        if ("test_processing" eq $state)
        {
            $result->{$tid} = {
                name=>$name,
                input=>$input,
                output=>$output,
                deps=>[@dependencies]
            };
            $state = "outside";
        }
    }
    die"Unexpected end of file"unless"outside"eq$state;
}

sub tsort($)
{
    my $inp = shift;
    my %color;
    $color{$_}=0 for (keys %$inp); # all vertexes are white
    my @result = ();
    my $dfs;
    $dfs = sub
    {
        my $test = shift;
        for (@{$inp->{$test}})
        {
            die "Dependent test '$_' for test '$test' could not be found\n" unless exists $color{$_};
            die "cycle was found" if 1==$color{$_};
            $color{$_} = 1, $dfs->($_), $color{$_} = 2, push @result, $_ unless $color{$_};
        }
    };
    for (keys %$inp)
    {
        die "cycle was found" if 1==$color{$_};
        $color{$_} = 1, $dfs->($_), $color{$_} = 2, push @result, $_ unless $color{$_};
    }
    @result;
}

sub runTest($$;$)
{
    my %terminated = ();
    my $input = shift;
    my $output = shift;
    my $error = shift;
    my $pid = open2(*READER, *WRITER, $APPLICATION);
    print WRITER $input;
    print WRITER "\x1a\n" if $^O eq "MSWin32";  # Windows hack: send ^Z. Seems like Windows waits for input even if file was closed.
    close WRITER;                               # ... for normal operating systems just close the input channel
    my $start_time = time;
    do
    {
        my $diff = time - $start_time;
        if (3 < $diff)
        {
            unless ("MSWin32"eq$^O)
            {
                my $SIGNAL = 'KILL';
                print STDERR "Sending signal '$SIGNAL' to hanged process with pid [$pid]...\n";
                kill $SIGNAL, $pid;
            }
            else
            {
                print STDERR "Seems like process hanged and we cannot do anything under this platform. :(\n";
                $terminated{$pid} = -1;
                last;
            }
        }
        my $code = waitpid $pid, WNOHANG;
        $terminated{$pid} = $?>>8 if 0 < $code;
        usleep 10  unless exists $terminated{$pid};
    } while not exists $terminated{$pid};
    my $produced = "";
    while (<READER>)
    {
        $_ =~ s/[\x0d\x0a]//g;
        $produced .= $_.$/;
    }
    close READER;
    unless (0==$terminated{$pid} and $produced eq $output)
    {
        if ($error)
        {
            $$error = "";
            my @po = split/\n/,$produced;
            my @eo = split/\n/,$output;
            $$error .= "Different number of lines in expected and produced output.\n" unless $#po==$#eo;
            my $c = $#po<$#eo?$#po:$#eo;
            for (0..$c)
            {
                $$error.="First difference at line #".(1+$_).":\n".$eo[$_]."\n".$po[$_]."\n",last unless $po[$_]eq$eo[$_];
            }
        }
        return 1;
    }
    return 0;
}

read_tests($INPUT_FILENAME, \%tests);
my %graph = ();
$graph{$_} = $tests{$_}{deps} for (keys %tests);
for my $tid (tsort(\%graph))
{
    my $error = "<logic error in error handling>\n";
    my $result = runTest($tests{$tid}{input}, $tests{$tid}{output}, \$error);
    printf "%s%-71s%s[%s", color("green"), $tests{$tid}{name}, color("bold blue"), color("reset");
    if (0 == $result)
    {
        print color("yellow");
        print "PASSED";
        print color("reset");
    }
    else
    {
        print color("red");
        print "FAILED";
        print color("reset");
    }
    print color("bold blue")."]\n".color("reset");
    print $error unless 0 == $result;
    exit 1 unless 0 == $result;
}
exit 0;

# vim: set ts=4 sw=4 tw=0 et syntax=perl :
