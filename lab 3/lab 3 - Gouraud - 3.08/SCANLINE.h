#ifndef SCANLINE_H
#define SCANLINE_H
#include <iostream>
#include <vector>
#include "TABLE.h"
using namespace std;
class SCANLINE
{
    public:
        vector< vector< vector<float> > > real_pixel;
        ///real pixel 0(exit pixel) 1(depth value) 2(Red value) 3(Green value)
        ///4(Blue value)
        SCANLINE(vector< vector<int> > , vector< vector<int> > ,
                 vector< vector<float> >,vector< vector<float> >);
        ///constructor

        //put polygon and vertices data to scanline class
        void pixelCompute();

        void find_near_vertex(vector< vector<float> > &,
                vector< vector<float> > &,
                vector< vector<float> >,
                int);

        void EdgeTable(vector< vector<float> > ,vector< vector<float> > ,
                    vector< vector<int> >,int);

        void get_Ycompare(vector< vector<float> > & ,int);
        //compute Y value of vertices in one polygon
        void get_round_vertex(vector< vector<float> > &);

        void ActiveEdgeTable(vector <TABLE>,int);

        void setDeviceCoord(int,int,int,int);


        float dot_product3D(vector<float> , vector<float> );
        //compute dot product
        void cross_product3D(vector<float> &, vector<float>, vector<float>);
        //compute cross product
        float vector_length(vector<float>);
        //compute vector length
        void vector_summation(vector<float> &, vector<float>, vector<float>);
        //compute vector subtraction (vector_2 + vector_1)
        void vector_subtraction(vector<float> &, vector<float>, vector<float>);
        //compute vector subtraction (vector_2 - vector_1)


        void Compute_Poly_Normal();
        //compute polygon normal

        virtual ~SCANLINE();

    private:
        vector< vector<int> > front_Poly;
        vector< vector<int> > Poly;// store all polygon data
        vector< vector<float> > device_vertex;
        vector< vector<float> > round_vertex;
        vector< vector<float> > world_vertex;

        vector< vector<float> > Light_Intensity;//vertex intensity

        vector< vector< vector<float> > > temp_pixel;//pixel for Z buffer
        float K_diffuse;
        float K_specular;
        float K_ambient;
        vector <float> vec_light;//infinity
        vector <float> vec_view;//infinity
        int Xmin;
        int Ymin;
        int Xmax;
        int Ymax;
};

#endif // SCANLINE_H
