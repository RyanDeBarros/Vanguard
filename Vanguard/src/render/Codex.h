#pragma once

#include "Draw.h"
#include "Transform.h"
#include "ShaderRegistry.h"
#include "TextureRegistry.h"

namespace vg
{
	namespace codex
	{
		struct IRenderCodex
		{
			using AttributePosition = GLuint;

			struct AttribEntry
			{
				DrawMode draw_mode;
				AttributePosition attrib;
				GLuint vertex;
				GLuint size = 0;
			};

			virtual void transmit_data(AttribEntry entry, void* destination) = 0;
			virtual void transmit_full_data(DrawMode draw_mode, void* destination, GLuint size = 0) = 0;
		};

		struct AbsoluteImage : public IRenderCodex
		{
			static ids::Shader load_shader(const Window& window, ShaderRegistry& registry);
			static GLuint natural_vertex_count();

			enum AttributePositions
			{
				VERTEX_POSITION,
				TEXTURE_SLOT,
				UVS,
				MODEL_MATRIX
			};

			Transform2D transform = {};
			GLuint texture_slot = 0;
			glm::vec2 size = { 1.0f, 1.0f };
			glm::vec2 pivot = { 0.5f, 0.5f };
			Rect<float> uv_rect = { 0.0f, 0.0f, 1.0f, 1.0f };

			virtual void transmit_data(AttribEntry entry, void* destination) override;
			virtual void transmit_full_data(DrawMode draw_mode, void* destination, GLuint size = 0);
		};

		struct AbsoluteImageColored : public IRenderCodex
		{
			static ids::Shader load_shader(const Window& window, ShaderRegistry& registry);
			static GLuint natural_vertex_count();

			enum AttributePositions
			{
				VERTEX_POSITION,
				TEXTURE_SLOT,
				UVS,
				MODEL_MATRIX,
				COLOR
			};

			Transform2D transform = {};
			GLuint texture_slot = 0;
			glm::vec2 size = { 1.0f, 1.0f };
			glm::vec2 pivot = { 0.5f, 0.5f };
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Rect<float> uv_rect = { 0.0f, 0.0f, 1.0f, 1.0f };

			virtual void transmit_data(AttribEntry entry, void* destination) override;
			virtual void transmit_full_data(DrawMode draw_mode, void* destination, GLuint size = 0);
		};

		struct Sprite2D : public IRenderCodex
		{
			static ids::Shader load_shader(const Window& window, ShaderRegistry& registry);
			static GLuint natural_vertex_count();

			enum AttributePositions
			{
				VERTEX_POSITION,
				TEXTURE_SLOT,
				UVS,
				MODEL_MATRIX
			};

			Transformer2D transformer = {};
			GLuint texture_slot = 0;
			glm::vec2 size = { 1.0f, 1.0f };
			glm::vec2 pivot = { 0.5f, 0.5f };
			Rect<float> uv_rect = { 0.0f, 0.0f, 1.0f, 1.0f };

			virtual void transmit_data(AttribEntry entry, void* destination) override;
			virtual void transmit_full_data(DrawMode draw_mode, void* destination, GLuint size = 0);
		};

		struct Sprite2DColored : public IRenderCodex
		{
			static ids::Shader load_shader(const Window& window, ShaderRegistry& registry);
			static GLuint natural_vertex_count();

			enum AttributePositions
			{
				VERTEX_POSITION,
				TEXTURE_SLOT,
				UVS,
				MODEL_MATRIX,
				COLOR
			};

			Transformer2D transformer = {};
			GLuint texture_slot = 0;
			glm::vec2 size = { 1.0f, 1.0f };
			glm::vec2 pivot = { 0.5f, 0.5f };
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Rect<float> uv_rect = { 0.0f, 0.0f, 1.0f, 1.0f };

			virtual void transmit_data(AttribEntry entry, void* destination) override;
			virtual void transmit_full_data(DrawMode draw_mode, void* destination, GLuint size = 0);
		};
	}
}
