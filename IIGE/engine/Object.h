#pragma once

#include <vector>
#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>

#include <utils/containers/vector_utils.h>

#include "core/Transform2.h"
#include "Logger.h"
#include "Collider.h"

namespace engine { class Scene; template <typename T> class Action_list; }
namespace engine::objects
	{
	class Move;

	class Object
		{
		friend class Scene;
		protected:
			Scene* scene;

		public:
			Object(Scene& scene);
		};

	class In_world : public virtual Object
		{
		public:
			In_world(Scene& scene) : Object(scene) {}
			core::Transform2 transform{};
		};

	class Move : public virtual Object, public virtual In_world
		{
		friend class Action_list<Move>;
		private:
			bool enabled = false;
			size_t index = 0;
			void finalize_disable() { transform_previous = transform; }

		public:
			Move(Scene& scene) : In_world(scene), Object(scene) { enable(); }
			~Move() { disable(); }

			core::Transform2 transform_previous;
			core::Transform2 movement;

			void enable();
			void disable();

			void movement_step() { transform_previous = transform; transform += movement; }
		};

	class Step : public virtual Object
		{
		friend class Action_list<Step>;
		private:
			bool enabled = false;
			size_t index = 0;
		public:
			Step(Scene& scene) : Object(scene) { enable(); }
			~Step() { disable(); }

			virtual void step() = 0;

			void enable();
			void disable();
		};

	class Draw : public virtual Object
		{
		friend class Action_list<Draw>;
		private:
			bool enabled = false;
			size_t index = 0;

			float z = 0;

		public:
			Draw(Scene& scene, float z = 0) : Object(scene), z(z) { enable(); }
			~Draw() { disable(); }

			virtual void draw(sf::RenderTarget& rt, float interpolation) = 0;

			float get_z() const noexcept { return z; }
			void set_z(float z) { disable(); this->z = z; enable(); };//Can be costy, don't spam

			void enable();
			void disable();
		};

	class Has_collision : public virtual In_world
		{
		friend class Action_list<Has_collision>;
		using Shape_ptr = std::unique_ptr<collisions::Shape>;
		private:
			bool enabled = false;
			size_t index = 0;
			size_t list_id = 0;

		public:
			Has_collision(Scene& scene) : Object(scene), In_world(scene) {}
			~Has_collision() { disable(); }

			Shape_ptr collider_ptr{nullptr};

			void enable(size_t list_id);
			void disable();
		};

	class Collide_discrete : public virtual Has_collision
		{
		friend class Action_list<Collide_discrete>;
		using Shape_ptr = std::unique_ptr<collisions::Shape>;
		private:
			bool enabled = false;
			size_t index = 0;

		public:
			Collide_discrete(Scene& scene) : Object(scene), In_world(scene), Has_collision(scene) { enable(); }
			~Collide_discrete() { disable(); }
			collisions::Collisions_list_discrete collisions;

			void enable();
			void disable();
		};
	}
