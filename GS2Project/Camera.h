//----------------------------------------------------------
// Camera.
// Bryn Stringer F6063476 
// University of Teesside: Computer Games Programming
//----------------------------------------------------------
#include "util_vector3d.h"
#include "Vis_Core.h"//Nasty dependancy but gives access to direct X stuff.
#include "math.h"

#pragma once

#define DEGTORAD(degree) ((D3DX_PI / 180.0f) * (degree)) // converts from degrees to radians
#define RADTODEG(radian) ((180.0f / D3DX_PI) * (radian)) // converts from radians to degrees

class Camera
{
	public:
	Camera() {m_vecPosition = util_vector3d(0,0,0); m_vecRotation = util_vector3d(0,0,0);}
	void SetPosition(util_vector3d p_vecPostion){m_vecPosition = p_vecPostion;}
	void SetRotation(util_vector3d p_vecRotation){m_vecRotation = p_vecRotation;}
	util_vector3d GetPosition(){return m_vecPosition;}
	util_vector3d GetRotationAngles(){return m_vecRotation;}
	D3DXMATRIX GetCameraViewMatrix()
	{
		up = D3DXVECTOR3(0.0f,1.0f,0.0f);
		forward = D3DXVECTOR3(0.0f,0.0f,1.0f);
		right = D3DXVECTOR3(1.0f,0.0f,0.0f);
		//yaw
		D3DXMATRIX yawMatrix;
		D3DXMatrixRotationAxis(&yawMatrix, &up, DEGTORAD(m_vecRotation.y));
		D3DXVec3TransformCoord(&forward, &forward, &yawMatrix);
		D3DXVec3TransformCoord(&right, &right, &yawMatrix);
		//pitch
		D3DXMATRIX pitchMatrix;
		D3DXMatrixRotationAxis(&pitchMatrix, &right, DEGTORAD(m_vecRotation.x));
		D3DXVec3TransformCoord(&forward, &forward, &pitchMatrix);
		D3DXVec3TransformCoord(&up, &up, &pitchMatrix);
		//Do a barrel roll!
		D3DXMATRIX rollMatrix;
		D3DXMatrixRotationAxis(&rollMatrix, &forward, DEGTORAD(m_vecRotation.z));
		D3DXVec3TransformCoord(&right, &right, &rollMatrix);
		D3DXVec3TransformCoord(&up, &up, &rollMatrix);
		//Fill our matrix
		D3DXMATRIX viewMatrix;
		D3DXMatrixIdentity(&viewMatrix);
		D3DXVECTOR3 position(m_vecPosition.x,m_vecPosition.y,m_vecPosition.z);
		viewMatrix._11 = right.x; viewMatrix._12 = up.x; viewMatrix._13 = forward.x;
		viewMatrix._21 = right.y; viewMatrix._22 = up.y; viewMatrix._23 = forward.y;
		viewMatrix._31 = right.z; viewMatrix._32 = up.z; viewMatrix._33 = forward.z;

		viewMatrix._41 = - D3DXVec3Dot( &position, &right );
		viewMatrix._42 = - D3DXVec3Dot( &position, &up );
		viewMatrix._43 = - D3DXVec3Dot( &position, &forward );
		return viewMatrix;
	}
	D3DXVECTOR3 up,forward,right;// CameraAxis

	private:
	util_vector3d m_vecPosition;
	util_vector3d m_vecRotation;	//yaw = rotation around the y axis
									//pitch = rotation around the x axis
									//roll = rotation around the z axis


};