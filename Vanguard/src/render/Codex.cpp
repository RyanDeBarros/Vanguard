#include "Codex.h"

vg::ids::Shader vg::codex::AbsoluteImage::load_shader(const Window& window, ShaderRegistry& registry)
{
	return registry.load_shader("vg/shaders/sprite2d.vert", {}, "vg/shaders/sprite2d.frag.tmpl", file_templates::num_texture_slots(window));
}

GLuint vg::codex::AbsoluteImage::natural_vertex_count()
{
	return 4;
}

vg::ids::Shader vg::codex::AbsoluteImageColored::load_shader(const Window& window, ShaderRegistry& registry)
{
	return registry.load_shader("vg/shaders/sprite2d_colored.vert", {}, "vg/shaders/sprite2d_colored.frag.tmpl", file_templates::num_texture_slots(window));
}

GLuint vg::codex::AbsoluteImageColored::natural_vertex_count()
{
	return 4;
}

vg::ids::Shader vg::codex::Sprite2D::load_shader(const Window& window, ShaderRegistry& registry)
{
	return registry.load_shader("vg/shaders/sprite2d.vert", {}, "vg/shaders/sprite2d.frag.tmpl", file_templates::num_texture_slots(window));
}

GLuint vg::codex::Sprite2D::natural_vertex_count()
{
	return 4;
}

vg::ids::Shader vg::codex::Sprite2DColored::load_shader(const Window& window, ShaderRegistry& registry)
{
	return registry.load_shader("vg/shaders/sprite2d_colored.vert", {}, "vg/shaders/sprite2d_colored.frag.tmpl", file_templates::num_texture_slots(window));
}

GLuint vg::codex::Sprite2DColored::natural_vertex_count()
{
	return 4;
}

