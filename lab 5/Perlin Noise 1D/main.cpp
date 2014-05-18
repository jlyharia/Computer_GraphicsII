
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#define PI 3.1415927
using namespace std;
void initRendering()
{
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);//camera view
    glLoadIdentity(); // replace the current matrix with the identity matrix
    //Set the Orthogonal Projection Volume to the

    glOrtho(-256, 256, -256, 256, -256, 256);
    glViewport(0,0,10,20);

}
float Noise1(int x)
{
    int prime_1 = 15731;
    int prime_2 = 789221;
    int prime_3 = 1376312589;
    float prime_4 = 1073741824.0;
    float output;
    x = (x<<13)^x;
    output = (1.0 - ((x * (x * x * prime_1 + prime_2) + prime_3) & 0x7fffffff) / prime_4);
    return output;
}
float SmoothNoise_1(float x)
{
    float output;
    output = Noise1(x)/2 + Noise1(x-1)/4 + Noise1(x+1)/4;
    return output;
}
float Cosine_Interpolate(float left, float right, float x)
{
    float ft = x * PI;
    float f = (1 - cos(ft)) * 0.5;
    float output = left*(1 - f) + right * f;
    return output;

}
float InterpolateNoise(float x)
{
    int integer_X = (int)x;
    float fractional_X = x - integer_X;

    float v1 = SmoothNoise_1(integer_X);
    float v2 = SmoothNoise_1(integer_X + 1);
    float output = Cosine_Interpolate(v1, v2, fractional_X);
    return output;
}
float PerlinNoise_1D(float x)
{
    float Sum = 0;
    float p = 2;//persistence
    float n = 3;//Number of Octaves -1
    float frequency, amplitude;
    for (int i=0; i< n ; i++ )
    {
        frequency = pow(2,i);
        amplitude = pow(p,i);

        Sum = Sum + InterpolateNoise(x * frequency) / amplitude;
    }
    return Sum;
}
void drawScene()
{

//    glColor3b(1,0,0);
//    glScalef(10,10,1);
    glBegin(GL_POINTS);
        for (int i=-100; i< 100 ; i+= 1)
        {
            glVertex2f(i,PerlinNoise_1D(i));
        }
    glEnd();
    ///---------------------------------------------------------

    glutSwapBuffers(); //Send the 3D scene to the screen
}

int main(int argc, char **argv)
{
//    for (int i=0; i< 10 ; i+= 1)
//    {
//        cout<<PerlinNoise_1D(i)<<endl;
//    }
    ///---------------------------------
    glutInit( &argc, argv);
    //Set your desired Windows Size
    glutInitWindowPosition (50, 100);  //set windows position
    glutInitWindowSize(512, 512);
    //OpenGL Display Mode
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    //The title of the Window
    glutCreateWindow("CG I - Assignment 1");
    //Rendering Function
    //-----------------------------------------
    initRendering(); //initialize basic rendering
    glutDisplayFunc(drawScene);//draw function

    //------------------------------------------------
    glutMainLoop();
    return 0;
}
