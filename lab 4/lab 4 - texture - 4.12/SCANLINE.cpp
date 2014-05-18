/** Tasks in SCANLINE.cpp
    This file implement Rasterisation by Edge Table and Active Edge Table,
    in the meantime, implement Phong illumination model and Phong shading
**/
#include "calculus.h"
#include "SCANLINE.h"
#include "TABLE.h"
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <fstream>
#include "textureloader.h"
#define PI 3.14159265
//ofstream datatest("ET.txt", ios::out);
SCANLINE::SCANLINE(vector< vector<int> > temp_front_Poly,
                   vector< vector<int> > temp_Poly,
                   vector< vector<float> > temp_device_vertex,
                   vector< vector<float> > temp_world_vertex,
                   vector< vector<float> > temp_M_Invert)
{//initialization
    Poly = temp_Poly;
    //put all polygon data to Poly
    front_Poly = temp_front_Poly;
    //put polygon data to front_Poly
    device_vertex = temp_device_vertex;
    //put vertices data to device vertices
    world_vertex = temp_world_vertex;
    //put vertices data to world vertices
    M_Invert = temp_M_Invert;
    get_round_vertex(round_vertex);
    //round up x,y value and put in round_vertex
    K_diffuse = 0.2;
    K_specular = 0.4;
    K_ambient = 0.1;
    //set vector to the light (sun light)
    vector <float> temp_vec_light (3,1);
    vec_light = temp_vec_light;
    vec_light[0] = 0.5;

    //set vector of view (infinity)
    vector <float> temp_vec_view (3,0);
    vec_view = temp_vec_view;
    vec_view[2] = 1;  //set view vector (0,0,1)


//    for(int i = 0 ; i < (int)device_vertex.size() ; i++)
//    {
//        datatest<<i<<'\t';
//        for(int j = 0; j<4; j++)
//            datatest<<setw(15)<<device_vertex[i][j];
//        datatest<<endl;
//    }
}
void SCANLINE::Compute_Poly_Normal()
{
    vector< vector<float> > temp_vertex_normal(world_vertex.size(),vector<float>(3));
    //light intensity
    I_light.push_back(1.0); //Red
    I_light.push_back(0.64); //Green
    I_light.push_back(0.0); //Blue
    vector <float> poly_vector_1(3);
    vector <float> poly_vector_2(3);
    vector <float> poly_normal(3);
    vector <float> temp_H(3);
    //calculus H
    vector_summation(temp_H, vec_light,vec_view);
    for (int j=0; j<3 ;j++ )
        H_specular.push_back(temp_H[j]/vector_length(temp_H));
    ///---Intensity ambient----Ka*I
    for (int j=0; j<3 ;j++ )
        Intensity_ambient.push_back(K_ambient*I_light[j]);
    for(int i=0; i<(int)Poly.size(); i++)
    {//compute polygon normal
        vector_subtraction(poly_vector_1,
                           world_vertex[Poly[i][2]-1],
                           world_vertex[Poly[i][1]-1]);
        vector_subtraction(poly_vector_2,
                           world_vertex[Poly[i][2]-1],
                           world_vertex[Poly[i][3]-1]);
        //compute 2 vector on one polygon
        cross_product3D(poly_normal, poly_vector_1, poly_vector_2);
        //calculus cross product

        for(int j=1; j<(int)Poly[i][0]+1 ;j++)
        {//add normal of neibor polygon to vertex
            for (int p=0; p<3 ;p++ )
                temp_vertex_normal[Poly[i][j]-1][p] += poly_normal[p];
        }

    }
    for(int i=0; i<(int)temp_vertex_normal.size() ;i++ )
    {
        float temp_length = vector_length(temp_vertex_normal[i]);
        for (int j=0; j<3 ;j++ )
        {   //normalize the normal on vertex
            temp_vertex_normal[i][j] = temp_vertex_normal[i][j] / temp_length;

        }
    }
    vertex_normal = temp_vertex_normal;
    //vertex normals as average of surrounding neibor polygon's normal


}
void SCANLINE::textureMapping(vector<float> pixel_normal,
                              float &cylinder_u,float &cylinder_v)
{
    float theta;
    //get theta
    theta = acos (pixel_normal[0]) * 180.0 / PI;
//    if(pixel_normal[2] > 0)
//        theta = 360 - theta;
    //get u,v in texture space----------
    cylinder_u = (theta / 180) * texture_width;
    if(cylinder_u > texture_width)
        cylinder_u = texture_width;
    cylinder_v = ((pixel_normal[1] +1 ) / 2) * texture_height;
}
void SCANLINE::compute_invert(vector<float> &device_xyz)
{
    //1 renormal
     device_xyz[0] = (2/(Xmax - Xmin))*(device_xyz[0])-1 ;//x
     device_xyz[1] = (2/(Ymax - Ymin))*(device_xyz[1])-1 ;//y
    //2 times invert matrix
    device_xyz.push_back(1); //add w value
    vector<float>  vec_invert(device_xyz);
       //scale normalize vertices data to device
    matrix_multi_4x1(device_xyz, M_Invert, vec_invert);

    for(int j = 0; j<4; j++)
        device_xyz[j] = device_xyz[j]/device_xyz[3];


//    for(int j = 0; j<4; j++)
//        datatest<<setw(15)<<device_xyz[j];
//    datatest<<endl;
}
void SCANLINE::Compute_pixel_intensity()
{
    vector <float> Intensity_diffuse(3);
    vector <float> Intensity_specular(3);

    float cylinder_u,cylinder_v;
    int shininess = 8;
    for (int i = 0; i< Ymax ; i++ )
    	for (int j = 0; j< Xmax; j++ )
    	{
    	    vector<float> pixel_normal(real_pixel[i][j].begin()+2,real_pixel[i][j].begin()+5);
    	    vector<float> device_xyz(real_pixel[i][j].begin()+5,real_pixel[i][j].end());

    	    if(vector_length(pixel_normal) == 0)
                continue;
//            if(vector_length(device_xyz) == 0)
//                continue;
//            datatest<<i<<'\t'<<j;
//            for(int p = 0; p<(int)pixel_normal.size(); p++)
//                datatest<<setw(15)<<pixel_normal[p];
//            datatest<<endl;
            vector<float> line_inter(4);
            compute_invert(device_xyz);//out put xyz in world space
            vector_subtraction(line_inter, ObjectCenter, device_xyz);

//            for(int p = 0; p<(int)line_inter.size(); p++)
//                datatest<<setw(15)<<line_inter[p];
//            datatest<<endl;

            Normalization(line_inter);
    	    //normalize the pixel normal
    	    Normalization(pixel_normal);

            ///texture mapping

//            textureMapping(pixel_normal,cylinder_u,cylinder_v);
            textureMapping(pixel_normal,cylinder_u,cylinder_v);

            ///-------------------------------
    	    float cos_diffuse = dot_product3D(pixel_normal,vec_light);
            float cos_specular = pow(dot_product3D(pixel_normal,H_specular),
                                     shininess);

            for (int p = 0; p< 3 ; p++ )
            {
                ///----------Intensity diffuse----Kd*I*(NL)
                Intensity_diffuse[p] = K_diffuse*I_light[p]*cos_diffuse;
                ///----------Intensity specular----Ks*I*(NH)^n
                Intensity_specular[p] = K_specular*I_light[p]*cos_specular;

                //---------------sum of all intensity--
                //replace normal value to RGB value in real_pixel
                real_pixel[i][j][p+2]= Intensity_diffuse[p] +
                    Intensity_specular[p] + Intensity_ambient[p]
                    +imagePixel[(int)cylinder_v][(int)cylinder_u][p];
                if(real_pixel[i][j][p+2] > 1)
                    real_pixel[i][j][p+2] = 1;
            }

    	}
//    datatest.close();
}
void SCANLINE::pixelCompute()
{
    ///------------Illumination Model-------------------------
    Compute_Poly_Normal(); //compute normal of all polygon

    ///-------------------------------------

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

        EdgeTable(startVertex,nearVertex,poly_num);
    }//end poly_num loop

    Compute_pixel_intensity();//compute Intensity on each pixel

}
void SCANLINE::ActiveEdgeTable(vector <TABLE> entry, int poly_num)
{//Active Edge Table

    vector<float> AP;

    //default all element= 0
    vector< vector<float> > AET;
    int begin = 0;
    float Z_increOnX;
    float Nx_increOnX;//normal x
    float Ny_increOnX;//normal y
    float Nz_increOnX;//normal z
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

            //(2)Fill in desired pixel values, x and y are integer
            for(int j = 0; j< (int)AET.size();j+=2)
            {//compute Z value and RGB value
                if(ceil(AET[j+1][1]) == ceil(AET[j][1]))// same X value
                {//when overlapping
                    Z_increOnX  = 0;
                    Nx_increOnX = 0;
                    Ny_increOnX = 0;
                    Nz_increOnX = 0;
                }
                else
                {
                    Z_increOnX = (AET[j+1][3] - AET[j][3])/
                        (ceil(AET[j+1][1]) - ceil(AET[j][1]));
                    //I = I + (I2 - I2)/ (X2 - X1)
                    //linear interpolate normal on X
                    Nx_increOnX = (AET[j+1][5] - AET[j][5])/
                        (ceil(AET[j+1][1]) - ceil(AET[j][1]));
                    Ny_increOnX = (AET[j+1][7] - AET[j][7])/
                        (ceil(AET[j+1][1]) - ceil(AET[j][1]));
                    Nz_increOnX = (AET[j+1][9] - AET[j][9])/
                        (ceil(AET[j+1][1]) - ceil(AET[j][1]));
                }

                for(int X_value = ceil(AET[j][1]); X_value<= ceil(AET[j+1][1]);X_value++)
                {//fill pixel
                    if(X_value == ceil(AET[j][1]))
                    {   //put Z start at first
                        temp_pixel[y_min][X_value][1] = AET[j][3];
                        //put Normal start at first and replace to RGB value later
                        temp_pixel[y_min][X_value][2] = AET[j][5];
                        temp_pixel[y_min][X_value][3] = AET[j][7];
                        temp_pixel[y_min][X_value][4] = AET[j][9];
                        //put xyz value
                        temp_pixel[y_min][X_value][5] = X_value;
                        temp_pixel[y_min][X_value][6] = y_min;
                        temp_pixel[y_min][X_value][7] = temp_pixel[y_min][X_value][1];
                    }
                    else
                    {
                        temp_pixel[y_min][X_value][1] = //increase Z incrementally
                            temp_pixel[y_min][X_value -1 ][1] + Z_increOnX;
                        temp_pixel[y_min][X_value][2] =
                            temp_pixel[y_min][X_value -1 ][2] + Nx_increOnX;
                        temp_pixel[y_min][X_value][3] =
                            temp_pixel[y_min][X_value -1 ][3] + Ny_increOnX;
                        temp_pixel[y_min][X_value][4] =
                            temp_pixel[y_min][X_value -1 ][4] + Nz_increOnX;
                        //put xyz value
                        temp_pixel[y_min][X_value][5] = X_value;
                        temp_pixel[y_min][X_value][6] = y_min;
                        temp_pixel[y_min][X_value][7] = temp_pixel[y_min][X_value][1];
                    }
                    ///--------------Z buffer---------------------------------
                    if(real_pixel[y_min][X_value][0] == 0)
                    {//if there doesn't exit pixel
                        real_pixel[y_min][X_value][0]= 1;
                        for(int i = 1 ; i < 8 ; i++)//replace pixel value to real buffer
                            real_pixel[y_min][X_value][i] =
                                temp_pixel[y_min][X_value][i];
                    }
                    else if(temp_pixel[y_min][X_value][1] < real_pixel[y_min][X_value][1])
                    {//while exist a nearer pixel
                        for(int i = 1 ; i < 8 ; i++)//replace pixel value to real buffer
                            real_pixel[y_min][X_value][i] =
                                temp_pixel[y_min][X_value][i];
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
                //increase normal value on edge
                AET[j][5] = AET[j][5] + AET[j][6];//Normal x
                AET[j][7] = AET[j][7] + AET[j][8];//Normal y
                AET[j][9] = AET[j][9] + AET[j][10];//Normal z
            }
        }
    }//end lopp (int)entry.size()

}



