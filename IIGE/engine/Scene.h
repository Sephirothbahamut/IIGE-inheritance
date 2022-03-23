#pragma once

#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include <utils/containers/polymorphic_container.h>
#include <utils/../../beta/include/utils/containers/enable_disable_vector.h>
#include <utils/tracking.h>

#include "graphics/Window.h"
#include "Object.h"

template<typename T>
struct utils::container_emplace_helper<utils::enable_disable_vector, T>
	{
	template<typename... Args>
	static T& emplace(utils::enable_disable_vector<T>& v, Args&&... args)
		{
		return v.emplace(std::forward<Args>(args)...);
		}
	};

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

	template <typename ...Types>
	class Scene
		{
		//Genius sarah from #include discord channel:
		template <typename T, typename U>
		using ignore_first = U;
		//it expands to
		//	container{
		//	(ignore_first<T0, void>(0), lambda),
		//	(ignore_first<T1, void>(0), lambda),
		//	(ignore_first<T2, void>(0), lambda),
		//	}
		//which is just
		//	container{
		//	(void(0), lambda),
		//	(void(0), lambda),
		//	(void(0), lambda),
		//	}
		//(void(0), lambda) is equivalent to lambda
		//well, almost equivalent
		//but in this case it should be equivalent

		friend class objects::Object;
		friend class objects::Step;
		friend class objects::Draw;
		friend class objects::Move;
		friend class objects::Has_collision;
		friend class objects::Collide_discrete;

		public:
			template <typename T, typename ...Args>
			utils::tracking_ptr<T> create(Args&&... args)
				{
				return {container.emplace<T>(std::forward<Args>(args)...)};
				}

			void update()
				{
				container.for_each_container([](auto& container) { container.update(); });
				}

			void movement_step()
				{
				container.for_each_element_of_type<objects::Move>([](objects::Move& object) { object.movement_step(); });
				}

			void step()
				{
				container.for_each_element_of_type<objects::Step>([](objects::Step& object) { object.step(); });
				}

			void draw(graphics::Window& window, float interpolation)
				{
				window.sf_window.clear();
				container.for_each_element_of_type<objects::Draw>([&](objects::Draw& object) { object.draw(window.sf_window, interpolation); });
				//container.for_each_element_of_type<objects::Has_collision>([&](objects::Has_collision& object) { object.collider_ptr->draw(window.sf_window); });
				window.sf_window.display();
				}

			void collisions()
				{
				/*have_collisions.disable_enable_pass();
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
					}*/
				}

			size_t active_objects_count() { return container.size(); }

		private:
			// enable_disable vector takes a lambda to access the enable_disable state of its contained type
			// polymorphic_container takes the parameters to pass to the container type it's using inside
			utils::polymorphic_container<utils::enable_disable_vector, objects::Object, Types...> container
				{
				[](utils::polymorphic_value<objects::Object>& polyobj) -> utils::enable_disable& { return polyobj->state; },
				(ignore_first<Types, void>(0), [](auto& object) -> utils::enable_disable& { return object.state; })...
				};
		};

	}