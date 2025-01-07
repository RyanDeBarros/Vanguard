#pragma once

#include <vector>

#include "Vendor.h"

namespace vg
{
	// LATER custom Vector2D, Vector3D, Rotator3D. Even maybe a distinction between Position2D and Scale2D, etc.

	struct Transform2D
	{
		glm::vec2 position = { 0.0f, 0.0f };
		float rotation = 0.0f;
		glm::vec2 scale = { 1.0f, 1.0f };

		Transform2D inverse() const;
		glm::mat3 matrix() const;
		glm::mat3 camera() const { return inverse().matrix(); }
	};

	struct Transform3D
	{
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::quat rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		
		Transform3D inverse() const;
		glm::mat4 matrix() const;
		glm::mat4 camera() const { return inverse().matrix(); }
	};

	struct Rotator
	{
		float pitch = 0.0f;
		float yaw = 0.0f;
		float roll = 0.0f;

		Rotator() = default;
		Rotator(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}
		Rotator(glm::quat quaternion);

		operator glm::quat() const;
	};

	struct Transformer2D
	{
		Transformer2D* parent = nullptr;
		std::vector<Transformer2D*> children;
		Transform2D local;

	private:
		glm::mat3 _global;
		bool _dirty = true;

	public:
		glm::mat3 global() const { return _global; }
		// Call after modifying self
		void mark();
		// Call before reading self
		void sync();
	};

	struct Transformer3D
	{
		Transformer3D* parent = nullptr;
		std::vector<Transformer3D*> children;
		Transform3D local;

	private:
		glm::mat4 _global;
		bool _dirty = true;

	public:
		glm::mat4 global() const { return _global; }
		// Call after modifying self
		void mark();
		// Call before reading self
		void sync();
	};

	extern void attach_transformer(Transformer2D* parent, Transformer2D* child);
	extern void detach_transformer(Transformer2D* parent, Transformer2D* child);
	extern void attach_transformer(Transformer3D* parent, Transformer3D* child);
	extern void detach_transformer(Transformer3D* parent, Transformer3D* child);
}
