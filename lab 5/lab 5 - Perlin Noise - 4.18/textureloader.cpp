#include "textureloader.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

textureloader::textureloader(const char* filename)
{
    FILE *image;
	image = fopen(filename,"rb+");
    if(!filename)
    {
        cerr << "Texture could not be opened" <<endl;
        exit(1); //#include <cstdlib>
    }
	fseek(image,18,SEEK_SET);
	fread(&width,4,1,image); //get width pixel number
	fseek(image,22,SEEK_SET);
	fread(&height,4,1,image);//get height pixel number

    typedef vector <float> String1D;
    typedef vector <String1D> String2D;
    typedef vector <String2D> String3D;
    String3D temp_pixels(width,String2D(height,String1D(3)));//bitmap

	fseek(image,54,SEEK_SET); //move to Pixel data

    for (int i = 0;i< (int)width ; i++ )
    {
        for (int j = 0;j < (int) height ; j++ )
        {

            fread(&blue,sizeof(blue),1,image);
            fread(&green,sizeof(green),1,image);
            fread(&red,sizeof(red),1,image);

            temp_pixels[j][i][0] = (float)red /255;
            temp_pixels[j][i][1] = (float)green /255;
            temp_pixels[j][i][2] = (float)blue /255;
        }
    }
    imagePixel = temp_pixels;
	fclose(image);
}

textureloader::~textureloader()
{
    //dtor
}
