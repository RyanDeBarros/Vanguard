#include <iostream>
#include <string>
#include <array>

#include "Vanguard.h"

#include "raii/Window.h"
#include "Renderable.h"
#include "Draw.h"
#include "utils/IO.h"
#include "raii/Texture.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;
	vg::init();

	vg::Window window(1440, 1080, "Hello World");

	vg::set_clear_color({ 0.5f, 0.7f, 0.9f, 1.0f });

	vg::Shader shader(vg::FilePath("shaders/color.vert"), vg::FilePath("shaders/color.frag"));
	auto vb_layout = std::make_shared<vg::VertexBufferLayout>(shader);

	vg::VertexBuffer vertex_buffer(vb_layout);
	vg::VoidArray cpubuf = vertex_buffer.init_immutable_cpu_buffer(4);

	vertex_buffer.set_attributes(cpubuf, 0, 0, std::array<glm::vec2, 4>{
		glm::vec2{ -0.5f, -0.5f },
		glm::vec2{  0.5f, -0.5f },
		glm::vec2{  0.0f,  0.5f },
		glm::vec2{  0.8f,  0.8f }
		});
	vertex_buffer.set_attribute(cpubuf, 1, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	vertex_buffer.bind_vb();
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, cpubuf.size(), cpubuf);
	// TODO create CPUVertexBuffer that abstracts this kind of stuff. Then, there could be a series of Renderable classes that hold VertexBuffer/VertexBufferBlock/MultiVertexBuffer, VertexBufferLayout, and CPUVertexBuffer.
	
	vg::CPUIndexBuffer index_buffer(vg::IndexDataType::UBYTE);
	index_buffer.bind_to_vertex_array(vertex_buffer.vao());
	index_buffer.init_immutable_quads(1);

	vg::VertexBufferBlock white_square(2, vb_layout, { { 0, { 0 } }, { 1, { 1 } } });
	vg::VoidArray wsbuf0 = white_square.init_immutable_cpu_buffer(0, 4);
	vg::VoidArray wsbuf1 = white_square.init_immutable_cpu_buffer(1, 4);

	white_square.set_attributes(wsbuf0, 0, 0, 0, std::array<glm::vec2, 4>{
		glm::vec2{ 0.7f, -0.7f },
		glm::vec2{ 0.9f, -0.7f },
		glm::vec2{ 0.9f, -0.9f },
		glm::vec2{ 0.7f, -0.9f }
	});
	white_square.bind_vb(0);
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, wsbuf0.size(), wsbuf0);
	white_square.set_attribute(wsbuf1, 1, 1, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	white_square.bind_vb(1);
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, wsbuf1.size(), wsbuf1);
	index_buffer.bind_to_vertex_array(white_square.vao());

	vg::MultiVertexBuffer tripair({ vb_layout, vb_layout });
	vg::VoidArray tribuf0 = tripair.init_immutable_cpu_buffer(0, 3);
	vg::VoidArray tribuf1 = tripair.init_immutable_cpu_buffer(1, 3);

	tripair.set_attributes(0, tribuf0, 0, 0, std::array<glm::vec2, 3>{
		glm::vec2{ -0.9f, 0.9f },
		glm::vec2{ -0.8f, 0.9f },
		glm::vec2{ -0.9f, 0.8f }
	});
	tripair.set_attributes(1, tribuf1, 0, 0, std::array<glm::vec2, 3>{
		glm::vec2{ -0.7f, 0.9f },
		glm::vec2{ -0.7f, 0.8f },
		glm::vec2{ -0.8f, 0.8f }
	});
	tripair.set_attribute(0, tribuf0, 1, glm::vec4{ 0.8f, 0.5f, 0.3f, 0.7f });
	tripair.set_attribute(1, tribuf1, 1, glm::vec4{ 0.7f, 0.5f, 0.4f, 0.6f });
	tripair.bind_vb(0);
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, tribuf0.size(), tribuf0);
	tripair.bind_vb(1);
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, tribuf1.size(), tribuf1);

	std::string image_vert = vg::io::read_file("shaders/image.vert");
	std::string image_frag = vg::io::read_template_file("shaders/image.frag.tmpl", { { "$NUM_TEXTURE_SLOTS", std::to_string(window.constants().max_texture_image_units)}});
	vg::Shader img_shader(image_vert, image_frag);
	auto img_layout = std::make_shared<vg::VertexBufferLayout>(img_shader);
	vg::VertexBuffer sprite(img_layout);
	
	index_buffer.bind_to_vertex_array(sprite.vao());

	auto sprite_buf = sprite.init_immutable_cpu_buffer(4);
	sprite.set_attributes(sprite_buf, 0, 0, std::array<glm::vec2, 4>{
		glm::vec2{ -0.8f, -0.8f },
		glm::vec2{ -0.2f, -0.8f },
		glm::vec2{ -0.2f, -0.2f },
		glm::vec2{ -0.8f, -0.2f }
	});
	sprite.set_attribute(sprite_buf, 1, GLint(0)); // TODO images should use a block, with texture slot in a different buffer, since it updates more than other attributes.
	sprite.set_attributes(sprite_buf, 2, 0, std::array<glm::vec2, 4>{
		glm::vec2{ 0.0f, 0.0f },
		glm::vec2{ 1.0f, 0.0f },
		glm::vec2{ 1.0f, 1.0f },
		glm::vec2{ 0.0f, 1.0f }
	});
	sprite.set_attribute(sprite_buf, 3, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	sprite.bind_vb();
	vg::buffers::subsend(vg::BufferTarget::VERTEX, 0, sprite_buf.size(), sprite_buf);

	vg::ImageTexture tex_einstein(vg::load_image("ex/flag.png"));

	GLboolean blendEnabled;
	glGetBooleanv(GL_BLEND, &blendEnabled);
	printf("Blending enabled: %d\n", blendEnabled);

	window.render_frame = [&]() {
		vg::bind_shader(shader);
		vertex_buffer.bind_vao();
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());

		auto offset0 = vertex_buffer.layout()->buffer_offset(0, 1) + 0 * sizeof(float);
		cpubuf.ref<float>(offset0) = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 0 * glm::pi<float>() / 3)));
		vertex_buffer.bind_vb();
		vg::buffers::subsend(vg::BufferTarget::VERTEX, offset0, sizeof(float), cpubuf[offset0]);

		auto offset1 = vertex_buffer.layout()->buffer_offset(1, 1) + 1 * sizeof(float);
		cpubuf.ref<float>(offset1) = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 1 * glm::pi<float>() / 3)));
		vertex_buffer.bind_vb();
		vg::buffers::subsend(vg::BufferTarget::VERTEX, offset1, sizeof(float), cpubuf[offset1]);

		auto offset2 = vertex_buffer.layout()->buffer_offset(2, 1) + 2 * sizeof(float);
		cpubuf.ref<float>(offset2) = glm::sqrt(0.5f * (1.0f + (float)glm::sin(glfwGetTime() + 2 * glm::pi<float>() / 3)));
		vertex_buffer.bind_vb();
		vg::buffers::subsend(vg::BufferTarget::VERTEX, offset2, sizeof(float), cpubuf[offset2]);

		white_square.ref<glm::vec2>(wsbuf0, 0, 0, 0).x -= float(0.008f * glm::sin(glfwGetTime() * 20.0f));
		white_square.bind_vb(0);
		vg::buffers::subsend(vg::BufferTarget::VERTEX, white_square.buffer_offset(0, 0, 0), sizeof(glm::vec2), wsbuf0);

		white_square.bind_vao();
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());

		tripair.bind_vao(0);
		vg::draw::arrays(vg::DrawMode::TRIANGLES, 0, tripair.vertex_count(0, tribuf0));
		tripair.bind_vao(1);
		vg::draw::arrays(vg::DrawMode::TRIANGLES, 0, tripair.vertex_count(1, tribuf1));

		vg::bind_shader(img_shader);
		tex_einstein.bind(0);
		sprite.bind_vao();
		vg::draw::elements(vg::DrawMode::TRIANGLES, index_buffer.size(), 0, index_buffer.data_type());
		};

	// TODO Interleaved Vertex Buffers. Not a separate VertexBuffer class, but a different struct that doesn't even have a reference to any buffers/VAOs. All it stores is offsets and object sizes.

	for (;;)
	{
		glfwPollEvents();
		if (window.should_close())
			break;
		window.frame_cycle();
	}

	vg::delete_image(tex_einstein.image()); // TODO raii Image ?

	vg::terminate();
	return 0;
}
