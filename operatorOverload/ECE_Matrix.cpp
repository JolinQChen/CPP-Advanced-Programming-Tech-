/*
    Author: Qiaoling Chen
    class: ECE6122
    Description: This is the cpp file to implement ECE_Matrix class. In this file, we defined the constructors, operators and functions declared in ECE_Matrix.h file.
 */

#include "ECE_Matrix.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>

/*
    Define constructors for ECE_Matrix
 */
ECE_Matrix::ECE_Matrix():m_rw(0), m_cl(0)
{
    // default constructor for ECE_matrix
}

ECE_Matrix::ECE_Matrix(int const &in1, double const &in2):m_rw(in1),m_cl(in1)
{
    // Constructor_2, set an in1*in1 matrix with all elements set to in2
    // Set m_rw = in1, m_cl = in1
    // Set a two-dimensional array as data and set all elements to in2
    data = new double *[in1];
    for(int ii = 0; ii<in1 ; ii++) {
    	data[ii] = new double[in1];
        for (int jj = 0; jj<in1;jj++) {
            data[ii][jj] = in2;
        }
    }
}

ECE_Matrix::ECE_Matrix(int const &in1, int const &in2, double const &in3):m_rw(in1),m_cl(in2)
{
    // Constructor_3, set an in1*in2 matrix with all elements set to in3
    // Set m_rw = in1, m_cl = in2
    // Set a two-dimensional array as data and set all element to in3
    data = new double* [in1];
    for(int ii = 0; ii<in1;ii++) {
    	data[ii] = new double[in2];
    	for(int jj = 0; jj<in2;jj++) {
    		data[ii][jj] = in3;
    	}
    }
}

ECE_Matrix::ECE_Matrix(string const &path)
{
    // Constructor_4, create a matrix by reading from .txt file
    // The first row of .txt file should be the row number and column number of matrix
    
    // Open the file from given path
	ifstream infile;
	infile.open(path.c_str(),ios::in);
    // Suppose the path is invalid or file doesn't exist
    if (!infile.is_open())
    {
        cout << "can not open file: " << path << endl;
        return;
    }
    // Get the row number and column number and store in private members
	infile >> m_rw;
	infile >> m_cl;
    // Set a two-dimensional array and read in all matrix elements
	data = new double* [m_rw];
	for(int ii = 0; ii< m_rw; ii++) {
		data[ii] = new double[m_cl];
		for(int jj=0; jj<m_cl; jj++) {
			infile >> data[ii][jj];
		}
	}
    //finish reading file
	infile.close();
};

ECE_Matrix::ECE_Matrix(ECE_Matrix const &in):m_rw(in.m_rw),m_cl(in.m_cl)
{
    // Constructor_5, copy the existed ECE_Matrix and create a new one
    data = new double *[m_rw];
    for (int i=0; i<m_rw; i++) {
        data[i] = new double[m_cl];
        for(int j=0; j<m_cl;j++) {
            data[i][j] = in.data[i][j];
        }
    }
}

// operators

/*
    Overload operators for ECE_Matrix
 */
ECE_Matrix ECE_Matrix::operator-(ECE_Matrix const &M1) const
{
    // Operator -: new_matrix = ECE_Matrix_1 - M1
    
    // Set a new matrix that has the maximun rows and columns to ensure that it can cover all the elements from both matrixes.
	int row = max(m_rw,M1.m_rw);
	int column = max(m_cl, M1.m_cl);
    // Construct a new matrix and pre-set all element to 0
	ECE_Matrix newMatrix(row, column,0);
    // Set the corresponding elements same as ECE_Matrix_1
	for (int ii = 0; ii< m_rw;ii++){
		for(int jj=0;jj<m_cl;jj++){
			newMatrix.data[ii][jj]=data[ii][jj];
		}
	}
    // Get the substraction of the new_matrix and ECE_Matrix_2 and return new_matrix as result
	for (int ii = 0; ii < M1.m_rw; ii++)
	{
		for (int jj = 0; jj < M1.m_cl; jj++)
		{
			newMatrix.data[ii][jj] = newMatrix.data[ii][jj] - M1.data[ii][jj];
		}
	}
	return newMatrix;
}


