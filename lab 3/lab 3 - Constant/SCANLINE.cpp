/** Tasks in SCANLINE.cpp
    This file implement Rasterisation by Edge Table and Active Edge Table,
    in the meantime, implement Phong illumination model and constant shading
**/
#include "SCANLINE.h"
#include "TABLE.h"
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <fstream>
SCANLINE::SCANLINE(vector< vector<int> > temp_front_Poly,
                   vector< vector<float> > temp_device_vertex,
                   vector< vector<float> > temp_world_vertex)
{
    front_Poly = temp_front_Poly;
    //put polygon data to front_Poly
    device_vertex = temp_device_vertex;
    //put vertices data to device vertices
    world_vertex = temp_world_vertex;
    //put vertices data to world vertices
    get_round_vertex(round_vertex);
    //round up x,y value and put in round_vertex
    K_diffuse = 0.5;
    K_specular = 0.5;
    K_ambient = 0.5;
    //set vector to the light (sun light)
    vector <float> temp_vec_light (3,1);
    vec_light = temp_vec_light;
    //set vector of view (infinity) (1 , 1, 1)
    vector <float> temp_vec_view (3,0);
    vec_view = temp_vec_view;
    vec_view[2] = 1;
    //set view vector (0,0,1)

}
void SCANLINE::Compute_Poly_Normal()
{
    vector <float> I_light(3);//light intensity
    I_light[0] = 1;//R
    I_light[1] = 0;//G
    I_light[2] = 0;//B
    vector <float> temp_Light_Intensity(3);
    vector <float> Intensity_diffuse(3);
    vector <float> Intensity_specular(3);
    vector <float> Intensity_ambient(3);
    vector <float> poly_vector_1(3);
    vector <float> poly_vector_2(3);
    vector <float> poly_normal(3);
    vector <float> H(3);
    vector <float> temp_H(3);
    vector_summation(temp_H, vec_light,vec_view);
    for (int j=0; j<3 ;j++ ) //alternate Phong illumination model on specular term
        H[j] = temp_H[j]/vector_length(temp_H);
    for(int i=0; i<(int)front_Poly.size(); i++)
    {//compute polygon normal
        vector_subtraction(poly_vector_1,
                           world_vertex[front_Poly[i][2]-1],
                           world_vertex[front_Poly[i][1]-1]);
        vector_subtraction(poly_vector_2,
                           world_vertex[front_Poly[i][2]-1],
                           world_vertex[front_Poly[i][3]-1]);
        //compute 2 vector on one polygon
        cross_product3D(poly_normal, poly_vector_1, poly_vector_2);
        //calculus cross product
        for (int j=0; j<3 ;j++ )//(normalized)
        	poly_normal[j] = poly_normal[j]/ vector_length(poly_normal);


        for (int j=0; j<3 ;j++ )
        {
            /**----------Intensity diffuse----Kd*I*(NL)-----------------*/
             Intensity_diffuse[j] =
                K_diffuse*I_light[j]*dot_product3D(poly_normal,vec_light);
            /**----------Intensity specular----Ks*I*(NH)^n-----------------*/
            Intensity_specular[j] =
                K_specular*I_light[j]*pow(dot_product3D(poly_normal,H),1);
            /**----------Intensity ambient----Ka*I-----------------*/
            Intensity_ambient[j] = K_ambient*I_light[j];
            //-------------------------------------------------
            temp_Light_Intensity[j]= Intensity_diffuse[j] +
                Intensity_specular[j] + Intensity_ambient[j];
        }
        Light_Intensity.push_back(temp_Light_Intensity);
        //index match to front_poly index
    }

}

