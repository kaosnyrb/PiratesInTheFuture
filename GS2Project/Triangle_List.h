//----------------------------------------------------------
// A group of triangles.
// Bryn Stringer F6063476 
// University of Teesside: Computer Games Programming
//----------------------------------------------------------
#include "util_vector3d.h"
#include "Vis_Core.h"//Nasty dependancy but gives access to direct X stuff.

#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H
class CTriangle_List
{
	public:
		CTriangle_List(IDirect3DDevice9* d3dDevice, LPCSTR TextureName);
		~CTriangle_List();
		void Create(IDirect3DDevice9* d3dDevice, int cellcountx, int cellcounty,D3DXVECTOR3 minBounds,D3DXVECTOR3 maxBounds, int *p_HeightArray = NULL);
		void Render(IDirect3DDevice9* d3dDevice);
		void ReleaseBuffers() {m_vb->Release();m_ib->Release();}
		void SetActive(bool p_active){m_bActive = p_active;}
		int GetActive(){return m_bActive;}
	private:
		LPDIRECT3DVERTEXBUFFER9 m_vb; //vertex buffer
		LPDIRECT3DINDEXBUFFER9 m_ib; //index buffer
		D3DMATERIAL9 m_material; //material 
		LPDIRECT3DTEXTURE9 gTexture;
		int TotalVerts;
		int TotalTriangles;
		bool m_bActive;
};
#endif