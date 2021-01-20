#ifndef OSGUI_H
#define OSGUI_H

#include <iostream>
#include <vector>
#include "drawing.h"
#include "mouse.h"
#include "osmath.hpp"
#include "os_engine.hpp"

#define logerr(x) std::cerr <<"[ERRO]: "<< x << std::endl
#define logstd(x) std::cout <<"[LOG]: "<< x << std::endl

class OSGui
{
	public:
        OSGui() {};
        ~OSGui() {};

		// Override functions        
		virtual void draw() {};
        virtual void poll_events() {};

        inline vect2<int> get_position() { return this->position; };

        static Drawing* guidraw;
        OS_MOUSE_STATE mouse;
        SDL_Event gui_events;
        vect2<int> mouse_coordinates;
        vect2<int> position;
        uint32_t color = C_RED;
        uint32_t border_color = C_TYRIAN_BLUE;
        uint32_t background_color = C_MIDNIGHT;
        uint32_t draggable_color = C_MEXICAN_PINK;
        uint32_t hover_color = C_WHITE;
        uint32_t clicked_color = C_JAZZBERRY_JAM;

        // controll colors for draggables
        uint32_t current_draggable_color = draggable_color;


	private:
};

Drawing* OSGui::guidraw = NULL;

/*
  _____                              _     _         _____ _          _
 |  __ \                            | |   | |       / ____(_)        | |
 | |  | |_ __ __ _  __ _  __ _  __ _| |__ | | ___  | |     _ _ __ ___| | ___
 | |  | | '__/ _` |/ _` |/ _` |/ _` | '_ \| |/ _ \ | |    | | '__/ __| |/ _ \
 | |__| | | | (_| | (_| | (_| | (_| | |_) | |  __/ | |____| | | | (__| |  __/
 |_____/|_|  \__,_|\__, |\__, |\__,_|_.__/|_|\___|  \_____|_|_|  \___|_|\___|
                    __/ | __/ |
                   |___/ |___/
*/

class DraggableCircle : public OSGui
{
    public:	    
	    DraggableCircle();
	    ~DraggableCircle();

        void GUI_init(int x, int y);
	    void draw() override;
	    void poll_events() override;
    private:
        int size = 10;

};

DraggableCircle::DraggableCircle(){}
DraggableCircle::~DraggableCircle(){}


void DraggableCircle::GUI_init(int x, int y)
{
    
    this->position = { x, y };
}

void DraggableCircle::draw()
{
    poll_events();

    guidraw->draw_line(position.x, position.y, mouse_coordinates.x, mouse_coordinates.y, C_BLUE);

    // draw circle
    guidraw->draw_circle(position.x, position.y, size, color);
    // middle point
    guidraw->draw_circle(position.x, position.y, 2, color);
}

void DraggableCircle::poll_events()
{
    SDL_GetMouseState(&mouse_coordinates.x, &mouse_coordinates.y);

    // distance to mouse
    float dist = distance_to_point<int>(mouse_coordinates, position);

    if (dist <= size + 1)
    {
        SDL_PollEvent(&gui_events);
        switch (gui_events.type)
        {
            case OS_MOUSE_TYPE::BUTTON_DOWN: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {
                    std::cout << "DOWN" << std::endl;
                    mouse = OS_MOUSE_STATE::MOUSE_DOWN;
                }
            }
            break;
            case OS_MOUSE_TYPE::BUTTON_UP: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {
                    std::cout << "UP" << std::endl;
                    mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
                }
            }
            break;
            case OS_MOUSE_TYPE::MOTION: {
                if (mouse == OS_MOUSE_STATE::MOUSE_DOWN) {
                    position.x = mouse_coordinates.x;
                    position.y = mouse_coordinates.y;
                }
            }
            break;
            default: {
                mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
            }
            break;
        }
    }
    else {
        mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
    }
}

