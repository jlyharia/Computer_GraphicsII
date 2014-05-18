/**
    Computer Graphics II
    Lab 5   : Procedural Texture Mapping - Perlin Noise
    Student : YiHung Lee
    GW id   : G42957795
**/

#include "loadModel.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include "TABLE.h"
#include "SCANLINE.h"
using namespace std;
float Xmin = 0;
float Ymin = 0;
float Xmax = 640;
float Ymax = 480;
vector< vector< vector<float> > > real_pixel;

void handleKeypress(unsigned char key, //The key that was pressed
					int x, int y) {    //The current mouse coordinates
	switch (key) {
		case 27: //Escape key
			exit(0); //Exit the program
	}
}
void initRendering ()
{
    // 2 specify which matrix is the current matrix
    glMatrixMode (GL_PROJECTION);
    //Reset the drawing perspective
    glLoadIdentity();
    // 4 set Cartesian coordinate
    //(0,0) at bottom-left corner
    //gluOrtho2D - set up any two-dimensional Cartesian reference frame
    gluOrtho2D(Xmin, Xmax, Ymin, Ymax);

}

void drawFunction()
{
    //-------------------draw ------------------
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    for(int i = 0 ; i < Xmax ; i++)
    {// front_Poly.size() is the number of polygon which face camera
        for(int j = 0 ; j < Ymax  ; j++)
        {//draw line from one vertex to next vertex
            glBegin(GL_POINTS);
                if(real_pixel[j][i][0] == 1)//if there is a pixel
                {   //set pixel color
                    glColor3f(real_pixel[j][i][2],real_pixel[j][i][3],
                              real_pixel[j][i][4]);
                    glVertex2i(i,j);
                }
            glEnd();
        }
    }
    glutSwapBuffers();

}
int main(int argc, char** argv)
{
    loadModel model_1;
//    model_1.setfileName("cow.d");
//    model_1.setShiftLocal(-2,1,-5);

    model_1.setfileName("better-ball.d");
    model_1.setShiftLocal(0,0,-1.3);
//    model_1.setfileName("atc.d");
//    model_1.setShiftLocal(1,-1,-3);

    model_1.setDeviceScale(Xmin, Xmax, Ymin, Ymax);

    model_1.LoadData();
    model_1.ComposeMatrix();

    SCANLINE scan_1(model_1.vec_frontface_poly, model_1.vec_polygon,
                    model_1.vec_device_vertex,model_1.vec_world_vertex);
    //delcare scan line 1
    scan_1.setDeviceCoord(Xmin, Xmax, Ymin, Ymax);
    scan_1.pixelCompute();
    real_pixel = scan_1.real_pixel;


    /***********************************************************/
    //----------------------------step 1 begin(set windows)--------------------
	glutInit (&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowPosition (50, 100);  //set windows position
    glutInitWindowSize (640, 480);		//set windows size
    glutCreateWindow ("CG 2 Lab 5 - Perlin Noise"); 	//create windows
	//----------------------------step 1 end-----------------------------------

	//----------------------------step 2 begin(set OpenGL function)------------
    initRendering();
    glutDisplayFunc (drawFunction);
    glutKeyboardFunc(handleKeypress); //control operation(handle key press)
    //----------------------------step 2 end--------------------
    //----------------------------step 3 begin(Start the main loop)-----

    glutMainLoop(); //  glutMainLoop doesn't return.
    //----------------------------step 3 end----------------------

    return 0; //This line is never reached
}