void SCANLINE::pixelCompute()
{
    ///------------Illumination Model-------------------------
    Compute_Poly_Normal(); //compute normal of front polygon

    ///-----------------following code are same as lab 2------------------
    vector< vector<float> > Ycompare;
    vector< vector<float> > nearVertex;
    vector< vector<float> > startVertex;
    for(int poly_num = 0; poly_num< (int)front_Poly.size();poly_num++)
    {
        Ycompare.clear();
        nearVertex.clear();
        startVertex.clear();
        //compute edge table
        //put y value and vertex index in Ycompare
        //sort by y value - buble sort
        get_Ycompare(Ycompare,poly_num);
        //find near vertices around start vertices

        find_near_vertex(nearVertex,startVertex,Ycompare,poly_num);

        EdgeTable(startVertex,nearVertex,
                       front_Poly,poly_num);
    }//end poly_num loop
}
void SCANLINE::ActiveEdgeTable(vector <TABLE> entry, int poly_num)
{//Active Edge Table

    float color_R = Light_Intensity[poly_num][0];
    float color_G = Light_Intensity[poly_num][1];
    float color_B = Light_Intensity[poly_num][2];
    //assigne Light Intensity to RGB variable
    vector<float> AP;
    //default all element= 0
    vector< vector<float> > AET;
    int begin = 0;
    float Z_increOnX;
    int limit = 10;
    for(int y_min = 0; y_min< (int)entry.size(); y_min++) //i -> Y min
    {
        if(entry[y_min].count == 0 && begin==0)
        {
            continue; //no edge in Edge Table
        }
        else //exist edge in Edge Table
        {
            begin=1;

            //(1.1)put edge value from ET to AET
            for(int j = 0; j< entry[y_min].count;j++)
            {
                entry[y_min].getEdgeValue(AP,j);
                AET.push_back(AP);
            }
            //AET are empty
            if(AET.size()==0)
                break;
            //(1.2)maintaining ATE sort order on x
            for(int row =0; row <( int)AET.size() - 1;row++)
                for(int col=0; col <( int)AET.size()-row-1; col++)
                    if(AET[col][1] > AET[col+1][1])
                        swap(AET[col], AET[col+1]);

//            if(limit >0)
//            {
//                for(int i =0; i< (int)AET.size(); i ++)
//                {
//                    for(int j = 0; j < (int)AET[i].size(); j++)
//                        cout<<AET[i][j]<<'\t';
//                    cout<<endl;
//                }
//                limit--;
//            }
            //(2)Fill in desired pixel values, x and y are integer
            for(int j = 0; j< (int)AET.size();j+=2)
            {//fill pixel
                if(ceil(AET[j+1][1]) == ceil(AET[j][1]))// same X value
                    Z_increOnX = 0;
                else
                    Z_increOnX = (AET[j+1][3] - AET[j][3])/
                        (ceil(AET[j+1][1]) - ceil(AET[j][1]));

                for(int X_value = ceil(AET[j][1]); X_value<= ceil(AET[j+1][1]);X_value++)
                {
                    //Z value
                    if(X_value == ceil(AET[j][1]))
                        temp_pixel[y_min][X_value][1] = AET[j][3];
                    else
                        temp_pixel[y_min][X_value][1] = //increase Z incrementally
                            temp_pixel[y_min][X_value -1 ][1] + Z_increOnX;
                    ///---------Z buffer(Visible surface detect)------------------
                    if(real_pixel[y_min][X_value][0] == 0)
                    {//if there doesn't exit a pixel
                        real_pixel[y_min][X_value][0]= 1;//exist a pixel
                        real_pixel[y_min][X_value][1] =
                            temp_pixel[y_min][X_value][1];
                        real_pixel[y_min][X_value][2]= color_R;
                        real_pixel[y_min][X_value][3]= color_G;
                        real_pixel[y_min][X_value][4]= color_B;
                    }
                    else if(temp_pixel[y_min][X_value][1] < real_pixel[y_min][X_value][1])
                    {//while exist a nearer pixel
                        real_pixel[y_min][X_value][1] =
                            temp_pixel[y_min][X_value][1];
                        real_pixel[y_min][X_value][2]= color_R;
                        real_pixel[y_min][X_value][3]= color_G;
                        real_pixel[y_min][X_value][4]= color_B;

                    }


                }
            }


            //(3)Remove from AET entries for which y = ymax
            for(int j = 0; j< (int)AET.size();j++)
            {//check Y max
                if(AET[j][0] == y_min)
                {
                    AET.erase(AET.begin()+j);
                    j--;
                }
            }

            //(4)replace x by x + increment
            //|0 Ymax | 1 X start | 2 X incre
            for(int j = 0; j< (int)AET.size();j++)
            {//increase X value
                AET[j][1] = AET[j][1] + AET[j][2];
                //increase Z value on edge
                AET[j][3] = AET[j][3] + AET[j][4];
            }
        }
    }//end lopp (int)entry.size()

}



