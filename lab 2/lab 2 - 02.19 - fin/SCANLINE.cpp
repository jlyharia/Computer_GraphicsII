/** Tasks in SCANLINE.cpp
    SCANLINE.cpp reads Device vertices data, carrying out scan line
    conversion and Z buffer algorithm.
**/
#include "SCANLINE.h"
#include "TABLE.h"
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <fstream>

SCANLINE::SCANLINE(vector< vector<int> > temp_front_Poly,
                   vector< vector<float> > temp_device_vertex)
{//read polygon data and vertices data from lab 1
    front_Poly = temp_front_Poly;
    //put polygon data to front_Poly
    device_vertex = temp_device_vertex;
    //put vertices data to device vertices
    get_round_vertex(round_vertex);
    //round up x,y value and put in round_vertex

}
void SCANLINE::pixelCompute()
{// main loop for compute each polygon
    srand(int(time(0)));
    vector< vector<float> > Ycompare;//put y value and vertex index in Ycompare
    vector< vector<float> > nearVertex;
    vector< vector<float> > startVertex;
    for(int poly_num = 0; poly_num< (int)front_Poly.size();poly_num++)
    {
        Ycompare.clear();
        nearVertex.clear();
        startVertex.clear();

        //find the y value order in a polygon
        get_Ycompare(Ycompare,poly_num);

        //find near vertices around start vertices
        for(int vert_num=0; vert_num<(int) Ycompare.size();vert_num++)
        {
            find_near_vertex(nearVertex,startVertex,Ycompare,
                             vert_num,poly_num);
        }

        //create Edge table
        EdgeTable(startVertex,nearVertex,
                       front_Poly,poly_num);
    }//end poly_num loop
}
void SCANLINE::ActiveEdgeTable(vector <TABLE> entry, int poly_num)
{//Active Edge Table
   //time seed
    float color_R =(float)rand()/RAND_MAX;
    float color_G =(float)rand()/RAND_MAX;
    float color_B =(float)rand()/RAND_MAX;
//    datatest<<color_R<<'\t'<<color_G<<'\t'<<color_B<<endl;
    vector<float> AP;
    vector< vector<int> > temp_pixel(Ymax, vector<int>(Xmax));
    //default all element= 0
    vector< vector<float> > AET;
    int begin = 0;
    float Z_increOnX;
    for(int y_min = 0; y_min< (int)entry.size(); y_min++) //i -> Y min
    {
        if(entry[y_min].count == 0 && begin==0)
        {
            continue; //no edge in Edge Table
        }
        else //exist edge in Edge Table
        {
            begin=1;

            //(1.1)put edge value from ET to AET
            for(int j = 0; j< entry[y_min].count;j++)
            {
                entry[y_min].getEdgeValue(AP,j);
                AET.push_back(AP);
            }
            //AET are empty
            if(AET.size()==0)
                break;
            //(1.2)maintaining ATE sort order on x
            for(int row =0; row <( int)AET.size() - 1;row++)
                for(int col=0; col <( int)AET.size()-row-1; col++)
                    if(AET[col][1] > AET[col+1][1])
                        swap(AET[col], AET[col+1]);

            //remove dot edge
            if((int)AET.size() %2 !=0)
            {
                for(int j = 0; j< (int)AET.size();j++)
                {//check Y max
                    if(AET[j][0] == y_min)
                    {
                        AET.erase(AET.begin()+j);
                        j--;
                    }
                }
            }
            //(2)Fill in desired pixel values, x and y are integer
            if(ceil(AET[1][1]) == ceil(AET[0][1]))// same X value
                Z_increOnX = 0;
            else//z1 - z2 / x1 - x2
                Z_increOnX = (AET[1][3] - AET[0][3])/
                    (ceil(AET[1][1]) - ceil(AET[0][1]));
            for(int j = 0; j< (int)AET.size();j+=2)
            {//fill pixel

                for(int X_value = ceil(AET[j][1]); X_value<= ceil(AET[j+1][1]);X_value++)
                {
                    real_pixel[y_min][X_value][0]= 1;
                    real_pixel[y_min][X_value][2]= color_R;
                    real_pixel[y_min][X_value][3]= color_G;
                    real_pixel[y_min][X_value][4]= color_B;
                    //Z value
                    if(X_value == ceil(AET[j][1]))
                        real_pixel[y_min][X_value][1] = AET[j][3];
                    else
                        real_pixel[y_min][X_value][1] =
                        real_pixel[y_min][X_value -1][1] + Z_increOnX;
                }
            }
            //(3)Remove from AET entries for which y = ymax
            for(int j = 0; j< (int)AET.size();j++)
            {//check Y max
                if(AET[j][0] == y_min)
                {
                    AET.erase(AET.begin()+j);
                    j--;
                }
            }
            //(4)replace x by x + increment
            //|0 Ymax | 1 X start | 2 X incre
            for(int j = 0; j< (int)AET.size();j++)
            {//increase X value
                AET[j][1] = AET[j][1] + AET[j][2];
                //increase Z value
                AET[j][3] = AET[j][3] + AET[j][4];
            }
        }
    }//end lopp (int)entry.size()

}



