//----------------------------------------------------
// The base class for every object in the world
//----------------------------------------------------
#include "util_vector3d.h"
#include <d3dx9.h>
#pragma once

//-------------------------------------------------
// Each obect stores what it is, meaning that it can
// be cast into the correct type with ease.
//-------------------------------------------------
enum //objectID enum
{
	PLAYER,
	ENEMY,
	MISSLE,
	STATIC
};

enum //Bounding box positions
{
	//POSITION			       X / Y / Z
	LEFT_BOTTOM_FRONT,		//MIN,MIN,MIN
	RIGHT_BOTTOM_FRONT,		//MAX,MIN,MIN
	LEFT_BOTTOM_BACK,		//MIN,MIN,MAX
	RIGHT_BOTTOM_BACK,		//MAX,MIN,MAX
	LEFT_TOP_FRONT,			//MIN,MAX,MIN
	RIGHT_TOP_FRONT,		//MAX,MAX,MIN
	LEFT_TOP_BACK,			//MIN,MAX,MAX
	RIGHT_TOP_BACK			//MAX,MAX,MAX
};

class CEntity
{
	public:
		CEntity();
		util_vector3d m_vecPosition;
		util_vector3d m_vecRotation;
		void SetModelIndex(int p_index){m_iModelIndex = p_index;}
		int GetModelIndex(){return m_iModelIndex;}
		virtual void Think(){};
		virtual void Collide(CEntity* p_OtherEntity){};
		//-------------------------------------------------
		// Object Id
		//-------------------------------------------------
		const int GetObjectID() { return m_iObjectID; }
		void SetObjectID(int ID) { m_iObjectID = ID; }
		//Are you alive?
		void SetActive(bool p_active){m_bActive = p_active;}
		int GetActive(){return m_bActive;}
		//-------------------------------------------------
		//Bounding box data
		//-------------------------------------------------
		void SetMinMaxBounds(util_vector3d pvecMin,util_vector3d pvecMax);
		util_vector3d GetMinBound() { return m_vecMin; }
		util_vector3d GetMaxBound() { return m_vecMax; }

	private:
		int m_iModelIndex;
		int m_iObjectID;
		bool m_bActive;
		util_vector3d m_vecMin;//world coords!
		util_vector3d m_vecMax;//world coords!
		util_vector3d m_vecInitialBounding[8]; //the model version
	protected:
		//So each child can set his boundings
		void SetInitialBounding(int p_iIdentifer, util_vector3d plocation)
		{
			if ( p_iIdentifer < 8 && p_iIdentifer >= 0 )
			{
				m_vecInitialBounding[p_iIdentifer] = plocation;
			}
		}
		void CalculateBoundingBox();
};
