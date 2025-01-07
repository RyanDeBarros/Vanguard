#include "Draw.h"

void vg::draw::arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count)
{
	glDrawArrays((GLenum)mode, first_vertex, vertex_count);
}

void vg::draw::elements(DrawMode mode, GLsizei index_count, GLuint first_index, IndexDataType idt)
{
	glDrawElements((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)));
}

void vg::draw::element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, IndexDataType idt)
{
	glDrawRangeElements((GLenum)mode, minimum_index, maximum_index, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)));
}

void vg::draw::instanced::arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count)
{
	glDrawArraysInstanced((GLenum)mode, first_vertex, vertex_count, instance_count);
}

void vg::draw::instanced::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, IndexDataType idt)
{
	glDrawElementsInstanced((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count);
}

void vg::draw::instanced::arrays(DrawMode mode, GLint first_vertex, GLsizei vertex_count, GLsizei instance_count, GLuint first_instance)
{
	glDrawArraysInstancedBaseInstance((GLenum)mode, first_vertex, vertex_count, instance_count, first_instance);
}

void vg::draw::instanced::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLuint first_instance, IndexDataType idt)
{
	glDrawElementsInstancedBaseInstance((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count, first_instance);
}

void vg::draw::base_vertex::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt)
{
	glDrawElementsBaseVertex((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), base_vertex);
}

void vg::draw::base_vertex::element_range(DrawMode mode, GLuint minimum_index, GLuint maximum_index, GLsizei index_count, GLuint first_index, GLint base_vertex, IndexDataType idt)
{
	glDrawRangeElementsBaseVertex((GLenum)mode, minimum_index, maximum_index, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), base_vertex);
}

void vg::draw::instanced_base_vertex::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, IndexDataType idt)
{
	glDrawElementsInstancedBaseVertex((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count, base_vertex);
}

void vg::draw::instanced_base_vertex::elements(DrawMode mode, GLsizei index_count, GLuint first_index, GLsizei instance_count, GLint base_vertex, GLuint first_instance, IndexDataType idt)
{
	glDrawElementsInstancedBaseVertexBaseInstance((GLenum)mode, index_count, (GLenum)idt, (void*)(first_index * index_data_type_size(idt)), instance_count, base_vertex, first_instance);
}

void vg::draw::multi::arrays(DrawMode mode, const GLint* first_vertices, const GLsizei* vertex_counts, GLsizei drawcount)
{
	glMultiDrawArrays((GLenum)mode, first_vertices, vertex_counts, drawcount);
}

void vg::draw::multi::elements(DrawMode mode, const GLsizei* index_counts, const GLsizeiptr* first_index_bytes, IndexDataType idt, GLsizei drawcount)
{
	glMultiDrawElements((GLenum)mode, index_counts, (GLenum)idt, (void**)first_index_bytes, drawcount);
}

void vg::draw::multi::elements_base_vertex(DrawMode mode, GLsizei* index_counts, const GLintptr* first_index_bytes, IndexDataType idt, GLint* base_vertices, GLsizei drawcount)
{
	glMultiDrawElementsBaseVertex((GLenum)mode, index_counts, (GLenum)idt, (void**)first_index_bytes, drawcount, base_vertices);
}

void vg::draw::indirect(const GPUIndirectArrays& indirect, DrawMode mode)
{
	glDrawArraysIndirect((GLenum)mode, (void*)0);
}

void vg::draw::indirect(const GPUIndirectArraysBlock& indirect, GLuint i, DrawMode mode)
{
	if (i >= indirect.get_count())
		throw block_index_out_of_range(indirect.get_count(), i);
	glDrawArraysIndirect((GLenum)mode, (void*)(i * sizeof(IndirectArraysCmd)));
}

void vg::draw::multi_indirect(const GPUIndirectArraysBlock& indirect, DrawMode mode, GLuint first, GLuint count)
{
	GLuint last = first + count - 1;
	if (last >= indirect.get_count())
		throw block_index_out_of_range(indirect.get_count(), last);
	glMultiDrawArraysIndirect((GLenum)mode, (void*)(first * sizeof(IndirectArraysCmd)), count, sizeof(IndirectArraysCmd));
}

void vg::draw::indirect(const CPUIndirectArrays& indirect, DrawMode mode)
{
	draw::indirect(indirect.gpu_arrays(), mode);
}

void vg::draw::indirect(const GPUIndirectElements& indirect, DrawMode mode, IndexDataType idt)
{
	glDrawElementsIndirect((GLenum)mode, (GLenum)idt, 0);
}

void vg::draw::indirect(const GPUIndirectElementsBlock& indirect, GLuint i, DrawMode mode, IndexDataType idt)
{
	if (i >= indirect.get_count())
		throw block_index_out_of_range(indirect.get_count(), i);
	glDrawElementsIndirect((GLenum)mode, (GLenum)idt, (void*)(i * sizeof(IndirectElementsCmd)));
}

void vg::draw::multi_indirect(const GPUIndirectElementsBlock& indirect, DrawMode mode, GLuint first, GLuint count, IndexDataType idt)
{
	GLuint last = first + count - 1;
	if (last >= indirect.get_count())
		throw block_index_out_of_range(indirect.get_count(), last);
	glMultiDrawElementsIndirect((GLenum)mode, (GLenum)idt, (void*)(first * sizeof(IndirectElementsCmd)), count, sizeof(IndirectElementsCmd));
}

void vg::draw::indirect(const CPUIndirectElements& indirect, DrawMode mode)
{
	draw::indirect(indirect.gpu_elements(), mode, indirect.idt);
}

void vg::draw::index_buffer::full(const CPUIndexBuffer& ib, DrawMode mode)
{
	elements(mode, ib.size(), 0, ib.data_type());
}

void vg::draw::index_buffer::part(const CPUIndexBuffer& ib, DrawMode mode, GLuint first)
{
	elements(mode, ib.size() - first, first, ib.data_type());
}

void vg::draw::index_buffer::part(const CPUIndexBuffer& ib, DrawMode mode, GLuint first, GLuint count)
{
	elements(mode, count, first, ib.data_type());
}

void vg::draw::index_buffer::instanced(const CPUIndexBuffer& ib, DrawMode mode, GLuint instance_count)
{
	instanced::elements(mode, ib.size(), 0, instance_count, ib.data_type());
}

void vg::draw::index_buffer::instanced(const CPUIndexBuffer& ib, DrawMode mode, GLuint instance_count, GLuint first)
{
	instanced::elements(mode, ib.size() - first, first, instance_count, ib.data_type());
}

void vg::draw::index_buffer::instanced(const CPUIndexBuffer& ib, DrawMode mode, GLuint instance_count, GLuint first, GLuint count)
{
	instanced::elements(mode, count, first, instance_count, ib.data_type());
}

void vg::draw::index_buffer::instanced_offset(const CPUIndexBuffer& ib, DrawMode mode, GLuint instance_count, GLuint first_instance)
{
	instanced::elements(mode, ib.size(), 0, instance_count, first_instance, ib.data_type());
}

void vg::draw::index_buffer::instanced_offset(const CPUIndexBuffer& ib, DrawMode mode, GLuint instance_count, GLuint first_instance, GLuint first)
{
	instanced::elements(mode, ib.size() - first, first, instance_count, first_instance, ib.data_type());
}

void vg::draw::index_buffer::instanced_offset(const CPUIndexBuffer& ib, DrawMode mode, GLuint instance_count, GLuint first_instance, GLuint first, GLuint count)
{
	instanced::elements(mode, count, first, instance_count, first_instance, ib.data_type());
}

void vg::draw::index_buffer::base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex)
{
	base_vertex::elements(mode, ib.size(), 0, base_vertex, ib.data_type());
}

