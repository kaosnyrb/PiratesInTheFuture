//--------------------------------------------------------
// A sea change. The mesh representing the sea, handles water movement
// Bryn Stringer
//--------------------------------------------------------
#include "Vis_Core.h"

#ifndef SEA_H
#define SEA_H
class CSea
{
	public:
		CSea();
		~CSea();
		void Think(CVis_Core* p_Vis_Core);
		void AddRipple(util_vector3d p_vecPosition, int p_iMagnitude = 100);
		float GetHeightAtPoint(util_vector3d p_vecPosition);
	private:
		int Width;
		int Height;
		int Buffer1[100][100];
		int Buffer2[100][100];
		int tick;
		util_vector3d m_vecMin;
		util_vector3d m_vecMax;
};

#endif