//--------------------------------------------------
// The player
//--------------------------------------------------
#include "Player.h"

CPlayer::CPlayer(CSea* p_Sea)
{
	m_vecPosition = util_vector3d(0,0,0);
	m_vecRotation = util_vector3d(0,0,0);
	m_vecForward = util_vector3d(0,0,1);
	SetObjectID(PLAYER);
	m_fDirectionInRadians = 0.0f;
	Sea_Acess = p_Sea;
	EnginePower = 0;
	m_bFiring = false;
}

void CPlayer::Think()
{
	m_vecPosition = m_vecPosition + (m_vecForward * EnginePower);
	//Convert the direction into degrees;
	m_vecRotation.y = RADTODEG(m_fDirectionInRadians);
	//Then set it to the forward vector
	m_vecForward = m_vecForward.ConvertRadianToVector(m_vecRotation.y);
	//Get the height from the sea.
	m_vecPosition.y = (float)Sea_Acess->GetHeightAtPoint(m_vecPosition); 
	//Add a engine wake depending on our speed.
	Sea_Acess->AddRipple(m_vecPosition - (m_vecForward*10) ,(int)EnginePower * 7);
	//If we collide with the outer bounds of the sea reverse our engine
	if ( m_vecPosition.x < 0 || m_vecPosition.x > 400 || 
     	 m_vecPosition.z < 0 || m_vecPosition.z > 400 )
	{
		EnginePower = -EnginePower;
	}
	CalculateBoundingBox();
	if ( EnginePower > 0 )
	{
		EnginePower = EnginePower - 0.01f;
	}
	else if ( EnginePower < 0 ) 
	{
		EnginePower = EnginePower + 0.01f;
	}
}

void CPlayer::Collide(CEntity* p_OtherEntity)
{
	if ( p_OtherEntity->GetObjectID() == ENEMY || p_OtherEntity->GetObjectID() == STATIC )
	{
		//Bounce off the enemy.
		EnginePower = -EnginePower;
	}
}

void CPlayer::FireMissle(CEntity* p_Missle)
{
	if ( p_Missle->GetObjectID() == MISSLE )
	{
		CMissle* tempmissle = (CMissle*)p_Missle;
		tempmissle->m_vecPosition = m_vecPosition + (m_vecForward*5) + util_vector3d(0,2,0);
		tempmissle->SetDirection(m_vecForward);
		tempmissle->SetActive(true);
	}	
	m_bFiring = false;
}

void CPlayer::Move(int direction)
{
	//picture the boat, it's north is 0, working round clockwise.
	if ( direction == 0 )//move forward
	{
		EnginePower += 0.03f;
	}
	if ( direction == 1 )//rotate clockwise
	{
		m_fDirectionInRadians = m_fDirectionInRadians + 0.0002f;
	}
	if ( direction == 2 )//move back
	{
		EnginePower -= 0.03f;
	}
	if ( direction == 3 )//rotate counter-clockwise
	{
		m_fDirectionInRadians = m_fDirectionInRadians - 0.0002f;
	}
}
