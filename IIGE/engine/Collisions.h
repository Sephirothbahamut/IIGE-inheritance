#pragma once

#include <utils/containers/multitype_container.h>
#include <utils/math/geometry/circle.h>
#include <utils/math/geometry/segment.h>
#include <utils/math/geometry/polygon.h>
#include <utils/math/geometry/interactions.h>
#include <utils/math/geometry/transformations.h>

namespace engine
	{
	/*using layer = utils::multitype_container<std::vector, utils::math::geometry::circle, utils::math::geometry::convex_polygon, utils::math::geometry::polygon, utils::math::geometry::segment, utils::math::vec2f>;

	template <size_t layers_count>
	class collisions_manager
		{
		std::array<layer, layers_count> layers;


		}

	enum class layers { ships, projectiles, rocks };


	template <size_t layer, typename collider_type>
	struct has_collision
		{
		has_collision()
			{
			collisions_manager->add<layer, collider_type>()
			}
		}

	class pippo : has_collision<ships, circle>
		{
		pippo() : has_collision<ships, circle>{radius}
			{
			}
		}*/
	}