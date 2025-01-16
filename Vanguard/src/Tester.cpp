#include <iostream>
#include <string>
#include <array>

#include "Vanguard.h"

#include "render/Codex.h"

int main()
{
	std::cout << "Welcome to Vanguard!" << std::endl;
	vg::init();

	vg::Window window(1920, 1080, "Hello World");
	window.clear_color = { 0.2f, 0.4f, 0.7f, 1.0f };

	vg::ShaderRegistry shaders;
	vg::TransientTextureRegistry textures;
	
	auto color_shader = shaders.load_shader("shaders/color.vert", "shaders/color.frag");
	vg::bind_shader(color_shader);
	vg::uniforms::send_3x3(*shaders.ref_shader(color_shader), "uVP", window.orthographic_projection());
	window.vp_updates.push_back({ shaders.ref_shader(color_shader), vg::Window::ProjectionMode::ORTHOGRAPHIC_2D });

	auto vb_color_layout = std::make_shared<vg::VertexBufferLayout>(*shaders.ref_shader(color_shader));

	vg::CPUVertexBuffer colorful_vertex_buffer(vg::VertexBuffer(vb_color_layout), 4, false);

	colorful_vertex_buffer.set_attributes(0, 0, std::array<glm::vec2, 4>{
		window.convert_coordinates({ -0.5f, -0.5f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  0.5f, -0.5f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  0.0f,  0.5f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({  0.8f,  0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
		});
	colorful_vertex_buffer.set_attribute(1, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	colorful_vertex_buffer.set_attribute(2, glm::mat3(1.0f));
	colorful_vertex_buffer.bind_vb();
	colorful_vertex_buffer.subsend_full();

	vg::CPUIndexBuffer index_buffer(vg::IndexDataType::UBYTE);
	index_buffer.bind_to_vertex_array(colorful_vertex_buffer.vao());
	index_buffer.init_immutable_units(1, vg::index_buffer::triangles::quad_template);

	vg::CPUVertexBufferBlock white_square(vg::VertexBufferBlock(vb_color_layout, { { 0, 2, 3, 4 }, { 1 } }), 4, false);

	white_square.block_index() = 0;
	white_square.set_attributes(0, 0, std::array<glm::vec2, 4>{
		window.convert_coordinates({ 0.7f, -0.7f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ 0.9f, -0.7f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ 0.9f, -0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ 0.7f, -0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
	});
	white_square.set_attribute(2, glm::mat3(1.0f));
	white_square.block_index() = 1;
	white_square.set_attribute(1, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	white_square.subsend_all_blocks();
	index_buffer.bind_to_vertex_array(white_square.vao());

	vg::CompactVBBlockIndexer tripair_indexer({ { 3, 3 }, { 1, 1 } });
	auto vb_color_split_layout = std::make_shared<vg::VertexBufferLayout>(*shaders.ref_shader(color_shader), vg::VertexAttributeSpecificationList{ {}, {}, { { 2, 1 }, { 3, 1 }, { 4, 1 } } });
	vg::CPUVertexBufferBlock tripair(vg::VertexBufferBlock(vb_color_split_layout, { { 0, 1 }, { 2, 3, 4 } }), { tripair_indexer.vertex_count(0), tripair_indexer.vertex_count(1) }, false);

	tripair.block_index() = 0;
	tripair.set_attributes(0, tripair_indexer.vertex(0, 0, 0), std::array<glm::vec2, 3>{
		window.convert_coordinates({ -0.9f, 0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.8f, 0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.9f, 0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
	});
	tripair.set_attributes(0, tripair_indexer.vertex(0, 1, 0), std::array<glm::vec2, 3>{
		window.convert_coordinates({ -0.7f, 0.9f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.7f, 0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN),
		window.convert_coordinates({ -0.8f, 0.8f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN)
	});
	tripair.set_attribute(1, tripair_indexer.vertex(0, 0, 0), tripair_indexer.vertex_count(0, 0), glm::vec4{ 0.8f, 0.5f, 0.3f, 1.0f });
	tripair.set_attribute(1, tripair_indexer.vertex(0, 1, 0), tripair_indexer.vertex_count(0, 1), glm::vec4{ 0.6f, 0.5f, 0.5f, 1.0f });
	tripair.block_index() = 1;
	tripair.set_attribute(2, tripair_indexer.vertex(1, 0, 0), tripair_indexer.vertex_count(1, 0), glm::mat3(1.0f));
	tripair.set_attribute(2, tripair_indexer.vertex(1, 1, 0), tripair_indexer.vertex_count(1, 1), glm::mat3({ 0.8f, 0.0f, 0.0f }, { 0.0f, 0.8f, 0.0f }, { 0.0f, 0.0f, 1.0f }));
	tripair.subsend_all_blocks();

	auto img_shader = shaders.load_shader("shaders/image.vert", {}, "shaders/image.frag.tmpl", vg::file_templates::num_texture_slots(window));
	vg::bind_shader(img_shader);
	vg::uniforms::send_3x3(*shaders.ref_shader(img_shader), "uVP", window.orthographic_projection());
	window.vp_updates.push_back({ shaders.ref_shader(img_shader), vg::Window::ProjectionMode::ORTHOGRAPHIC_2D });

	// It is possible to have a vertex buffer with only one vertex's attribute, but only if setting the divisor. This works even with non-instanced rendering
	auto img_layout = std::make_shared<vg::VertexBufferLayout>(*shaders.ref_shader(img_shader), vg::VertexAttributeSpecificationList{ {}, {}, { { 1, 1 }, { 4, 1 }, { 5, 1 }, { 6, 1 } } });

	vg::CPUVertexBufferBlock sprite(vg::VertexBufferBlock(img_layout, { { 0, 2, 3 }, { 1 }, { 4, 5, 6 } }), { 4, 1, 1 }, false);
	index_buffer.bind_to_vertex_array(sprite.vao());

	sprite.block_index() = 0;
	sprite.set_attributes(0, 0, vg::quad_vertex_positions({ 34, 27 }, { 0.0f, 1.0f }));
	sprite.set_attributes(2, 0, vg::quad_full_uvs);
	sprite.set_attribute(3, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	sprite.block_index() = 1;
	sprite.set_attribute(1, GLint(0));

	vg::Transformer2D sprite_transformer({ window.convert_coordinates({ 0.5f, 0.25f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN), 0.4f, glm::vec2(10) });
	sprite.block_index() = 2;
	sprite.set_attribute(4, sprite_transformer.global());
	
	sprite.subsend_all_blocks();

	auto tex_einstein = textures.load_texture_2d("ex/flag.png", vg::TextureParams::STANDARD_NEAREST_2D);
	// TODO test gif using T2D_ARRAY. Will require a custom shader for that though.
	auto serotonin_frames = new vg::SeparatedFramesArray("ex/serotonin.gif", *vg::TextureParams::STANDARD_LINEAR_2D);

	vg::Transformer2D sprite_parent({ window.convert_coordinates({ -0.5f, 0.0f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN), 0.0f, { 0.5f, 1.0f } });
	vg::attach_transformer(&sprite_parent, &sprite_transformer);

	vg::Transformer2D sprite_grandparent;
	vg::attach_transformer(&sprite_grandparent, &sprite_parent);

	float frame_index = 0;

	auto sprite_2d_shader = vg::codex::Sprite2D::load_shader(window, shaders);
	vg::bind_shader(sprite_2d_shader);
	vg::uniforms::send_3x3(*shaders.ref_shader(sprite_2d_shader), "uVP", window.orthographic_projection());
	window.vp_updates.push_back({ shaders.ref_shader(sprite_2d_shader), vg::Window::ProjectionMode::ORTHOGRAPHIC_2D });
	auto sprite_2d_layout = std::make_shared<vg::VertexBufferLayout>(*shaders.ref_shader(sprite_2d_shader)); // LATER VertexBufferLayout registry
	vg::codex::Sprite2D flag_codex;
	flag_codex.size = { 34, 27 };
	flag_codex.transformer.local.scale = glm::vec2(20);
	vg::CPUVertexBuffer flag_cvb(vg::VertexBuffer(sprite_2d_layout), vg::codex::Sprite2D::natural_vertex_count(), false);
	flag_codex.transmit_full_data(vg::DrawMode::TRIANGLES, flag_cvb.cpubuf());
	flag_cvb.subsend_full();
	index_buffer.bind_to_vertex_array(flag_cvb.vao());

	window.sync_resize();
	window.render_frame = [&]() {
		vg::bind_shader(color_shader);

		colorful_vertex_buffer.bind_vao();
		vg::draw::index_buffer::full(index_buffer, vg::DrawMode::TRIANGLES);

		colorful_vertex_buffer.bind_vb();
		colorful_vertex_buffer.ref<glm::vec4>(0, 1).x = glm::sqrt(0.5f * (1.0f + glm::sin(vg::data::elapsed_time() + 0 * glm::pi<float>() / 3)));
		colorful_vertex_buffer.subsend_single(0, 1);
		colorful_vertex_buffer.ref<glm::vec4>(1, 1).y = glm::sqrt(0.5f * (1.0f + glm::sin(vg::data::elapsed_time() + 1 * glm::pi<float>() / 3)));
		colorful_vertex_buffer.subsend_single(1, 1);
		colorful_vertex_buffer.ref<glm::vec4>(2, 1).z = glm::sqrt(0.5f * (1.0f + glm::sin(vg::data::elapsed_time() + 2 * glm::pi<float>() / 3)));
		colorful_vertex_buffer.subsend_single(2, 1);

		white_square.block_index() = 0;
		white_square.ref<glm::vec2>(0, 0).x -= window.convert_coordinates({ 0.5f * glm::sin(vg::data::elapsed_time() * 20.0f), 0.0f }, vg::Window::CoordinateSystem::CLIP, vg::Window::CoordinateSystem::SCREEN).x * vg::data::delta_time;
		white_square.bind_vb();
		white_square.subsend_single(0, 0);

		white_square.bind_vao();
		vg::draw::index_buffer::full(index_buffer, vg::DrawMode::TRIANGLES);

		tripair.bind_vao();
		vg::draw::instanced::arrays(vg::DrawMode::TRIANGLES, 0, tripair_indexer.vertex_count(0), tripair_indexer.vertex_count(1), 0);

		vg::bind_shader(img_shader);
		
		vg::select_texture_slot(0);
		serotonin_frames->bind((int)frame_index);
		frame_index = serotonin_frames->increment_frame_index(frame_index);
		
		sprite.bind_vao();
		vg::draw::index_buffer::full(index_buffer, vg::DrawMode::TRIANGLES);

		sprite_grandparent.local.position.x += 60.0f * vg::data::delta_time;
		sprite_grandparent.mark();
		sprite_parent.local.rotation += 1.2f * vg::data::delta_time;
		sprite_parent.mark();
		sprite_transformer.local.rotation -= 0.6f * vg::data::delta_time;
		sprite_transformer.mark();

		sprite_transformer.sync(); // call before sending to vertex buffer
		sprite.block_index() = 2;
		sprite.ref<glm::mat3>(0, 4) = sprite_transformer.global();
		sprite.bind_vb();
		sprite.subsend_single(0, 4, sizeof(glm::mat3));

		vg::bind_shader(sprite_2d_shader);

		vg::select_texture_slot(flag_codex.texture_slot);
		vg::bind_texture(tex_einstein, vg::TextureTarget::T2D);
		flag_cvb.bind_vao();
		vg::draw::index_buffer::full(index_buffer, vg::DrawMode::TRIANGLES);
		};

	VANGUARD_ASSERT_GL_OKAY;
	for (;;)
	{
		vg::new_frame();
		if (window.should_close())
			break;
		window.frame_cycle();
	}
	VANGUARD_ASSERT_GL_OKAY;

	VANGUARD_INVALIDATE_POINTER(serotonin_frames);
	shaders.unload_all();
	textures.unload_all();
	vg::terminate();
	return 0;
}
