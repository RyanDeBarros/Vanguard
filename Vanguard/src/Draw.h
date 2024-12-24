#pragma once

#include "raii/GLBuffer.h"

namespace vg
{
	enum class DrawMode
	{
		POINTS = GL_POINTS,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP,
		LINES = GL_LINES,
		LINE_STRIP_ADJENCY = GL_LINE_STRIP_ADJACENCY,
		LINES_ADJENCY = GL_LINES_ADJACENCY,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		PATCHES = GL_PATCHES
	};

	namespace draw
	{
		extern void arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count);
		extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, IndexDataType idt);
		extern void element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, IndexDataType idt);

		namespace instanced
		{
			extern void arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count);
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, IndexDataType idt);

			extern void arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count, GLuint first_instance);
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLuint first_instance, IndexDataType idt);
		}

		namespace base_vertex
		{
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt);
			extern void element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt);
		}

		namespace instanced_base_vertex
		{
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, IndexDataType idt);
			extern void elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, GLuint first_instance, IndexDataType idt);
		}

		namespace multi
		{
			extern void arrays(DrawMode mode, const GLint* first_vertices, const GLsizei* vertex_counts, GLsizei drawcount);
			extern void elements(DrawMode mode, const GLsizei* index_counts, const GLsizeiptr* first_index_bytes, IndexDataType idt, GLsizei drawcount);
			extern void elements_base_vertex(DrawMode mode, GLsizei* index_counts, const GLintptr* first_index_bytes, IndexDataType idt, GLint* base_vertices, GLsizei drawcount);
		}

		extern void indirect(const GPUIndirectArrays& indirect, DrawMode mode);
		extern void indirect(const GPUIndirectArraysBlock& indirect, GLuint i, DrawMode mode);
		extern void multi_indirect(const GPUIndirectArraysBlock& indirect, DrawMode mode, GLuint first, GLuint count);
		extern void indirect(const CPUIndirectArrays& indirect, DrawMode mode);
		
		extern void indirect(const GPUIndirectElements& indirect, DrawMode mode, IndexDataType idt);
		extern void indirect(const GPUIndirectElementsBlock& indirect, GLuint i, DrawMode mode, IndexDataType idt);
		extern void multi_indirect(const GPUIndirectElementsBlock& indirect, DrawMode mode, GLuint first, GLuint count, IndexDataType idt);
		extern void indirect(const CPUIndirectElements& indirect, DrawMode mode);
	}
}
