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
        static TTF_Font* get_font(const char* name, int size);
        void open_font(const char* name, int size);
        int get_font_size();
        TTF_Font* get_font();
    private:
        TTF_Font* font;
        int font_size;
};

OSFont::OSFont() {
    font = NULL;
}

OSFont::OSFont(const char* name, int size)
{
    font_size = size;
    font = OSFont::get_font(name, font_size);
}

OSFont::~OSFont() {}

TTF_Font* OSFont::get_font(const char* name, int size)
{
    TTF_Font* _font = TTF_OpenFont(name, size);
    if (!_font) {
        std::cout << "Error open font TTF_OpenFont()" << std::endl;
        return NULL;
    };
    return _font;
}

void OSFont::open_font(const char* name, int size)
{
    font = OSFont::get_font(name, size);
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