void SCANLINE::EdgeTable(vector< vector<float> > startVertex,
                    vector< vector<float> > nearVertex,
                    vector< vector<int> >front_Poly,
                    int poly_num)
{//put edge value in edge table
    vector<float> edgeValue;
    vector <TABLE> entry(Ymax);
    vector<int> repeatCheck;//check if an edge is already in stack
    float X_incre;
    float Z_incre;
    for(int i = 0; i< (int)startVertex.size();i++)
    {
        for(int j = 0; j<2 ;j++)
        {    //i*2 + j
            edgeValue.clear();
//            //test if the edge value is already in stack by compare
//            //sum of index
            if(find(repeatCheck.begin(), repeatCheck.end(),
                    startVertex[i][4] + nearVertex[i*2 + j][4])
                    != repeatCheck.end()) //found repeated edge, skip
                continue;
            else//startVertex[i][4] store vertex index
                repeatCheck.push_back(startVertex[i][4] * nearVertex[i*2 + j][4]);

            //put 1/m
            if((startVertex[i][1] == nearVertex[i*2 + j][1]) &&
                startVertex[i][0] != nearVertex[i*2 + j][0])//Horizontal edge
            {
                continue;
            }
            else if((startVertex[i][1] == nearVertex[i*2 + j][1]) &&
                startVertex[i][0] == nearVertex[i*2 + j][0])
            {//dot  overlapped
                X_incre = 0;
            }
            else
                X_incre = (startVertex[i][0]-nearVertex[i*2 + j][0])/
                        (startVertex[i][1]-nearVertex[i*2 + j][1]);
            //put Y max
            if(startVertex[i][1] >= nearVertex[i*2 + j][1])
            {
                edgeValue.push_back(startVertex[i][1]);

            }
            else
            {
                edgeValue.push_back(nearVertex[i*2 + j][1]);//Ymax
            }
            //put X start
            edgeValue.push_back(startVertex[i][0]);

            edgeValue.push_back(X_incre);
            //compute Z incre
            //Z_incre
            if(startVertex[i][1] == nearVertex[i*2 + j][1])//Y value are the same
            {
                Z_incre = 0;
            }
            else
            {//(z1 - z2) / (y1 - y2)
                Z_incre = (nearVertex[i*2 + j][2] - startVertex[i][2])/
                            (nearVertex[i*2 + j][1] - startVertex[i][1]);
            }
            //put in Edge table
            entry[startVertex[i][1]].insertValue
            (edgeValue[0] , startVertex[i][0] , X_incre,
             startVertex[i][2],Z_incre);

        }
    }

    ActiveEdgeTable(entry,poly_num);
    //invoke Active Edge Table

}

