#pragma once

#include <concepts>
#include <vector>
#include <functional>

namespace vg
{
	struct Event
	{
		mutable bool consumed = false;
	};

	template<std::derived_from<Event> _Event>
	class EventHandler
	{
		EventHandler<_Event>* parent = nullptr;
		std::vector<EventHandler<_Event>*> children;
		
	public:
		std::function<void(const _Event&)> callback = [](const _Event&) {};

		EventHandler() = default;

		EventHandler(const EventHandler<_Event>& other)
		{
			if (other.parent)
				other.parent->add_child(this);
			children = other.children;
			for (auto child : children)
				child->parent = this;
		}

		EventHandler(EventHandler<_Event>&& other) noexcept
			: parent(other.parent), children(std::move(other.children))
		{
			if (parent)
			{
				auto iter = std::find(parent->children.begin(), parent->children.end(), &other);
				*iter = this;
				other.parent = nullptr;
			}
			for (auto child : children)
				child->parent = this;
		}

		EventHandler<_Event>& operator=(const EventHandler<_Event>& other)
		{
			if (this != &other)
			{
				if (parent)
				{
					if (parent != other.parent)
					{
						parent->remove_child(this);
						parent = other.parent;
						parent->add_child(this);
					}
				}
				else
				{
					parent = other.parent;
					parent->add_child(this);
				}
				while (!children.empty())
				{
					auto iter = children.begin();
					iter->parent = nullptr;
					children.erase(iter);
				}
				children = other.children;
				for (auto child : children)
					child->parent = this;
			}
			return *this;
		}

		EventHandler<_Event>& operator=(EventHandler<_Event>&& other) noexcept
		{
			if (this != &other)
			{
				if (parent)
				{
					if (parent != other.parent)
					{
						parent->remove_child(this);
						parent = other.parent;
						auto iter = std::find(parent->children.begin(), parent->children.end(), &other);
						*iter = this;
						other.parent = nullptr;
					}
					else
						parent->remove_child(&other);
				}
				else
				{
					parent = other.parent;
					if (parent)
					{
						auto iter = std::find(parent->children.begin(), parent->children.end(), &other);
						*iter = this;
						other.parent = nullptr;
					}
				}
				while (!children.empty())
				{
					auto iter = children.begin();
					iter->parent = nullptr;
					children.erase(iter);
				}
				children = std::move(other.children);
				for (auto child : children)
					child->parent = this;
			}
			return *this;
		}
		
		~EventHandler()
		{
			if (parent)
				parent->remove_child(this);
			for (auto child : children)
				child->parent = nullptr;
		}

		const EventHandler<_Event>* get_parent() const { return parent; }
		EventHandler<_Event>* get_parent() { return parent; }
		const std::vector<EventHandler<_Event>*>& get_children() const { return children; }

		void on_callback(const _Event& event)
		{
			for (auto child : children)
			{
				child->on_callback(event);
				if (event.consumed)
					return;
			}
			callback(event);
		}

		void add_child(EventHandler<_Event>* child)
		{
			if (child)
			{
				child->set_parent(this);
				children.push_back(child);
			}
		}

		void insert_child(EventHandler<_Event>* child, size_t pos)
		{
			if (child)
			{
				child->set_parent(this);
				children.insert(children.begin() + pos, child);
			}
		}

		void remove_child(EventHandler<_Event>* child)
		{
			if (child)
			{
				auto iter = std::find(children.begin(), children.end(), child);
				if (iter != children.end())
				{
					children.erase(iter);
					child->parent = nullptr;
				}
			}
		}

		void erase_child(EventHandler<_Event>* child, size_t pos)
		{
			if (child && pos < children.size() && children[pos] == child)
			{
				children.erase(children.begin() + pos);
				child->parent = nullptr;
			}
		}

		void set_parent(EventHandler<_Event>* parent)
		{
			if (this->parent)
				this->parent->remove_child(this);
			parent->add_child(this);
		}
	};
}
