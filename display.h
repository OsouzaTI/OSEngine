#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>

class Display
{
	public:
		// largura da janela em pixels
		int width;
		// altura da janela em pixels
		int height;
		// titulo da janela em pixels
		const char* title;

		Display();
		~Display();
		
		// Calculo de pixel para limpar a tela ( clear_buffer ) 
		inline int pixel(int x, int y) { return ((this->width * y) + x); };		

		// inicia a janela
		bool init_window(const char* title, int width, int height);
		// inicia os buffers e variaveis
		void setup_window();
		// desenha no buffer
		void draw_buffer();
		// limpa o buffer
		void clear_buffer();

		// atualiza a tela apos as escritas
		inline void sdl_render_present() { SDL_RenderPresent(this->renderer); };

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
		char buffer_title[200];
};


#endif // !DISPLAY_H
