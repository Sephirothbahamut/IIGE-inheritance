#pragma once

#include <utils/math/vec2.h>
#include <SFML/Graphics.hpp>

namespace iige::sfml
	{
	template <template<typename> class To, typename T, template<typename> class From, typename F>
	inline To<T> vec_cast(From<F> from) { return {static_cast<T>(from.x), static_cast<T>(from.y)}; }
	}

/*inline int mainzq()
	{
	utils::math::vec2f utmf{1.f, 1.f};
	utils::math::vec2i utmi{2  , 1};
	sf::Vector2f sfmf{1.f, 2.f};
	sf::Vector2i sfmi{1  , 2};

	sfmf = iige::sfml::vec_cast<sf::Vector2, float>(utmf);

	return 0;
	}*/