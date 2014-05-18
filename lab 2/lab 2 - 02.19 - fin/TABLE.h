/** Tasks in TABLE.h
    TABLE.h is a entry for Edge Table.
    Access Edge Value at EDGETABLE.h
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
        void insertValue(float ymax, float X_start, float X_slope,
                         float Z_start, float Z_slope)
        {
            beg.setEdgeValue(ymax,X_start,X_slope, Z_start,Z_slope);
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
