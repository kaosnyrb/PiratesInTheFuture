//----------------------------------------------------------
// A group of triangles.
// Bryn Stringer F6063476 
// University of Teesside: Computer Games Programming
//----------------------------------------------------------
#include "Triangle_List.h"

CTriangle_List::CTriangle_List(IDirect3DDevice9* d3dDevice, LPCSTR TextureName)
{
	m_bActive = false;
	gTexture = NULL;
	D3DXCreateTextureFromFile(d3dDevice,TextureName, &gTexture );
}

CTriangle_List::~CTriangle_List()
{
	gTexture->Release();
	m_material->Release();
}

void CTriangle_List::Create(IDirect3DDevice9* d3dDevice, int cellcountx, 
							int cellcounty,D3DXVECTOR3 minBounds,D3DXVECTOR3 maxBounds,int *p_HeightArray)
{
	int VertCountX = cellcountx + 1;
	int VertCountY = cellcounty + 1;
	TotalVerts = VertCountX * VertCountY;
	TotalTriangles = cellcountx * cellcounty * 2;
	float stepX = (maxBounds.x-minBounds.x) / cellcountx;
	float stepY = (maxBounds.z-minBounds.z) / cellcounty;
	//Create index buffer first, so we can make the normals
	d3dDevice->CreateIndexBuffer(TotalTriangles*3*2,D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_ib, NULL);
	WORD *indices=NULL;
	m_ib->Lock( 0, 0, (void**)&indices, 0 );
	int indicescount=0;
	int vIndex=0;
	for (int z=0;z<cellcounty;z++)
	{
		for (int x=0;x<cellcountx;x++)
		{
			// first triangle
			indices[indicescount++]=vIndex;
			indices[indicescount++]=vIndex+1;
			indices[indicescount++]=vIndex+VertCountX;

			// second triangle
			indices[indicescount++]=vIndex+VertCountX;
			indices[indicescount++]=vIndex+1;
			indices[indicescount++]=vIndex+VertCountX+1;

			vIndex++;
		}
		vIndex++;
	}
	m_ib->Unlock();
	//Create and fill the buffer
	d3dDevice->CreateVertexBuffer( TotalVerts*sizeof(BTSVERTEX_SIMPLE),
		D3DUSAGE_WRITEONLY, D3DFVF_BTSVERTEX_SIMPLE,
		D3DPOOL_MANAGED, &m_vb, NULL );
	BTSVERTEX_SIMPLE* pVertices;
	m_vb->Lock( 0, 0, (void**)&pVertices, 0 );

	D3DXVECTOR3 pos(minBounds.x, 0, maxBounds.z);
	int count=0;
	// Loop across and down
	for (int y = 0; y < VertCountY; y++)
	{
		pos.x=minBounds.x;
		for (int x = 0; x < VertCountX; x++)
		{
			// Create the verts
			pVertices[count].p = pos;
			pVertices[count].tu = ((float)y) / (VertCountY / 8);
			pVertices[count].tv = ((float)x) / (VertCountX / 8);
			if ( p_HeightArray != 0 )
			{
				if ( p_HeightArray[(100 * x - 1 ) + y ]/ 15 > 50 ||p_HeightArray[(100 * x - 1) + y ]/ 15 < -50 )
				{
					pVertices[count].p.y = 0;
				}
				else
				{
					float tempheight = (float)p_HeightArray[(100 * x - 1) + y ];
					//Pesevere accuracy.
					pVertices[count].p.y = tempheight / 14;
					if ( pVertices[count].p.y != 0 )
					{
						//pVertices[count].colour = D3DCOLOR_XRGB(100,100,255);
					}
				}
			}
			// Increment x across
			pos.x += stepX;
			count++;
		}
		// Increment Z down
		pos.z -= stepY;
	}
	//Create Normals (only work on none edges)
	count = 1 + VertCountX;
	for (int y = 1; y < VertCountY; y++)
	{
		for (int x = 1; x < VertCountX; x++)
		{
			D3DXVECTOR3 Normal,v1,v2;
			D3DXVECTOR3 TotalNormal(0,1,0);
			//Which ones do we need to know?
			//middle = count
			//top = count - VertCountX
			//top right = count - VertCountX + 1
			//right = count + 1
			//bottom = count + VertCountX
			//bottom left = count + VertCountX - 1
			//left = count - 1
			float xpos = minBounds.x + (stepX * x);
			float ypos = minBounds.z + (stepY * y);
			D3DXVECTOR3 left(xpos - stepX,		pVertices[count - 1].p.y,				ypos);
			D3DXVECTOR3 top(xpos,				pVertices[count - VertCountX].p.y,		ypos - stepY);
			D3DXVECTOR3 topright(xpos + stepX,	pVertices[count - VertCountX + 1].p.y,	ypos - stepY);
			D3DXVECTOR3 right(xpos + stepX,		pVertices[count + 1].p.y,				ypos);
			D3DXVECTOR3 bottom(xpos,			pVertices[count + VertCountX].p.y,		ypos + stepY);
			D3DXVECTOR3 bottomleft(xpos - stepX,pVertices[count + VertCountX - 1].p.y,	ypos + stepY);
			D3DXVECTOR3 middle(xpos,			pVertices[count].p.y,					ypos);				
			//stepX = distance of a X move
			//stepY = distance of a Y move
			//First triangle
			//left -> top -> middle
			D3DXVec3Subtract(&v1, &top, &left);
			D3DXVec3Subtract(&v2, &middle, &left);
			D3DXVec3Cross(&Normal, &v1, &v2);
			D3DXVec3Normalize(&Normal, &Normal);
			TotalNormal = TotalNormal + Normal;
			//Second triangle
			//top -> topright -> middle
			D3DXVec3Subtract(&v1, &topright, &top);
			D3DXVec3Subtract(&v2, &middle, &top);
			D3DXVec3Cross(&Normal, &v1, &v2);
			D3DXVec3Normalize(&Normal, &Normal);
			TotalNormal = TotalNormal + Normal;
			//thrid triangle
			//topright -> right -> middle
			D3DXVec3Subtract(&v1, &right, &topright);
			D3DXVec3Subtract(&v2, &middle, &topright);
			D3DXVec3Cross(&Normal, &v1, &v2);
			D3DXVec3Normalize(&Normal, &Normal);
			TotalNormal = TotalNormal + Normal;
			//fourth triangle
			//middle -> right -> bottom
			D3DXVec3Subtract(&v1, &right, &middle);
			D3DXVec3Subtract(&v2, &bottom, &middle);
			D3DXVec3Cross(&Normal, &v1, &v2);
			D3DXVec3Normalize(&Normal, &Normal);
			TotalNormal = TotalNormal + Normal;
			//fifth triangle
			//middle -> bottom -> bottomleft
			D3DXVec3Subtract(&v1, &bottom, &middle);
			D3DXVec3Subtract(&v2, &bottomleft, &middle);
			D3DXVec3Cross(&Normal, &v1, &v2);
			D3DXVec3Normalize(&Normal, &Normal);
			TotalNormal = TotalNormal + Normal;
			//sixth triangle
			//middle -> bottomleft -> left
			D3DXVec3Subtract(&v1, &bottomleft, &middle);
			D3DXVec3Subtract(&v2, &left, &middle);
			D3DXVec3Cross(&Normal, &v1, &v2);
			D3DXVec3Normalize(&Normal, &Normal);
			TotalNormal = TotalNormal + Normal;
			//
			pVertices[count].n = TotalNormal;
			//pVertices[count].n.y = -pVertices[count].n.y;
			count++;
		}
	}
	m_vb->Unlock();
	ZeroMemory( &m_material, sizeof(D3DMATERIAL9) );
	m_material.Diffuse.r = m_material.Ambient.r = 1.0f;
	m_material.Diffuse.g = m_material.Ambient.g = 1.0f;
	m_material.Diffuse.b = m_material.Ambient.b = 1.0f;
	m_material.Diffuse.a = m_material.Ambient.a = 1.0f;
	m_bActive = true;
}

void CTriangle_List::Render(IDirect3DDevice9* d3dDevice)
{
	D3DXMATRIX matRotX,matRotY,matRotZ;
	D3DXMATRIX matTrans;
	//Figure out where we are?
	D3DXMatrixRotationX( &matRotX, 0);
	D3DXMatrixRotationY( &matRotY, 0);
	D3DXMatrixRotationZ( &matRotZ, 0);
	D3DXMatrixTranslation(&matTrans,0,0,0);
	D3DXMATRIX matWorld=(matRotY*matRotX*matRotZ)*matTrans;
	d3dDevice->SetTransform( D3DTS_WORLD, &matWorld);
	d3dDevice->SetMaterial( &m_material );
	d3dDevice->SetTexture(0,gTexture);
	d3dDevice->SetStreamSource( 0, m_vb,0, sizeof(BTSVERTEX_SIMPLE) );
	d3dDevice->SetFVF( D3DFVF_BTSVERTEX_SIMPLE );
	d3dDevice->SetIndices( m_ib );
	d3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,TotalVerts,0,TotalTriangles);

}