#ifndef OSGUI_H
#define OSGUI_H

#include <iostream>
#include "drawing.h"
#include "mouse.h"
#include "osmath.hpp"

class OSGui
{
	public:
        OSGui() {};
        ~OSGui() {};

		// Override functions        
		virtual void GUI_draw() {};
        virtual void GUI_poll_events() {};

        inline vect2<int> get_position() { return this->position; };

	    Drawing* draw;
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

        void GUI_init(Drawing* draw, int x, int y);
	    void GUI_draw() override;
	    void GUI_poll_events() override;
    private:
        int size = 10;

};

DraggableCircle::DraggableCircle(){
    this->draw = NULL;
}
DraggableCircle::~DraggableCircle(){}


void DraggableCircle::GUI_init(Drawing* draw, int x, int y)
{
    this->draw = draw;
    this->position = { x, y };
}

void DraggableCircle::GUI_draw()
{
    GUI_poll_events();

    draw->draw_line(position.x, position.y, mouse_coordinates.x, mouse_coordinates.y, C_BLUE);

    // draw circle
    draw->draw_circle(position.x, position.y, size, color);
    // middle point
    draw->draw_circle(position.x, position.y, 2, color);
}

void DraggableCircle::GUI_poll_events()
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

        void GUI_init(Drawing* draw, int x, int y, int size);
        void GUI_init(Drawing* draw, int min_value, int max_value, int x, int y, int size);
        void GUI_draw() override;
        void GUI_poll_events() override;

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

void SliderRange::GUI_init(Drawing* draw, int x, int y, int width)
{
    this->draw = draw;
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

void SliderRange::GUI_init(Drawing* draw, int min_value, int max_value, int x, int y, int size)
{
    this->draw = draw;
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

void SliderRange::GUI_draw()
{
    GUI_poll_events();
    // background slider
    draw->draw_fill_rect(position.x , position.y , width , height , background_color);
    // dragabble
    draw->draw_fill_rect(
        draggable.x,
        draggable.y,
        draggable_width,
        draggable_height,
        current_draggable_color
    );

    // border slider
    draw->draw_rect(position.x, position.y, width, height, border, border_color);

}

void SliderRange::GUI_poll_events()
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


#endif // !OSGUI_H
