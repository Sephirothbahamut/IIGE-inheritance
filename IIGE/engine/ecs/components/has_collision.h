#pragma once

#include <variant>
#include <utils/math/vec2.h>
#include <utils/math/geometry/aabb.h>
#include <utils/math/geometry/circle.h>
#include <utils/math/geometry/polygon.h>
#include <utils/math/geometry/interactions.h>

namespace iige::ecs::components
	{
	namespace 
		{
		namespace utmg = utils::math::geometry;
		namespace utm = utils::math;
		}

	using collider = std::variant<utm::vec2f, /*utmg::aabb,*/ utmg::circle, utmg::polygon>;

	template <size_t layer>
	struct has_collision {};
	}
