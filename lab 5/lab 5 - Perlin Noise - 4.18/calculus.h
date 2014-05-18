#include <iostream>
#include <vector>

using namespace std;
void matrix_multi_4x1(vector <float> &matrix_out,
      vector< vector<float> >matrix_L,vector <float>matrix_R);

void matrix_multi_4x4(vector< vector<float> > &matrix_out,
        vector< vector<float> >matrix_L,vector< vector<float> >matrix_R);
void vector_subtraction(vector<float> &sub,
           vector<float> vector_1, vector<float> vector_2);
void vector_summation(vector<float> &sub,
           vector<float> vector_1, vector<float> vector_2);

float dot_product3D(vector<float> vector_1,vector<float> vector_2);
void cross_product3D(vector<float> &normal,
                        vector<float>vector_1, vector<float>vector_2);

float vector_length(vector<float> vector_1);
void Normalization(vector<float> &normal);


