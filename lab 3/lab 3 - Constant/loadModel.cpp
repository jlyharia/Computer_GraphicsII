/** Tasks in LoadModel.cpp
    LoadModel.cpp contains all routine regarding vertices transformation ,
    and back face eliminate.
    Vertices transformations compute vertices from local coordinate to
    perspective coordinate and translate to normalized format.
    Back face culling process in view space and skip invisible vertices enter
    perspective transformation.


**/
#include "loadModel.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include <iomanip>
using namespace std;


loadModel::loadModel()
{   //set up default parameter
    setfileName("house.d"); //input file name
    setDistanceNear(10); //d
    setDistanceFar(200); //far
    setWidth(10); //h
    setUpVector(0,1,0);//up vector of camera
    setCameraPosition(0,0,0);
    //default camera position on world coordinate
    setProjectRef(0,0,-100);
    //default camera project reference on world coordinate

    //initialize vector
    vector< vector<float> > patern(4, vector<float>(4));
    M_model_4D = patern;
    M_pers_4D  = patern;
    M_view_4D  = patern;
}


void loadModel::LoadData()
{   //load geometric data from file to program

    //initialize default parameter
    ifstream inGeometricData(fileName, ios::in);
    //load Geometric data *.d, constructor name - inGeometricData
    if(!inGeometricData)
    {
        cerr << "File could not be opened" <<endl;
        exit(1); //#include <cstdlib>
    }

    int num_vertex, num_polygon;
    // store 1)number of vertex and 2)number of polygon
    string first_word;
    //store first word - data

    string headline_string;
    char headline[256];
    inGeometricData.getline(headline, 256, '\n');
    inGeometricData.seekg(0L, ios::beg);//put cursor back to beginning
    headline_string = headline; //convert to string type
    if(!headline_string.find("data"))
    {
        inGeometricData >>first_word >> num_vertex >> num_polygon;
        //first line include word "data"
    }
    else
    {
        inGeometricData >> num_vertex >> num_polygon;
    }


    vector< vector<float> > vertex(num_vertex, vector<float>(4));
    //declare 2 dimensional vector to store vertices data
    for(int i = 0; i< num_vertex; i++)
    {
        //put vertex data to vector - vec_vertex
        inGeometricData >> vertex[i][0]
                        >> vertex[i][1]
                        >> vertex[i][2];
        vertex[i][3] = 1;
    }
    vec_vertex = vertex; // assign to public vector vec_vertex

    //declare 2 dimensional vector to store polygon data
    //dynamic array to fit different number of vertex index

    for(int i = 0; i< num_polygon; i++)
    {
        //put element to vector vec_polygon
        int element,index;
        inGeometricData>>index; //get number of vertex index
        vector<int> row;
        row.push_back(index);//put in a new vector
        for (int j = 0; j < index  ; j++)
        {
            inGeometricData>>element;
            row.push_back(element); // Add an element to the row
        }
        vec_polygon.push_back(row);
        // Add the vector to the main vector (vec_polygon)
    }
    inGeometricData.close(); // close file
    //end of load vertices from file
}

