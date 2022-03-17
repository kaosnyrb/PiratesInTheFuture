//--------------------------------------------------
// An Enemy Ship, floats on the "sea"
//--------------------------------------------------
#include "Missle.h"

#define MISSLE_SPEED 5

CMissle::CMissle()
{
	SetObjectID(MISSLE);
	SetActive(false);
}

void CMissle::Think()
{
	m_vecPosition = m_vecPosition + (m_vecForward * MISSLE_SPEED);
	CalculateBoundingBox();
	if ( m_vecPosition.x < 0 || m_vecPosition.x > 400 || 
     	 m_vecPosition.z < 0 || m_vecPosition.z > 400 )
	{
		//we're out of the playing zone
		SetActive(false);
	}
}

void CMissle::Collide(CEntity* p_OtherEntity)
{
	//The missle doesn't handle it's collisions, the other entity does.
}