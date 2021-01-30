#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <upng.h>

class OSTexture
{
    public:
        OSTexture();
        ~OSTexture();
        upng_t* png_texture;
        uint32_t* mesh_texture;
        int texture_width;
        int texture_height;
        void load_png_texture(const char* filename);
    private:

};



#endif