void loadModel::ComposeMatrix()
{   //compute back face and normalized vertices data

    matrix_model(M_model_4D, offset_world);
    //compute Modeling transform Matrix to world space
    matrix_view(M_view_4D , camera, up_vector);
    //compute viewing transform Matrix to view space
    matrix_pers(M_pers_4D , d , far , h);
    //compute Perspective transform Matrix to perspective & nomorlized space
    ///-Compute vertex in world coordinate for illumation model
    vector< vector<float> > temp_world_vertex(vec_vertex);
    for(int i = 0 ; i < (int)vec_vertex.size() ; i++)
        matrix_multi_4x1(temp_world_vertex[i] , M_model_4D, vec_vertex[i]);
        //vertice in world space
    vec_world_vertex = temp_world_vertex; //assign to public world vertex

    ///---------------------------------------------------------
    vector< vector<float> > M_view_space(4, vector<float> (4));
    matrix_multi_4x4(M_view_space, M_view_4D, M_model_4D);
    //compute composite matrix to view space
    vector< vector<float> > vec_view_vertex(vec_vertex);
    //vec_view_vertex stores vertices in view space
    for(int i = 0 ; i < (int)vec_vertex.size() ; i++)
        matrix_multi_4x1(vec_view_vertex[i] , M_view_space, vec_vertex[i]);
        //vertice in view space

    //start front face calculus in view space
    vector <float> sightLine(3);//Sight of Line
    vector <float> poly_vector_1(3);
    vector <float> poly_vector_2(3);
    vector <float> poly_normal(3);
    vector<float> front_vertex;//store all 'front vertices' index

    for(int i=0; i<(int)vec_polygon.size(); i++)
    {//compute visible face
        vector_subtraction(poly_vector_1,
                           vec_view_vertex[vec_polygon[i][2]-1],
                           vec_view_vertex[vec_polygon[i][1]-1]);
        vector_subtraction(poly_vector_2,
                           vec_view_vertex[vec_polygon[i][2]-1],
                           vec_view_vertex[vec_polygon[i][3]-1]);
        //compute 2 vector on one polygon
        vector_subtraction(sightLine,
                           camera,
                           vec_view_vertex[vec_polygon[i][2]-1]);
        //compute sight of line
        cross_product3D(poly_normal, poly_vector_1, poly_vector_2);
        //calculus cross product
        if(dot_product3D(poly_normal,sightLine) > 0)
        {//detect visible face if dot product greater than 0
            vec_frontface_index.push_back(i);
            //store front 'polygon index'
            for(int j=1; j< (int)vec_polygon[i][0]+1; j++)
                front_vertex.push_back(vec_polygon[i][j]);
        }
    }

    for(int i=0; i< (int)vec_frontface_index.size(); i++)
        vec_frontface_poly.push_back(vec_polygon[vec_frontface_index[i]]);
        //store front face polygon data
    //---------------------------------------------------------
    vector< vector<float> > vec_temp(vec_view_vertex);
    // create temporary vector
    for(int i = 0 ; i < (int)front_vertex.size() ; i++)
    {   //mark visible vertices by add extra number at the back of them
        if(vec_temp[front_vertex[i]-1].size() == 4)
            vec_temp[front_vertex[i]-1].push_back(-1);
    }

    vec_normalized_vertex = vec_temp;
    //initialize vec_normalized_vertex
    for(int i = 0 ; i < (int)vec_temp.size() ; i++)
    {//compute perspective vertices from view space vertices
        if(vec_temp[i].size() == 5)//detect if the vertices is visible
        {//ignore invisible vertices
            matrix_multi_4x1(vec_temp[i], M_pers_4D, vec_view_vertex[i]);
            //compute vertices in perspective space without invisible vertices
            for(int j=0; j<4; j++)
                vec_normalized_vertex[i][j] = vec_temp[i][j]/vec_temp[i][3];
                //compute nomalized vertices through divide by W
        }
    }
    //Scale normalized vertices to device vertices
    vec_device_vertex = vec_normalized_vertex;
    //initialize vec_device_vertex with normalized vertices
    for(int i=0;i<(int)vec_normalized_vertex.size();i++)
    {   //scale normalize vertices data to device
        if(vec_normalized_vertex[i].size() == 5)
        {
            vec_device_vertex[i][0] =
                ((Xmax - Xmin)/2)*(vec_normalized_vertex[i][0]+1) ;//x
            vec_device_vertex[i][1] =
                ((Ymax - Ymin)/2)*(vec_normalized_vertex[i][1]+1) ;//y
        }
    }
}

void loadModel::setUpVector(float x, float y, float z)
{   //set up vector for camera
    up_vector.push_back(x);
    up_vector.push_back(y);
    up_vector.push_back(z);
}
void loadModel::setDistanceNear(float Near)
{   //set near clipping plan =>d
    d = Near;
}
void loadModel::setDistanceFar(float Far)
{   //set far clipping plan => far
    far = Far;
}
void loadModel::setWidth(float width)
{   //set h for perspective matrix
    h = width;
}
void loadModel::setShiftLocal(float x, float y, float z)
{   //set vector that object shift from local to world
    offset_world.push_back(x);
    offset_world.push_back(y);
    offset_world.push_back(z);
}
void loadModel::setProjectRef(float x, float y, float z)
{   //set project reference for camera
    P_ref.push_back(x);
    P_ref.push_back(y);
    P_ref.push_back(z);
}

void loadModel::setfileName(string name)
{   //set geometric file name
    fileName = new char[name.length() + 1];
    strcpy(fileName, name.c_str());
}

void loadModel::setCameraPosition(float x, float y, float z)
{   //set Camera position in world coordinate
    camera.push_back(x);
    camera.push_back(y);
    camera.push_back(z);
}
void loadModel::setDeviceScale(float x, float X,
                               float y, float Y)
{
    Xmin = x;
    Xmax = X;
    Ymin = y;
    Ymax = Y;

}
void loadModel::view_N(vector<float> &N_vector ,
                       vector<float> camera, vector<float> vertex)
{   //compute N vector for camera
    //vertex[3] => P_reference
    //camera[3] => camera position at World coordinate
    //N = P.ref - C/|P.ref - C|
    vector<float> numerator(3); //P.ref - C
    float denominator; //|P.ref - C|
    //P.ref - C ===> vertex - camera
    vector_subtraction(numerator, camera, vertex);
    //|P.ref - C|
    denominator = vector_length(numerator);
    for(int i = 0 ; i < 3 ; i++)
        N_vector[i] = numerator[i]/denominator;

}

