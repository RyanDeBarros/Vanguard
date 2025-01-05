#include <iostream>
#include <string>
#include <array>

#include "Vanguard.h"

#include "raii/Window.h"
#include "Renderable.h"
#include "Draw.h"
#include "utils/IO.h"
#include "raii/Texture.h"
#include "raii/FrameBuffer.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;
	vg::init();

	vg::Window window(1440, 1080, "Hello World");

	vg::Shader shader(vg::FilePath("shaders/color.vert"), vg::FilePath("shaders/color.frag"));
	auto vb_layout = std::make_shared<vg::VertexBufferLayout>(shader);

	vg::CPUVertexBuffer vertex_buffer(vg::VertexBuffer(vb_layout), 4, false);

	vertex_buffer.set_attributes(0, 0, std::array<glm::vec2, 4>{
		glm::vec2{ -0.5f, -0.5f },
		glm::vec2{  0.5f, -0.5f },
		glm::vec2{  0.0f,  0.5f },
		glm::vec2{  0.8f,  0.8f }
		});
	vertex_buffer.set_attribute(1, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	vertex_buffer.bind_vb();
	vertex_buffer.subsend_full();

	vg::CPUIndexBuffer index_buffer(vg::IndexDataType::UBYTE);
	index_buffer.bind_to_vertex_array(vertex_buffer.vao());
	index_buffer.init_immutable_quads(1);

	vg::CPUVertexBufferBlock white_square(vg::VertexBufferBlock(2, vb_layout, { { 0, { 0 } }, { 1, { 1 } } }), 4, false);

	white_square.set_attributes(0, 0, 0, std::array<glm::vec2, 4>{
		glm::vec2{ 0.7f, -0.7f },
		glm::vec2{ 0.9f, -0.7f },
		glm::vec2{ 0.9f, -0.9f },
		glm::vec2{ 0.7f, -0.9f }
	});
	white_square.set_attribute(1, 1, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	white_square.subsend_all_blocks();
	index_buffer.bind_to_vertex_array(white_square.vao());

	vg::MultiCPUVertexBuffer tripair(vg::MultiVertexBuffer(vb_layout, 2), 3, false);

	tripair.set_attributes(0, 0, 0, std::array<glm::vec2, 3>{
		glm::vec2{ -0.9f, 0.9f },
		glm::vec2{ -0.8f, 0.9f },
		glm::vec2{ -0.9f, 0.8f }
	});
	tripair.set_attributes(1, 0, 0, std::array<glm::vec2, 3>{
		glm::vec2{ -0.7f, 0.9f },
		glm::vec2{ -0.7f, 0.8f },
		glm::vec2{ -0.8f, 0.8f }
	});
	tripair.set_attribute(0, 1, glm::vec4{ 0.8f, 0.5f, 0.3f, 0.7f });
	tripair.set_attribute(1, 1, glm::vec4{ 0.7f, 0.5f, 0.4f, 0.6f });
	tripair.subsend_all_blocks();

	std::string image_vert = vg::io::read_file("shaders/image.vert");
	std::string image_frag = vg::io::read_template_file("shaders/image.frag.tmpl", { { "$NUM_TEXTURE_SLOTS", std::to_string(window.constants().max_texture_image_units)}});
	vg::Shader img_shader(image_vert, image_frag);
	// It is possible to have a vertex buffer with only one vertex's attribute, but only if setting the divisor. This works even with non-instanced rendering
	auto img_layout = std::make_shared<vg::VertexBufferLayout>(img_shader, vg::VertexAttributeSpecificationList{ {}, {}, { { 1, 1 } } });

	vg::CPUVertexBufferBlock sprite(vg::VertexBufferBlock(2, img_layout, { { 0, { 0, 2, 3 } }, { 1, { 1 } } }), { 4, 1 }, false);
	index_buffer.bind_to_vertex_array(sprite.vao());

	sprite.set_attributes(0, 0, 0, std::array<glm::vec2, 4>{
		glm::vec2{ -0.8f, -0.8f },
		glm::vec2{ -0.2f, -0.8f },
		glm::vec2{ -0.2f, -0.2f },
		glm::vec2{ -0.8f, -0.2f }
	});
	sprite.set_attribute(1, 1, GLint(0));
	sprite.set_attributes(0, 2, 0, std::array<glm::vec2, 4>{
		glm::vec2{ 0.0f, 0.0f },
		glm::vec2{ 1.0f, 0.0f },
		glm::vec2{ 1.0f, 1.0f },
		glm::vec2{ 0.0f, 1.0f }
	});
	sprite.set_attribute(0, 3, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	sprite.subsend_all_blocks();

	vg::raii::Image2D img_einstein = vg::load_image_2d("ex/flag.png");
	vg::raii::Texture tex_einstein;
	vg::image_2d::send_texture(img_einstein, tex_einstein);
	vg::texture_params::nearest(vg::texture_params::T2D);

	vg::FrameBufferObject fbo(0, 0, 1440, 1080, { 0.5f, 0.7f, 0.9f, 1.0f });
	fbo.bind();

	vg::raii::Texture color_texture;
	vg::image_2d::send_texture(1440, 1080, 4, color_texture);
	vg::texture_params::nearest(vg::texture_params::T2D);
	vg::raii::Texture normal_texture;
	vg::image_2d::send_texture(1440, 1080, 4, normal_texture);
	vg::texture_params::nearest(vg::texture_params::T2D);

	vg::raii::Texture depth_texture;
	vg::bind_texture(depth_texture, vg::TextureTarget::T2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1440, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	vg::texture_params::nearest(vg::texture_params::T2D);

	fbo.attach_texture(color_texture, vg::framebuffers::Attachment::COLOR0);
	fbo.attach_texture(normal_texture, vg::framebuffers::Attachment::COLOR1);
	fbo.attach_texture(depth_texture, vg::framebuffers::Attachment::DEPTH);
	VANGUARD_ASSERT(vg::framebuffers::is_complete(fbo.framebuffer()));

	const vg::framebuffers::Attachment draw_attachments[] = {vg::framebuffers::Attachment::COLOR0, vg::framebuffers::Attachment::COLOR1};
	fbo.draw_into(draw_attachments, 2);

	window.render_frame = [&]() {
		fbo.begin_render();
		fbo.draw_into(draw_attachments, 2);

		vg::bind_shader(shader);
		vertex_buffer.bind_vao();
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());

		vertex_buffer.bind_vb();
		vertex_buffer.ref<glm::vec4>(0, 1).x = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 0 * glm::pi<float>() / 3)));
		vertex_buffer.subsend_single(0, 1);
		vertex_buffer.ref<glm::vec4>(1, 1).y = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 1 * glm::pi<float>() / 3)));
		vertex_buffer.subsend_single(1, 1);
		vertex_buffer.ref<glm::vec4>(2, 1).z = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 2 * glm::pi<float>() / 3)));
		vertex_buffer.subsend_single(2, 1);

		fbo.draw_into(vg::framebuffers::Attachment::COLOR0);

		white_square.ref<glm::vec2>(0, 0, 0).x -= float(0.008f * glm::sin(glfwGetTime() * 20.0f));
		white_square.bind_vb(0);
		white_square.subsend_single(0, 0, 0);

		white_square.bind_vao();
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());

		tripair.bind_vao(0);
		vg::draw::arrays(vg::DrawMode::TRIANGLES, 0, tripair.vertex_count(0)); // TODO arrays draw function abstraction on vertex buffer classes, and elements draw function abstraction on index buffer classes.
		tripair.bind_vao(1);
		vg::draw::arrays(vg::DrawMode::TRIANGLES, 0, tripair.vertex_count(1));

		window.unbind_framebuffer();
		vg::tex::barrier();

		vg::bind_shader(img_shader);
		
		vg::select_texture_slot(0);
		vg::bind_texture(tex_einstein, vg::TextureTarget::T2D);
		vg::select_texture_slot(1);
		vg::bind_texture(color_texture, vg::TextureTarget::T2D);
		vg::select_texture_slot(2);
		vg::bind_texture(normal_texture, vg::TextureTarget::T2D);
		
		sprite.bind_vao();
		sprite.bind_vb(1);
		
		sprite.set_attribute(1, 1, GLint(0));
		sprite.subsend_full(1);
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());
		
		sprite.set_attribute(1, 1, GLint(1));
		sprite.subsend_full(1);
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());
		
		sprite.set_attribute(1, 1, GLint(2));
		sprite.subsend_full(1);
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());

		sprite.ref<glm::vec2>(0, 0, 0).x += 0.002f;
		sprite.bind_vb(0);
		sprite.subsend_single(0, 0, 0);
		};

	// TODO Interleaved Vertex Buffers. Not a separate VertexBuffer class, but a different struct that doesn't even have a reference to any buffers/VAOs. All it stores is offsets and object sizes.

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
