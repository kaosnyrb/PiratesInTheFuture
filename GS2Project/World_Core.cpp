//--------------------------------------------------------
// The game world
// Bryn Stringer 
//--------------------------------------------------------
#include "World_Core.h"

#define TICKLENGTH 40

CWorld_Core::CWorld_Core(CVis_Core* p_Vis_Core)
{
	//Save access to the graphics component.
	Graphics = p_Vis_Core;
	//Get the min max of the model for it's bounding box
	D3DXVECTOR3 Shipmin(0,0,0);
	D3DXVECTOR3 Shipmax(0,0,0);
	int shipid = Graphics->LoadSimpleModel("ship1.x",&Shipmin,&Shipmax);
	//Create the sea.
	Sea_Acess = new CSea();
	//Populate the world with some enemy ships
	for ( int i = 0; i < 10; i++ )
	{
		CEnemyShip* tempentity = new CEnemyShip(Sea_Acess);
		tempentity->SetModelIndex(shipid);
		tempentity->SetMinMaxBounds(util_vector3d(Shipmin.x,Shipmin.y,Shipmin.z),util_vector3d(Shipmax.x,Shipmax.y,Shipmax.z));
		tempentity->m_vecPosition = util_vector3d((float)10 + rand()%390,(float)1,(float)20 + rand()%380);
		tempentity->m_vecRotation = util_vector3d(0,(float)(rand()%100),0);
		m_vObjectList.push_back(tempentity);
	}
	//Load the missle and get it's min/max
	D3DXVECTOR3 Misslemin(0,0,0);
	D3DXVECTOR3 Misslemax(0,0,0);
	int missleid = Graphics->LoadSimpleModel("missle.x",&Misslemin,&Misslemax);
	//Load a set of missles into the list so we can create them without using memory allocations
	for ( int i = 0; i < 10; i++ )
	{
		CMissle* tempmissle = new CMissle();
		tempmissle->SetModelIndex(missleid);
		tempmissle->SetMinMaxBounds(util_vector3d(Misslemin.x,Misslemin.y,Misslemin.z),util_vector3d(Misslemax.x,Misslemax.y,Misslemax.z));
		tempmissle->m_vecPosition = util_vector3d(0,0,0);
		tempmissle->m_vecRotation = util_vector3d(0,0,0);
		m_vObjectList.push_back(tempmissle);
	}
	//Add the player to the m_vObjectList
	D3DXVECTOR3 Playermin(0,0,0);
	D3DXVECTOR3 Playermax(0,0,0);
	int playermodelid = Graphics->LoadSimpleModel("player.x",&Playermin,&Playermax);
	CPlayer* tempplayer = new CPlayer(Sea_Acess);
	tempplayer->SetModelIndex(playermodelid);
	tempplayer->m_vecPosition = util_vector3d(100,0,100);
	tempplayer->m_vecRotation = util_vector3d(0,0,0);
	tempplayer->SetMinMaxBounds(util_vector3d(Playermin.x,Playermin.y,Playermin.z),util_vector3d(Playermax.x,Playermax.y,Playermax.z));
	m_vObjectList.push_back(tempplayer);
	//Finally scatter the world with some random pylon scenery
	D3DXVECTOR3 Pylonmin(0,0,0);
	D3DXVECTOR3 Pylonmax(0,0,0);
	int pylonid = Graphics->LoadSimpleModel("Pylon.x",&Shipmin,&Shipmax);
	for ( int i = 0; i < 10; i++ )
	{
		CStatic* temppylon = new CStatic();
		temppylon->SetModelIndex(pylonid);
		temppylon->SetMinMaxBounds(util_vector3d(Pylonmin.x,Pylonmin.y,Pylonmin.z),util_vector3d(Pylonmax.x,Pylonmax.y,Pylonmax.z));
		temppylon->m_vecPosition = util_vector3d((float)10 + rand()%390,(float)1,(float)20 + rand()%380);
		temppylon->m_vecRotation = util_vector3d(0,0,0);
		m_vObjectList.push_back(temppylon);
	}
	//Place the camera in an arbitary position till it is linked to the player.
	Graphics->PositionCamera(util_vector3d(200,500,200),util_vector3d(89,0,0));
	lasttickcount = GetTickCount();
	m_iPlayer_FireCooldown = 0;
}

CWorld_Core::~CWorld_Core()
{
	for ( unsigned int i = 0; i < m_vObjectList.size(); i++ )
	{
		delete m_vObjectList[i];
	}
	delete Sea_Acess;
}

