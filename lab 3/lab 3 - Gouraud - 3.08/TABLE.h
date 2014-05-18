/** Task in TABLE.h
    Implement edge Table
**/
#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#include <iostream>
#include <vector>
#include "EDGETABLE.h"
using namespace std;
class TABLE
{
    public:
        EDGETABLE beg;
        int count;//Number of edge with same Y min
        vector < EDGETABLE > first;
        TABLE()
        {
            count = 0;
        }

        void insertValue(float ymax, float xstart, float X_slope,
                         float Z_start, float Z_slope, float R_start, float R_slope,
                         float G_start, float G_slope, float B_start, float B_slope)
        {
            beg.setEdgeValue(ymax,xstart,X_slope, Z_start,Z_slope,
                             R_start,R_slope,G_start,G_slope,
                             B_start,B_slope);
            first.push_back(beg);
            count++;
        }
        void getEdgeValue(vector<float> &E_value, int index)
        {
            vector<float> temp(first[index].EdgeValue);
            E_value = temp;

        }
        virtual ~TABLE(){};
};

#endif // TABLE_H_INCLUDED
