//--------------------------------------------------
// A Static object, collide and render. Thats bout it
//--------------------------------------------------
#include "Entity.h"
#pragma once

class CStatic : public CEntity
{
	public:
		CStatic(){SetObjectID(STATIC);}
		void Think(){CalculateBoundingBox();}
		void Collide(CEntity* p_OtherEntity){}//things will collide with me, not me with things.
};