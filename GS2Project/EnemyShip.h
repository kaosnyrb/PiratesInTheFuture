//--------------------------------------------------
// An Enemy Ship, floats on the "sea"
//--------------------------------------------------
#include "Entity.h"
#include "Sea.h"
#pragma once

class CEnemyShip : public CEntity
{
	public:
		CEnemyShip(CSea* p_Sea);
		void Think();
		void Collide(CEntity* p_OtherEntity);
	private:
		CSea* Sea_Acess;//We need to know the sea so we can get hieghts
};