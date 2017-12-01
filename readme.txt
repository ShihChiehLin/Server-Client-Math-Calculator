/*
* Shih-Chieh Lin
* CSCE 5580 pogramming assignment 1
* 2/12/2017
* A server (cse01.cse.unt.edu) in the internet using TCP domain, and the port number is get from argument number 1
* Can calculate the numerical expression from client and write the result back to display on client.
* A client (cse02.cse.unt.edu) in the internet using TCP domain, and need to enter port number as argument number 1
* Can connect to math server (cse01..cse.unt.edu)and enter the numerical expression the user want to calculate and display the result of it from the math server.
* Not finish:
* check space between parenthesis
* check space in right operand
* sqrt and log function calculate
* negative number check
* It works when the space is in the parenthesis, but not between operator.
* Reference:http://www.linuxhowtos.org/C_C++/socket.htm
* http://stackoverflow.com/questions/31104531/calculator-in-c-using-stack
*/

How to compile:
server.c:
gcc server.c -lm
./a.out 1124  

client.c
gcc client.c
./a.out 1124 

(User can change port number as they need)
Client side input/ouput testing:

exprsn: (-5*6)+(5+5)
result: -20.0000
exprsn: (10^2)+(5/5)
result: 101.0000
exprsn: (e^2)+(10-5)
result: 12.3891
exprsn: (5*6)+(-5)
result: 25.0000
exprsn: quit
result: bye bye!