void SCANLINE::get_Ycompare(vector< vector<float> > & Ycompare,
               int poly_num)
{   //Ycompare[original index][Y value]
    for(int i=1;i<front_Poly[poly_num][0]+1;i++)
    {
        vector<float> temp;
        temp.push_back(front_Poly[poly_num][i]);//index start from 1
        temp.push_back(round_vertex[front_Poly[poly_num][i]-1][1]);//y
        if(i == 1)//at first
        {
            temp.push_back(front_Poly[poly_num][front_Poly[poly_num][0]]);
            //front vertex
            temp.push_back(front_Poly[poly_num][i+1]); //back vertex
        }
        else if (i == front_Poly[poly_num][0])//at end
        {
            temp.push_back(front_Poly[poly_num][i-1]); //front vertex
            temp.push_back(front_Poly[poly_num][1]); //back vertex
        }
        else
        {
            temp.push_back(front_Poly[poly_num][i-1]); //front vertex
            temp.push_back(front_Poly[poly_num][i+1]); //back vertex
        }
        Ycompare.push_back(temp);
    }
    //sort by y value - buble sort
    for(int i=0;i<(int)Ycompare.size() - 1;i++)
        for(int j=0; j<(int)Ycompare.size()-i-1; j++)
            if(Ycompare[j][1] > Ycompare[j+1][1])
                swap(Ycompare[j],Ycompare[j+1]);

}
void SCANLINE::get_round_vertex(vector< vector<float> > &round_vertex)
{//round up vertices
    round_vertex = device_vertex;
    for(int i=0;i<(int)round_vertex.size();i++)
        for(int j=0;j<2;j++)
            round_vertex[i][j] = ceil(device_vertex[i][j]);
}

void SCANLINE::setDeviceCoord(int x, int X,int y, int Y)
{
    Xmin = x;
    Xmax = X;
    Ymin = y;
    Ymax = Y;
    //initialize real pixel
    typedef vector <float> String1D;
    typedef vector <String1D> String2D;
    typedef vector <String2D> String3D;

    String3D temp(Ymax,String2D(Xmax,String1D(5)));
    for (int i = 0; i< Ymax ; i++ )
    	for (int j = 0; j< Xmax ; j++ )
    		temp[i][j][1] = -1000; //negative infinity

    real_pixel = temp;
    temp_pixel = temp;
}

void SCANLINE::find_near_vertex(vector< vector<float> > &nearVertex,
                vector< vector<float> > &startVertex,
                vector< vector<float> >Ycompare,
                int poly_num)
{
    for (int i = 0; i < (int)Ycompare.size() ; i++)
    {
        vector< float > temp_0(round_vertex[Ycompare[i][0]-1]);
        temp_0[4] = Ycompare[i][0]-1;//real index
        startVertex.push_back(temp_0); //start vertex index from 0
        ///----------------------------------------------
        vector< float > temp_1(round_vertex[Ycompare[i][2]-1]);
        vector< float > temp_2(round_vertex[Ycompare[i][3]-1]);
        temp_1[4] = Ycompare[i][2]-1;
        temp_2[4] = Ycompare[i][3]-1;

        nearVertex.push_back(temp_1);
        nearVertex.push_back(temp_2);
    }

}
float SCANLINE::vector_length(vector<float> vector_1)
{//compute vector length
    return pow(pow(vector_1[0],2) + pow(vector_1[1],2) +
               pow(vector_1[2],2),0.5);
}
void SCANLINE::vector_summation(vector<float> &sub,
                   vector<float> vector_1, vector<float> vector_2)
{   //compute vector summation
    //vector_2 + vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] + vector_1[i];
}
void SCANLINE::vector_subtraction(vector<float> &sub,
                   vector<float> vector_1, vector<float> vector_2)
{//compute vector subtraction
    //vector_2 - vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] - vector_1[i];
}
float SCANLINE::dot_product3D(vector<float> vector_1,vector<float> vector_2)
{//compute dot product
    return vector_1[0]*vector_2[0] + vector_1[1]*vector_2[1] +
           vector_1[2]*vector_2[2];

}
void SCANLINE::cross_product3D(vector<float> &normal,
                        vector<float>vector_1, vector<float>vector_2)
{//compute cross product
    normal[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    normal[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    normal[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
}

SCANLINE::~SCANLINE()
{
    //dtor
}
