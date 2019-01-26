#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <Circe/Circe.h>

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
			virtual void update(Camera& camera)=0;
			virtual bool shouldClose() const=0;
			virtual void terminate()=0;
			virtual void swapBuffers()=0;
			
		protected:
			const int width, height;
			const char* title;
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
			
			void update(Camera& camera)
			{
				window.update(camera);
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
		
		private:
			T window;
	};
	
	class GLFWWindow : public IWindow
	{
		public:
			GLFWWindow(const int& width, const int& height, const char* title);
			virtual int init();
			virtual void update(Camera& camera);
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
			//~SDLWindow();
			virtual int init();
			virtual void update(Camera& camera);
			virtual bool shouldClose() const;
			virtual void terminate();
			virtual void swapBuffers();
			
		private:
			SDL_Window* sdlWindow;
			SDL_GLContext sdlContext;
			SDL_Renderer* renderer;
	};
	
}