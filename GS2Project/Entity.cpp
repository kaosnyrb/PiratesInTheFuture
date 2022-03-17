//----------------------------------------------------
// The base class for every object in the world
//----------------------------------------------------
#include "Entity.h"

CEntity::CEntity()
{
	m_vecPosition = util_vector3d(0,0,0);
	m_vecRotation = util_vector3d(0,0,0);
	m_iModelIndex = 0;
	m_bActive = true;
}
void CEntity::SetMinMaxBounds(util_vector3d pvecMin,util_vector3d pvecMax)
{
	//Take the intial min and max then figure all points of the models bounding rectangle
	util_vector3d world_min = pvecMin;
	util_vector3d world_max = pvecMax;
	SetInitialBounding(LEFT_BOTTOM_FRONT,world_min);
	SetInitialBounding(RIGHT_BOTTOM_FRONT,util_vector3d(world_max.x,world_min.y,world_min.z));
	SetInitialBounding(LEFT_BOTTOM_BACK,util_vector3d(world_min.x,world_min.y,world_max.z));
	SetInitialBounding(RIGHT_BOTTOM_BACK,util_vector3d(world_max.x,world_min.y,world_max.z));
	SetInitialBounding(LEFT_TOP_FRONT,util_vector3d(world_min.x,world_max.y,world_min.z));
	SetInitialBounding(RIGHT_TOP_FRONT,util_vector3d(world_max.x,world_max.y,world_min.z));
	SetInitialBounding(LEFT_TOP_BACK,util_vector3d(world_min.x,world_max.y,world_max.z));
	SetInitialBounding(RIGHT_TOP_BACK,world_max);
}

void CEntity::CalculateBoundingBox()
{
	D3DXVECTOR3 worldBounds[8];
	D3DXVECTOR3 objectBounds[8];
	//Fill the objectbound vector
	for ( int i = 0; i < 8; i++ )
	{
		objectBounds[i].x = m_vecInitialBounding[i].x;
		objectBounds[i].y = m_vecInitialBounding[i].y;
		objectBounds[i].z = m_vecInitialBounding[i].z;
	}
	//Need to create the transform matrix
	D3DXMATRIX matRotX,matRotY,matRotZ;
	D3DXMATRIX matTrans;
	//Figure out where we are?
	D3DXMatrixRotationX( &matRotX, m_vecRotation.x);
	D3DXMatrixRotationY( &matRotY, m_vecRotation.y);
	D3DXMatrixRotationZ( &matRotZ, m_vecRotation.z);
	D3DXMatrixTranslation(&matTrans,m_vecPosition.x,m_vecPosition.y,m_vecPosition.z);
	// Note: the order of multiplication here is very important
	D3DXMATRIX matWorld=(matRotY*matRotX*matRotZ)*matTrans;
	for( int i = 0; i < 8; i++ )
	{
		D3DXVec3TransformCoord( &worldBounds[i], &objectBounds[i], &matWorld );
	}
	//we now have a bounding box, just need to get the world min and max
	m_vecMin.x=m_vecMax.x=worldBounds[0].x;
	m_vecMin.y=m_vecMax.y=worldBounds[0].y;
	m_vecMin.z=m_vecMax.z=worldBounds[0].z;

	for (int i=1;i<8;i++)
	{
		if (worldBounds[i].x < m_vecMin.x) m_vecMin.x=worldBounds[i].x;
		if (worldBounds[i].x > m_vecMax.x) m_vecMax.x=worldBounds[i].x;
		if (worldBounds[i].y < m_vecMin.y) m_vecMin.y=worldBounds[i].y;
		if (worldBounds[i].y > m_vecMax.y) m_vecMax.y=worldBounds[i].y;
		if (worldBounds[i].z < m_vecMin.z) m_vecMin.z=worldBounds[i].z;
		if (worldBounds[i].z > m_vecMax.z) m_vecMax.z=worldBounds[i].z;
	}
	//we now have the world bounds of the model, which we can use for collisions
}