void vg::codex::AbsoluteImage::transmit_data(AttribEntry entry, void* destination)
{
	static const std::array<glm::vec2, 4> vps_triangles = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 1, 1 },
		glm::vec2{ 0, 1 }
	};
	static const std::array<glm::vec2, 4> vps_triangle_strip = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 0, 1 },
		glm::vec2{ 1, 1 }
	};

	switch (entry.attrib)
	{
	case AttributePositions::VERTEX_POSITION:
	{
		glm::vec2 vp;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			vp = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			vp = vps_triangle_strip[entry.vertex];
		else
			return;
		vp = (vp - pivot) * size;
		memcpy(destination, &vp, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::TEXTURE_SLOT:
		memcpy(destination, &texture_slot, entry.size ? std::min(entry.size, (GLuint)sizeof(decltype(texture_slot))) : sizeof(decltype(texture_slot)));
		break;
	case AttributePositions::UVS:
	{
		glm::vec2 uvs;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			uvs = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			uvs = vps_triangle_strip[entry.vertex];
		else
			return;
		uvs.x = uv_rect.x + uvs.x * uv_rect.w;
		uvs.y = uv_rect.y + uvs.y * uv_rect.h;
		memcpy(destination, &uvs, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::MODEL_MATRIX:
		memcpy(destination, glm::value_ptr(transform.matrix()), entry.size ? std::min(entry.size, (GLuint)sizeof(glm::mat3)) : sizeof(glm::mat3));
		break;
	}
}

void vg::codex::AbsoluteImage::transmit_full_data(DrawMode draw_mode, void* destination, GLuint size)
{
	AttribEntry entry;
	entry.draw_mode = draw_mode;
	entry.size = size;
	static const GLuint attrib_count = 4;
	static const GLuint attrib_sizes[attrib_count] = { sizeof(glm::vec2), sizeof(decltype(texture_slot)), sizeof(glm::vec2), sizeof(glm::mat3) };
	static const AttributePositions positions[attrib_count] = { AttributePositions::VERTEX_POSITION, AttributePositions::TEXTURE_SLOT, AttributePositions::UVS, AttributePositions::MODEL_MATRIX };
	for (GLuint i = 0; i < natural_vertex_count(); ++i)
	{
		entry.vertex = i;
		for (GLuint j = 0; j < attrib_count; ++j)
		{
			entry.attrib = positions[j];
			transmit_data(entry, destination);
			destination = (char*)destination + attrib_sizes[j];
			entry.size -= attrib_sizes[j];
		}
	}
}

void vg::codex::AbsoluteImageColored::transmit_data(AttribEntry entry, void* destination)
{
	static const std::array<glm::vec2, 4> vps_triangles = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 1, 1 },
		glm::vec2{ 0, 1 }
	};
	static const std::array<glm::vec2, 4> vps_triangle_strip = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 0, 1 },
		glm::vec2{ 1, 1 }
	};

	switch (entry.attrib)
	{
	case AttributePositions::VERTEX_POSITION:
	{
		glm::vec2 vp;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			vp = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			vp = vps_triangle_strip[entry.vertex];
		else
			return;
		vp = (vp - pivot) * size;
		memcpy(destination, &vp, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::TEXTURE_SLOT:
		memcpy(destination, &texture_slot, entry.size ? std::min(entry.size, (GLuint)sizeof(decltype(texture_slot))) : sizeof(decltype(texture_slot)));
		break;
	case AttributePositions::UVS:
	{
		glm::vec2 uvs;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			uvs = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			uvs = vps_triangle_strip[entry.vertex];
		else
			return;
		uvs.x = uv_rect.x + uvs.x * uv_rect.w;
		uvs.y = uv_rect.y + uvs.y * uv_rect.h;
		memcpy(destination, &uvs, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::MODEL_MATRIX:
		memcpy(destination, glm::value_ptr(transform.matrix()), entry.size ? std::min(entry.size, (GLuint)sizeof(glm::mat3)) : sizeof(glm::mat3));
		break;
	case AttributePositions::COLOR:
		memcpy(destination, glm::value_ptr(color), entry.size ? std::min(entry.size, (GLuint)sizeof(decltype(color))) : sizeof(decltype(color)));
		break;
	}
}

void vg::codex::AbsoluteImageColored::transmit_full_data(DrawMode draw_mode, void* destination, GLuint size)
{
	AttribEntry entry;
	entry.draw_mode = draw_mode;
	entry.size = size;
	static const GLuint attrib_count = 5;
	static const GLuint attrib_sizes[attrib_count] = { sizeof(glm::vec2), sizeof(decltype(texture_slot)), sizeof(glm::vec2), sizeof(glm::mat3), sizeof(glm::vec4) };
	static const AttributePositions positions[attrib_count] = { AttributePositions::VERTEX_POSITION, AttributePositions::TEXTURE_SLOT, AttributePositions::UVS, AttributePositions::MODEL_MATRIX, AttributePositions::COLOR };
	for (GLuint i = 0; i < natural_vertex_count(); ++i)
	{
		entry.vertex = i;
		for (GLuint j = 0; j < attrib_count; ++j)
		{
			entry.attrib = positions[j];
			transmit_data(entry, destination);
			destination = (char*)destination + attrib_sizes[j];
			entry.size -= attrib_sizes[j];
		}
	}
}

void vg::codex::Sprite2D::transmit_data(AttribEntry entry, void* destination)
{
	static const std::array<glm::vec2, 4> vps_triangles = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 1, 1 },
		glm::vec2{ 0, 1 }
	};
	static const std::array<glm::vec2, 4> vps_triangle_strip = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 0, 1 },
		glm::vec2{ 1, 1 }
	};

	switch (entry.attrib)
	{
	case AttributePositions::VERTEX_POSITION:
	{
		glm::vec2 vp;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			vp = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			vp = vps_triangle_strip[entry.vertex];
		else
			return;
		vp = (vp - pivot) * size;
		memcpy(destination, &vp, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::TEXTURE_SLOT:
		memcpy(destination, &texture_slot, entry.size ? std::min(entry.size, (GLuint)sizeof(decltype(texture_slot))) : sizeof(decltype(texture_slot)));
		break;
	case AttributePositions::UVS:
	{
		glm::vec2 uvs;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			uvs = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			uvs = vps_triangle_strip[entry.vertex];
		else
			return;
		uvs.x = uv_rect.x + uvs.x * uv_rect.w;
		uvs.y = uv_rect.y + uvs.y * uv_rect.h;
		memcpy(destination, &uvs, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::MODEL_MATRIX:
		transformer.sync();
		memcpy(destination, glm::value_ptr(transformer.global()), entry.size ? std::min(entry.size, (GLuint)sizeof(glm::mat3)) : sizeof(glm::mat3));
		break;
	}
}

void vg::codex::Sprite2D::transmit_full_data(DrawMode draw_mode, void* destination, GLuint size)
{
	AttribEntry entry;
	entry.draw_mode = draw_mode;
	bool limit_size = size > 0;
	entry.size = size;
	static const GLuint attrib_count = 4;
	static const GLuint attrib_sizes[attrib_count] = { sizeof(glm::vec2), sizeof(decltype(texture_slot)), sizeof(glm::vec2), sizeof(glm::mat3) };
	static const AttributePositions positions[attrib_count] = { AttributePositions::VERTEX_POSITION, AttributePositions::TEXTURE_SLOT, AttributePositions::UVS, AttributePositions::MODEL_MATRIX };
	for (GLuint i = 0; i < natural_vertex_count(); ++i)
	{
		entry.vertex = i;
		for (GLuint j = 0; j < attrib_count; ++j)
		{
			entry.attrib = positions[j];
			transmit_data(entry, destination);
			destination = (char*)destination + attrib_sizes[j];
			if (limit_size)
				entry.size -= attrib_sizes[j];
		}
	}
}

void vg::codex::Sprite2DColored::transmit_data(AttribEntry entry, void* destination)
{
	static const std::array<glm::vec2, 4> vps_triangles = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 1, 1 },
		glm::vec2{ 0, 1 }
	};
	static const std::array<glm::vec2, 4> vps_triangle_strip = {
		glm::vec2{ 0, 0 },
		glm::vec2{ 1, 0 },
		glm::vec2{ 0, 1 },
		glm::vec2{ 1, 1 }
	};

	switch (entry.attrib)
	{
	case AttributePositions::VERTEX_POSITION:
	{
		glm::vec2 vp;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			vp = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			vp = vps_triangle_strip[entry.vertex];
		else
			return;
		vp = (vp - pivot) * size;
		memcpy(destination, &vp, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::TEXTURE_SLOT:
		memcpy(destination, &texture_slot, entry.size ? std::min(entry.size, (GLuint)sizeof(decltype(texture_slot))) : sizeof(decltype(texture_slot)));
		break;
	case AttributePositions::UVS:
	{
		glm::vec2 uvs;
		if (entry.draw_mode == DrawMode::TRIANGLES || entry.draw_mode == DrawMode::TRIANGLE_FAN)
			uvs = vps_triangles[entry.vertex];
		else if (entry.draw_mode == DrawMode::TRIANGLE_STRIP)
			uvs = vps_triangle_strip[entry.vertex];
		else
			return;
		uvs.x = uv_rect.x + uvs.x * uv_rect.w;
		uvs.y = uv_rect.y + uvs.y * uv_rect.h;
		memcpy(destination, &uvs, entry.size ? std::min(entry.size, (GLuint)sizeof(glm::vec2)) : sizeof(glm::vec2));
		break;
	}
	case AttributePositions::MODEL_MATRIX:
		transformer.sync();
		memcpy(destination, glm::value_ptr(transformer.global()), entry.size ? std::min(entry.size, (GLuint)sizeof(glm::mat3)) : sizeof(glm::mat3));
		break;
	case AttributePositions::COLOR:
		memcpy(destination, glm::value_ptr(color), entry.size ? std::min(entry.size, (GLuint)sizeof(decltype(color))) : sizeof(decltype(color)));
		break;
	}
}

void vg::codex::Sprite2DColored::transmit_full_data(DrawMode draw_mode, void* destination, GLuint size)
{
	AttribEntry entry;
	entry.draw_mode = draw_mode;
	entry.size = size;
	static const GLuint attrib_count = 5;
	static const GLuint attrib_sizes[attrib_count] = { sizeof(glm::vec2), sizeof(decltype(texture_slot)), sizeof(glm::vec2), sizeof(glm::mat3), sizeof(glm::vec4) };
	static const AttributePositions positions[attrib_count] = { AttributePositions::VERTEX_POSITION, AttributePositions::TEXTURE_SLOT, AttributePositions::UVS, AttributePositions::MODEL_MATRIX, AttributePositions::COLOR };
	for (GLuint i = 0; i < natural_vertex_count(); ++i)
	{
		entry.vertex = i;
		for (GLuint j = 0; j < attrib_count; ++j)
		{
			entry.attrib = positions[j];
			transmit_data(entry, destination);
			destination = (char*)destination + attrib_sizes[j];
			entry.size -= attrib_sizes[j];
		}
	}
}
