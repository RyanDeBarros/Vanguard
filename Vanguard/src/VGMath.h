#pragma once

#include "Vendor.h"

namespace vg
{
	struct IntBounds
	{
		GLint x0, x1, y0, y1;
	};

	template<typename Type>
	struct Rect
	{
		Type x = Type(0), y = Type(0), w = Type(0), h = Type(0);
	};
}
