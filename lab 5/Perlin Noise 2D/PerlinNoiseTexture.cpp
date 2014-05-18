// PerlinNoiseTexture.cpp
//
// functions used to create Perlin Noise Texture

#include <iostream>
#include <cmath>
#define PI 3.1415927
using namespace std;
float Noise(int x, int y)
{
    int prime_1 = 15731;
    int prime_2 = 789221;
//    int prime_1 = 60493;
//    int prime_2 = 19990303;
    int prime_3 = 1376312589;
    float prime_4 = 1073741824.0;
    float output;
    int n;
    n = x + y *57;
    n = (n<<13)^n;
    output = (1.0 - ((n * (n * n * prime_1 + prime_2) + prime_3) & 0x7fffffff) / prime_4);
    return output;
}
float SmoothNoise(float x, float y)
{
    float corners, sides, center;
    center = Noise(x, y)/4;
    sides = (Noise(x-1, y) + Noise(x+1, y) + Noise(x, y-1) + Noise(x, y+1)) /8;
    corners = (Noise(x-1, y-1) + Noise(x+1, y-1) +
               Noise(x-1, y+1) + Noise(x+1, y+1)) /16;

    return center + sides + corners;
}
float Cosine_Interpolate(float left, float right, float x)
{
    float ft = x * PI;
    float f = (1 - cos(ft))* 0.5;
    float output = left*(1.0 - f) + right * f;
    return output;

}
float InterpolateNoise(float x, float y)
{
    int integer_X = (int)x;
    float fractional_X = x - integer_X;
    int integer_Y = (int)y;
    float fractional_Y = y - integer_Y;

    float v1, v2, v3, v4;
    v1 = SmoothNoise(integer_X,     integer_Y);
    v2 = SmoothNoise(integer_X + 1, integer_Y);
    v3 = SmoothNoise(integer_X,     integer_Y + 1);
    v4 = SmoothNoise(integer_X + 1, integer_Y + 1);

    float i1, i2;
    i1 = Cosine_Interpolate(v1, v2, fractional_X);
    i2 = Cosine_Interpolate(v3, v4, fractional_X);

    return Cosine_Interpolate(i1, i2, fractional_Y);
}
float PerlinNoise_2D(float x, float y)
{   //zoom wel zoom in and out on it, I usually
    float Sum = 0;
    float p = 2;//persistence
    int n = 5;//Number of Octaves -1
    float frequency, amplitude;
    float zoom = pow(2,4);
    for (int i=0; i< n ; i++ )
    {
        frequency = pow(2,i);
        amplitude = pow(p,i);
//        Sum += InterpolateNoise(x * frequency /zoom, y * frequency/zoom)
//                   / amplitude;
        Sum += sin(i + abs(InterpolateNoise(x * frequency /zoom, y * frequency/zoom)
                   / amplitude));
//        Sum += abs(InterpolateNoise(x * frequency /zoom, y * frequency/zoom)
//                   / amplitude);
    }//It calculates all our zoom and frequency and amplitude
    return Sum;
}

