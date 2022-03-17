//----------------------------------------------------------
// Holds which model to draw, where it is and it's rotation
// Bryn Stringer F6063476 
// University of Teesside: Computer Games Programming
//----------------------------------------------------------
#include "RenderEntity.h"

CRenderEntity::CRenderEntity()
{
	m_vecPosition = util_vector3d(0,0,0);
	m_vecRotation = util_vector3d(0,0,0);
	m_iModelIndex = 0;
	m_bActive = false;
}