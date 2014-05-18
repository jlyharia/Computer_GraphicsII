#include "calculus.h"
#include <iostream>
#include <vector>

using namespace std;
void matrix_multi_4x1(vector <float> &matrix_out,
        vector< vector<float> >matrix_L,vector <float>matrix_R)
{// compute 4 x 4 and 4 x 1 matrices muliplication
    for(int row = 0 ; row < 4 ; row++ )
    {
        matrix_out[row] =
            matrix_L[row][0] * matrix_R[0] +
            matrix_L[row][1] * matrix_R[1] +
            matrix_L[row][2] * matrix_R[2] +
            matrix_L[row][3] * matrix_R[3] ;
    }
}
void vector_subtraction(vector<float> &sub,
           vector<float> vector_1, vector<float> vector_2)
{   //compute vector subtraction
    //vector_2 - vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] - vector_1[i];
}
