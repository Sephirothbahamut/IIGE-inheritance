#pragma once

#include <unordered_map>
#include <string>
#include <initializer_list>

#include <SFML/Graphics.hpp>

#include "Logger.h"

namespace engine
	{
	namespace _inner_resman
		{
		template <typename T>
		struct Counter { size_t count; T content; };

		template <typename T>
		class Resources_container
			{
			private:
				std::unordered_map<std::string, Counter<T>> map;

			public:
				typename std::unordered_map<std::string, Counter<T>>::iterator add(const std::string& fname)
					{
					auto ret = map.emplace(std::pair<std::string, _inner_resman::Counter<T>>{fname, Counter<T>{0, T{}}});
					if (ret.second)  // Didn't already exist
						{
						if (!ret.first->second.content.loadFromFile(fname)) 
							{ return map.end(); } // Return nulopt if load failed
						}
					return ret.first;
					}

				using iterator = typename std::unordered_map<std::string, Counter<T>>::iterator;
				void erase(const iterator& iterator) { map.erase(iterator); }
				iterator end() { return map.end(); }
			};

		}

	template <typename T>
	class Resource_referencer
		{
		friend class Resources_manager;
		private:
			_inner_resman::Resources_container<T>* container;
			typename _inner_resman::Resources_container<T>::iterator iterator;

			Resource_referencer(_inner_resman::Resources_container<T>* container) : container(container) {}//Only use for default resource
			Resource_referencer(_inner_resman::Resources_container<T>* container, typename std::unordered_map<std::string, _inner_resman::Counter<T>>::iterator iterator)
				: container(container), iterator(iterator)
				{
				iterator->second.count++;
				}

		public:
			Resource_referencer() = delete;
			Resource_referencer(const Resource_referencer& copy) = delete;
			Resource_referencer(Resource_referencer&& move) : container(move.container), iterator(std::move(move.iterator)) {}

			~Resource_referencer()
				{
				if (iterator->second.count == 1) { container->erase(iterator); }
				else { iterator->second.count--; }
				}

			T* get() { return &(iterator->second.content); }
		};

	template <typename T>
	class Resources_referencer
		{
		friend class Resources_manager;
		private:
			_inner_resman::Resources_container<T>* container;
			std::vector<typename _inner_resman::Resources_container<T>::iterator> iterators;

			Resources_referencer(_inner_resman::Resources_container<T>* container) : container(container) {}//Only use for default resource
			Resources_referencer(_inner_resman::Resources_container<T>* container, std::vector<typename _inner_resman::Resources_container<T>::iterator>& iterators)
				: container(container), iterators(std::move(iterators))
				{
				for (auto& iterator : iterators) { iterator->second.count++; }
				}
		public:
			Resources_referencer() = delete;
			Resources_referencer(const Resources_referencer& copy) = delete;
			Resources_referencer(Resources_referencer&& move) : container(move.container), iterators(std::move(move.iterators))
				{
				move.iterators.clear();
				}

			~Resources_referencer()
				{
				for (auto& iterator : iterators)
					{
					if (iterator->second.count == 1) { container->erase(iterator); }
					else { iterator->second.count--; }
					}
				}

			T* get(size_t index) { return &(iterators[index]->second.content); }
		};


	class Resources_manager
		{	
		private:
			_inner_resman::Resources_container<sf::Texture> textures;

			Resource_referencer<sf::Texture> default_texture;


			_inner_resman::Resources_container<sf::Texture>::iterator _add_texture(const std::string& fname)
				{
				auto ret = textures.add(fname);

				if (ret != textures.end()) { return ret; }
				else { return _add_texture("data/textures/default.png"); }
				}

		public:
			Resources_manager()
				: textures{}, default_texture(&textures, [&]() 
					{
					auto it = textures.add("data/textures/default.png");
					if (it == textures.end()) { throw std::exception("Failed to find default texture."); }
					else { return it; }
					}())
				{
				/*auto it = textures.add("data/textures/default.png");
				if (it == textures.end()) { throw std::exception("Failed to find default texture."); }
				else { default_texture.iterator = it; }*/
				}

			[[nodiscard]] Resource_referencer<sf::Texture> add_texture(const std::string& fname) { return Resource_referencer<sf::Texture>{&textures, _add_texture(fname)}; }
			[[nodiscard]] Resources_referencer<sf::Texture> add_texture(std::initializer_list<std::string> fnames)
				{
				std::vector<_inner_resman::Resources_container<sf::Texture>::iterator> iterators;
				for (auto& fname : fnames) { iterators.push_back(_add_texture(fname)); }
				return Resources_referencer<sf::Texture>{&textures, iterators};
				}
		};
	}