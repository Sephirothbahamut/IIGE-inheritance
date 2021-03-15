#pragma once

#include "Object.h"
#include "Scene.h"

namespace engine::objects
	{
	Object::Object(Scene& scene) : scene(&scene) {}

	void Move            ::enable ()               { if(!enabled) { scene->moving_entities        .enable (this); enabled = true;  } }
	void Move            ::disable()               { if( enabled) { scene->moving_entities        .disable(this); enabled = false; } }
	void Step            ::enable ()               { if(!enabled) { scene->steps                  .enable (this); enabled = true;  } }
	void Step            ::disable()               { if( enabled) { scene->steps                  .disable(this); enabled = false; } }
	void Draw            ::enable ()               { if(!enabled) { scene->draws                  .enable (this); enabled = true;  } }
	void Draw            ::disable()               { if( enabled) { scene->draws                  .disable(this); enabled = false; } }
	void Has_collision   ::enable (size_t list_id) { if(!enabled) { scene->collider_lists[list_id].enable (this); enabled = true;  } }
	void Has_collision   ::disable()               { if( enabled) { scene->collider_lists[list_id].disable(this); enabled = false; } }
	void Collide_discrete::enable ()               { if(!enabled) { scene->collide_discretes      .enable (this); enabled = true;  } }
	void Collide_discrete::disable()               { if( enabled) { scene->collide_discretes      .disable(this); enabled = false; } }
	}