ECE_Matrix ECE_Matrix::operator+(ECE_Matrix const &M1) const
{
    // Operator +: new_matrix = ECE_Matrix_1 + M1
    // Set the row and column number of new_matrix to maximun of the two addends.
	int row = max(m_rw,M1.m_rw);
	int column = max(m_cl, M1.m_cl);
    // Create the new_matrix and set all elements to 0
	ECE_Matrix newMatrix(row, column, 0.0);
    // Get the corresponding elements from ECE_Matrix_1
	for (int ii = 0; ii< m_rw;ii++){
		for(int jj=0;jj<m_cl;jj++){
			newMatrix.data[ii][jj]=data[ii][jj];
		}
	}
    // Get the sum of corresponding elements of ECE_Matrix_1 and ECE_Matrix_2 and return new_matrix as result
	for (int ii = 0; ii < M1.m_rw; ii++)
	{
		for (int jj = 0; jj < M1.m_cl; jj++)
		{
			newMatrix.data[ii][jj] = newMatrix.data[ii][jj] + M1.data[ii][jj];
		}
	}
	return newMatrix;
}

 ECE_Matrix ECE_Matrix::operator/(double const &num) const
 {
     // Operator /: new_matrix = ECE_Matrix_1 / num
     // If num == 0, then the operation is invalid, return a matrix with all elements set to 0
     if(num == 0) {
         return ECE_Matrix(m_rw,m_cl,0.0);
     }
     // Create a new ECE_Matrix with row and column number set to the same as ECE_Matrix_1
     ECE_Matrix newMatrix(m_rw,m_cl);
     // Get the division and return newMatrix as result
 	for(int ii = 0; ii<m_rw;ii++) {
 		for(int jj=0;jj<m_cl;jj++) {
 			newMatrix.data[ii][jj] /= num;
 		}
 	}
 	return newMatrix;
 }


ECE_Matrix& ECE_Matrix::operator-=(ECE_Matrix const &M1) 
{
    // Operator -=: ECE_Matrix_1 = ECE_Matrix_1 - M1
    // Create a new matrix with row and column number set to the maximun of the two matrixes and all elements set to 0
	int row = max(m_rw, M1.m_rw);
	int column = max(m_cl,M1.m_cl);
	ECE_Matrix newMatrix(row,column,0.0);
    // Set the corresponding elements the same as ECE_Matrix_1
    for (int i = 0; i < m_rw; i++)
	{
		for (int j = 0; i < m_cl; i++)
		{
			newMatrix.data[i][j] = data[i][j];
		}
	}
    // Get the substraction of corresponding elements
	for (int i = 0; i < M1.m_rw; i++)
	{
		for (int j = 0; j < M1.m_cl; j++)
		{
			newMatrix.data[i][j] = newMatrix.data[i][j] - M1.data[i][j];
		}
	}
    // Assign newMatrix to ECE_Matrix_1 and return
	*this = newMatrix;
	return *this;
}


ECE_Matrix& ECE_Matrix::operator+=(ECE_Matrix const &M1) 
{
    // Operator +=: ECE_Matrix_1 = ECE_Matrix_1 + M1
    // Create a new matrix with row and column number set to the maximun of the two matrixes and all elements set to 0
	int row = max(m_rw, M1.m_rw);
	int column = max(m_cl,M1.m_cl);
	ECE_Matrix newMatrix(row,column,0.0);
    // Set the corresponding elements the same as ECE_Matrix_1
	for (int i = 0; i < m_rw; i++)
	{
		for (int j = 0; j < m_cl; i++)
		{
			newMatrix.data[i][j] = data[i][j];
		}
	}
    // Get the sum of corresponding elements
	for (int i = 0; i < M1.m_rw; i++)
	{
		for (int j = 0; j < M1.m_cl; j++)
		{
			newMatrix.data[i][j] = newMatrix.data[i][j] + M1.data[i][j];
		}
	}
    // Assign newMatrix to ECE_Matrix_1 and return
	*this = newMatrix;
	return *this;
}

