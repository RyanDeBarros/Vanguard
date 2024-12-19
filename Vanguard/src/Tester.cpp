#include <iostream>
#include <string>

#include "Vanguard.h"

#include "raii/Window.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;
	vg::init();
	
	vg::Window window(1440, 1080, "Hello World");

	vg::set_clear_color({ 0.5f, 0.7f, 0.9f, 1.0f });

	for (;;)
	{
		glfwPollEvents();
		if (window.should_close())
			break;

		window.new_frame();

		// frame

		window.end_frame();
	}

	vg::terminate();
	return 0;
}
