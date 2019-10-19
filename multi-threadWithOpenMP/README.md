This file contains examples of implementing multi-thread method in c++ using openMP.

`By setting the threads to 1, you can compare the efficiency of multi-thread with singular thread.`

## Prob 1.
(www.projecteuler.net) Write a console program that takes as a command line arguments the height and width of a lattice.
Example for a lattice 20 high and 10 wide.
Problem1.exe –h 20 –w 10
Using the logic described below use multithreading to determine the number of possible paths through the lattice as quickly as possible. You should output to the console the number of paths exactly like the following (remember to add a line feed at the end of the ouput)
>\> Problem1.exe –h 2 –w 2

>\> Number of Routes: 6
>\>

Path rules:
Starting in the top left corner of the lattice, and only being able to move to the right and down, determine the number of routes to the bottom right corner of the lattice.

## Prob 2. 
(www.projecteuler.net) Write a multithreaded console program using OpenMP that takes as a command line argument the name of 
a data file. The data file will contain a M x N grid of numbers. Your program must read in the data file and determine the 
largest product of four adjacent numbers in the same direction (up, down, left, right, or diagonally).

File format:
All values in the text data file are space delimited. You can assume that the input data file does not contain errors, 
such as missing numbers or nonnumeric numbers and all numbers are greater than or equal to 0. You are free to create as 
many files as you like and use whatever filenames in order to create a solution. All the files *.cpp and *.h files you 
create should reside in a single folder that can be compiled to create the executable.

The first line in the file will have the number of M rows and N columns. The next M lines in the data file will contain 
the rows of N numbers.

Use the file data_Problem2.txt included in the assignment as a test input file.

## Prob 3. (multi-thread with dynamic planning)
(www.projecteuler.net)

(https://www.mathblog.dk/project-euler-18/)

Similar to Problem #2, write a multi-threaded console program using OpenMP that takes as a
command line argument the name of a data file. Your program must read in a triangle of
arbitrary size from a data file. The first line will contain the number of levels in the triangle. A
sample data file is attached (data_triangle.txt) to the homework assignment.

assume that the input data file does not contain errors, such as missing numbers or nonnumeric numbers and all numbers are greater than or equal to 0.