ECE_Matrix ECE_Matrix::operator-() const
{
    // Get the nagative matrix: return a new matrix in which each element is the negative of that in original matrix
    // Copy the original and create a new matrix
    ECE_Matrix temp(*this);
    for (int i = 0; i < m_rw; ++i)
    {
        for (int j =0; j < m_cl; ++j)
        {
            // Take the nagative value
            temp.data[i][j] = -data[i][j];
        }
    }
    return temp;
}


ECE_Matrix operator+ (double const &num, ECE_Matrix const &matrix)
{
    // Friend operator +: new_matrix = num + matrix
    // Copy matrix and create a new matrix
	ECE_Matrix newMatrix(matrix);
    // Increase each element in the new matrix by num
	for(int i=0; i<matrix.m_rw;i++){
		for (int j = 0; j < matrix.m_cl; j++)
		{
			newMatrix.data[i][j]+=num;
		}
	}
	return newMatrix;
}


ECE_Matrix ECE_Matrix::operator+ (double const &num) const
{
    // Operator +: new_matrix = matrix + num
    // Same as num + matrix
	ECE_Matrix newMatrix(*this);
	for(int i=0; i<m_rw;i++){
		for (int j = 0; j < m_cl; j++)
		{
			newMatrix.data[i][j]+=num;
		}
	}
	return newMatrix;
}


ECE_Matrix operator- (double const &num, ECE_Matrix const &matrix)
{
    // Friend operator -: new_matrix = num - matrix
    // Copy matrix and create a new matrix
	ECE_Matrix newMatrix(matrix);
    // Substract num by each element in the new matrix
	for(int i=0; i<matrix.m_rw;i++){
		for (int j = 0; j < matrix.m_cl; j++)
		{
			newMatrix.data[i][j]=num-newMatrix.data[i][j];
		}
	}
	return newMatrix;
}


ECE_Matrix ECE_Matrix::operator- (double const &num) const
{
    // Operator -: new_matrix = matrix - num
    // Same as num + matrix, except that substract each element in the new matrix by num
	ECE_Matrix newMatrix(*this);
	for(int i=0; i<m_rw;i++){
		for (int j = 0; j < m_cl; j++)
		{
			newMatrix.data[i][j] -= num;
		}
	}
	return newMatrix;
}


ECE_Matrix operator* (double const &num, ECE_Matrix const &matrix)
{
    // Friend operator *: new_matrix = num * matrix
    // Same as matrix * num
	ECE_Matrix newMatrix(matrix);
	return newMatrix * num;
}


ECE_Matrix ECE_Matrix::operator* (double const &num) const
{
    // Operator *: new_matrix = matrix * num
    // Copy matrix and create a new matrix
	ECE_Matrix newMatrix(*this);
    // Get the product of num and each element in matrix and return
	for(int i=0; i<m_rw;i++){
		for (int j = 0; j < m_cl; j++)
		{
			newMatrix.data[i][j] *= num;
		}
	}
	return newMatrix;
}

/*
    Define the function(s) of ECE_Matrix class
 */
ECE_Matrix ECE_Matrix::transpose() const
{
    // Get the transpose result of an ECE_Matrix
    // Create a new matrix the transposed size as the original matrix with every element set to 0
    ECE_Matrix newMatrix(m_cl,m_rw,0.0);
    // Get the elements from transposed place in original matrix and return
    for (int ii=0;ii<m_cl;ii++) {
        for(int jj=0;jj<m_rw;jj++) {
            newMatrix.data[ii][jj] = data[jj][ii];
        }
    }
    return newMatrix;
}


ostream& operator<<(ostream& os, const ECE_Matrix& matrix)
{
    // Stream insertion operator <<: output elements in matrix
    for (int i = 0; i < matrix.m_rw; i++)
    {
    	for (int j = 0; j < matrix.m_cl; ++j)
    	{
    		cout << scientific << setw(12) << setprecision(3) << matrix.data[i][j] << " ";
    	}
    	cout << endl;
    }
    return os;
}
























