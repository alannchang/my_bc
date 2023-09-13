# Welcome to My Bc
***

## Task
The assignment was to recreate a simple implementation of the "bc" (basic calculator) command that is available on most Unix-based systems including Linux.  
You can use the bc command to perform arithmetic calculations, including addition, subtraction, multiplication, division, and more. 
It supports various mathematical functions and provides a simple interactive environment for performing calculations.

## Description
my_bc was written in C and will accept one argument, in addition to './my_bc'.  If more than one argument is given or an invalid infix expression is given, the user will get an error message.
Please note, the infix expression must be enclosed in quotes as a string.  A number of checks are run on the string/expression to confirm that the expression is valid.
Then the string is separated into an array of strings.  An output queue and operator stack are initialized and the shunting yard algorithm is performed on the array of strings.
The resulting queue is then evaluated to give us the final result of the mathematical expression, which is printed to the standard output.

## Installation
To install my_bc, simply type in the 'make' command, hit enter, and my_bc is read for use.

## Usage
To use my_bc, type "./my_bc" into the terminal followed by a space and the infix expression (enclosed in quotes) that you would like to have evaluated.
After pressing enter, the result of the expression will be returned on the next line.
```
./my_bc "1 + 2"
3
```

### The Core Team
Alan Chang

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
