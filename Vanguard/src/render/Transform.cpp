#include "Transform.h"

vg::Transform2D vg::Transform2D::inverse() const
{
	glm::vec2 inv_scale = 1.0f / scale;
	return { -position * inv_scale, -rotation, inv_scale };
}

glm::mat3 vg::Transform2D::matrix() const
{
	float cos = glm::cos(rotation);
	float sin = glm::sin(rotation);
	return { { scale.x * cos, scale.x * sin, 0.0f }, { -scale.y * sin, scale.y * cos, 0.0f }, glm::vec3(position, 1.0f) };
}

vg::Transform3D vg::Transform3D::inverse() const
{
	glm::vec3 inv_scale = 1.0f / scale;
	glm::quat inv_rotation = glm::conjugate(rotation);
	return { -(inv_rotation * (position * inv_scale)), inv_rotation, inv_scale };
}

glm::mat4 vg::Transform3D::matrix() const
{
	glm::mat4 translation{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		glm::vec4(position, 1.0f)
	};
	glm::mat4 scaling{
		{ scale.x, 0.0f, 0.0f, 0.0f },
		{ 0.0f, scale.y, 0.0f, 0.0f },
		{ 0.0f, 0.0f, scale.z, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	return translation * glm::mat4_cast(rotation) * scaling;
}

vg::Rotator::Rotator(glm::quat quaternion)
{
	glm::vec3 angles = glm::eulerAngles(quaternion);
	roll = angles.x;
	pitch = angles.y;
	yaw = angles.z;
}

vg::Rotator::operator glm::quat() const
{
	return glm::quat({ roll, pitch, yaw });
}

void vg::Transformer2D::mark()
{
	if (!_dirty)
	{
		_dirty = true;
		for (Transformer2D* child : children)
			child->mark();
	}
}

bool vg::Transformer2D::sync()
{
	if (_dirty)
	{
		_dirty = false;
		if (parent)
		{
			parent->sync();
			_global = parent->_global * local.matrix();
		}
		else
		{
			_global = local.matrix();
		}
		return true;
	}
	return false;
}

void vg::Transformer3D::mark()
{
	_dirty = true;
	for (Transformer3D* child : children)
		child->mark();
}

bool vg::Transformer3D::sync()
{
	if (_dirty)
	{
		_dirty = false;
		if (parent)
		{
			parent->sync();
			_global = parent->_global * local.matrix();
		}
		else
		{
			_global = local.matrix();
		}
		return true;
	}
	return false;
}

void vg::attach_transformer(Transformer2D* parent, Transformer2D* child)
{
	if (parent && child && parent != child)
	{
		if (child->parent)
		{
			if (child->parent == parent)
				return;
			detach_transformer(child->parent, child);
		}
		child->parent = parent;
		parent->children.push_back(child);
		child->mark();
	}
}

void vg::detach_transformer(Transformer2D* parent, Transformer2D* child)
{
	if (parent && child && child->parent == parent)
	{
		child->parent = nullptr;
		auto iter = std::find(parent->children.begin(), parent->children.end(), child);
		if (iter != parent->children.end())
			parent->children.erase(iter);
		child->mark();
	}
}

void vg::attach_transformer(Transformer3D* parent, Transformer3D* child)
{
	if (parent && child && parent != child)
	{
		if (child->parent)
		{
			if (child->parent == parent)
				return;
			detach_transformer(child->parent, child);
		}
		child->parent = parent;
		parent->children.push_back(child);
		child->mark();
	}
}

void vg::detach_transformer(Transformer3D* parent, Transformer3D* child)
{
	if (parent && child && child->parent == parent)
	{
		child->parent = nullptr;
		auto iter = std::find(parent->children.begin(), parent->children.end(), child);
		if (iter != parent->children.end())
			parent->children.erase(iter);
		child->mark();
	}
}

std::array<glm::vec2, 4> vg::quad_vertex_positions(glm::vec2 size, glm::vec2 pivot)
{
	return std::array<glm::vec2, 4>{
		size * (pivot + glm::vec2{ -1.0f, -1.0f }),
		size * (pivot + glm::vec2{  0.0f, -1.0f }),
		size * (pivot + glm::vec2{  0.0f,  0.0f }),
		size * (pivot + glm::vec2{ -1.0f,  0.0f })
	};
}

std::array<glm::vec3, 8> vg::cube_vertex_positions(glm::vec3 size, glm::vec3 pivot)
{
	return std::array<glm::vec3, 8>{
		// Front face
		size * (pivot + glm::vec3{ -1.0f, -1.0f,  0.0f }),
		size * (pivot + glm::vec3{  0.0f, -1.0f,  0.0f }),
		size * (pivot + glm::vec3{  0.0f,  0.0f,  0.0f }),
		size * (pivot + glm::vec3{ -1.0f,  0.0f,  0.0f }),
		// Back face
		size * (pivot + glm::vec3{ -1.0f, -1.0f, -1.0f }),
		size * (pivot + glm::vec3{  0.0f, -1.0f, -1.0f }),
		size * (pivot + glm::vec3{  0.0f,  0.0f, -1.0f }),
		size * (pivot + glm::vec3{ -1.0f,  0.0f, -1.0f })
	};
}

void vg::rotate_quaternion(glm::quat& quaternion, float angle, glm::vec3 axis)
{
	quaternion = glm::normalize(quaternion * glm::angleAxis(angle, axis));
}
