/** Tasks in EDGETABLE.h
    EDGETABLE.h contains edge value and be accessed by TABLE.h
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
        void setEdgeValue(float ymax, float X_start, float X_slope,
                           float Z_start, float Z_slope)
        {
            EdgeValue.clear();
            EdgeValue.push_back(ymax);
            EdgeValue.push_back(X_start);
            EdgeValue.push_back(X_slope);
            EdgeValue.push_back(Z_start);
            EdgeValue.push_back(Z_slope);
        }
        virtual ~EDGETABLE(){};
    private:
};



#endif // EDGETABLE_H_INCLUDED
