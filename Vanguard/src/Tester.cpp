#include <iostream>
#include <string>
#include <array>

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

	vg::VertexBuffer vertex_buffer(&vao_binding);
	vertex_buffer.bind();
	vg::VoidArray cpubuf = vg::buffers::init_immutable_cpu_vertex_buffer(vertex_buffer, 4);

	vertex_buffer.ref<glm::vec2>(cpubuf, 0, 0) = { -0.5f, -0.5f };
	vertex_buffer.ref<glm::vec2>(cpubuf, 1, 0) = {  0.5f, -0.5f };
	vertex_buffer.ref<glm::vec2>(cpubuf, 2, 0) = {  0.0f,  0.5f };
	vertex_buffer.ref<glm::vec2>(cpubuf, 3, 0) = {  0.8f,  0.8f };

	vertex_buffer.ref<glm::vec4>(cpubuf, 0, 1) = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertex_buffer.ref<glm::vec4>(cpubuf, 1, 1) = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertex_buffer.ref<glm::vec4>(cpubuf, 2, 1) = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertex_buffer.ref<glm::vec4>(cpubuf, 3, 1) = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	vertex_buffer.bind();
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, cpubuf.size(), cpubuf);
	
	vg::raii::GLBuffer index_buffer;
	vertex_buffer.binding()->bind_to_index_buffer(index_buffer);
	std::array<GLubyte, 6> indbuf = { 0, 1, 2, 2, 3, 0 };
	vg::buffers::bind(index_buffer, vg::BufferTarget::INDEX);
	vg::buffers::init_immutable(vg::BufferTarget::INDEX, sizeof(indbuf), &indbuf);

	for (;;)
	{
		vg::new_frame();
		if (window.should_close())
			break;
		window.new_frame();

		// TODO put this rendering in between new_frame() and end_frame() in a function called render_frame(). In framebuffer resize, call render_frame(). When using Vanguard, set Window's render_frame function pointer/std::function, which will be called in render_frame().

		// frame

		vg::bind_shader(shader);
		vao_binding.bind();
		vertex_buffer.bind();
		vg::draw::elements(vg::DrawMode::TRIANGLES, 6, 0, vg::IndexDataType::UBYTE);

		auto offset0 = vao_binding.layout().buffer_offset(0, 1) + 0 * sizeof(float);
		cpubuf.ref<float>(offset0) = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 0 * glm::pi<float>() / 3)));
		vertex_buffer.bind();
		vg::buffers::subsend(vg::BufferTarget::VERTEX, offset0, sizeof(float), cpubuf[offset0]);
		
		auto offset1 = vao_binding.layout().buffer_offset(1, 1) + 1 * sizeof(float);
		cpubuf.ref<float>(offset1) = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 1 * glm::pi<float>() / 3)));
		vertex_buffer.bind();
		vg::buffers::subsend(vg::BufferTarget::VERTEX, offset1, sizeof(float), cpubuf[offset1]);
		
		auto offset2 = vao_binding.layout().buffer_offset(2, 1) + 2 * sizeof(float);
		cpubuf.ref<float>(offset2) = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 2 * glm::pi<float>() / 3)));
		vertex_buffer.bind();
		vg::buffers::subsend(vg::BufferTarget::VERTEX, offset2, sizeof(float), cpubuf[offset2]);

		window.end_frame();
	}

	vg::terminate();
	return 0;
}
