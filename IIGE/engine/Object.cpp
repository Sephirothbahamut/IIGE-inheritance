#pragma once

#include "Object.h"
#include "Scene.h"

namespace engine::objects
	{
	Object::Object(Scene& scene) : scene(&scene) {}

	void Move            ::enable () { if(!enabled) { scene->moving_entities        .enable (this);  enabled = true;  } }
	void Move            ::disable() { if( enabled) { scene->moving_entities        .disable(index); enabled = false; } }
	void Step            ::enable () { if(!enabled) { scene->steps                  .enable (this);  enabled = true;  } }
	void Step            ::disable() { if( enabled) { scene->steps                  .disable(index); enabled = false; } }
	void Draw            ::enable () { if(!enabled) { scene->draws                  .enable (this);  enabled = true;  } }
	void Draw            ::disable() { if( enabled) { scene->draws                  .disable(index); enabled = false; } }
	void Collide_discrete::enable () { if(!enabled) { scene->collide_discretes      .enable (this);  Has_collision::enable (); enabled = true;  } } // <-- important
	void Collide_discrete::disable() { if( enabled) { scene->collide_discretes      .disable(index); enabled = false; } }


	//TODO verify that it makes sense
	void Has_collision::enable()
		{
		if (!enabled) { scene->have_collisions.enable(this);  enabled = true; }
		for (auto& list : collider_lists_subscribed) { disable_list(list.first); }
		}
	void Has_collision::disable() { if (enabled)  { scene->have_collisions.disable(index); enabled = false; } }

	void Has_collision::enable_list(size_t list_id)
		{
		auto& list_data = collider_lists_subscribed[list_id];
		if (!list_data.enabled) { scene->collider_lists[list_id].enable(&list_data); list_data.enabled = true; }
		}

	void Has_collision::disable_list(size_t list_id)
		{
		auto& list_data = collider_lists_subscribed[list_id];
		if ( list_data.enabled) { scene->collider_lists[list_id].disable(list_data.index); list_data.enabled = false; }
		}

	void Has_collision::add_to_list(size_t list_id) 
		{
		if (collider_lists_subscribed.find(list_id) == collider_lists_subscribed.end()) 
			{
			collider_lists_subscribed[list_id] = {this, false, 0, list_id};
			if (enabled) { enable_list(list_id); }
			}
		else { logger << utils::message::err("Trying to add object to a colliders list it's already in."); }
		}
	void Has_collision::remove_from_list(size_t list_id)
		{
		if (collider_lists_subscribed.find(list_id) != collider_lists_subscribed.end())
			{
			collider_lists_subscribed[list_id] = {this, false, 0, list_id};
			if (enabled) { disable_list(list_id); }
			}
		else { logger << utils::message::err("Trying remove objects from a colliders list it's not inside."); }
		}
	}