void vg::draw::index_buffer::base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint first)
{
	base_vertex::elements(mode, ib.size() - first, first, base_vertex, ib.data_type());
}

void vg::draw::index_buffer::base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint first, GLuint count)
{
	base_vertex::elements(mode, count, first, base_vertex, ib.data_type());
}

void vg::draw::index_buffer::instanced_base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint instance_count)
{
	instanced_base_vertex::elements(mode, ib.size(), 0, instance_count, base_vertex, ib.data_type());
}

void vg::draw::index_buffer::instanced_base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint instance_count, GLuint first)
{
	instanced_base_vertex::elements(mode, ib.size() - first, first, instance_count, base_vertex, ib.data_type());
}

void vg::draw::index_buffer::instanced_base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint instance_count, GLuint first, GLuint count)
{
	instanced_base_vertex::elements(mode, count, first, instance_count, base_vertex, ib.data_type());
}

void vg::draw::index_buffer::instanced_offset_base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint instance_count, GLuint first_instance)
{
	instanced_base_vertex::elements(mode, ib.size(), 0, instance_count, base_vertex, first_instance, ib.data_type());
}

void vg::draw::index_buffer::instanced_offset_base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint instance_count, GLuint first_instance, GLuint first)
{
	instanced_base_vertex::elements(mode, ib.size() - first, first, instance_count, base_vertex, first_instance, ib.data_type());
}

void vg::draw::index_buffer::instanced_offset_base_vertex(const CPUIndexBuffer& ib, DrawMode mode, GLuint base_vertex, GLuint instance_count, GLuint first_instance, GLuint first, GLuint count)
{
	instanced_base_vertex::elements(mode, count, first, instance_count, base_vertex, first_instance, ib.data_type());
}
