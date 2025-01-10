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

		class EntityTracker
		{
			Entity next_entity_id = 0;
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
				freed_ids.push(entity);
			}
		};

		struct IComponent
		{
			virtual ~IComponent() = default;
		};

		class ECSManager
		{
			std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<IComponent>>> components;

		public:
			template<typename Component, typename... Args>
			void add_component(Entity entity, Args&&... args)
			{
				components[entity][typeid(Component)] = std::make_unique<Component>(std::forward<Args>(args)...);
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
			};

			struct Velocity2D : public IComponent
			{
				float x, y;
			};

			struct Acceleration2D : public IComponent
			{
				float x, y;
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
							position->x += velocity->x * vg::delta_time;
							position->y += velocity->y * vg::delta_time;
						});
				}
			};

			struct Acceleration2D : public ISystem
			{
				void update(ECSManager& manager)
				{
					manager.travel_tri_components<components::Position2D, components::Velocity2D, components::Acceleration2D>(
						[](components::Position2D* position, components::Velocity2D* velocity, const components::Acceleration2D* acceleration, Entity entity) {
							position->x += velocity->x * vg::delta_time;
							position->y += velocity->y * vg::delta_time;
							velocity->x += acceleration->x * vg::delta_time;
							velocity->y += acceleration->y * vg::delta_time;
						});
				}
			};
		}
	}
}
