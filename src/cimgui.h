#ifndef OSIMGUI_H
#define OSIMGUI_H

#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_impl_sdl.h"
#include "imgui_internal.h"
#include "scene_controller.h"
#include "color.h" 
class OSImgui
{
	public:
		void setup();
        void pool_events();
        void draw();
        static OSImgui* get_instance();
        void set_window(SDL_Window* window);
        void set_renderer(SDL_Renderer* renderer);
        void set_texture(SDL_Texture* texture);
        void set_event(SDL_Event* event);
        void set_scene(std::vector<Shape*>* scene);
        void set_mesh(Mesh* mesh);
        void set_display(Display* display);
        void set_scene_controller(OSceneController* scene_controller);
        void clear_glcolor();
        void create_layout(int type);
        void update_shape(int index);
        void update_mesh(int index);
        void update_light(int index);
        void play_angle_amimation(bool x, bool y, bool z);
	private:
		OSImgui();
		~OSImgui();
        static OSImgui* instance;        
        SDL_Event* event;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_GLContext gl_context;
        Display* display;
        bool show_demo_window = true;
        bool show_another_window = false;       
        // Main loop
        bool open = true;     

        OSceneController* scene_controller;
        std::vector<Shape*>* scene;
        Mesh* mesh;

        //Transform Scale
        int current_selected = -1;
        float step_animation = 0.3f;
        bool animation_controller[3]{ 0, 0, 0 };
        float angle_animation_x = 0.0f;
        float angle_animation_y = 0.0f;
        float angle_animation_z = 0.0f;
        float transform_scale[3]{ 0 ,0, 0 };
        float transform_rotate[3]{ 0 ,0, 0 };
        float transform_translation[3]{ 0 ,0, 0 };
        float transform_shear[2]{ 0 ,0 };
        // light
        float light_position[3]{ 0 ,0, 0 };
        bool  use_camera_position = false;
        float camera_position[3]{ 0 ,0, 0 };

        float color[4]{ 0 ,0, 0, 1 };

        //Variables ImGui
        bool isDocked = false;
        ImGuiWindowFlags flags;
        ImGuiID dockspace_id; 
        ImGuiID dock_main_id;
        ImGuiID dock_id_left;
        ImGuiID dock_id_top;
        ImGuiID dock_id_right;
        ImGuiID dock_id_right_top;
        ImGuiID dock_id_bottom;
        int layout_type = -1;
        // GUI widgets
        char buffer[64];
        int  numberbuffer = 0;
        int color_buffer[4]{ 0, 0, 0, 0 };
        bool backface_mode_controll = true;
        bool shading_mode_controll = false;
        bool draw_mode_fill_controll = false;
        bool draw_mode_wireframe_controll = false;
        bool draw_mode_textured_controll = false;
        bool draw_mode_textured_wireframe_controll = false;
        void menu_bar();
        void sideleft_menu();
        void sideright_menu();
        void bottom_logs();
        void central_view();
        void backface_mode();
        void draw_docking();

        // popup menus
        void create_shapes();

        // separate windows imgui
        void draw_transforms();

};

OSImgui* OSImgui::instance = NULL;

OSImgui::OSImgui() {}

OSImgui::~OSImgui()
{
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
}

void OSImgui::menu_bar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {

            if (ImGui::MenuItem("Show Scene", "", !open))
            {
                open = !open;
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, open != NULL))
                open = false;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void OSImgui::sideright_menu()
{
    ImGui::Begin("Properties");
        
    ImGui::Text("Animation rotate");
    ImGui::SliderFloat("Step per sec", &step_animation, 0.01f, 1.0f);
    ImGui::Checkbox("X:", &animation_controller[0]);
    ImGui::SameLine();
    ImGui::Checkbox("Y:", &animation_controller[1]);
    ImGui::SameLine();
    ImGui::Checkbox("Z:", &animation_controller[2]);    
    ImGui::Separator();
    ImGui::Text("Transform Scale");
    ImGui::SliderFloat3("Scale", transform_scale, 0, 10000);
    ImGui::Separator();
    ImGui::Text("Transform Rotation");
    ImGui::SliderFloat3("Rotate", transform_rotate, -PI, PI);
    ImGui::Separator();
    ImGui::Text("Transform Translation");
    ImGui::SliderFloat3("Translation", transform_translation, -60, 60);
    ImGui::Separator();
    ImGui::Checkbox("Use This", &use_camera_position);
    ImGui::Text("Camera Position");
    ImGui::SliderFloat3("Eye position", camera_position, -60, 60);
    ImGui::Separator();    
    ImGui::Text("Transform Shear");
    ImGui::SliderFloat("ShearXY", &transform_shear[0], -PI, PI);
    ImGui::SliderFloat("ShearYX", &transform_shear[1], -PI, PI);
    ImGui::Separator();
    ImGui::Text("Light Position XY");
    ImGui::SliderFloat2("World Position", light_position, 0, 1200);
    ImGui::Text("Light Position Z");
    ImGui::SliderFloat("World Position", &light_position[2], -100, 100);
    ImGui::Separator();
    ImGui::Text("Change Color");
    ImGui::ColorPicker4("Color", color); 
    ImGui::Separator();
    ImGui::InputInt4("Color ARGB", color_buffer);
    ImGui::Separator();
    backface_mode();


    ImGui::End();
}

