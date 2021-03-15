#pragma once

namespace engine::settings
	{
#ifdef NDEBUG
	inline static constexpr bool log = false;
#else
	inline static constexpr bool log = true;
#endif

	inline static constexpr bool colliders_draw = true;
	}