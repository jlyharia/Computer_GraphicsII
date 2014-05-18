/** Tasks in SCANLINE.h
    SCANLINE.h is a interface to SCANLINE.cpp
**/
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
        //store pixel data
        SCANLINE(vector< vector<int> > , vector< vector<float> >);
        //put polygon and vertices data to scanline class

        void pixelCompute();// main loop for compute each polygon

        void find_near_vertex(vector< vector<float> > &,
                vector< vector<float> > &,
                vector< vector<float> >,
                int ,int );
        //find the vertices data near by start vertices which is sorted by Y value
        void EdgeTable(vector< vector<float> > ,
                    vector< vector<float> > ,
                    vector< vector<int> >,int);
        //put edge value in edge table
        void get_Ycompare(vector< vector<float> > & ,int);
        //find the y value order in a polygon
        void get_round_vertex(vector< vector<float> > &);
        //round up x and y value
        void ActiveEdgeTable(vector <TABLE>,int);
        //Active Edge Table
        void setDeviceCoord(int,int,int,int);
        //set Device coordinate
        virtual ~SCANLINE();
    protected:
    private:
        vector< vector<int> > front_Poly;
        vector< vector<float> > device_vertex;
        vector< vector<float> > round_vertex;

        int Xmin;
        int Ymin;
        int Xmax;
        int Ymax;
};

#endif // SCANLINE_H
