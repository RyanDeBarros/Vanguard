#include <iostream>
#include <string>
#include <array>

#include "Vanguard.h"

#include "raii/Window.h"
#include "Renderable.h"
#include "Draw.h"
#include "utils/IO.h"
#include "raii/Texture.h"
#include "Transform.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;
	vg::init();

	vg::Window window(1440, 1080, "Hello World");
	window.clear_color = { 0.2f, 0.4f, 0.7f, 1.0f };

	vg::raii::Shader color_shader(vg::FilePath("shaders/color.vert"), vg::FilePath("shaders/color.frag"));
	vg::bind_shader(color_shader);
	vg::uniforms::send_3x3(color_shader, "uVP", window.orthographic_projection());

	auto vb_color_layout = std::make_shared<vg::VertexBufferLayout>(color_shader);

	vg::CPUVertexBuffer colorful_vertex_buffer(vg::VertexBuffer(vb_color_layout), 4, false);

	colorful_vertex_buffer.set_attributes(0, 0, std::array<glm::vec2, 4>{
		window.convert_coordinates({ -0.5f, -0.5f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  0.5f, -0.5f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  0.0f,  0.5f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  0.8f,  0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
		});
	colorful_vertex_buffer.set_attribute(1, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	colorful_vertex_buffer.bind_vb();
	colorful_vertex_buffer.subsend_full();

	vg::CPUIndexBuffer index_buffer(vg::IndexDataType::UBYTE);
	index_buffer.bind_to_vertex_array(colorful_vertex_buffer.vao());
	index_buffer.init_immutable_quads(1);

	vg::CPUVertexBufferBlock white_square(vg::VertexBufferBlock(vb_color_layout, { { 0, { 0 } }, { 1, { 1 } } }), 4, false);

	white_square.set_attributes(0, 0, 0, std::array<glm::vec2, 4>{
		window.convert_coordinates({ 0.7f, -0.7f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ 0.9f, -0.7f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ 0.9f, -0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ 0.7f, -0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
	});
	white_square.set_attribute(1, 1, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	white_square.subsend_all_blocks();
	index_buffer.bind_to_vertex_array(white_square.vao());

	vg::CompactVBIndexer tripair_indexer({ 3, 3 });
	vg::CPUVertexBuffer tripair(vg::VertexBuffer(vb_color_layout), tripair_indexer.vertex_count(), false);

	tripair.set_attributes(0, tripair_indexer.vertex(0, 0), std::array<glm::vec2, 3>{
		window.convert_coordinates({ -0.9f, 0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.8f, 0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.9f, 0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
	});
	tripair.set_attributes(0, tripair_indexer.vertex(1, 0), std::array<glm::vec2, 3>{
		window.convert_coordinates({ -0.7f, 0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.7f, 0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.8f, 0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
	});
	tripair.set_attribute(1, tripair_indexer.vertex(0, 0), tripair_indexer.vertex_count(0), glm::vec4{0.8f, 0.5f, 0.3f, 1.0f});
	tripair.set_attribute(1, tripair_indexer.vertex(1, 0), tripair_indexer.vertex_count(1), glm::vec4{0.6f, 0.5f, 0.5f, 1.0f});
	tripair.subsend_full();

	std::string image_vert = vg::io::read_file("shaders/image.vert");
	std::string image_frag = vg::io::read_template_file("shaders/image.frag.tmpl", { { "$NUM_TEXTURE_SLOTS", std::to_string(window.constants().max_texture_image_units)}});
	vg::raii::Shader img_shader(image_vert, image_frag);
	vg::bind_shader(img_shader);
	vg::uniforms::send_3x3(img_shader, "uVP", window.orthographic_projection());

	// It is possible to have a vertex buffer with only one vertex's attribute, but only if setting the divisor. This works even with non-instanced rendering
	auto img_layout = std::make_shared<vg::VertexBufferLayout>(img_shader, vg::VertexAttributeSpecificationList{ {}, {}, { { 1, 1 }, { 4, 1 }, { 5, 1 }, { 6, 1 } } });

	vg::CPUVertexBufferBlock sprite(vg::VertexBufferBlock(img_layout, { { 0, { 0, 2, 3 } }, { 1, { 1 } }, { 2, { 4, 5, 6 } } }), { 4, 1, 1 }, false);
	index_buffer.bind_to_vertex_array(sprite.vao());

	sprite.set_attributes(0, 0, 0, std::array<glm::vec2, 4>{
		window.convert_coordinates({ -1.0f, -1.0f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  1.0f, -1.0f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  1.0f,  1.0f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -1.0f,  1.0f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
	});
	sprite.set_attributes(0, 2, 0, std::array<glm::vec2, 4>{
		glm::vec2{ 0.0f, 0.0f },
		glm::vec2{ 1.0f, 0.0f },
		glm::vec2{ 1.0f, 1.0f },
		glm::vec2{ 0.0f, 1.0f }
	});
	sprite.set_attribute(0, 3, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	sprite.set_attribute(1, 1, GLint(0));

	vg::Transform2D sprite_transform{ window.convert_coordinates({ 0.5f, 0.25f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN), 0.4f, { 0.25f, 0.25f }};
	sprite.set_attribute(2, 4, sprite_transform.matrix());
	
	sprite.subsend_all_blocks();

	vg::raii::Image2D img_einstein = vg::load_image_2d("ex/flag.png");
	vg::raii::Texture tex_einstein;
	vg::image_2d::send_texture(img_einstein, tex_einstein);
	vg::texture_params::nearest(vg::texture_params::T2D);
	vg::texture_params::clamp_to_edge(vg::texture_params::T2D);

	// TODO resizing window is not working
	window.render_frame = [&]() {
		vg::bind_shader(color_shader);

		colorful_vertex_buffer.bind_vao();
		vg::draw::index_buffer::full(index_buffer, vg::DrawMode::TRIANGLES);

		colorful_vertex_buffer.bind_vb();
		colorful_vertex_buffer.ref<glm::vec4>(0, 1).x = glm::sqrt(0.5f * (1.0f + glm::sin((float)glfwGetTime() + 0 * glm::pi<float>() / 3)));
		colorful_vertex_buffer.subsend_single(0, 1);
		colorful_vertex_buffer.ref<glm::vec4>(1, 1).y = glm::sqrt(0.5f * (1.0f + glm::sin((float)glfwGetTime() + 1 * glm::pi<float>() / 3)));
		colorful_vertex_buffer.subsend_single(1, 1);
		colorful_vertex_buffer.ref<glm::vec4>(2, 1).z = glm::sqrt(0.5f * (1.0f + glm::sin((float)glfwGetTime() + 2 * glm::pi<float>() / 3)));
		colorful_vertex_buffer.subsend_single(2, 1);

		white_square.ref<glm::vec2>(0, 0, 0).x -= window.convert_coordinates({ 0.008f * glm::sin((float)glfwGetTime() * 20.0f), 0.0f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN).x;
		white_square.bind_vb(0);
		white_square.subsend_single(0, 0, 0);

		white_square.bind_vao();
		vg::draw::index_buffer::full(index_buffer, vg::DrawMode::TRIANGLES);

		tripair.bind_vao();
		vg::draw::vertex_buffer::full(tripair, vg::DrawMode::TRIANGLES);

		vg::bind_shader(img_shader);
		
		vg::select_texture_slot(0);
		vg::bind_texture(tex_einstein, vg::TextureTarget::T2D);
		
		sprite.bind_vao();
		vg::draw::index_buffer::full(index_buffer, vg::DrawMode::TRIANGLES);

		sprite_transform.rotation -= 0.01f;
		sprite.ref<glm::mat3>(2, 0, 4) = sprite_transform.matrix();
		sprite.bind_vb(2);
		sprite.subsend_single(2, 0, 4, sizeof(glm::mat3));
		};

	for (;;)
	{
		glfwPollEvents();
		if (window.should_close())
			break;
		window.frame_cycle();
	}

	vg::terminate();
	return 0;
}