void CWorld_Core::World_Main()
{
	//Run the world
	if( GetTickCount() > lasttickcount + TICKLENGTH )
	{
		lasttickcount = GetTickCount();		
		//Clean out the render list, called every frame so we can update things positions
		//There is also an option to render static models, ones which never move.
		//They aren't deleted every frame and only need to be made once.
		Graphics->ClearNonStaticRenderEntityList();
		for ( unsigned int i = 0; i < m_vObjectList.size(); i++ )
		{
			//Don't think/render/collide if your inactive
			if ( m_vObjectList[i]->GetActive() )
			{
				m_vObjectList[i]->Think();//Run your code
				Graphics->AddModelToRenderList(m_vObjectList[i]->GetModelIndex(),
					m_vObjectList[i]->m_vecPosition,m_vObjectList[i]->m_vecRotation);//Add the model to be rendered
				//Object Specfic, depending on the object ID.
				//Each child of the entity class set there own IDs.
				if ( m_vObjectList[i]->GetObjectID() == PLAYER )
				{
					CPlayer* tempplayer = (CPlayer*)m_vObjectList[i];
					//Place the camera behind the player, and rotate it correctly
					util_vector3d CameraLocation = m_vObjectList[i]->m_vecPosition - (tempplayer->GetForward()*65) + util_vector3d(0,15,0);
					CameraLocation.y = 15;
					Graphics->PositionCamera(CameraLocation,m_vObjectList[i]->m_vecRotation * 57.3f );//57.3 = radian to degress
				}
				//Collisions!
				for ( unsigned int j = 0; j < m_vObjectList.size(); j++ )
				{
					if ( m_vObjectList[j]->GetActive() )
					{
						//Don't collide with yourself!
						if ( j != i )
						{
							//Check for collisions
							if ( ObjectsColliding(m_vObjectList[i],m_vObjectList[j]) )
							{
								//Tell the object it's collided with the other object
								m_vObjectList[i]->Collide(m_vObjectList[j]);
							}
						}
					}
				}
				//handled all collisions with this object.
			}
		}
		//all objects have ran there code.
		Sea_Acess->Think(Graphics);
		if ( m_iPlayer_FireCooldown > 0 )
		{
			m_iPlayer_FireCooldown--;
		}
	}
}

bool CWorld_Core::ObjectsColliding(CEntity* object1,CEntity* object2)
{
	//Collision work interestingly, we check if objects aren't colliding.
	//however if an object fails all these checks then it IS colliding
	util_vector3d object1_MinVec = object1->GetMinBound();
	util_vector3d object1_MaxVec = object1->GetMaxBound();
	util_vector3d object2_MinVec = object2->GetMinBound();
	util_vector3d object2_MaxVec = object2->GetMaxBound();
	//X axis
	if ( object1_MinVec.x > object2_MaxVec.x )
	{
		return false;
	}
	if ( object1_MaxVec.x < object2_MinVec.x )
	{
		return false;
	}
	//Y axis
	if ( object1_MinVec.y > object2_MaxVec.y )
	{
		return false;
	}
	if ( object1_MaxVec.y < object2_MinVec.y )
	{
		return false;
	}
	//Z Axis
	if ( object1_MinVec.z > object2_MaxVec.z )
	{
		return false;
	}
	if ( object1_MaxVec.z < object2_MinVec.z )
	{
		return false;
	}
	//If we've reached this point then we're colliding
	return true;
}

void CWorld_Core::Player_FireMissle()
{
	if ( m_iPlayer_FireCooldown != 0 )
	{
		return; //still cooling down
	}
	CPlayer* tempplayer;
	bool FoundPlayer = false;
	//Find the player in the objectlist
	for ( unsigned int i = 0;i < m_vObjectList.size() && !FoundPlayer; i++ )
	{
		if ( m_vObjectList[i]->GetObjectID() == PLAYER )
		{
			tempplayer = (CPlayer*)m_vObjectList[i];
			FoundPlayer = true;
		}
	}
	for ( unsigned int i = 0; i < m_vObjectList.size(); i++ )
	{
		if ( m_vObjectList[i]->GetObjectID() == MISSLE && !m_vObjectList[i]->GetActive() )
		{
			//We've found an unused missle
			//pass it to the player
			tempplayer->FireMissle(m_vObjectList[i]);
			m_iPlayer_FireCooldown = 10;
			return;
		}
	}
}

void CWorld_Core::Player_Move(int direction)
{
	//Tell the player to move, used by the control_core.
	for ( unsigned int i = 0; i < m_vObjectList.size(); i++ )
	{
		if ( m_vObjectList[i]->GetObjectID() == PLAYER )
		{
			CPlayer* tempplayer = (CPlayer*)m_vObjectList[i];
			tempplayer->Move(direction);
		}
	}
}
