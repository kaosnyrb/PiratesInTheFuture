//--------------------------------------------------
// An Enemy Ship, floats on the "sea"
//--------------------------------------------------
#include "EnemyShip.h"

CEnemyShip::CEnemyShip(CSea* p_Sea)
{
	Sea_Acess = p_Sea;//we need to know the sea to get height and add effects to it.
	SetObjectID(ENEMY);
}

void CEnemyShip::Think()
{
	m_vecPosition.y = (float)Sea_Acess->GetHeightAtPoint(m_vecPosition);
	CalculateBoundingBox();//Find our bounding box again.
}

void CEnemyShip::Collide(CEntity* p_OtherEntity)
{
	if ( p_OtherEntity->GetObjectID() == MISSLE )
	{
		//We've been hit by a missle. Blow up.
		p_OtherEntity->SetActive(false);
		Sea_Acess->AddRipple(m_vecPosition,500);
		SetActive(false);
	}
}