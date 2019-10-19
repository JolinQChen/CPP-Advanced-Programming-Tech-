/*
 Author: Qiaoling Chen
 Class: ECE6122
 Description: This file Uses multi-threads, takes as a command line arguments the height and width of a lattice and returns the number of possible paths through the lattice.
 */

#include <iostream>
#include <omp.h>
#include <stdlib.h>
#define CHUNKSIZE 10
using namespace std;


int main(int argc, char *argv[]) {
    
    // Take the parameters from command line
    int h = atoi(argv[2]);
    int w = atoi(argv[4]);
    // return 0 if either h, w is 0.
    if (h == 0 || w == 0) {
        cout <<"Number of Routes: 0"<<endl;
        return 0;
    }
    // return max(h,w)+1 is either h or w is 1;
    else if(h == 1|| w == 1) {
        cout<<"Number of Routes: "<< max(h,w)+1 << endl;
        return 0;
    }
    
    else {
        // Set a two-dimensional array
        int** f = new int* [h+1];
        // initialize the array
        for(int i=0; i<=h; i++){
            f[i] = new int[w+1];
        }
        
        int chunk,i,j;
        // set the chunk to divide the for loop and asign to threads
        chunk = CHUNKSIZE;
        // initialize the array to store path information
        for(i=0;i<=w;i++)
            f[0][i] = 1;
        for (j = 0; j <= h; ++j)
            f[j][0] = 1;
        // begin multi-thread process
        // Dynamic programming
#pragma omp parallel shared(f, h, w) private(i,j) num_threads(8)

        {
            for(i = 1; i<=h; i++){
                {
#pragma omp for schedule(dynamic, chunk)
                    // from the second layer, accumulatively add the path number and store in f[][]
                    for (j = 1; j <= w; j++) {
                        f[i][j] = f[i][j - 1] + f[i - 1][j];
                    }
                }
            }
        }
        // eventually, the result is stored in the last number of f[][]
        cout<< "Number of Routes: "<< f[h][w]<<endl;
    }
    return 0;

}
