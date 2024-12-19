#include <iostream>

#include "Vanguard.h"

#include "raii/Window.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;

	if (glfwInit() != GLFW_TRUE)
		return 1;
	
	vg::Window window(1440, 1080, "Hello World");

	vg::set_clear_color({ 1.0f, 0.0f, 0.0f, 1.0f });

	for (;;)
	{
		glfwPollEvents();
		if (window.should_close())
			break;

		window.new_frame();

		// frame

		window.end_frame();
	}
	glfwTerminate();
	return 0;
}
