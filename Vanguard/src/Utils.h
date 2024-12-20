#pragma once

#include <functional>
#include <memory>
#include <stack>

namespace vg
{
	template<typename Handle, typename Object>
	class StrictOwnership
	{
		template<typename Handle, typename Object>
		struct Trace
		{
			Handle* owner = nullptr;
			std::unique_ptr<Object> obj;
		};

		std::unique_ptr<Object> default_obj;
		std::stack<Trace<Handle, Object>> traces;

	public:
		StrictOwnership(std::unique_ptr<Object>&& default_obj) : default_obj(std::move(default_obj)) {}

		std::function<void(Object&)> doer;

		bool is_available(const Handle* new_owner) const
		{

			return new_owner && (!_owner || new_owner == _owner);
		}

		bool is_owner(const Handle* potential_owner) const
		{
			return potential_owner == _owner;
		}

		void (const Handle* owner, std::unique_ptr<Object>&& obj)
		{
			if (is_available(owner))
			{
				std::unique_ptr<Object> old_obj = std::move(_obj);
				_obj = std::move(obj);

			}
			else
				return nullptr;
		}

		std::unique_ptr<Object> release(const Handle* owner);
		std::unique_ptr<Object> eject();
	};
}
