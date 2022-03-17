//--------------------------------------------------
// A missle
//--------------------------------------------------
#include "Entity.h"
#pragma once

class CMissle : public CEntity
{
	public:
		CMissle();
		void SetDirection(util_vector3d p_vecForward)
		{
			p_vecForward.y = 0;
			m_vecForward = p_vecForward;
		}
		void Think();
		void Collide(CEntity* p_OtherEntity);
	private:
		util_vector3d m_vecForward;
};