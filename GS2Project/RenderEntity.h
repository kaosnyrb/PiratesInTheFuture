//----------------------------------------------------------
// Holds which model to draw, where it is and it's rotation
// Bryn Stringer F6063476 
// University of Teesside: Computer Games Programming
//----------------------------------------------------------
#include "util_vector3d.h"

class CRenderEntity
{
	public:
		CRenderEntity();
		util_vector3d m_vecPosition;
		util_vector3d m_vecRotation;
		void SetModelIndex(int p_index){m_iModelIndex = p_index;}
		int GetModelIndex(){return m_iModelIndex;}
		void SetActive(bool p_active){m_bActive = p_active;}
		int GetActive(){return m_bActive;}
		void SetStatic(bool p_static){m_bStatic = p_static;}
		int GetStatic(){return m_bStatic;}
private:
		int m_iModelIndex;
		bool m_bActive;
		bool m_bStatic;
};