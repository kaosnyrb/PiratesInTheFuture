//-------------------------------------------------------
// The Visualisation component
// Bryn Stringer 
//-------------------------------------------------------

#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Mesh.h"
#include "RenderEntity.h"
#include "Camera.h"
#include "Triangle_List.h"

//My utils
#include "util_vector2d.h"
#include "util_vector3d.h"
#include <vector>

using namespace std;

struct BTSVERTEX_SIMPLE
{
   D3DXVECTOR3 p; // Vertex position
   D3DXVECTOR3 n; // Vertex normal
   float tu,tv;
};

#define D3DFVF_BTSVERTEX_SIMPLE (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )


class CVis_Core
{
public:
	CVis_Core(LPDIRECT3D9* graphicobject,IDirect3DDevice9* graphicDevice);
	~CVis_Core(void);
	int Render();
	int LoadSimpleModel(LPCSTR p_Filename,D3DXVECTOR3* minBounds,D3DXVECTOR3* maxBounds);//returns the position in the vector of the new model
	void ClearRenderEntityList();
	void ClearNonStaticRenderEntityList();
	void AddModelToRenderList(int p_ModelID,util_vector3d p_vecPosition,util_vector3d p_vecRotation, bool p_static = false);
	void AddTriangleListToRenderList(int p_xRes, int p_yRes,util_vector3d p_vecMin, util_vector3d p_vecMax, int* Array = NULL);
	void PositionCamera(util_vector3d p_vecPosition,util_vector3d p_vecRotation);
	void RotateCamera(int Axis, float Degrees);
	void MoveCamera(int Axis, float Distance);
private:
	LPDIRECT3D9* d3dObject;//Object
	IDirect3DDevice9* d3dDevice;//Device
	void SetupScene();
	void CreateBaseLight();
	Camera* m_cMainCamera;
	//Meshs
	vector<Mesh*> m_Meshes;
	vector<CRenderEntity*> m_RenderEntitys;
	vector<CTriangle_List*> m_TriangleLists;
};
