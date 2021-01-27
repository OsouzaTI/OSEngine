#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL.h>
#include <SDL_ttf.h>
#include "vector.h"
#include "light.h"

typedef struct Camera {
	vect3<float> point;
	vect3<float> rotate;
	float fov;
	float znear;
	float zfar;
	float aspect;

	Camera() {};
	Camera(vect3<float> point, vect3<float> rotate, float fov, float znear, float zfar, float aspect)
	{
		this->point = point;
		this->rotate = rotate;
		this->fov = fov;
		this->znear = znear;
		this->zfar = zfar;
		this->aspect = aspect;
	}

} Camera;

typedef struct {
	int width;
	int height;	
} WindowViewPort;

enum BACKFACE_TYPE {
	CULLING = 0,
	NONBACKFACE
};

enum DRAW_MODE {
	WIREFRAME = 0,
	FILLED,
	TEXTURED,
	TEXTURED_WIREFRAME
};

class Display
{
	public:
		WindowViewPort view_port;
		static Camera* camera;
		BACKFACE_TYPE drawing_type = BACKFACE_TYPE::CULLING;
		DRAW_MODE draw_mode = DRAW_MODE::WIREFRAME;
		SHADING_TYPE shading = SHADING_TYPE::DISABLED;
		// largura da janela em pixels
		int width;
		// altura da janela em pixels
		int height;
		// titulo da janela em pixels
		const char* title;

		Display();
		~Display();
		
		// Calculo de pixel para limpar a tela ( clear_buffer ) 
		inline int pixel(int x, int y) { return ((view_port.width * y) + x); };

		// inicia a janela
		bool init_window(const char* title, int width, int height, int wview_port, int hview_port);
		// inicia os buffers e variaveis
		void setup_window();
		// desenha no buffer
		void draw_buffer();
		// limpa o buffer
		void clear_buffer();

		SDL_Renderer* get_renderer();
		SDL_Window* get_window();
		SDL_Texture* get_texture();
		Camera* get_camera();

		// atualiza a tela apos as escritas
		inline void sdl_render_present() { SDL_RenderPresent(this->renderer); };

		void create_camera(vect3<float> position, vect3<float> rotate, float fov, float znear, float zfar, float aspect = NULL);
		void set_camera_fov(float fov);
		void set_clear_color_screen(uint32_t color);

		// retorna a referencia do buffer de cor
		uint32_t* get_color_buffer();
		void frame_rate_control();
		void set_frame_rate(int frame_rate);
	private:	
		SDL_Window* window;
		SDL_Renderer* renderer;
		uint32_t* color_buffer;
		SDL_Texture* texture_buffer;
		uint32_t default_clear_color_buffer;
		Uint32 previous_frame_time;
		float frame_target_time;
		float frame_rate;
		char* buffer_title;
};


#endif // !DISPLAY_H
