//--------------------------------------------------------
// The game world
// Bryn Stringer 
//--------------------------------------------------------
#pragma once
#include "Vis_Core.h"
#include "Entity.h"
#include "Player.h"
#include "Sea.h"
#include "EnemyShip.h"
#include "Missle.h"
#include "Static.h"
//My utils
#include "util_vector2d.h"
#include "util_vector3d.h"
#include <vector>

class CWorld_Core
{
	public:
		CWorld_Core(CVis_Core* p_Vis_Core);
		~CWorld_Core();
		void World_Main();
		//Control inputs
		void Player_Move(int direction);
		void Player_FireMissle();
	private:
		bool ObjectsColliding(CEntity* object1,CEntity* object2); 
		CVis_Core* Graphics;
		vector<CEntity*> m_vObjectList;
		CSea* Sea_Acess;
		int m_iPlayer_FireCooldown;
		unsigned int lasttickcount;
};