void OSImgui::bottom_logs()
{
    ImGui::Begin("Logs");
    ImGui::Text("OLA");
    ImGui::End();
}

void OSImgui::central_view()
{        
    ImGui::Begin("Central");

    ImGui::End();
}

void OSImgui::backface_mode()
{

    if (ImGui::Checkbox("BackFace Culling", &backface_mode_controll)) {
        if (!backface_mode_controll) {
            display->drawing_type = BACKFACE_TYPE::NONBACKFACE;
        }
        else {
            display->drawing_type = BACKFACE_TYPE::CULLING;
        }
    }
    
    if (ImGui::Checkbox("FLAT SHADING", &shading_mode_controll)) {
        if (shading_mode_controll) {
            display->shading = SHADING_TYPE::FLAT;
        }
        else {
            display->shading = SHADING_TYPE::DISABLED;
        }
    }

    ImGui::Separator();
    if (ImGui::Checkbox("FILLED", &draw_mode_fill_controll)) {
        if (draw_mode_fill_controll) {
            display->draw_mode = DRAW_MODE::FILLED;
        }
    }

    ImGui::SameLine();
    if (ImGui::Checkbox("WIREFRAME", &draw_mode_wireframe_controll)) {
        if (draw_mode_wireframe_controll) {
            display->draw_mode = DRAW_MODE::WIREFRAME;
        }
    }

    ImGui::Separator();
    if (ImGui::Checkbox("TEXTURED", &draw_mode_textured_controll)) {
        if (draw_mode_textured_controll) {
            display->draw_mode = DRAW_MODE::TEXTURED;
        }
    }

    ImGui::Separator();
    if (ImGui::Checkbox("TEXTURED WIREFRAME", &draw_mode_textured_wireframe_controll)) {
        if (draw_mode_textured_wireframe_controll) {
            display->draw_mode = DRAW_MODE::TEXTURED_WIREFRAME;
        }
    }

    // if there is none checked
    if(
        !draw_mode_fill_controll &&
        !draw_mode_wireframe_controll &&
        !draw_mode_textured_controll &&
        !draw_mode_textured_wireframe_controll
        )
    {
        display->draw_mode = DRAW_MODE::WIREFRAME;
    }

}

void OSImgui::draw_docking()
{
    ImGui::NewFrame();

    dockspace_id = ImGui::GetID("MyDockspace");

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    //window_flags |= ImGuiWindowFlags_Popup;

    ImGui::SetNextWindowBgAlpha(0);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Dock", 0, window_flags);
    ImGui::PopStyleVar(3);
    menu_bar();
    if (!isDocked)
        create_layout(2);

    if (open) {
        if (layout_type == 1) {
            ImGui::DockSpace(dockspace_id);
            sideleft_menu();
            sideright_menu();
            central_view();
            bottom_logs();
        }
        else if (layout_type == 2) {
            ImGui::DockSpace(dockspace_id);
            central_view();
            ImGui::SetNextWindowBgAlpha(1);
            sideright_menu();
            ImGui::SetNextWindowBgAlpha(1);
            sideleft_menu();
        }
    }
    else {
        //ImGui::ShowDemoWindow();
    }


    ImGui::End();
}

