#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <vector>
using namespace std;
class textureloader
{
    public:
        textureloader(const char*);
        vector< vector< vector<float> > > imagePixel;
        unsigned int width;
        unsigned int height;

        virtual ~textureloader();

    private:
        unsigned char blue;
        unsigned char green;
        unsigned char red;
    protected:
};

#endif // TEXTURELOADER_H
