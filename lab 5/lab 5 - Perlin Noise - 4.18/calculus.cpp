#include "calculus.h"
#include <iostream>
#include <vector>
#include <cmath>
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
void matrix_multi_4x4(vector< vector<float> > &matrix_out,
        vector< vector<float> >matrix_L,vector< vector<float> >matrix_R)
{// compute 2 4 x 4 matrices muliplication
    for(int row = 0 ; row < 4 ; row++ )
        for(int m3_column = 0 ; m3_column < 4 ; m3_column++ )
        {
            matrix_out[row][m3_column] =
                matrix_L[row][0] * matrix_R[0][m3_column] +
                matrix_L[row][1] * matrix_R[1][m3_column] +
                matrix_L[row][2] * matrix_R[2][m3_column] +
                matrix_L[row][3] * matrix_R[3][m3_column];
        }
}
void vector_subtraction(vector<float> &sub,
           vector<float> vector_1, vector<float> vector_2)
{   //compute vector subtraction
    //vector_2 - vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] - vector_1[i];
}
void vector_summation(vector<float> &sub,
           vector<float> vector_1, vector<float> vector_2)
{   //compute vector summation
    //vector_2 + vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] + vector_1[i];
}
float dot_product3D(vector<float> vector_1,vector<float> vector_2)
{//compute dot product
    return vector_1[0]*vector_2[0] + vector_1[1]*vector_2[1] +
           vector_1[2]*vector_2[2];

}
void cross_product3D(vector<float> &normal,
            vector<float>vector_1, vector<float>vector_2)
{//compute cross product
    normal[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    normal[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    normal[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
}
float vector_length(vector<float> vector_1)
{//compute vector length
    return pow(pow(vector_1[0],2) + pow(vector_1[1],2) +
               pow(vector_1[2],2),0.5);
}
void Normalization(vector<float> &normal)
{//normalize the pixel normal
    float temp_length = vector_length(normal);
    for (int i=0; i<3 ;i++ )
        normal[i] = normal[i] / temp_length;
}
