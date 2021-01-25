#ifndef OSIMGUI_H
#define OSIMGUI_H

#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_impl_sdl.h"
#include "imgui_internal.h"
#include "oscene_controller.h"
#include "oscolor.h" 
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
        void set_scene_controller(OSceneController* scene_controller);
        void clear_glcolor();
        void create_layout();
        void update_shape(int index);
	private:
		OSImgui();
		~OSImgui();
        static OSImgui* instance;
        SDL_Event* event;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_GLContext gl_context;
        bool show_demo_window = true;
        bool show_another_window = false;       
        // Main loop
        bool open = true;     

        OSceneController* scene_controller;
        std::vector<Shape*>* scene;

        //Transform Scale
        int current_selected = -1;
        float transform_scale[3]{ 0 ,0, 0 };
        float transform_rotate[3]{ 0 ,0, 0 };
        float color[4]{ 0 ,0, 0, 1 };

        //Variables ImGui
        bool isDocked = false;
        ImGuiWindowFlags flags;
        ImGuiID dockspace_id; 
        ImGuiID dock_main_id;
        ImGuiID dock_id_left;
        ImGuiID dock_id_top;
        ImGuiID dock_id_right;
        ImGuiID dock_id_bottom;

        // GUI widgets
        void menu_bar();
        void sideleft_menu();
        void sideright_menu();
        void bottom_logs();
        void central_view();



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
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::Begin("Properties");
        
    ImGui::Text("Transform Scale");
    ImGui::SliderFloat3("Scale", transform_scale, 0, 1);
    ImGui::Separator();
    ImGui::Text("Transform Rotation");
    ImGui::SliderFloat3("Rotate", transform_rotate, -PI, PI);
    ImGui::Separator();
    ImGui::Text("Change Color");
    ImGui::ColorEdit4("Color", color); 
    

    ImGui::End();
}

void OSImgui::bottom_logs()
{
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::Begin("Logs");
    ImGui::Text("OLA");
    ImGui::End();
}

void OSImgui::central_view()
{    
    ImGui::Begin("Central");
     //ImGui::Image(texture, {1200 , 800});
    ImGui::End();
}

void OSImgui::sideleft_menu()
{
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::Begin("Objects");
    static int selected = -1;
    int n = 0;
    for (auto& _scene : *scene) {
        char buf[32];
        sprintf(buf, "%s", _scene->name);
        if (ImGui::Selectable(buf, selected == n))
            selected = n;
        n++;
    }
    update_shape(selected);    
    ImGui::End();
}

void OSImgui::setup()
{
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, 1200, 800);
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
    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    io.MousePos = ImVec2(mouseX, mouseY);
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

void OSImgui::draw()
{
    pool_events();

	ImGui::NewFrame();

        dockspace_id = ImGui::GetID("MyDockspace");

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::SetNextWindowBgAlpha(0);  

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Dock", 0, window_flags);
        ImGui::PopStyleVar(3);
        menu_bar();
        if (!isDocked)
            create_layout();

    if (open) {
        ImGui::DockSpace(dockspace_id);
        sideleft_menu();
        sideright_menu();
        central_view();
        bottom_logs();
    }
    else {
        ImGui::ShowDemoWindow();
    }
    
    
    ImGui::End();
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

void OSImgui::set_scene_controller(OSceneController* scene_controller)
{
    this->scene_controller = scene_controller;
}

void OSImgui::clear_glcolor()
{
   
}

void OSImgui::create_layout()
{
    logstd("Dockspace created");
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
    scene->at(index)->set_color(
        color32_reverse(ImGui::ColorConvertFloat4ToU32(ImVec4{color[0], color[1], color[2], color[3]}))
    );
    vect3<float> _scale{ transform_scale[0], transform_scale[1],  transform_scale[2] };
    vect3<float> _rotate{ transform_rotate[0], transform_rotate[1],  transform_rotate[2] };
    scene_controller->update(index, { 0, 0, 0 }, _scale, _rotate);
}

#endif // !OSIMGUI_H
