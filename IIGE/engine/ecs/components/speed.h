#pragma once

#include <utils/math/Transform2.h>
#include "transform.h"

namespace iige::ecs::components
	{
	struct interpolated   { utils::math::Transform2 t; };
	struct transform_next { utils::math::Transform2 t; };
	struct speed          { utils::math::Transform2 t; };

	}
