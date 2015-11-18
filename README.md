Calculator
==========

Calculator which has been written on C++ using re2c and lemon. Currently implemented simple operation on four different types:

  1. integer
  2. float
  3. boolean
  4. string
    
Also calculator makes possible to use variables. One predefined constant is 'pi'

At this time implemented following operations:

  1. add (+)
  2. sub (-)
  3. division (/)
  4. mod (%)
  5. times (*)
  6. All relationship operations (<, <=, >, >=, ==, !=)
  7. Negative (-)
  8. logical AND, OR and NOT
  9. Assignment ([variable name]=[variable value])

Requirements
============

For building project usually you need only cmake. But if you want to regenerate scanner and syntax analyzer, you should have re2c and lemon utilities accordingly.
    
Building
========

Just type following commands from root of project directory:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

To perform tests, from build directory run test/test.it.pl:

  $ ../test/test.it.pl

You must see something like:

    $ ../test/test.it.pl
    (Expressions parser) Escape string                                     [PASSED]
    (Expressions parser) Constants test                                    [PASSED]
    (Expressions parser) Sum test                                          [PASSED]
    (WHERE parser) Escape string                                           [PASSED]
    (WHERE parser) Constants test                                          [PASSED]
    (WHERE parser) Compare test                                            [PASSED]
    (WHERE parser) Unary minus and NOT                                     [PASSED]
    (WHERE parser) LIKE operator                                           [PASSED]
    (WHERE parser) Booleans test                                           [PASSED]
    (WHERE parser) Sum test                                                [PASSED]
    (WHERE parser) Mul test                                                [PASSED]
    (WHERE parser) Div test                                                [PASSED]
    (WHERE parser) Null test                                               [PASSED]
    (WHERE parser) IS and IS NOT test                                      [PASSED]
    (Expressions parser) Compare test                                      [PASSED]
    (Expressions parser) Unary minus and NOT                               [PASSED]
    (Expressions parser) Booleans test                                     [PASSED]
    (Expressions parser) Mul test                                          [PASSED]
    (Expressions parser) Variable test                                     [PASSED]
    (Expressions parser) Div test                                          [PASSED]
    (Expressions parser) Fake assignment test                              [PASSED]
    (WHERE parser) Variable test                                           [PASSED]
    $ 

>  vim: set ts=2 sw=2 tw=0 et syntax=markdown :
