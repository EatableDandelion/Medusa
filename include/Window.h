#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <Circe/Circe.h>
#include "Input.h"
#include "RenderingEngine.h"
#include "GUI.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace Medusa
{	
	class IWindow
	{
		public:
			IWindow(const int& width, const int& height, const char* title);
			virtual ~IWindow();
						
			virtual int init()=0;
			virtual void update()=0;
			virtual bool shouldClose() const=0;
			virtual void terminate()=0;
			virtual void swapBuffers()=0;
			
			void refresh();
			
			Keyboard& getKeyboard();
			Mouse& getMouse();
			int getWidth() const;
			int getHeight() const;
			
		protected:
			int width, height;
			const char* title;
			Keyboard m_keyboard;
			Mouse m_mouse;
	};
	
	template<typename T>
	class Window
	{
		public:
			Window(const int& width, const int& height, const char* title):window(width, height, title)
			{}
			
			~Window()
			{
				window.terminate();
			}
						
			int init()
			{
				return window.init();
			}
			
			void update()
			{
				window.update();
			}
			
			bool shouldClose() const
			{
				return window.shouldClose();
			}
			
			void terminate()
			{
				window.terminate();
			}
			
			void swapBuffers()
			{
				window.swapBuffers();
			}
			
			Keyboard& getKeyboard()
			{
				return window.getKeyboard();
			}
			
			Mouse& getMouse()
			{
				return window.getMouse();
			}
		
		private:
			T window;
			
	};
	
	class GLFWWindow : public IWindow
	{
		public:
			GLFWWindow(const int& width, const int& height, const char* title);
			virtual int init();
			virtual void update();
			virtual bool shouldClose() const;
			virtual void terminate();
			virtual void swapBuffers();
			
		private:
			GLFWwindow* glfwWindow;
	};
	
	class SDLWindow : public IWindow
	{
		public:
			SDLWindow(const int& width, const int& height, const char* title);
			virtual int init();
			virtual void update();
			virtual bool shouldClose() const;
			virtual void terminate();
			virtual void swapBuffers();
			
		private:
			SDL_Window* sdlWindow;
			SDL_GLContext sdlContext;
			SDL_Renderer* renderer;
	};
	
}