#pragma once

#include <entt.h>

#include "../../graphics/Window.h"
#include "../../Scene.h"

#include "../../sfml_interfacing/vec.h"

#include "../components/bad_draw.h"
#include "../components/transform.h"
#include "../components/speed.h"

namespace iige::ecs::systems
	{
	//template <typename Scene_t>
	void draw(engine::Scene& scene, sf::RenderWindow& window, float interpolation)
		{
		/*auto static_view{scene.ecs_registry.view<components::bad_draw, components::transform>(entt::exclude<components::interpolated>)};

		static_view.each([&](components::bad_draw& drawable, const components::transform& transform)
			{
			sf::CircleShape c{drawable}; c.setPosition(sfml::vec_cast<sf::Vector2, float>(transform.t.position));
			window.draw(c);
			});*/

		/*auto movement_view{scene.ecs_registry.view<components::interpolated, components::bad_draw>()};

		movement_view.each([&](const auto& transform, auto& drawable)
			{
			drawable.circle.setPosition(sfml::vec_cast<sf::Vector2, float>(transform.t.position));
			window.draw(drawable.circle);
			});*/

		auto movement_view{scene.ecs_registry.view<components::interpolated, components::bad_draw, components::transform, components::transform_next>()};

		movement_view.each([&](auto& interpolated, auto& drawable, const auto& transform, const auto& transform_next)
			{
			interpolated.t = {utils::math::Transform2::lerp(transform.t, transform_next.t, interpolation)};
			drawable.cs.setPosition(sfml::vec_cast<sf::Vector2, float>(interpolated.t.position));
			drawable.cs.setRotation(interpolated.t.orientation.value);
			window.draw(drawable.cs);
			});
		}
	}