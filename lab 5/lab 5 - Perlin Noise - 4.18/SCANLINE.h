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



        void Compute_Poly_Normal();
        //compute polygon normal
        void Compute_pixel_intensity();//compute pixel intensity
        ///----------------for texture mapping
        void compute_noise();
        //compute noise, change noise function need to change here and PerlinNoiseTexutre.cpp
        void textureMapping(vector<float>, float &, float &);
        virtual ~SCANLINE();
        float InterpolateCylinder(float x, float y);
    private:
        vector< vector<int> > front_Poly;
        vector< vector<int> > Poly;
        vector< vector<float> > device_vertex;
        vector< vector<float> > round_vertex;
        vector< vector<float> > world_vertex;

        vector< vector<float> > polygon_normal;
        vector< vector<float> > Light_Intensity;//vertex intensity
        vector< vector<float> > vertex_normal;
        //vertex normals as average of surrounding neibor polygon's normal
        vector< vector< vector<float> > > temp_pixel;
        float K_diffuse;
        float K_specular;
        float K_ambient;
        vector <float> vec_light;//infinity
        vector <float> vec_view;//infinity
        vector <float> H_specular;
        vector <float> Intensity_ambient;

        vector <float> I_light;
        int Xmin;
        int Ymin;
        int Xmax;
        int Ymax;
        vector< vector<float> > Texel_noise;//[x][y]
        int texture_width, texture_height;

};

#endif // SCANLINE_H
