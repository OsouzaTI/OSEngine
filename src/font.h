#ifndef OSFONT_H
#define OSFONT_H

#include <SDL_ttf.h>

/*
   ____   _____ ______          _
  / __ \ / ____|  ____|        | |
 | |  | | (___ | |__ ___  _ __ | |_
 | |  | |\___ \|  __/ _ \| '_ \| __|
 | |__| |____) | | | (_) | | | | |_
  \____/|_____/|_|  \___/|_| |_|\__|
*/

class OSFont
{
    public:
        OSFont();
        OSFont(const char* name, int size);
        ~OSFont();        
        void open_font(const char* name, int size);
        int get_font_size();
        TTF_Font* get_font();
    private:
        TTF_Font* font;
        int font_size = 10;
};

OSFont::OSFont() {
    font = NULL;
}

OSFont::OSFont(const char* name, int size)
{
    font_size = size;
    font = TTF_OpenFont(name, size);
    if (!font) {
        std::cout << "Error open font TTF_OpenFont()" << std::endl;        
    };    
}

OSFont::~OSFont() {}

void OSFont::open_font(const char* name, int size)
{
    font_size = size;
    font = TTF_OpenFont(name, size);
    if (!font) {
        std::cout << "Error open font TTF_OpenFont()" << std::endl;
    };
}

int OSFont::get_font_size()
{
    return font_size;
}

TTF_Font* OSFont::get_font()
{
    return font;
}


#endif // !OSFONT_H
