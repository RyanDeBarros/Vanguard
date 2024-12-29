#pragma once

#include <concepts>

namespace vg
{
	namespace meta
	{
		template <class Type, class... OtherTypes>
		constexpr bool is_any_of_v = std::disjunction_v<std::is_same<Type, OtherTypes>...>;
	}
}
