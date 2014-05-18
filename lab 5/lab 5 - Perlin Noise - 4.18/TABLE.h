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
                         float Z_start, float Z_slope, float Nx_start, float Nx_slope,
                         float Ny_start, float Ny_slope, float Nz_start, float Nz_slope)
        {
            beg.setEdgeValue(ymax,xstart,X_slope, Z_start,Z_slope,
                             Nx_start,Nx_slope,Ny_start,Ny_slope,
                             Nz_start,Nz_slope);
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