void SCANLINE::EdgeTable(vector< vector<float> > startVertex,
                    vector< vector<float> > nearVertex,
                    vector< vector<int> >front_Poly,
                    int poly_num)
{//put edge value in edge table
    vector<float> edgeValue;
    vector <TABLE> entry(Ymax);
    vector<int> repeatCheck;//check if an edge is already in stack
    float X_incre;
    float Z_incre;
    for(int i = 0; i< (int)startVertex.size();i++)
    {
        for(int j = 0; j<2 ;j++)
        {    //i*2 + j
            edgeValue.clear();
//            //test if the edge value is already in stack by compare
//            //sum of index
            if(find(repeatCheck.begin(), repeatCheck.end(),
                    startVertex[i][4] + nearVertex[i*2 + j][4])
                    != repeatCheck.end()) //found repeated edge, skip
                continue;
            else//startVertex[i][4] store vertex index
                repeatCheck.push_back(startVertex[i][4] * nearVertex[i*2 + j][4]);

            //put 1/m
            if((startVertex[i][1] == nearVertex[i*2 + j][1]) &&
                startVertex[i][0] != nearVertex[i*2 + j][0])//Horizontal edge
            {
                continue;
            }
            else if((startVertex[i][1] == nearVertex[i*2 + j][1]) &&
                startVertex[i][0] == nearVertex[i*2 + j][0])
            {//dot  overlapped
                X_incre = 0;
            }
            else
                X_incre = (startVertex[i][0]-nearVertex[i*2 + j][0])/
                        (startVertex[i][1]-nearVertex[i*2 + j][1]);
            //put Y max
            if(startVertex[i][1] >= nearVertex[i*2 + j][1])
            {
                edgeValue.push_back(startVertex[i][1]);

            }
            else
            {
                edgeValue.push_back(nearVertex[i*2 + j][1]);//Ymax

            }
            //put X start
            edgeValue.push_back(startVertex[i][0]);

            edgeValue.push_back(X_incre);
            //compute Z incre
            //Z_incre
            if(startVertex[i][2] == nearVertex[i*2 + j][2]) //Z value are the same
            {
                Z_incre = 0;
            }
            else if(startVertex[i][1] == nearVertex[i*2 + j][1])//Y value are the same
            {
                Z_incre = 0;
            }
            else
            {//(z1 - z2) / (y1 - y2)
                Z_incre = (nearVertex[i*2 + j][2] - startVertex[i][2])/
                            (nearVertex[i*2 + j][1] - startVertex[i][1]);
            }
            //put in Edge table
            entry[startVertex[i][1]].insertValue
            (edgeValue[0] , startVertex[i][0] , X_incre,
             startVertex[i][2],Z_incre);

        }
    }
    //apply active edge table
    ActiveEdgeTable(entry,poly_num);


}