/*
   _____ _ _     _             _____
  / ____| (_)   | |           |  __ \
 | (___ | |_  __| | ___ _ __  | |__) |__ _ _ __   __ _  ___
  \___ \| | |/ _` |/ _ | '__| |  _  // _` | '_ \ / _` |/ _ \
  ____) | | | (_| |  __| |    | | \ | (_| | | | | (_| |  __/
 |_____/|_|_|\__,_|\___|_|    |_|  \_\__,_|_| |_|\__, |\___|
                                                  __/ |
                                                 |___/
*/

class SliderRange : public OSGui
{
    public:
        SliderRange();
        ~SliderRange();

        int get_value();

        void GUI_init(int x, int y, int size);
        void GUI_init(int min_value, int max_value, int x, int y, int size);
        void draw() override;
        void poll_events() override;

    private:
        int value = 0;
        int min_value = 0;
        int max_value = 100;
        int draggable_off_size;
        int width;
        int height;
        int draggable_width;
        int draggable_height;
        int border = 2;
        vect2<int> draggable;


};

SliderRange::SliderRange(){}
SliderRange::~SliderRange(){}

int SliderRange::get_value()
{
    return this->value;
}

void SliderRange::GUI_init(int x, int y, int width)
{
    this->position = { x, y };
    this->width = width;
    this->height = width * 0.2;
    this->draggable_width = width * 0.1;
    this->draggable_height = height - border;
    this->draggable_off_size = (draggable_width / 2 - 1);
    this->draggable = {
        position.x + border,
        position.y + border,
    };
}

void SliderRange::GUI_init(int min_value, int max_value, int x, int y, int size)
{
    this->min_value = min_value;
    this->max_value = max_value;
    this->position = { x, y };
    this->width = width;
    this->height = width * 0.2;
    this->draggable_width = width * 0.1;
    this->draggable_height = height - border;
    this->draggable_off_size = (draggable_width / 2 - 1);
    this->draggable = {
        position.x + border,
        position.y + border,
    };
    // Define value with min_value
    this->value = min_value;
}

void SliderRange::draw()
{
    poll_events();
    // background slider
    guidraw->draw_fill_rect(position.x , position.y , width , height , background_color);
    // dragabble
    guidraw->draw_fill_rect(
        draggable.x,
        draggable.y,
        draggable_width,
        draggable_height,
        current_draggable_color
    );

    // border slider
    guidraw->draw_rect(position.x, position.y, width, height, border, border_color);

}

void SliderRange::poll_events()
{
    SDL_GetMouseState(&mouse_coordinates.x, &mouse_coordinates.y);
    // distance to mouse
    float dist = distance_to_point<int>(mouse_coordinates, position);
    bool condicion = mouse_coordinates.x >= position.x &&
        mouse_coordinates.x < (position.x + width)&&
        mouse_coordinates.y >= position.y &&
        mouse_coordinates.y < (position.y + height);

    bool draggable_condicion = mouse_coordinates.x >= draggable.x &&
        mouse_coordinates.x <  (draggable.x + draggable_width)&&
        mouse_coordinates.y >= draggable.y &&
        mouse_coordinates.y < (draggable.y + draggable_height);

    bool mouse_condicion = mouse_coordinates.x <= ((position.x + width) - draggable_off_size) - border
        && mouse_coordinates.x >= (position.x + draggable_off_size) + border;

    if (condicion && draggable_condicion)
    {

        // hover color
        current_draggable_color = hover_color;

        SDL_PollEvent(&gui_events);
        switch (gui_events.type)
        {
            case OS_MOUSE_TYPE::BUTTON_DOWN: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {
                    //std::cout << "DOWN" << std::endl;
                    mouse = OS_MOUSE_STATE::MOUSE_DOWN;
                }
            }
            break;
            case OS_MOUSE_TYPE::BUTTON_UP: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {
                    //std::cout << "UP" << std::endl;
                    mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
                }
            }
            break;
            case OS_MOUSE_TYPE::MOTION: {
                if (mouse == OS_MOUSE_STATE::MOUSE_DOWN) { 
                    int direction = 0;
                    if (gui_events.motion.xrel > 0) {
                        direction = draggable_width/2 + 1;
                    }
                    if (gui_events.motion.xrel < 0)  {
                        direction = draggable_width/2 - 1;
                    }
                
                    if (mouse_condicion) {
                        // clicked color
                        current_draggable_color = clicked_color;

                        int range_min_x = (position.x + draggable_width) - border;
                        int range_max_x = ((position.x + width) - draggable_width) - border;
                        int _value = draggable.x;
                        value = static_cast<int>(Math::map(_value, range_min_x, range_max_x, min_value, max_value));
                        value = Math::max(value, 0);
                        std::cout << value << std::endl;

                        // flicker draggable fix
                        if(direction != 0)
                            draggable.x = mouse_coordinates.x - direction;                    
                    }

                    //position.y = mouse_coordinates.y;
                }
            }
            break;
            default: {
                mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
            }
            break;
        }
    }
    else {
        mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
        current_draggable_color = draggable_color;
    }
   
}

