/*
 Author: Qiaoling Chen
 Class: ECE6122
 Description: This file is a multithreaded console program using OpenMP that takes as a command line argument the name of a data file.
 The data file will contain a M x N grid of numbers. The file reads in the data and returns the largest product of four adjacent numbers
 in the same direction (up, down, left, right, or diagonally).
 */

#include <iostream>
#include <fstream>
#include <omp.h>
#include <stdlib.h>
#define CHUNKSIZE 10
using namespace std;

int main(int argc, const char * argv[]) {
    string path;
    // Create variable to store row and column number
    int row, column;
    // Create a two-dimensional array to store data information
    long int **data;
    path = argv[1];
    ifstream infile;
    infile.open(path.c_str());
    // Read in and store matrix information in data, row, column;
    infile >> row;
    infile >> column;
    data = new long int* [row];
    for(int ii = 0; ii< row; ii++) {
        data[ii] = new long int[column];
        for(int jj=0; jj<column; jj++) {
            infile >> data[ii][jj];
        }
    }
    // Finish reading file
    infile.close();
    //int maxProduct = FindMaxProduct(data,row,column);

    long int max_1=0, max_2=0, max_3=0;
    int chunk = CHUNKSIZE;
    // Iterate the rows.
    // Check the maximum product in horizontal direction.
    // Begin multi-threading
#pragma omp parallel
    {
        // Use different sections to process the question
#pragma omp sections
        {
// Check the maximum product in horizontal direction
#pragma omp section
            {
        for ( int i = 0; i < row; i++) {
            // Iterate the columns.
            for (int j = 3; j < column; j++) {
                    long int result_1;

                    if ((j - 3) >= 0) {
                        result_1 = data[i][j] * data[i][j - 1] * data[i][j - 2] * data[i][j - 3];
                        if (max_1 < result_1)
                            max_1 = result_1;
                    }
                }
            }
        }
// Check the maximum product in vertical direction.
#pragma omp section
                    {
                for(int i=3;i<row;i++) {
                    for (int j = 0; j < column; j++) {
                        long int result_2;
                        if ((i - 3) >= 0) {
                            result_2 = data[i][j] * data[i - 1][j] * data[i - 2][j] * data[i - 3][j];
                            if (max_2 < result_2)
                                max_2 = result_2;
                        }
                    }
                }
                    }
// Check the maximum product in both diagonal and anti-diagonal direction
#pragma omp section
                    {
                for(int i=3;i<row;i++) {
                    for (int j = 3; j < column; j++) {
                        long int result_3;
                        if ((i - 3) >= 0 && (j - 3) >= 0) {
                            result_3 = data[i][j] * data[i - 1][j - 1] * data[i - 2][j - 2] * data[i - 3][j - 3];
                            if (max_3 < result_3)
                                max_3 = result_3;
                            result_3 = data[i][j - 3] * data[i - 1][j - 2] * data[i - 2][j - 1] * data[i - 3][j];
                            if (max_3 < result_3)
                                max_3 = result_3;
                        }
                    }
                }
                    }
                }
            }
    // Get the largest output from threads
    long int max_result;
    if(max_1>max_2) {
        max_result = max_1;
    }
    else max_result = max_2;
    if(max_result<max_3) max_result = max_3;
    ofstream outfile;
    // Write the result to output.txt
    outfile.open("output2.txt");
    outfile << max_result << endl;
    // finish writing, close file
    outfile.close();
    return 0;
}
