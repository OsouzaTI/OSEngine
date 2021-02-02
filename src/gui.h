#ifndef OSGUI_H
#define OSGUI_H

#include <iostream>
#include <ctype.h>
#include <vector>
#include "drawing.h"
#include "mouse.h"
#include "keyboard.h"
#include "cmath.h"
#include "color.h"
#include "font.h"

/*
   ____   _____  _____       _
  / __ \ / ____|/ ____|     (_)
 | |  | | (___ | |  __ _   _ _
 | |  | |\___ \| | |_ | | | | |
 | |__| |____) | |__| | |_| | |
  \____/|_____/ \_____|\__,_|_|

*/

class OSGui
{
	public:

		// Override functions        
		virtual void draw() {};
        virtual void poll_events() {};
        vect2<int> get_position();
        vect2<int> position;


        static Drawing* guidraw;
        static OSFont* global_font;
        static OSPallet default_pallet;

	private:

    protected:
        OSGui() {};
        ~OSGui() {};
        OS_MOUSE_STATE mouse;
        SDL_Event gui_events;
        vect2<int> mouse_coordinates;
        // Style of GUI
        SDL_Color text_color = default_pallet.text_color;
        uint32_t color = default_pallet.color5;
        uint32_t border_color = C_WHITE;
        uint32_t background_color = default_pallet.color3;
        uint32_t draggable_color = default_pallet.color2;
        uint32_t hover_color = default_pallet.color3;
        uint32_t clicked_color = C_JAZZBERRY_JAM;

        // controll colors for draggables
        uint32_t current_draggable_color = draggable_color;
        uint32_t current_background_color = background_color;


};

Drawing* OSGui::guidraw = NULL;
OSPallet OSGui::default_pallet;
OSFont* OSGui::global_font = NULL;

vect2<int> OSGui::get_position() {
    return position;
}
// Singleton
class OSGuiController
{
    public:
        template<typename T, typename... TArgs>
        T& add(TArgs&&... args);
        //void update();
        void draw();
        static OSGuiController* get_instance();
    protected:
        OSGuiController();
        ~OSGuiController();
    private:
        static OSGuiController* constroller;
        std::vector<OSGui*> GUI;
};

OSGuiController* OSGuiController::constroller = NULL;
OSGuiController::OSGuiController(){
    LOG("Init the class FrustumPlane");
}
OSGuiController::~OSGuiController(){
    GUI.clear();
}

template<typename T, typename ...TArgs>
T& OSGuiController::add(TArgs&& ...args)
{
    T* newGui(new T(std::forward<TArgs>(args)...));
    GUI.emplace_back(newGui);
    return *newGui;
}


void OSGuiController::draw()
{
    for (auto& gui : GUI) {        
        gui->draw();
    }
}

OSGuiController* OSGuiController::get_instance()
{
    if (OSGuiController::constroller == NULL) {
        LOG("OSGuiController created");
        OSGuiController::constroller = new OSGuiController();
    }
    return OSGuiController::constroller;
}


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
	    DraggableCircle(int x, int y);
	    ~DraggableCircle();

        void GUI_init(int x, int y);
	    void draw() override;
	    void poll_events() override;
    private:
        int size = 10;
        int offsize = 1;

};

DraggableCircle::DraggableCircle(){}
DraggableCircle::DraggableCircle(int x, int y)
{
    position = { x, y };
}
DraggableCircle::~DraggableCircle(){}


void DraggableCircle::GUI_init(int x, int y)
{
    
    position = { x, y };
}