/*
  _______        _   _           _          _
 |__   __|      | | | |         | |        | |
    | | _____  _| |_| |     __ _| |__   ___| |
    | |/ _ \ \/ / __| |    / _` | '_ \ / _ \ |
    | |  __/>  <| |_| |___| (_| | |_) |  __/ |
    |_|\___/_/\_\\__|______\__,_|_.__/ \___|_|
*/

class TextLabel : public OSGui
{
public:
    TextLabel();
    ~TextLabel();

    void set_position(int x, int y);
    void set_text(const char* text);

    void GUI_init(const char* text, int x, int y, int size);
    void draw() override;
    // void poll_events() override;
private:
    int size;
    char text[100];
    TTF_Font* Sans;
    SDL_Surface* surface_label;
    SDL_Texture* message;

    Display* display;
    SDL_Renderer* renderer;
};

TextLabel::TextLabel(){}
TextLabel::~TextLabel(){}

void TextLabel::set_position(int x, int y)
{
    this->position = { x, y };
}

void TextLabel::set_text(const char* text)
{
    strcpy(this->text, text);
}

void TextLabel::GUI_init(const char* text, int x, int y, int size)
{
    strcpy(this->text, text);    
    this->position = {x, y};
    this->size = size;
    this->Sans = TTF_OpenFont("F:\\Projects\\cpp\\OSEngine\\fonts\\aAbsoluteEmpire.ttf", this->size);
    if (!this->Sans) {
        logerr("Error open font TTF_OpenFont()");
    }

    this->display = guidraw->get_display();
    this->renderer = this->display->get_renderer();
    this->surface_label = NULL;
    this->message = NULL;
}

void TextLabel::draw(){
    
    surface_label = TTF_RenderText_Blended(this->Sans, this->text, CS_WHITE);
    message = SDL_CreateTextureFromSurface(renderer, surface_label);

    SDL_Rect message_rect;
    message_rect.x = this->position.x;
    message_rect.y = this->position.y;
    message_rect.w = (strlen(this->text) * (size*0.5));
    message_rect.h = 16;
    if (SDL_RenderCopy(renderer, message, NULL, &message_rect) == -1) {
        logerr("Erro mano");
    }
    SDL_FreeSurface(surface_label);
    SDL_DestroyTexture(message);
}

/*
                          _
     /\                  | |
    /  \   _ __ ___  __ _| |     ___   __ _  __ _  ___ _ __
   / /\ \ | '__/ _ \/ _` | |    / _ \ / _` |/ _` |/ _ \ '__|
  / ____ \| | |  __/ (_| | |___| (_) | (_| | (_| |  __/ |
 /_/    \_\_|  \___|\__,_|______\___/ \__, |\__, |\___|_|
                                       __/ | __/ |
                                      |___/ |___/
*/

class AreaLogger : public OSGui
{
    public:
        AreaLogger();
        ~AreaLogger();

        void add_log(const char* text);
        void add_log(std::string text);
        void lock_view_end(bool end);
        void GUI_init(int x, int y, int width, int height);
        void draw() override;
        void poll_events() override;
        static Uint32 timer(Uint32 interval, void* param);
    private:
        bool end_logger = false;
        int width;
        int height;
        int position_scroll_x = 1;
        int position_scroll_y = 1;
        int items_margin_y = 16;
        int items_count;
        int padding = 2;
        int spacing;
        int border_size = 2;