void OSImgui::create_shapes()
{
    if (ImGui::BeginPopupModal("Stacked 1", NULL, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Some menu item")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        static char name[32] = "Label1";        
        ImGui::Text("Edit name:");
        ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));

        ImGui::InputInt("radius", &numberbuffer);
        if (ImGui::Button("Create")) {
            scene_controller->add<Ellipse>(name,
                vect3<float>{ 0, 0, 0 },
                vect3<float>{ 1, 1, 1 },
                vect3<float>{ 0, 0, 0 },
                numberbuffer
            );            
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupContextWindow("item context menu"))
    {
        bool openPopup = false;
        if (ImGui::Selectable("New Ellipse")) {
            LOG("Create a ellipse");
            openPopup = true;            
        }        
        if (ImGui::Selectable("Clear all")) {
            scene->clear();
        }                
        ImGui::EndPopup();            
        if(openPopup) ImGui::OpenPopup("Stacked 1");
    }    


}

void OSImgui::draw_transforms()
{
    ImGui::NewFrame();
    sideright_menu();   
    sideleft_menu();
}

void OSImgui::sideleft_menu()
{
    ImGui::Begin("Objects");
    create_shapes();
    static int selected = -1;
    if (ImGui::Selectable("Mesh", selected == -99)) {
        selected = -99;
    }

    if (ImGui::Selectable("Light", selected == -98)) {
        selected = -98;
    }
        
    int n = 0;
    for (auto& _scene : *scene) {
        char buf[32];
        sprintf(buf, "%s", _scene->name);
        if (ImGui::Selectable(buf, selected == n))
            selected = n;
        n++;
    }
    update_shape(selected);    
    update_mesh(selected);  
    update_light(selected);
    
    if (use_camera_position) {
        Display::camera->position = vect3<float>{
            camera_position[0],
            camera_position[1],
            camera_position[2],
        };
    }

    play_angle_amimation(animation_controller[0], animation_controller[1], animation_controller[2]);
    ImGui::End();
}

void OSImgui::setup()
{
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, display->width, display->height);
    flags = ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;    
}

void OSImgui::pool_events()
{
    ImGui_ImplSDL2_ProcessEvent(event);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigDockingWithShift = true;
    // 5 dias
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiWindowFlags_NoMove;    
    io.ConfigWindowsResizeFromEdges = true;
    int mouseX, mouseY, keyboard_key;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);
    io.MousePos = ImVec2(mouseX, mouseY);
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);    
    io.KeyMap[ImGuiKey_Backspace] = 42;
    event->type = NULL;
}

void OSImgui::draw()
{
    pool_events();
    draw_docking();
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
}

OSImgui* OSImgui::get_instance()
{
    if (OSImgui::instance == NULL) {
        OSImgui::instance = new OSImgui();
    }
    return OSImgui::instance;
}

void OSImgui::set_window(SDL_Window* window)
{
    this->window = window;
}

void OSImgui::set_renderer(SDL_Renderer* renderer)
{
    this->renderer = renderer;
}

void OSImgui::set_texture(SDL_Texture* texture)
{
    this->texture = texture;
}

void OSImgui::set_event(SDL_Event* event)
{
    this->event = event;
}

void OSImgui::set_scene(std::vector<Shape*>* scene)
{
    this->scene = scene;
}

void OSImgui::set_mesh(Mesh* mesh)
{
    this->mesh = mesh;
}

void OSImgui::set_display(Display* display)
{
    this->display = display;
}

void OSImgui::set_scene_controller(OSceneController* scene_controller)
{
    this->scene_controller = scene_controller;
}

void OSImgui::clear_glcolor()
{
   
}

void OSImgui::create_layout(int type)
{
    if (type == 1) {
        LOG("Dockspace 1 created");
        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspace_id); // Add empty node

        dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        dock_id_left    = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
        dock_id_top     = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.80f, NULL, &dock_main_id);
        dock_id_right   = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
        dock_id_bottom  = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);
   
        ImGui::DockBuilderDockWindow("Central", dock_main_id);
        ImGui::DockBuilderDockWindow("Objects", dock_id_left);
        ImGui::DockBuilderDockWindow("James_2", dock_id_top);
        ImGui::DockBuilderDockWindow("Properties", dock_id_right);
        ImGui::DockBuilderDockWindow("Logs", dock_id_bottom);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    else if (type = 2) {
        LOG("Dockspace 1 created");
        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspace_id); // Add empty node

        dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
        dock_id_right_top = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Up, 0.20f, NULL, &dock_id_right);

        ImGui::DockBuilderDockWindow("Central", dock_main_id);
        ImGui::DockBuilderDockWindow("Properties", dock_id_right);
        ImGui::DockBuilderDockWindow("Objects", dock_id_right_top);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    layout_type = type;
    isDocked = true;

}