void DraggableCircle::draw()
{
    poll_events();

    //guidraw->draw_line(position.x, position.y, mouse_coordinates.x, mouse_coordinates.y, C_BLUE);

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

    if (mouse == OS_MOUSE_STATE::MOUSE_DOWN) { offsize = size; }
    else { offsize = 1; };

    if (dist <= size + offsize)
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
        SliderRange(int x, int y, int width);
        ~SliderRange();

        int get_value();

        void GUI_init(int x, int y, int width);
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
SliderRange::SliderRange(int x, int y, int width)
{
    GUI_init(x, y, width);
}
SliderRange::~SliderRange(){}

int SliderRange::get_value()
{
    return value;
}

void SliderRange::GUI_init(int x, int y, int width)
{
    position = { x, y };
    this->width = width;
    height = width * 0.2;
    draggable_width = width * 0.1;
    draggable_height = height - border;
    draggable_off_size = (draggable_width / 2 - 1);
    draggable = {
        position.x + border,
        position.y + border,
    };
}

void SliderRange::GUI_init(int min_value, int max_value, int x, int y, int size)
{
    this->min_value = min_value;
    this->max_value = max_value;
    position = { x, y };
    this->width = width;
    this->height = width * 0.2;
    draggable_width = width * 0.1;
    draggable_height = height - border;
    draggable_off_size = (draggable_width / 2 - 1);
    draggable = {
        position.x + border,
        position.y + border,
    };
    // Define value with min_value
    value = this->min_value;
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
    TextLabel(const char* text, int x, int y);
    ~TextLabel();

    void set_position(int x, int y);
    void set_text(const char* text);    
    void set_text_color(SDL_Color text_color);
    int get_total_width();

    void GUI_init(const char* text, int x, int y);
    void draw() override;
    // void poll_events() override;
    void free_resources();
private:
    int size;
    char text[100];
    int total_width;
    SDL_Surface* surface_label;
    SDL_Texture* message;
    SDL_Rect message_rect;
    Display* display;
    SDL_Renderer* renderer;
};

TextLabel::TextLabel(){
    surface_label = NULL;
    message = NULL;
}

TextLabel::TextLabel(const char* text, int x, int y)
{
    GUI_init(text, x, y);
}

TextLabel::~TextLabel(){
    
}

void TextLabel::set_position(int x, int y)
{
    position = { x, y };
}

void TextLabel::set_text(const char* text)
{
    strcpy(this->text, text);
}

void TextLabel::set_text_color(SDL_Color text_color)
{
    this->text_color = text_color;
}

int TextLabel::get_total_width()
{
    return total_width;
}

void TextLabel::GUI_init(const char* text, int x, int y)
{
    strcpy(this->text, text);    
    position = {x, y};
    this->size = size;
    display = guidraw->get_display();
    renderer = display->get_renderer();
    surface_label = NULL;
    message = NULL;

    total_width = (strlen(text) * (OSGui::global_font->get_font_size() * 0.6));
    message_rect.x = position.x;
    message_rect.y = position.y;
    message_rect.w = total_width;
    message_rect.h = OSGui::global_font->get_font_size() * 1.5;

}

void TextLabel::draw(){
    
    surface_label = TTF_RenderText_Blended(OSGui::global_font->get_font(), text, text_color);
    message = SDL_CreateTextureFromSurface(renderer, surface_label);

    total_width = (strlen(text) * (OSGui::global_font->get_font_size() * 0.6));
    message_rect.x = position.x;
    message_rect.y = position.y;
    message_rect.w = total_width;
    message_rect.h = OSGui::global_font->get_font_size()*1.5;

    if (SDL_RenderCopy(renderer, message, NULL, &message_rect) == -1) {
        //logerr("Erro mano");
    }
    
    SDL_FreeSurface(surface_label);
    SDL_DestroyTexture(message);     
}

void TextLabel::free_resources()
{
    
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
        AreaLogger(int x, int y, int width, int height);
        ~AreaLogger();

        void add_log(const char* text);
        void add_log(std::string text);
        void lock_view_end(bool end);
        void GUI_init(int x, int y, int width, int height);
        void GUI_init(int x, int y, int width, int height, OSFont otherfont);
        void draw() override;
        void poll_events() override;
        static Uint32 timer(Uint32 interval, void* param);
        std::vector<TextLabel> logs;
    private:
        OSFont font;
        bool end_logger = false;
        int width = 0;
        int height = 0;
        int position_scroll_x = 1;
        int position_scroll_y = 1;
        int items_margin_y = 16;
        int items_count;
        int padding = 2;
        int spacing = 0;
        int border_size = 2;
        void auto_clear();

};

AreaLogger::AreaLogger(){}
AreaLogger::AreaLogger(int x, int y, int width, int height)
{
    GUI_init(x, y, width, height);
}
AreaLogger::~AreaLogger(){}

void AreaLogger::add_log(const char* text)
{
    TextLabel label = TextLabel();
    label.GUI_init(text, position.x + spacing, position.y + spacing);
    logs.push_back(label);   
}

void AreaLogger::add_log(std::string text)
{
    TextLabel label;
    label.GUI_init(text.c_str(), position.x + spacing, position.y + spacing);
    logs.push_back(label);     
}

void AreaLogger::lock_view_end(bool end)
{
    end_logger = end;
}

void AreaLogger::GUI_init(int x, int y, int width, int height)
{
    position = { x, y };
    this->width = width;
    this->height = height;
    spacing = (padding + border_size);
    // start the timer func at (1second)
    //SDL_TimerID my_timer_id = SDL_AddTimer(200, timer, this);
}

void AreaLogger::GUI_init(int x, int y, int width, int height, OSFont font)
{
    position = { x, y };
    this->width = width;
    this->height = height;
    spacing = (padding + border_size);
    this->font = font;

    // start the timer func at (1second)
    //SDL_TimerID my_timer_id = SDL_AddTimer(200, timer, this);
}

void AreaLogger::draw()
{
    auto_clear();
    poll_events();
    guidraw->draw_fill_rect(position.x, position.y, width, height, background_color);
    guidraw->draw_rect(position.x, position.y, width, height, border_size, border_color);
    
    items_count = static_cast<int>(height / items_margin_y);
    
    int _lim_min, _lim_max;    

    if(end_logger){
        if (logs.size() >= items_count) {
            position_scroll_y = (logs.size() - items_count) + 1;
        }        
    }        
    
    _lim_min = position_scroll_y;
    _lim_max = position_scroll_y + items_count;    
   
    std::vector<TextLabel*> _logs;

    for (int j = _lim_min; j < _lim_max; j++)
    {
        if (j > 0 && j <= logs.size()) {
            _logs.push_back(&logs[j-1]);
        }
        else { break; }
    }

    int i = 0;
    for (TextLabel* label : _logs) {                        
        label->position.y = position.y + (i * items_margin_y + spacing);        
        label->draw();        
        i++;
    }

    _logs.clear();

}

void AreaLogger::poll_events()
{
    
    SDL_GetMouseState(&mouse_coordinates.x, &mouse_coordinates.y);
    bool mouse_condicion = mouse_coordinates.x >= position.x &&
        mouse_coordinates.x < (position.x + width) &&
        mouse_coordinates.y >= position.y &&
        mouse_coordinates.y < (position.y + height);

    if (mouse_condicion) {        
        SDL_PollEvent(&gui_events);
        switch (gui_events.type)
        {
            case OS_MOUSE_TYPE::MOUSE_SCROLL: {                

                if (gui_events.wheel.y > 0) // scroll up
                {
                    LOG("UP");
                    if (position_scroll_y > 1) position_scroll_y--;                
                }
                else if (gui_events.wheel.y < 0) // scroll down
                {
                    LOG("DOWN");
                    if (position_scroll_y < logs.size()) position_scroll_y++;
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
// não está sendo usado
Uint32 AreaLogger::timer(Uint32 interval, void* param)
{
    AreaLogger* my_logger = (AreaLogger*)param;   
    if (my_logger->mouse != OS_MOUSE_STATE::MOUSE_RELEASE) {
        my_logger->mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
    }
    LOG("THREAD LOGGER");
    return(interval);
}

void AreaLogger::auto_clear()
{
    if (logs.size() > items_count * 100) {
        std::vector<TextLabel> _temp;
        for (int i = logs.size() / 2; i < logs.size(); i++)
        {
            _temp.push_back(logs[i]);
        }     
        logs.clear();        
        logs = _temp;
    }    
}

/*
  _______        _   _____                   _
 |__   __|      | | |_   _|                 | |
    | | _____  _| |_  | |  _ __  _ __  _   _| |_
    | |/ _ \ \/ / __| | | | '_ \| '_ \| | | | __|
    | |  __/>  <| |_ _| |_| | | | |_) | |_| | |_
    |_|\___/_/\_\\__|_____|_| |_| .__/ \__,_|\__|
                                | |
                                |_|
*/

class TextInput : public OSGui
{
    public:
        TextInput();
        TextInput(const char* text, int x, int y);
        ~TextInput();
        
        void set_numerical_value(char controller);
        void set_value(const char* value);
        int get_total_width();
        void* get_numerical_value();
        void GUI_init(const char* text, int x, int y);
        void draw() override;
        void poll_events() override;

    private:
        bool field_focus;
        bool max_lenght;
        int font_size = 16;
        int offsize_x;
        int offsize_y;
        int field_padding_x = 2;
        int field_padding_y = 2;
        int start_field_x;
        int start_field_y;
        int field_width = 120;
        int field_height = 25;
        int total_width;
        int lim_characteres = 6;
        char text[100];
        std::string value;
        char numerical_controller = 'f';
        void* numerical_value;
        TextLabel label;
        TextLabel field;        

    protected:
        int inumerical_value = 1;
        float fnumerical_value = 1.0f;
};

TextInput::TextInput(){
    numerical_value = NULL;
}
TextInput::TextInput(const char* text, int x, int y)
{
    GUI_init(text, x, y);
}
TextInput::~TextInput(){}

void TextInput::set_numerical_value(char controller)
{
    switch (controller)
    {
        case 'i':
            inumerical_value = atoi(value.c_str());
            numerical_value = &inumerical_value;
            break;
        case 'f':
            fnumerical_value = atof(value.c_str());
            numerical_value = &fnumerical_value;
            break;
        default:
            break;
    }
}

void TextInput::set_value(const char* value)
{
    this->value = std::string(value);
}

int TextInput::get_total_width()
{
    return total_width;
}

void* TextInput::get_numerical_value()
{
    if (numerical_value == NULL) {        
        numerical_value = &fnumerical_value;
    }
    return numerical_value;
}

void TextInput::GUI_init(const char* text, int x, int y)
{
    field_focus = false;
    strcpy(this->text, text);
    position = { x, y };    

    offsize_x = strlen(this->text) * (font_size*0.5);
    offsize_y = (font_size*0.3);

    label.GUI_init(this->text, position.x, position.y);
    start_field_x = position.x + label.get_total_width();
    start_field_y = position.y - offsize_y;

    field.GUI_init(value.c_str(), start_field_x + field_padding_x, position.y);    
    total_width = label.get_total_width() + field_width;

}

void TextInput::draw()
{
    poll_events();
    guidraw->draw_fill_rect(
        start_field_x,
        start_field_y,
        field_width,
        field_height,
        current_background_color
    );
    label.draw();
    field.draw();
}

void TextInput::poll_events()
{
    max_lenght = value.length() > (field_width/lim_characteres) - (lim_characteres-1);
    SDL_GetMouseState(&mouse_coordinates.x, &mouse_coordinates.y);
    bool condicions = mouse_coordinates.x >= start_field_x &&
        mouse_coordinates.x <= (start_field_x + field_width) &&
        mouse_coordinates.y >= start_field_y &&
        mouse_coordinates.y <= (start_field_y + field_height);
    if (condicions && !field_focus) {
        // hover color 
        current_background_color = hover_color;
        
        SDL_PollEvent(&gui_events);
        switch (gui_events.type)
        {
            case OS_MOUSE_TYPE::BUTTON_DOWN: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {                    
                    mouse = OS_MOUSE_STATE::MOUSE_DOWN;
                    field_focus = true;
                }
            }
            break;
            case OS_MOUSE_TYPE::BUTTON_UP: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {                    
                    mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
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
        // hover color 
        current_background_color = color;
    }

    if (field_focus) {
        while (SDL_PollEvent(&gui_events) != 0) {
            if (gui_events.type == OS_TEXT::INPUT || gui_events.type == OS_KEYBOARD_TYPE::KEY_DOWN)
            {
                if (gui_events.type == OS_KEYBOARD_TYPE::KEY_DOWN &&
                    gui_events.key.keysym.sym == OS_KEYBOARD_KEY::BT_BACKSPACE &&
                    value.length() > 0
                )
                {
                    value = value.substr(0, value.length() - 1);
                    field.set_text(value.c_str());
                }
                else if (gui_events.type == OS_TEXT::INPUT) {
                    if (max_lenght)return;
                    value += gui_events.text.text;
                    field.set_text(value.c_str());
                }
                else if (gui_events.type == OS_KEYBOARD_TYPE::KEY_DOWN &&
                    gui_events.key.keysym.sym == OS_KEYBOARD_KEY::BT_ENTER)
                {
                    field_focus = false;
                    if (isdigit(value.c_str()[0])) {
                        set_numerical_value(numerical_controller);
                        LOG(*(float*)numerical_value);
                    }
                }
            }
        }
    }

}

class TransformInput : public OSGui
{
    public:
        TransformInput();
        TransformInput(const char* label, int x, int y);
        ~TransformInput();

        vect3<float> get_transform_xyz();

        void GUI_init(const char* label, int x, int y);
        void draw() override;
        void poll_events() override;

    private:
        int margin_y = 15;
        int offsizebox = 10;
        vect3<float> transform_xyz;
        TextInput transform_x;
        TextInput transform_y;
        TextInput transform_z;
        TextLabel label;
};

TransformInput::TransformInput(){
    transform_xyz = { 1.0f , 1.0f, 1.0f };
}
TransformInput::TransformInput(const char* label, int x, int y)
{
    GUI_init(label, x, y);
}
TransformInput::~TransformInput(){}

vect3<float> TransformInput::get_transform_xyz()
{
    return transform_xyz;
}

void TransformInput::GUI_init(const char* label, int x, int y)
{
    transform_xyz = { 1, 1, 1 };
    position = { x, y };     
    this->label.GUI_init(label, x, y);
    transform_x.set_value("1");
    transform_y.set_value("1");
    transform_z.set_value("1");
    transform_x.GUI_init("X: ", position.x, position.y + margin_y * 2);    
    transform_y.GUI_init("Y: ", position.x, position.y + margin_y * 4);    
    transform_z.GUI_init("Z: ", position.x, position.y + margin_y * 6);

}

void TransformInput::draw()
{
    poll_events();
    guidraw->draw_rect(
        position.x - offsizebox,
        position.y ,
        transform_x.get_total_width() + offsizebox*2,
        130, 2,
        border_color
    );

    transform_x.draw();
    transform_y.draw();
    transform_z.draw();
    label.draw();
}

void TransformInput::poll_events(){
    transform_xyz.x = *(float*)transform_x.get_numerical_value();
    transform_xyz.y = *(float*)transform_y.get_numerical_value();
    transform_xyz.z = *(float*)transform_z.get_numerical_value();
    //std::cout << transform_xyz << std::endl;
}


class RadioButton : public OSGui
{
    public:
        RadioButton();
        RadioButton(const char* label, int x, int y, bool* target, int size);
        ~RadioButton();

        void GUI_init(const char* label, int x, int y, bool* target, int size);
        void draw() override;
        void poll_events() override;

    private:
        int size;
        bool* target;
        int border_size = 2;
        int offsize_button_x = 6;
        int offsize_button_y = 6;
        TextLabel label;
};

RadioButton::RadioButton()
{
    target = NULL;
}

RadioButton::RadioButton(const char* label, int x, int y, bool* target, int size)
{
    GUI_init(label, x, y, target, size);
}

RadioButton::~RadioButton(){}

void RadioButton::GUI_init(const char* label, int x, int y, bool* target, int size)
{
    this->label.GUI_init(label, x, y);
    int width_label = this->label.get_total_width() + offsize_button_x;
    position = { width_label + x, offsize_button_y + y };
    this->target = target;
    this->size = size;    
}

void RadioButton::draw()
{
    poll_events();
    label.draw();
    if(*target)
        guidraw->draw_fill_rect(position.x, position.y, size, size, background_color);
    guidraw->draw_rect(position.x, position.y, size, size, border_size, border_color);
}

void RadioButton::poll_events()
{
    SDL_GetMouseState(&mouse_coordinates.x, &mouse_coordinates.y);
    bool condicions = mouse_coordinates.x >= position.x &&
        mouse_coordinates.x <= (position.x + size) &&
        mouse_coordinates.y >= position.y &&
        mouse_coordinates.y <= (position.y + size);

    if (condicions) {
        LOG("RADIO BUTTON");
        LOG(*target);
        SDL_PollEvent(&gui_events);
        switch (gui_events.type)
        {
            case OS_MOUSE_TYPE::BUTTON_DOWN: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {
                    mouse = OS_MOUSE_STATE::MOUSE_DOWN;
                    *target = !*target;
                }
            }
            break;
            case OS_MOUSE_TYPE::BUTTON_UP: {
                if (gui_events.button.button == OS_MOUSE_BUTTON::BUTTON_LEFT) {
                    mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
                }
            }
                                         break;
            default: {
                mouse = OS_MOUSE_STATE::MOUSE_RELEASE;
            }
            break;
        }
        gui_events.type = NULL;
    }

}

#endif // !OSGUI_H