void SCANLINE::get_Ycompare(vector< vector<float> > & Ycompare,
               int poly_num)
{   //Ycompare[index][Y value]
    for(int i=1;i<front_Poly[poly_num][0]+1;i++)
    {
        vector<float> temp;
        temp.push_back(front_Poly[poly_num][i]);//index start from 1
        temp.push_back(round_vertex[front_Poly[poly_num][i]-1][1]);//y
        Ycompare.push_back(temp);
    }
    //sort by y value - buble sort
    for(int i=0;i<(int)Ycompare.size() - 1;i++)
        for(int j=0; j<(int)Ycompare.size()-i-1; j++)
            if(Ycompare[j][1] > Ycompare[j+1][1])
                swap(Ycompare[j],Ycompare[j+1]);

}
void SCANLINE::get_round_vertex(vector< vector<float> > &round_vertex)
{//round up x and y value
    round_vertex = device_vertex;
    for(int i=0;i<(int)round_vertex.size();i++)
        for(int j=0;j<4;j++)
            if(j < 2)
                round_vertex[i][j] = ceil(device_vertex[i][j]);
}

void SCANLINE::setDeviceCoord(int x, int X,int y, int Y)
{//set Device coordinate
    Xmin = x;
    Xmax = X;
    Ymin = y;
    Ymax = Y;
    //initialize real pixel
    typedef vector <float> String1D;
    typedef vector <String1D> String2D;
    typedef vector <String2D> String3D;

    String3D temp(Ymax,String2D(Xmax,String1D(5)));
    real_pixel = temp;
}
void SCANLINE::find_near_vertex(vector< vector<float> > &nearVertex,
                vector< vector<float> > &startVertex,
                vector< vector<float> >Ycompare,
                int Ycom,int poly_num)
{//find the vertices data near by start vertices which is sorted by Y value
    int x =poly_num;
    for(int i = 1; i < (int)front_Poly[x][0]+1; i++)
    {
        if(front_Poly[x][i] == Ycompare[Ycom][0])
        {
            vector< float > temp_0(round_vertex[front_Poly[x][i]-1]);
            temp_0[4] = Ycompare[Ycom][0]-1;//real index
            startVertex.push_back(temp_0); //start vertex ok
            if(i == 1)//begin of polygon index
            {
                vector< float > temp_1(round_vertex[front_Poly[x][front_Poly[x][0]]-1]);
                vector< float > temp_2(round_vertex[front_Poly[x][i+1]-1]);
                temp_1[4] = front_Poly[x][front_Poly[x][0]]-1;
                temp_2[4] = front_Poly[x][i+1]-1;

                nearVertex.push_back(temp_1);
                nearVertex.push_back(temp_2);

            }
            else if(i == front_Poly[x][0])
            {   //end of polygon
                vector< float > temp_1(round_vertex[front_Poly[x][i-1]-1]);
                vector< float > temp_2(round_vertex[front_Poly[x][1]-1]);
                temp_1[4] = front_Poly[x][i-1]-1;
                temp_2[4] = front_Poly[x][1]-1;
                nearVertex.push_back(temp_1);
                nearVertex.push_back(temp_2);
            }
            else
            {   //temp_index = i;//for start vertex
                vector< float > temp_1(round_vertex[front_Poly[x][i-1]-1]);
                vector< float > temp_2(round_vertex[front_Poly[x][i+1]-1]);
                temp_1[4] = front_Poly[x][i-1]-1;
                temp_2[4] = front_Poly[x][i+1]-1;
                nearVertex.push_back(temp_1);
                nearVertex.push_back(temp_2);

            }

            break;
        }

    }//end for


}
SCANLINE::~SCANLINE()
{
    //dtor
}
