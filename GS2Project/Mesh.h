//----------------------------------------------------------
// The base mesh
// Bryn Stringer F6063476 
// University of Teesside: Computer Games Programming
//----------------------------------------------------------
#include "util_vector3d.h"
#include "Vis_Core.h"//Nasty dependancy but gives access to direct X stuff.

#ifndef MESH_H
#define MESH_H
class Mesh
{
	public:
	Mesh(IDirect3DDevice9* d3dDevice,LPCSTR p_Filename,D3DXVECTOR3* minBounds,D3DXVECTOR3* maxBounds);
	~Mesh();
	void GetTranslationMatrix(D3DXMATRIX &p_matTrans);
	void Render(IDirect3DDevice9* d3dDevice, util_vector3d p_vecPosition, util_vector3d p_vecRotation);
	//private:
	LPD3DXMESH dxmesh;
	LPD3DXBUFFER materialBuffer;
	DWORD numMaterials;
	D3DXMATERIAL* d3dxMaterials;
	D3DMATERIAL9 *meshMaterials;
	LPDIRECT3DTEXTURE9 *meshTextures;
};
#endif