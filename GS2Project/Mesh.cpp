//-------------------------------------------------------
// Loads and handles a .x mesh
//-------------------------------------------------------
#include "Mesh.h"

Mesh::Mesh(IDirect3DDevice9* d3dDevice,LPCSTR p_Filename,D3DXVECTOR3* minBounds,D3DXVECTOR3* maxBounds)
{
	HRESULT hr=D3DXLoadMeshFromX(p_Filename, D3DXMESH_SYSTEMMEM,
               d3dDevice, NULL,
               &materialBuffer,NULL, &numMaterials,
               &dxmesh );
	d3dxMaterials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
	meshMaterials = new D3DMATERIAL9[numMaterials];
	meshTextures = new LPDIRECT3DTEXTURE9[numMaterials];
	for (DWORD i=0; i<numMaterials; i++)
	{
       // Copy the material
       meshMaterials[i] = d3dxMaterials[i].MatD3D;
       // Set the ambient color for the material (D3DX does not do this)
       meshMaterials[i].Ambient = meshMaterials[i].Diffuse;
       // Create the texture if it exists - it may not
       meshTextures[i] = NULL;
       if (d3dxMaterials[i].pTextureFilename)
         D3DXCreateTextureFromFile(d3dDevice, d3dxMaterials[i].pTextureFilename,   &meshTextures[i]) ;
	}
	//Work out our bounding box.
	BYTE* pVertices=NULL;
	hr=dxmesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
	//D3DXVECTOR3 minBounds,maxBounds;
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, dxmesh->GetNumVertices(), D3DXGetFVFVertexSize(dxmesh->GetFVF()), minBounds, maxBounds);
	dxmesh->UnlockVertexBuffer();
	materialBuffer->Release();
}

Mesh::~Mesh()
{	
	IDirect3DTexture9_Release(*meshTextures);
	dxmesh->Release();
	delete meshMaterials;
	delete meshTextures;
}

void Mesh::Render(IDirect3DDevice9* d3dDevice, util_vector3d p_vecPosition,util_vector3d p_vecRotation)
{
	D3DXMATRIX matRotX,matRotY,matRotZ;
	D3DXMATRIX matTrans;
	//Figure out where we are?
	D3DXMatrixRotationX( &matRotX, p_vecRotation.x);
	D3DXMatrixRotationY( &matRotY, p_vecRotation.y);
	D3DXMatrixRotationZ( &matRotZ, p_vecRotation.z);
	D3DXMatrixTranslation(&matTrans,p_vecPosition.x,p_vecPosition.y,p_vecPosition.z);
	// Note: the order of multiplication here is very important
	D3DXMATRIX matWorld=(matRotY*matRotX*matRotZ)*matTrans;
	d3dDevice->SetTransform( D3DTS_WORLD, &matWorld);
	for (DWORD j=0; j<numMaterials; j++)
	{
		// Set the material and texture for this subset
		d3dDevice->SetMaterial(&meshMaterials[j]);
		d3dDevice->SetTexture(0,meshTextures[j]);
		// Draw the mesh subset
		dxmesh->DrawSubset( j );
	}
}
