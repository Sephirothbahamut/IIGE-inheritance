#pragma once

#include <utility>

#include <utils/math/geometry/interactions.h>
#include <utils/definitions.h>

#include <entt.h>

#include "../components/collides_with.h"
#include "../components/has_collision.h"
#include "../components/transform.h"
#include "../../Scene.h"


namespace iige::ecs::systems
	{
	namespace
		{
		namespace utmg = utils::math::geometry;
		namespace utm = utils::math;
		template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
		}

	class collision
		{
		public:
			virtual void operator()(engine::Scene& scene) const noexcept = 0;
		};

	template <size_t layers_count>
	class collision_impl : public collision
		{
		public:
			virtual void operator()(engine::Scene& scene) const noexcept final override
				{
				[&] <std::size_t... I>(std::index_sequence<I...>) {
					(evaluate<I>(scene), ...);
					}(std::make_index_sequence<layers_count>{});
				}
		private:
			template<size_t layer>
			void evaluate(engine::Scene& scene) const noexcept
				{

				auto targets/*TODO better names*/{scene.ecs_registry.view<components::has_collision<layer>, components::collider, components::transform>()};
				auto active /*TODO better names*/{scene.ecs_registry.view<components::collides_with<layer>, components::collider, components::transform>()};

				//active.each([&](const components::collides_with<layer>& collides_with, const components::collider& a_collider_variant, const components::transform& a_transform)
				active.each([&](const auto& collides_with, const components::collider& a_collider_variant, const components::transform& a_transform)
					{
					targets.each([&](const auto& has_collision, const components::collider& b_collider_variant, const components::transform& b_transform)
						{
						std::visit([&](const auto& a_collider)
							{
							std::visit([&](const auto& b_collider)
								{
								//bool collides{utmg::collides(a_collider, b_collider)};
								std::cout << "A collision!!!";
								}, b_collider_variant);
							}, a_collider_variant);
						});
					});
				}
		};
	}