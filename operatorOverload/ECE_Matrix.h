/*
 Author: Qiaoling Chen
 Class: ECE6122
 
 Description:
   This is the header file for ECE_Matrix class. In this file we declare the member data of this class and its functions and operators.
*/


#pragma once
#include <iostream>
#include <fstream>

using namespace std;

/*
    Declare the class ECE_Matrix, including private and public members.
 */
class ECE_Matrix {
    int m_rw;  // Row of a ECE_Matrix
    int m_cl;  // Column of a ECE_Matrix
    double **data; //Store the data information in the ECE_Matrix
public:
    /*
        ECE_Matrix constructors declaration
     */
    // Default Constructor
    ECE_Matrix();
    // Constructor_2, create a int in1*in1 martix with all elements set to in2
    ECE_Matrix(int const &, double const &);
    // Constructor_3, create a in1*in2 matrix with all elements set to double in3
    ECE_Matrix(int const &, int const &, double const &);
    // Constructor_4, read file as a matrix
    ECE_Matrix(string const &);
    // Constructor_5, copy matrix to a new one
    ECE_Matrix(ECE_Matrix const &);
   
    
    /*
        ECE_Matrix operators overload
     */
    // Operator +: newMatrix = ECE_Matrix_1 + ECE_Matrix_2
    ECE_Matrix operator+(ECE_Matrix const &) const;
    // Operator -: newMatrix = ECE_Matrix_1 - ECE_Matrix_2
    ECE_Matrix operator-(ECE_Matrix const &) const;
    // Operator /: newMatrix = ECE_Matric/double
    ECE_Matrix operator/(double const &) const;
    // Operator -=: ECE_Matrix_1 -= ECE_matrix_2
    ECE_Matrix& operator-=(ECE_Matrix const &);
    // Operator +=: ECE_Matrix_1 += ECE_Matrix_2
    ECE_Matrix& operator+=(ECE_Matrix const &);
    // Operator -: ECE_Matrix = -ECE_Matrix
    ECE_Matrix operator-() const;
    // Operator +: newMatrix = double + ECE_Matrix
    friend ECE_Matrix operator+ (double const&, ECE_Matrix const &);
    // Operator +: newMatrix = ECE_Matrix + double
    ECE_Matrix operator+ (double const&) const;
    // Operator -: newMatrix = double - ECE_Matrix
    friend ECE_Matrix operator- (double const&, ECE_Matrix const &);
    // Operator -: newMatrix = ECE_Matrix - double
    ECE_Matrix operator- (double const&) const;
    // Operator *: newMatrix = double * ECE_Matrix
    friend ECE_Matrix operator* (double const&, ECE_Matrix const &);
    // Operator *: newMatrix = ECE_Matrix * double
    ECE_Matrix operator* (double const&) const;
    // Stream inserction operator
    friend ostream& operator<<(ostream&, const ECE_Matrix&);

    /*
        Function declaration
     */
    // Get the transpose for a ECE_Matrix
    ECE_Matrix transpose() const;
};
