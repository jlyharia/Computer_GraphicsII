/** Task in EDGETABLE_H
    Implement edge Value
**/
#ifndef EDGETABLE_H_INCLUDED
#define EDGETABLE_H_INCLUDED
#include <vector>
using namespace std;
class EDGETABLE
{
    public:
        vector<float> EdgeValue;
        EDGETABLE(){};
        void setEdgeValue(float ymax, float xstart, float X_slope,
                           float Z_start, float Z_slope, float R_start, float R_slope,
                         float G_start, float G_slope, float B_start, float B_slope)
        {
            EdgeValue.clear();
            EdgeValue.push_back(ymax);
            EdgeValue.push_back(xstart);
            EdgeValue.push_back(X_slope);
            EdgeValue.push_back(Z_start);
            EdgeValue.push_back(Z_slope);
            EdgeValue.push_back(R_start);
            EdgeValue.push_back(R_slope);
            EdgeValue.push_back(G_start);
            EdgeValue.push_back(G_slope);
            EdgeValue.push_back(B_start);
            EdgeValue.push_back(B_slope);
        }
        virtual ~EDGETABLE(){};
    private:
};



#endif // EDGETABLE_H_INCLUDED
