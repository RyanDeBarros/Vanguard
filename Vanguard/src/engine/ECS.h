#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <queue>

#include "Vanguard.h"

namespace vg
{
	namespace ecs
	{
		using Entity = size_t;

		class EntityGenerator
		{
			Entity next_entity_id = 1;
			std::queue<Entity> freed_ids;

		public:
			Entity create_entity()
			{
				if (freed_ids.empty())
					return next_entity_id++;

				Entity id = freed_ids.front();
				freed_ids.pop();
				return id;
			}

			void destroy_entity(Entity entity)
			{
				if (entity != 0)
					freed_ids.push(entity);
			}
		};

		struct IComponent
		{
			virtual ~IComponent() = default;
			virtual std::unique_ptr<IComponent> clone() const = 0;
		};

#define VANGUARD_ICOMPONENT_CLONE(class_name) std::unique_ptr<IComponent> clone() const override { return std::make_unique<class_name>(*this); }

		class ECSManager
		{
			std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<IComponent>>> components;

		public:
			EntityGenerator entity_generator;
			
			template<typename Component, typename... Args>
			void add_component(Entity entity, Args&&... args)
			{
				components[entity][typeid(Component)] = std::make_unique<Component>(std::forward<Args>(args)...);
			}

			void emplace_component(Entity entity, std::unique_ptr<IComponent>&& component, std::type_index type_index)
			{
				components[entity][type_index] = std::move(component);
			}

			template<typename Component>
			Component* get_component(Entity entity)
			{
				auto it = components.find(entity);
				if (it == components.end())
					return nullptr;

				auto& component_map = it->second;
				auto cit = component_map.find(typeid(Component));
				if (cit == component_map.end())
					return nullptr;

				return static_cast<Component*>(cit->second.get());
			}

			const std::unordered_map<std::type_index, std::unique_ptr<IComponent>>& get_all_components(Entity entity)
			{
				return components[entity];
			}

			template<typename Component>
			void remove_component(Entity entity)
			{
				auto it = components.find(entity);
				if (it != components.end())
					it->second.erase(typeid(Component));
			}

			void remove_entity(Entity entity)
			{
				components.erase(entity);
			}

			bool has_entity(Entity entity)
			{
				return components.find(entity) != components.end();
			}

			template<typename Component, typename Visitor>
			void travel_components(const Visitor& visitor)
			{
				for (auto& [entity, component_map] : components)
				{
					auto it = component_map.find(typeid(Component));
					if (it != component_map.end())
						visitor(static_cast<Component*>(it->second.get()), entity);
				}
			}
			
			template<typename Component1, typename Component2, typename Visitor>
			void travel_bi_components(const Visitor& visitor)
			{
				for (auto& [entity, component_map] : components)
				{
					auto it1 = component_map.find(typeid(Component1));
					if (it1 != component_map.end())
					{
						auto it2 = component_map.find(typeid(Component2));
						if (it2 != component_map.end())
							visitor(static_cast<Component1*>(it1->second.get()), static_cast<Component2*>(it2->second.get()), entity);
					}
				}
			}

			template<typename Component1, typename Component2, typename Component3, typename Visitor>
			void travel_tri_components(const Visitor& visitor)
			{
				for (auto& [entity, component_map] : components)
				{
					auto it1 = component_map.find(typeid(Component1));
					if (it1 != component_map.end())
					{
						auto it2 = component_map.find(typeid(Component2));
						if (it2 != component_map.end())
						{
							auto it3 = component_map.find(typeid(Component3));
							if (it3 != component_map.end())
								visitor(static_cast<Component1*>(it1->second.get()), static_cast<Component2*>(it2->second.get()), static_cast<Component3*>(it3->second.get()), entity);
						}
					}
				}
			}
		};

		class EntityObject
		{
			Entity entity;
			ECSManager* manager;

		public:
			EntityObject(ECSManager* manager) : entity(manager->entity_generator.create_entity()), manager(manager) {}
			
			EntityObject(const EntityObject& other)
				: entity(other.manager->entity_generator.create_entity()), manager(other.manager)
			{
				for (const auto& [type_index, component] : manager->get_all_components(other.entity))
					manager->emplace_component(entity, component->clone(), type_index);
			}

			EntityObject(EntityObject&& other) noexcept
				: entity(other.entity), manager(other.manager)
			{
				other.entity = 0;
				other.manager = nullptr;
			}

			EntityObject& operator=(const EntityObject& other)
			{
				if (this != &other)
				{
					destroy_entity();
					entity = other.manager->entity_generator.create_entity();
					manager = other.manager;
					for (const auto& [type_index, component] : manager->get_all_components(other.entity))
						manager->emplace_component(entity, component->clone(), type_index);
				}
				return *this;
			}

			EntityObject& operator=(EntityObject&& other) noexcept
			{
				if (this != &other)
				{
					destroy_entity();
					entity = other.entity;
					manager = other.manager;
					other.entity = 0;
					other.manager = nullptr;
				}
				return *this;
			}

			~EntityObject()
			{
				destroy_entity();
			}

			Entity get_entity() const { return entity; }
			void destroy_entity()
			{
				if (entity != 0)
				{
					manager->remove_entity(entity);
					manager->entity_generator.destroy_entity(entity);
					entity = 0;
				}
			}

			template<typename Component, typename... Args>
			void add_component(Args&&... args)
			{
				manager->add_component<Component>(entity, std::forward<Args>(args)...);
			}

			template<typename Component>
			Component* get_component()
			{
				return manager->get_component<Component>(entity);
			}

			template<typename Component>
			void remove_component()
			{
				manager->remove_component<Component>(entity);
			}
		};

		struct ISystem
		{
			virtual ~ISystem() = default;
			virtual void update(ECSManager& manager) = 0;
		};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

		namespace components
		{
			struct Position2D : public IComponent
			{
				float x, y;
				VANGUARD_ICOMPONENT_CLONE(Position2D);
			};

			struct Velocity2D : public IComponent
			{
				float x, y;
				VANGUARD_ICOMPONENT_CLONE(Velocity2D);
			};

			struct Acceleration2D : public IComponent
			{
				float x, y;
				VANGUARD_ICOMPONENT_CLONE(Acceleration2D);
			};
		}

		namespace systems
		{
			struct Velocity2D : public ISystem
			{
				void update(ECSManager& manager)
				{
					manager.travel_bi_components<components::Position2D, components::Velocity2D>(
						[](components::Position2D* position, const components::Velocity2D* velocity, Entity entity) {
							position->x += velocity->x * vg::data::delta_time;
							position->y += velocity->y * vg::data::delta_time;
						});
				}
			};

			struct Acceleration2D : public ISystem
			{
				void update(ECSManager& manager)
				{
					manager.travel_tri_components<components::Position2D, components::Velocity2D, components::Acceleration2D>(
						[](components::Position2D* position, components::Velocity2D* velocity, const components::Acceleration2D* acceleration, Entity entity) {
							position->x += velocity->x * vg::data::delta_time;
							position->y += velocity->y * vg::data::delta_time;
							velocity->x += acceleration->x * vg::data::delta_time;
							velocity->y += acceleration->y * vg::data::delta_time;
						});
				}
			};
		}
	}
}
