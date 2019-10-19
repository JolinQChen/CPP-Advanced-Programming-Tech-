/*
 Author: Qiaoling Chen
 Class: ECE6122
 Description: This file uses multi-threads, takes as a command line argument the name of a data file, reads in a triangle of arbitrary size from a data file and returns the the maximum sum by path through the input triangle starting at the top of the triangle.
 */

#include <omp.h>
#define CHUNKSIZE 10
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, const char * argv[])
{
    // begin reading and storing matrix information in data, row, column;
    ofstream outfile;
    string path;
    int depth;
    int **data;
    // get path from the command line
    path = argv[1];
    // read in the data stored in file
    ifstream infile;
    infile.open(path.c_str());
    infile >> depth;
    // create a two-dimensional matrix to store the triangular data
    data = new int* [depth];
    for(int ii = 0; ii< depth; ii++) {
        data[ii] = new int[depth];
        for(int jj=0; jj<=ii; jj++) {
            infile >> data[ii][jj];
        }
    }
    infile.close();
    //finish reading file
int chunk = CHUNKSIZE;
    // dynamic programming
int sum[depth];
#pragma omp parallel num_threads(2)
    {
        for (int i = depth - 1; i >= 0; i--) {
#pragma omp for schedule(dynamic, chunk)
            for (int j = 0; j <= i; j++) {
                if (i == depth - 1) {
                    // for the last line, sum equals the number it own
                    sum[j] = data[i][j];
                } else {
                    // for the rest lines, take the maximum sum of neighboring numbers and renew the forst j-1 numbers of sum[]
                    sum[j] = data[i][j] + max(sum[j], sum[j + 1]);
                }
            }
        }
    }
    // when get to the top of triangle, the first number stored in sum[] is the maximum sum of a particular path from top to bottom
    int result = sum[0];

    // Write the result to a new .txt file
    outfile.open("output3.txt");
    outfile << result;
    outfile.close();
    return 0;
}
