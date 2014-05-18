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
                         float Z_start, float Z_slope, float Nx_start, float Nx_slope,
                         float Ny_start, float Ny_slope, float Nz_start, float Nz_slope)
        {
            EdgeValue.clear();
            EdgeValue.push_back(ymax);
            EdgeValue.push_back(xstart);
            EdgeValue.push_back(X_slope);
            EdgeValue.push_back(Z_start);
            EdgeValue.push_back(Z_slope);
            EdgeValue.push_back(Nx_start);
            EdgeValue.push_back(Nx_slope);
            EdgeValue.push_back(Ny_start);
            EdgeValue.push_back(Ny_slope);
            EdgeValue.push_back(Nz_start);
            EdgeValue.push_back(Nz_slope);
        }
        virtual ~EDGETABLE(){};
    private:
};



#endif // EDGETABLE_H_INCLUDED
