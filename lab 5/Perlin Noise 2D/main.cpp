
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "PerlinNoiseTexture.h"
#define PI 3.1415927

using namespace std;
int bound = 512;
int ortho = bound/2;
float perlin[512][512];
void initRendering()
{
    glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);//camera view
    glLoadIdentity(); // replace the current matrix with the identity matrix
    //Set the Orthogonal Projection Volume to the

    glOrtho(-ortho, ortho, -ortho, ortho, -128, 128);
    glViewport(0,0,10,20);

}
void drawScene()
{
    int u,v;
//    float color[3]={0.68, 0.9, 0.9};
//    float color[3]={1, 0.27, 0};
    float color[3]={0.8, 0.52, 0.24};
    glBegin(GL_POINTS);
        for (int i= 0 ; i< bound; i+= 1)
        {
            u = i - ortho;
            for (int j=0; j< bound ; j+= 1)
            {
                v = j - ortho;
                glColor3f(perlin[i][j]*color[0],perlin[i][j]*color[1],
                          perlin[i][j]*color[2]);
                glVertex2f(u,v);
            }
        }
    glEnd();
    ///---------------------------------------------------------

    glutSwapBuffers(); //Send the 3D scene to the screen
}

int main(int argc, char **argv)
{
    for (int i= 0 ; i< bound; i+= 1)
        for (int j=0; j< bound ; j+= 1)
        {
//            perlin[i][j] = abs(PerlinNoise_2D(i,j));
            perlin[i][j] = PerlinNoise_2D(i,j);
//            perlin[i][j] = (PerlinNoise_2D(i,j) +1)/2;
        }

    ///---------------------------------
    glutInit( &argc, argv);
    //Set your desired Windows Size
    glutInitWindowPosition (50, 100);  //set windows position
    glutInitWindowSize(bound, bound);
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