void loadModel::view_U(vector<float> &U_vector ,
                   vector<float> N_vector , vector<float> up_vector)
{   //compute U vector for camera
    //N_vector => N vector for camera
    //V' <== up_vector[3]
    //U = N x V' / |N x V'|
    vector<float> numerator(3); // N x V'
    float denominator; //|N x V'|
    // N x V' = numerator
    cross_product3D(numerator , N_vector, up_vector);
    //|N x V'| = denominator
    denominator = vector_length(numerator);

    for(int i = 0 ; i < 3 ; i++)
        U_vector[i] = numerator[i]/denominator;
}

void loadModel::view_V(vector<float> &V_vector ,
                       vector<float> U_vector, vector<float> N_vector)
{   //compute V vector for camera
    //V = U x N
    cross_product3D(V_vector , U_vector , N_vector);
}

float loadModel::vector_length(vector<float> vector_1)
{//compute vector length
    return pow(pow(vector_1[0],2) + pow(vector_1[1],2) +
               pow(vector_1[2],2),0.5);
}

void loadModel::vector_subtraction(vector<float> &sub,
                   vector<float> vector_1, vector<float> vector_2)
{   //compute vector subtraction
    //vector_2 - vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] - vector_1[i];
}
void loadModel::vector_summation(vector<float> &sub,
                   vector<float> vector_1, vector<float> vector_2)
{   //compute vector summation
    //vector_2 + vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] + vector_1[i];
}
float loadModel::dot_product3D(vector<float> vector_1,vector<float> vector_2)
{//compute dot product
    return vector_1[0]*vector_2[0] + vector_1[1]*vector_2[1] +
           vector_1[2]*vector_2[2];

}

void loadModel::cross_product3D(vector<float> &normal,
                        vector<float>vector_1, vector<float>vector_2)
{//compute cross product
    normal[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    normal[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    normal[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
}

void loadModel::matrix_multi_4x1(vector <float> &matrix_out,
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

void loadModel::matrix_multi_4x4(vector< vector<float> > &matrix_out,
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
void loadModel::matrix_view(vector< vector<float> >& M_view_4D,
                            vector<float> camera, vector<float> up_vector)
{   //compute view transformation Matrix
    //input camera position => camera[3]
    //input up_vector (i.e., up vector for camera) =>up_vector[3]
    //input target vertex => *vertex
    vector< vector<float> > M_viewR_4D(4, vector<float>(4));
    //rotate matrix
    vector< vector<float> > M_viewT_4D(4, vector<float>(4));
    //translate matrix
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
        {
            if(i == j)
            {
                M_viewR_4D[i][j] = 1;
                M_viewT_4D[i][j] = 1;
            }
            else
            {
                M_viewR_4D[i][j] = 0;
                M_viewT_4D[i][j] = 0;
            }
        }
    //homogeneous -Cx -Cy -Cz => M_viewT_4D
    for(int i=0; i<3; i++)
        M_viewT_4D[i][3] = -camera[i];

    //    M_viewR_4D[2] => Nx Ny Nz
    view_N(M_viewR_4D[2], camera , P_ref);
    //    M_viewR_4D[0] => Ux Uy Uz
    view_U(M_viewR_4D[0], M_viewR_4D[2], up_vector);
    //    M_viewR_4D[1] => Vx Vy Vz
    view_V(M_viewR_4D[1], M_viewR_4D[0], M_viewR_4D[2]);

    matrix_multi_4x4(M_view_4D, M_viewR_4D , M_viewT_4D);
}

void loadModel::matrix_pers(vector< vector<float> >& M_pers_4D
                            , float d, float f, float h)
{   //compute perspective matrix
    // d => near clipping plan
    // f => far clipping plan
    // h => half of clipping length
    // M_pers_4D => Perspective transform Matrix
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
        {
            if(i == 0 && j ==0)
                M_pers_4D[i][j] = 0.375;
            else if(i == 1 && j ==1)
                M_pers_4D[i][j] = 0.5;
            else if (i == 2 && j ==2)
                M_pers_4D[i][j] = f / (f - d);
            else if (i == 2 && j ==3)
                M_pers_4D[i][j] = - f * d / (f - d);
            else if (i == 3 && j ==2)
                M_pers_4D[i][j] = 1;
            else
                M_pers_4D[i][j] = 0;
        }
}

void loadModel::matrix_model(vector< vector<float> >& M_model_4D, vector<float> offset)
{   //compute modeling matrix
    //M_model_4D => Modeling Matrix
    //offset => vector from local to world
    for(int row = 0 ; row < 4 ; row++)
        for(int column = 0 ; column < 4 ; column++ )
        {
            if(row == column)
                M_model_4D[row][column] = 1;
            else if(row == 0 && column ==3 )
                M_model_4D[row][column] = offset[0];
            else if(row == 1 && column ==3 )
                M_model_4D[row][column] = offset[1];
            else if(row == 2 && column ==3 )
                M_model_4D[row][column] = offset[2];
            else
                M_model_4D[row][column] = 0;
        }
}


loadModel::~loadModel()
{
    //deconstructor
}
