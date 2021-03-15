#pragma once
#include <utils/containers/listed_array.h>
#include <utils/variadic.h>
/*
#include "Game_object.h"
#include <vector>
#include <deque>

namespace engine
	{
	enum class Container_properties { step, draw, move, collide };

	//HELP Container must be a type that contains either T or unique_ptr<T>;
	//T must inherit from Game_object
	template <typename Container, typename T, Container_properties ...properties>
	class Objects_container
		{
		private:
			Container container;

		public:
			void step() //same is repeated for draw, move and collide
				{
				if constexpr (utils::variadic::contains_value<Container_properties, Container_properties::step, properties...>())
					{
					if constexpr (true)//HELP if contains unique_ptr
						{
						for (const auto& object : container) { object->step(); }
						}
					else { for (auto& object : container) { object.step(); } }
					}
				}

			template <typename ... Args>
			const T* emplace(Args&& ... args)
				{
				if constexpr (true)//HELP if contains unique_ptr
					{
					std::cout << "hello world" << std::endl;
					asc
					return container[container.size() - 1].get();
					}
				else
					{
					container.emplace_back(args...);
					return &(container[container.size() - 1]);
					}
				}
		};

	//NOTE interface without templates to make them available in a non-templated scene class
	class Containers_set_interface
		{
		friend class Scene;
		protected:
			virtual void step() {};
			virtual void draw() {};
			virtual void move() {};
			virtual void collide() {};

		public:
			//HELP virtual not allowed in template
			/*template <typename T, typename ... Args>
			virtual const T* create(Args&& ... args) = delete;*//*
		};

	template <typename ...Containers_types>
	class Containers_set : public Containers_set_interface;
		{
		protected:
			std::tuple<Containers_types...> containers;

			virtual void step()    { utils::tuple::for_each_in_tuple(containers, [](Container container) { container.step();    } }
			virtual void draw()    { utils::tuple::for_each_in_tuple(containers, [](Container container) { container.draw();    } }
			virtual void move()    { utils::tuple::for_each_in_tuple(containers, [](Container container) { container.move();    } }
			virtual void collide() { utils::tuple::for_each_in_tuple(containers, [](Container container) { container.collide(); } }

		public:
			template <typename T, typename ... Args>
			const T* create(Args&& ... args)
				{
				//HELP find container that holds type T, 
				//if none is present, use container that holds unique_ptr<Game_object>
				return container.emplace(args...);
				}
		};

	class Scene
		{
		private:
			std::unique_ptr<Containers_set_interface> containers;

		public:
			Scene(std::unique_ptr<Containers_set_interface> containers) : containers(std::move(containers)) {}

			void step() { containers->step(); }
			void draw() { containers->draw(); }

			template <typename T, typename ... Args>
			const T* create(Args&& ... args)
				{
				return containers->create<T>(args...);
				}

		};

	//usage example
	class A : public Game_object {};
	class B : public Game_object {};
	class Projectile : public Game_object 
		{
		public:
			Projectile(float speed, float direction) {}
		};

	int main()
		{
		auto set = std::make_unique<Containers_set<
			//will use the following to hold As and Bs; they are few and i want pointers to remain valid, so deque of pointers
			Objects_container<std::deque<std::unique_ptr<Game_object>>, Game_object,
				Container_properties::step>,  
			//will use the following to hold projectiles, since there are many of them i want contiguous memory instead of pointers, so vector of that type
			Objects_container<std::vector<Projectile>, Projectile,
				Container_properties::step>,  
			>();

		Scene scene(set);

		scene.create<Projectile>(5.f, 90.f); //this will be in the vector<Projectile>
		const A* a = scene.create<A>(); //this will be in the vector<Projectile>
		const B* b = scene.create<B>(); //this will be in the deque<unique_ptr<Game_object>

		scene.step(); //will call step for all the three created objects
		}
	}*/