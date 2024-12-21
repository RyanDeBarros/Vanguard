#include <iostream>
#include <string>

#include "Vanguard.h"

#include "raii/Window.h"
#include "Renderable.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;
	vg::init();
	
	vg::Window window(1440, 1080, "Hello World");

	vg::set_clear_color({ 0.5f, 0.7f, 0.9f, 1.0f });

	vg::Shader shader(vg::FilePath("shaders/color.vert"), vg::FilePath("shaders/color.frag"));
	vg::VAOBinding vao_binding(shader);
	vg::raii::GLBuffer vertex_buffer;

	vao_binding.attach_vertex_buffer(vertex_buffer);
	vg::VoidArray cpubuf;
	vg::buffers::bind(vertex_buffer, vg::BufferTarget::VERTEX);
	vg::buffers::init_immutable_cpu_vertex_buffer(cpubuf, vao_binding.layout(), 3);

	cpubuf.ref<float>(0 * vao_binding.layout().stride() + vao_binding.layout().attributes()[0].offset() + 0 * sizeof(float)) = -0.5f;
	cpubuf.ref<float>(0 * vao_binding.layout().stride() + vao_binding.layout().attributes()[0].offset() + 1 * sizeof(float)) = -0.5f;
	cpubuf.ref<float>(1 * vao_binding.layout().stride() + vao_binding.layout().attributes()[0].offset() + 0 * sizeof(float)) =  0.5f;
	cpubuf.ref<float>(1 * vao_binding.layout().stride() + vao_binding.layout().attributes()[0].offset() + 1 * sizeof(float)) = -0.5f;
	cpubuf.ref<float>(2 * vao_binding.layout().stride() + vao_binding.layout().attributes()[0].offset() + 0 * sizeof(float)) =  0.0f;
	cpubuf.ref<float>(2 * vao_binding.layout().stride() + vao_binding.layout().attributes()[0].offset() + 1 * sizeof(float)) =  0.5f;
	vg::buffers::bind(vertex_buffer, vg::BufferTarget::VERTEX);
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, cpubuf.size(), cpubuf);
	for (;;)
	{
		vg::new_frame();
		if (window.should_close())
			break;
		window.new_frame();

		// frame
		vao_binding.vertex_array().bind();
		vg::buffers::bind(vertex_buffer, vg::BufferTarget::VERTEX);
		vg::draw::arrays(vg::DrawMode::TRIANGLES, 0, 3);

		window.end_frame();
	}

	vg::terminate();
	return 0;
}
