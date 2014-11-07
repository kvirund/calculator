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