        void auto_clear();

        std::vector<TextLabel> logs;
};

AreaLogger::AreaLogger(){}
AreaLogger::~AreaLogger(){}

void AreaLogger::add_log(const char* text)
{
    TextLabel label;
    label.GUI_init(text, this->position.x + this->spacing, this->position.y + this->spacing, 16);
    this->logs.push_back(label);
}

void AreaLogger::add_log(std::string text)
{
    TextLabel label;
    label.GUI_init(text.c_str(), this->position.x + this->spacing, this->position.y + this->spacing, 16);
    this->logs.push_back(label);
}

void AreaLogger::lock_view_end(bool end)
{
    this->end_logger = end;
}

void AreaLogger::GUI_init(int x, int y, int width, int height)
{
    this->position = { x, y };
    this->width = width;
    this->height = height;
    this->spacing = (this->padding + border_size);
    // start the timer func at (1second)
    //SDL_TimerID my_timer_id = SDL_AddTimer(200, timer, this);
}

void AreaLogger::draw()
{
    auto_clear();
    poll_events();
    guidraw->draw_fill_rect(this->position.x, this->position.y, this->width, this->height, this->background_color);
    guidraw->draw_rect(this->position.x, this->position.y, this->width, this->height, this->border_size, this->border_color);
    
    items_count = static_cast<int>(this->height / this->items_margin_y);
    
    std::vector<TextLabel*> _logs;

    int _lim_min, _lim_max;    

    if(end_logger){
        if (this->logs.size() >= items_count) {
            position_scroll_y = (logs.size() - items_count) + 1;
        }        
    }        
    std::cout << position_scroll_y << std::endl;
    _lim_min = position_scroll_y;
    _lim_max = position_scroll_y + items_count;    

    for (int j = _lim_min; j < _lim_max; j++)
    {
        if (j > 0 && j <= logs.size()) {
            _logs.push_back(&this->logs[j-1]);
        }
        else { break; }
    }

    int i = 0;
    for (TextLabel* label : _logs) {                        
        label->position.y = this->position.y + (i * this->items_margin_y + this->spacing);        
        label->draw();        
        i++;
    }

    _logs.clear();
}

void AreaLogger::poll_events()
{
    
    SDL_GetMouseState(&mouse_coordinates.x, &mouse_coordinates.y);
    bool mouse_condicion = this->mouse_coordinates.x >= this->position.x &&
        this->mouse_coordinates.x < (this->position.x + this->width) &&
        this->mouse_coordinates.y >= this->position.y &&
        this->mouse_coordinates.y < (this->position.y + this->height);

    if (mouse_condicion) {        
        SDL_PollEvent(&gui_events);
        switch (gui_events.type)
        {
            case OS_MOUSE_TYPE::MOUSE_SCROLL: {                

                if (gui_events.wheel.y > 0) // scroll up
                {
                    logstd("UP");
                    if (this->position_scroll_y > 1) this->position_scroll_y--;                
                }
                else if (gui_events.wheel.y < 0) // scroll down
                {
                    logstd("DOWN");
                    if (this->position_scroll_y < this->logs.size()) this->position_scroll_y++;
                }
                gui_events.type = NULL;                
       
            }            
            break;
            default: {                
                mouse = OS_MOUSE_STATE::MOUSE_RELEASE;                                
            }
        }
    }
}

Uint32 AreaLogger::timer(Uint32 interval, void* param)
{
    AreaLogger* my_logger = (AreaLogger*)param;   
    if (my_logger->mouse != OS_MOUSE_STATE::MOUSE_RELEASE) {
        my_logger->mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
    }
    logstd("THREAD LOGGER");
    return(interval);
}

void AreaLogger::auto_clear()
{
    if (logs.size() > this->items_count * 10) {
        std::vector<TextLabel> _temp;
        for (int i = logs.size() / 2; i < logs.size(); i++)
        {
            _temp.push_back(logs[i]);
        }
        logs.clear();
        logs = _temp;
    }    
}


#endif // !OSGUI_H