void SCANLINE::EdgeTable(vector< vector<float> > startVertex,
                    vector< vector<float> > nearVertex,
                    int poly_num)
{//put edge value in edge table
    vector<float> edgeValue;
    vector <TABLE> entry(Ymax);
    vector<int> repeatCheck;//check if an edge is already in stack
    float X_incre;
    float Z_incre;
    float Nx_incre;
    float Ny_incre;
    float Nz_incre;
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
            if(startVertex[i][1] == nearVertex[i*2 + j][1]) //Y value are the same
            {
                Z_incre = 0;
                Nx_incre = 0;
                Ny_incre = 0;
                Nz_incre = 0;
            }
            else
            {   //(z1 - z2) / (y1 - y2)
                Z_incre = (nearVertex[i*2 + j][2] - startVertex[i][2])/
                            (nearVertex[i*2 + j][1] - startVertex[i][1]);

                // Normal increment (N1 - N2) / (y1 - y2)
                Nx_incre = (nearVertex[i*2 + j][5] - startVertex[i][5])/
                            (nearVertex[i*2 + j][1] - startVertex[i][1]);
                Ny_incre = (nearVertex[i*2 + j][6] - startVertex[i][6])/
                            (nearVertex[i*2 + j][1] - startVertex[i][1]);
                Nz_incre = (nearVertex[i*2 + j][7] - startVertex[i][7])/
                            (nearVertex[i*2 + j][1] - startVertex[i][1]);
            }
            //put in Edge table
            entry[startVertex[i][1]].insertValue
            (edgeValue[0] , startVertex[i][0] , X_incre,
             startVertex[i][2],Z_incre,
             startVertex[i][5],Nx_incre,
             startVertex[i][6],Ny_incre,
             startVertex[i][7],Nz_incre);

        }
    }

    ActiveEdgeTable(entry,poly_num);


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
{
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

    String3D temp(Ymax,String2D(Xmax,String1D(8)));
    for (int i = 0; i< Ymax ; i++ )
    	for (int j = 0; j< Xmax ; j++ )
    		temp[i][j][1] = -1.0/0.0; //negative infinity
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
        //add vertex Normal value to start vertex
        temp_0.insert(temp_0.end(),vertex_normal[Ycompare[i][0]-1].begin(),
                      vertex_normal[Ycompare[i][0]-1].end());
        startVertex.push_back(temp_0); //start vertex index from 0
        ///----------------------------------------------
        vector< float > temp_1(round_vertex[Ycompare[i][2]-1]);
        vector< float > temp_2(round_vertex[Ycompare[i][3]-1]);
        temp_1[4] = Ycompare[i][2]-1;
        temp_2[4] = Ycompare[i][3]-1;
        //add RGB value to near vertex
        temp_1.insert(temp_1.end(),vertex_normal[Ycompare[i][2]-1].begin(),
                      vertex_normal[Ycompare[i][2]-1].end());

        temp_2.insert(temp_2.end(),vertex_normal[Ycompare[i][3]-1].begin(),
                      vertex_normal[Ycompare[i][3]-1].end());
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
{   //compute vector subtraction
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
void SCANLINE::setImage(const char* imageFile)
{
    textureloader texture_1(imageFile);
    imagePixel = texture_1.imagePixel;
    texture_width = (int)texture_1.width;
    texture_height = (int)texture_1.height;
}
void SCANLINE::Normalization(vector<float> &normal)
{
    float temp_length = vector_length(normal);
    for (int i=0; i<3 ;i++ )
    {   //normalize the pixel normal
        normal[i] = normal[i] / temp_length;

    }
}
void SCANLINE::setObjectCenter(float *array)
{
    ObjectCenter.push_back(array[0]);
    ObjectCenter.push_back(array[1]);
    ObjectCenter.push_back(array[2]);
}
SCANLINE::~SCANLINE()
{
    //dtor
}
