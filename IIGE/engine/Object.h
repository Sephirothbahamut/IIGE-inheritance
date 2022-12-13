#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <vector>
#include <map>

#include <utils/tracking.h>
#include <utils/../../CPP_Utilities_old/beta/include/utils/containers/vector_utils.h>
#include <utils/../../CPP_Utilities_old/beta/include/utils/containers/enable_disable_vector.h>
#include <utils/logger.h>

#include <utils/math/transform2.h>

#include <SFML/Graphics.hpp>

#include <entt.h>
//#include "Collider.h"

namespace engine { template <typename ...Types> class Scene; }

namespace engine::objects
	{
	class Move;

	class Object : public utils::trackable
		{
		template <typename ...Types>
		friend class engine::Scene;
		public:
			utils::enable_disable state;
			//Object() { std::cout << "created" << std::endl; };
			//Object(const Object& copy) : state(copy.state), utils::trackable(copy) { std::cout << "copyed" << std::endl; }
			//Object& operator=(const Object& copy) { state = copy.state; this->utils::trackable::operator=(copy); std::cout << "copyed" << std::endl; return *this; }
			//
			//Object(Object&& move) noexcept : state(move.state), utils::trackable(std::move(move)) { std::cout << "moved" << std::endl; }
			//Object& operator=(Object&& move) noexcept { state = move.state; this->utils::trackable::operator=(std::move(move)); std::cout << "moved" << std::endl; return *this; }
			
		private:
			virtual void this_function_is_only_here_to_make_the_type_polymorphic() {}
		};

	class In_world
		{
		public:
			utils::math::transform2 transform{};
		};

	class Move : public virtual In_world
		{
		public:
			utils::math::transform2 transform_previous;
			utils::math::transform2 movement;

			void movement_step() { transform_previous = transform; transform += movement; }

		private:
			void finalize_disable() { transform_previous = transform; }
		};

	class Step
		{
		public:
			virtual void step() = 0;
		};

	class Draw
		{
		public:
			virtual void draw(sf::RenderTarget& rt, float interpolation) = 0;
		};

	class Has_collision : public virtual In_world
		{
		public:
			//utils::polymorphic_value<collisions::Shape> collider_ptr{};
			void collider_update() { /*collider_ptr->update(transform);*/ }
		};

	class Collide_discrete : public virtual Has_collision
		{
		};
	}
