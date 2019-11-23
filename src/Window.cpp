#include "Window.h"
#include <stdlib.h>
#include <stdio.h>
#include <Circe/Circe.h>

namespace Medusa
{
	using namespace Circe;
	using namespace std;

	Keyboard& IWindow::getKeyboard()
	{
		return m_keyboard;
	}
	
	Mouse& IWindow::getMouse()
	{
		return m_mouse;
	}
	
	int IWindow::getWidth() const
	{
		return width;
	}
	
	int IWindow::getHeight() const
	{
		return height;
	}
	
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}
	
	static void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);

		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow));
		if(window != NULL){
			if(action == GLFW_PRESS)
			{
				window->getKeyboard().onChange(key, 1);
			}
			else if(action == GLFW_RELEASE)
			{
				window->getKeyboard().onChange(key, 0);
			}
		}
	}
	
	static void mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow));
		if(window != NULL){
			if(action == GLFW_PRESS)
			{
				window->getMouse().onPress(button);
			}
			else if(action == GLFW_RELEASE)
			{
				window->getMouse().onRelease(button);
			}
		}
	}
	
	static void cursor_position_callback(GLFWwindow* glfwWindow, double xpos, double ypos)
	{
		GLFWWindow* window = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow));
		if(window != NULL){
			window->getMouse().onMoved(Circe::Vec2(xpos/window->getWidth(), ypos/window->getHeight()));
		}
	}
	
	IWindow::IWindow(const int& width, const int& height, const char* title):width(width),height(height),title(title)
	{}
	
	IWindow::~IWindow()
	{
		delete[] title;
	}
	
	void IWindow::refresh()
	{
		update();
		swapBuffers();
	}
	
	GLFWWindow::GLFWWindow(const int& width, const int& height, const char* title):IWindow(width, height, title)
	{
		init();
	}
	
	int GLFWWindow::init()
	{
		GLint vpos_location, vcol_location;
		glfwSetErrorCallback(error_callback);
		if (!glfwInit())
			exit(EXIT_FAILURE);
		std::cout << glfwInit() << std::endl;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		//Create a windowed mode window and its OpenGL context
		glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!glfwWindow)
		{
			glfwTerminate();
			return -1;
		}
		
		
		glfwSetInputMode(glfwWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);
		glfwSetWindowUserPointer(glfwWindow, this);
		glfwSetKeyCallback(glfwWindow, key_callback);
		glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
		glfwSetCursorPosCallback(glfwWindow, cursor_position_callback);
		
		
		glfwMakeContextCurrent(glfwWindow);
		gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		glfwSwapInterval(1);
		CIRCE_INFO("GLFW window initialized.");
		
		//Hide the cursor:
		
		
		return 0;
	}
	
	void GLFWWindow::update()
	{
		//Update window size if necessary
		glfwGetFramebufferSize(glfwWindow, &width, &height);
		glViewport(0, 0, width, height);
		
		glfwPollEvents();
	}
	
	bool GLFWWindow::shouldClose() const
	{
		return glfwWindowShouldClose(glfwWindow);
	}
	
	void GLFWWindow::swapBuffers()
	{
		glfwSwapBuffers(glfwWindow);
	}
	
	void GLFWWindow::terminate()
	{
		glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
		glfwDestroyWindow(glfwWindow);
		glfwTerminate();
		CIRCE_INFO("GLFW window terminated.");	
	}
	
	void GLFWWindow::setCursorVisible(const bool& visible)
	{
		if(!visible)
		{
			glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	
	
	/*SDLWindow::SDLWindow(const int& width, const int& height, const char* title):IWindow(width, height, title)
	
	{
		init();
	}
	
	int SDLWindow::init()
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			CIRCE_ERROR("Failed to init SDL");
		}			// Initialize SDL2

		sdlWindow = SDL_CreateWindow(
			title,                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			width,                               // width, in pixels
			height,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
		);

		
		SDL_GL_LoadLibrary(NULL);
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);				
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


		sdlContext = SDL_GL_CreateContext(sdlWindow);
		renderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

		gladLoadGLLoader(SDL_GL_GetProcAddress);

		SDL_GL_SetSwapInterval(1);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		return 0;
	}
	
	void SDLWindow::update()
	{
		//Update window size if necessary
		int windowWidth, windowHeight;
		SDL_GL_GetDrawableSize(sdlWindow, &windowWidth, &windowHeight);

		glViewport(0, 0, windowWidth, windowHeight);
	}
	
	bool SDLWindow::shouldClose() const
	{
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {			
				return true;
			}
		}
		return false;
	}
	
	void SDLWindow::swapBuffers()
	{
		SDL_GL_SwapWindow(sdlWindow);
	}
	
	void SDLWindow::terminate()
	{
		SDL_GL_DeleteContext(sdlContext);
		SDL_DestroyWindow(sdlWindow);
		SDL_Quit();
	}*/
	
}