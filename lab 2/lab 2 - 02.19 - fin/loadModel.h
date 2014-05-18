#ifndef LOADMODEL_H
#define LOADMODEL_H
#include <string>
#include <vector>
using namespace std;
class loadModel
{
    public:
        vector< vector<float> > vec_vertex;
        //vector for original vertices data
        vector< vector<int> > vec_polygon;
        //vector for polygon order data

        vector< vector<float> > vec_normalized_vertex;
        vector< vector<float> > vec_device_vertex;
        //vector for normalized vertices data
        //only visible vertices is normalized
        vector<int> vec_frontface_index;
        //store visible face index of polygon
        vector< vector<int> > vec_frontface_poly;
        //store visible face polygon data



        loadModel();
        //constructor and set up default parameter

        void LoadData();
        //load geometric data from file to program
        void ComposeMatrix();
        //compute back face and normalized vertices data


        virtual ~loadModel();

        void setDeviceScale(float, float, float, float);
        //set scale to device coordinate Xmin, Ymin, Xmax, Ymax
        void setfileName (string);
        //set geometric file name
        void setCameraPosition(float, float, float);
        //set Camera position in world coordinate
        void setUpVector(float, float, float);
        //set up vector for camera
        void setDistanceNear(float);
        //set near clipping plan =>d
        void setDistanceFar(float);
        //set far clipping plan => far
        void setWidth(float);
        //set h for perspective matrix
        void setShiftLocal(float, float, float);
        //set vector that object shift from local to world
        void setShiftDevice(float, float, float);
        //offset from normalize vertex to device
        void setScaleDevice(float, float, float);
        //Scale normalize vertex to device
        void setProjectRef(float, float, float);
        //set project reference for camera
        float dot_product3D(vector<float> , vector<float> );
        //compute dot product
        void cross_product3D(vector<float> &, vector<float>, vector<float>);
        //compute cross product
        float vector_length(vector<float>);
        //compute vector length
        void vector_subtraction(vector<float> &, vector<float>, vector<float>);
        //compute vector subtraction (vector_2 - vector_1)
        void matrix_multi_4x4(vector< vector<float> >&,
                               vector< vector<float> >,vector< vector<float> >);
        // compute 2 4 x 4 matrices muliplication
        void matrix_multi_4x1(vector <float> &,
                               vector< vector<float> >,vector <float>);
        // compute 4 x 4 and 4 x 1 matrices muliplication

        void view_N(vector<float> &, vector<float>, vector<float>);
        //compute N vector for camera
        void view_V(vector<float> &, vector<float>, vector<float>);
        //compute V vector for camera
        void view_U(vector<float> &, vector<float>, vector<float>);
        //compute U vector for camera
        void matrix_model(vector< vector<float> >&, vector<float>);
        //compute modeling matrix
        void matrix_view(vector< vector<float> >& , vector<float> , vector<float>);
        //compute view transformation Matrix
        void matrix_pers(vector< vector<float> >&  , float , float , float );
        //compute perspective matrix


    private:
        char *fileName;

        vector <float> offset_world;
        vector <float> camera; //camera position in world
        vector <float> up_vector;//up vector for camera
        vector <float> P_ref;//project reference for camera
        vector <float> deviceScale;
        float d;//near clipping plan
        float h;//for perspective matrix
        float far;//far clipping plan
        float Xmin;
        float Ymin;
        float Xmax;
        float Ymax;

        vector< vector<float> > M_model_4D;//Modeling transform Matrix
        vector< vector<float> > M_pers_4D;//Perspective transform Matrix
        vector< vector<float> > M_view_4D;//view transformation Matrix

    protected:

};

#endif // LOADMODEL_H