void OSImgui::update_shape(int index)
{
    if (index < 0) return;
    if (current_selected != index) {
        vect3<float>* scale  = scene->at(index)->get_scale();
        vect3<float>* rotate = scene->at(index)->get_rotation();
        uint32_t _color  = scene->at(index)->get_color();
        ImVec4 fcolor = ImGui::ColorConvertU32ToFloat4(_color);
        transform_scale[0] = scale->x;
        transform_scale[1] = scale->y;
        transform_scale[2] = scale->z;
        transform_rotate[0] = rotate->x;
        transform_rotate[1] = rotate->y;
        transform_rotate[2] = rotate->z;
        color[0] = fcolor.x;
        color[1] = fcolor.y;
        color[2] = fcolor.z;
        color[3] = fcolor.w;
        current_selected = index;
    }
    scene->at(index)->set_color(ImGui::ColorConvertFloat4ToU32(ImVec4{ color[0], color[1] , color[2] , color[3] }));
    vect3<float> _scale{ transform_scale[0], transform_scale[1],  transform_scale[2] };
    vect3<float> _rotate{ transform_rotate[0], transform_rotate[1],  transform_rotate[2] };
    scene_controller->update(index, { 0, 0, 0 }, _scale, _rotate);
}

void OSImgui::update_mesh(int index)
{
    if (index != -99) return;
    if (current_selected != index) {
        vect3<float>* scale = mesh->get_scale();
        vect3<float>* rotate = mesh->get_rotate();
        vect3<float>* translation = mesh->get_translation();        
        transform_scale[0] = scale->x;
        transform_scale[1] = scale->y;
        transform_scale[2] = scale->z;
        transform_rotate[0] = rotate->x;
        transform_rotate[1] = rotate->y;
        transform_rotate[2] = rotate->z;
        transform_translation[0] = translation->x;
        transform_translation[1] = translation->y;
        transform_translation[2] = translation->z;
        uint32_t _color = mesh->get_color();
        ImVec4 fcolor = ImGui::ColorConvertU32ToFloat4(_color);
        color[0] = fcolor.x;
        color[1] = fcolor.y;
        color[2] = fcolor.z;
        color[3] = fcolor.w;
        current_selected = index;
    }
    mesh->set_color(ImGui::ColorConvertFloat4ToU32(ImVec4{ color[0], color[1] , color[2] , color[3] }));
    vect3<float> _translation{ transform_translation[0], transform_translation[1],  transform_translation[2] };
    vect3<float> _scale{ transform_scale[0], transform_scale[1],  transform_scale[2] };
    vect3<float> _rotate{ transform_rotate[0], transform_rotate[1],  transform_rotate[2] };
    mesh->set_translation(_translation);
    mesh->set_scale(_scale);
    mesh->set_rotate(_rotate);
    mesh->set_shearXY(transform_shear[0]);
    mesh->set_shearYX(transform_shear[1]);
}

void OSImgui::update_light(int index)
{
    if (index != -98) return;
    GlobalLight* light = GlobalLight::get_instance();
    if (current_selected != index) {
        vect3<float>* scale = mesh->get_scale();
        vect3<float>* rotate = mesh->get_rotate();
        vect3<float>* translation = mesh->get_translation();
        light_position[0] = light->light.direction.x;
        light_position[1] = light->light.direction.y;
        light_position[2] = light->light.direction.z;
        current_selected = index;
    }
    vect3<float> _light_position{ light_position[0], light_position[1], light_position[2] };
    light->set_position(_light_position);
}

void OSImgui::play_angle_amimation(bool x, bool y, bool z)
{
    angle_animation_x += step_animation * display->delta_time;
    angle_animation_y += step_animation * display->delta_time;
    angle_animation_z += step_animation * display->delta_time;
    if (angle_animation_z > TWO_PI) angle_animation_z = 0;
    if (angle_animation_y > TWO_PI) angle_animation_y = 0;
    if (angle_animation_x > TWO_PI) angle_animation_x = 0;

    if (x) {
        mesh->set_rotate_mesh_x(angle_animation_x);
    }
    
    if (y) {
        mesh->set_rotate_mesh_y(angle_animation_y);
    }
    
    if (z) {
        mesh->set_rotate_mesh_z(angle_animation_z);
    }

}

#endif // !OSIMGUI_H
