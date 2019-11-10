https://www.rapidtables.com/code/linux/gcc/gcc-d.html

# Debugging flags
These flags must be provided during compile time, so that those portion of
code does not interfere when actually running the code for generating data.

DEBUG_FLAG   
        then certain debugging information  will be printed in the console
        when running the program
    
UNIT_TEST       
        For checking if some variables is taking any garbage/wrong values
         



# How to compile
g++ -DDEBUG_FLAG source.cpp

or

g++ -D DEBUG_FLAG source.cpp