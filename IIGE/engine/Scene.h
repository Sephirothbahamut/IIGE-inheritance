#pragma once

#include <vector>
#include <algorithm>


#include <SFML/Graphics.hpp>

#include "graphics/Window.h"
#include "Object.h"

namespace engine
	{
	namespace objects
		{
		class Object;
		class Step;
		class Draw;
		class Move;
		class Has_collision;
		class Collide_discrete;
		}

	template <typename T>
	class Action_list
		{
		private:
			std::vector<size_t> to_disable;
			std::vector<T*> to_enable;

		public:
			std::vector<T*> enabled;

			void enable(T* element) { to_enable.push_back(element); }
			void disable(size_t index) { to_disable.push_back(index); }

			void disable_enable_pass()
				{
				//start removing from last
				std::sort(to_disable.begin(), to_disable.end(), [](const auto& a, const auto& b) { return a > b; });

				size_t size = enabled.size();
				for (size_t index : to_disable) 
					{
					if constexpr (std::is_same<T, objects::Move>::value) { enabled[index]->finalize_disable(); }

					if (index != (size - 1)) //if removing last there's nothing to swap
						{
						enabled[index] = enabled[size - 1];
						enabled[index]->index = index;
						}
					size--;
					}
				enabled.resize(size);
				to_disable.clear();

				for (T* e : to_enable) { e->index = enabled.size(); enabled.push_back(e); }
				to_enable.clear();
				}
		};

	class Scene
		{
		friend class objects::Object;
		friend class objects::Step;
		friend class objects::Draw;
		friend class objects::Move;
		friend class objects::Has_collision;
		friend class objects::Collide_discrete;

		private:
			std::vector<std::unique_ptr<objects::Object>> objects;

			Action_list<objects::Move>             moving_entities;
			Action_list<objects::Draw>             draws;
			Action_list<objects::Step>             steps;

			Action_list<objects::Has_collision> have_collisions; //stores all objects with a collision, passive or active
			Action_list<objects::Collide_discrete> collide_discretes;//stores all objects that actively look for collisions and run callbacks
			std::vector<Action_list<objects::Has_collision::Coll_list_subscr>> collider_lists; //stores subscriptions of have_collision objects to various lists

		public:
			template <typename T, typename ...Args>
			void create(Args... args)
				{
				objects.emplace_back(std::make_unique<T>(*this, args...));
				}

			void movement_step()
				{
				moving_entities.disable_enable_pass();
				for (objects::Move* move : moving_entities.enabled) { move->movement_step(); }
				}

			void step()
				{
				steps.disable_enable_pass();
				for (objects::Step* step : steps.enabled) { step->step(); }
				}

			void draw(graphics::Window& window, float interpolation)
				{
				draws.disable_enable_pass();
				window.sf_window.clear();

				for (objects::Draw* draw : draws.enabled) { draw->draw(window.sf_window, interpolation); }
				for (objects::Has_collision* e : have_collisions.enabled) { e->collider_ptr->draw(window.sf_window); }
				window.sf_window.display();
				}

			void collisions()
				{
				have_collisions.disable_enable_pass();
				for (objects::Has_collision* collide : have_collisions.enabled) { collide->collider_update(); }
				for (Action_list<objects::Has_collision::Coll_list_subscr>& list : collider_lists) { list.disable_enable_pass(); }

				collide_discretes.disable_enable_pass();


				for (objects::Collide_discrete* collide : collide_discretes.enabled)
					{
					for (const collisions::Collision_discrete& collision : collide->collisions)
						{
						for (objects::Has_collision::Coll_list_subscr* subscr: collider_lists[collision.collider_list_id].enabled)
							{
							bool result = collide->collider_ptr->collide(*(subscr->obj->collider_ptr.get()));
							if (result) { collision.callback({subscr->obj}); }
							}
						}
					}
				}

			void set_collider_lists(size_t amount) { collider_lists.resize(amount); }
		};

	}