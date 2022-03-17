//--------------------------------------------------
// The Player.
//--------------------------------------------------
#include "Entity.h"
#include "math.h"
#include "Sea.h"
#include "Missle.h"

#pragma once

class CPlayer : public CEntity
{
	public:
		CPlayer(CSea* p_Sea);
		void Think();
		void Collide(CEntity* p_OtherEntity);
		util_vector3d GetForward() {return m_vecForward;}
		bool GetNeedToFire() {return m_bFiring;}
		void FireMissle(CEntity* p_Missle);
		//Controls
		void Move(int direction);
	private:
		util_vector3d m_vecForward;
		float m_fDirectionInRadians;
		float EnginePower;
		bool m_bFiring;
		CSea* Sea_Acess;//We need to know the sea so we can get heights
};