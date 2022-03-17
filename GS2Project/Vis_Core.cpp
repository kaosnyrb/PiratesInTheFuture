//-------------------------------------------------------
// The Visualisation component
// Bryn Stringer 
//-------------------------------------------------------
#include "Vis_Core.h"
#include "Camera.h"

#define MAXRENDERENTITYS 1000 //How many models you can render on screen at once.
#define MAXTRIANGLELISTS 5//How many triangle lists are allowed
#define DEBUG_WIREFRAME 0//when == 1 renders triangle lists in wireframe.

CVis_Core::CVis_Core(LPDIRECT3D9* graphicobject,IDirect3DDevice9* graphicDevice)
{
	d3dObject = graphicobject;
	d3dDevice = graphicDevice;
	SetupScene();//some basic directx calls.
	m_cMainCamera = new Camera();
	//Create our set of render entitys to store the scene data in
	for ( int i = 0; i < MAXRENDERENTITYS; i++ )
	{
		CRenderEntity* tempRenderEntity = new CRenderEntity();
		m_RenderEntitys.push_back(tempRenderEntity);
	}
	for ( int i = 0; i < MAXTRIANGLELISTS; i++ )
	{
		CTriangle_List* templist = new CTriangle_List(d3dDevice,"water06.jpg");
		m_TriangleLists.push_back(templist);
	}
	D3DXVECTOR3 Skyboxmin(0,0,0);
	D3DXVECTOR3 Skyboxmax(0,0,0);
	LoadSimpleModel("skybox.x",&Skyboxmin,&Skyboxmax);
	CreateBaseLight();
}

CVis_Core::~CVis_Core(void)
{
	for ( unsigned int i = 0; i < m_Meshes.size(); i++ )
	{
		delete m_Meshes[i];
	}
	for ( unsigned int i = 0; i < m_RenderEntitys.size(); i++ )
	{
		delete m_RenderEntitys[i];
	}
	for ( unsigned int i = 0; i < m_TriangleLists.size(); i++ )
	{
		if ( m_TriangleLists[i]->GetActive() )
		{
			m_TriangleLists[i]->ReleaseBuffers();
		}
		delete m_TriangleLists[i]; 
	}
	delete m_cMainCamera;
}


int CVis_Core::Render()
{
	//Set our view matrix
	d3dDevice->Clear(0,NULL,D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(255,255,255),1.0f,0);
	d3dDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	d3dDevice->BeginScene();
	if ( DEBUG_WIREFRAME == 1 ){d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );}
	//Set up our camera
	D3DXMATRIX matView, matSave;
	matView = m_cMainCamera->GetCameraViewMatrix();

		//SkyBox	
		matSave = matView;
		matView._41 = 0.0f; 
		matView._42 = -0.4f; 
		matView._43 = 0.0f;
		d3dDevice->SetTransform( D3DTS_VIEW, &matView ); 
		d3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
		d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		d3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		//Render the skybox
		m_Meshes[0]->Render(d3dDevice,util_vector3d(0,0,0),util_vector3d(0,0,0));
	
	//Restore the real camera and draw settings
	d3dDevice->SetTransform( D3DTS_VIEW, &matSave ); 
	d3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	d3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	//Render all items in the m_RenderEntitys vector
	d3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(20,20,20) );
	for ( unsigned int i = 0; i < m_RenderEntitys.size(); i++ )
	{
		if ( m_RenderEntitys[i]->GetActive() )
		{
			m_Meshes[m_RenderEntitys[i]->GetModelIndex()]->Render(d3dDevice,
				m_RenderEntitys[i]->m_vecPosition,m_RenderEntitys[i]->m_vecRotation);
		}
	}
	//Render all our trianglelists
	//sea needs different ambient (becuase it's kinda transparent
	if ( DEBUG_WIREFRAME == 1 ){d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );}

	d3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(100,100,160) );
	for ( unsigned int i = 0; i < m_TriangleLists.size(); i++ )
	{
		if ( m_TriangleLists[i]->GetActive() )
		{
			m_TriangleLists[i]->Render(d3dDevice);
		}
	}
	//finish up
	d3dDevice->EndScene();
	d3dDevice->Present(NULL,NULL,NULL,NULL);
	return 0;
}

void CVis_Core::SetupScene()
{
	//Create our projection matrix
	D3DXMATRIX matProj;
	FLOAT fAspect = ((FLOAT)800) / 600;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 1000.0f );
	d3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	d3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(30,30,30) );
	d3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	d3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	d3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
}

int CVis_Core::LoadSimpleModel(LPCSTR p_Filename,D3DXVECTOR3* minBounds,D3DXVECTOR3* maxBounds)
{
	Mesh* Tempmesh = new Mesh(d3dDevice,p_Filename,minBounds,maxBounds);
	m_Meshes.push_back(Tempmesh);
	unsigned int newMeshSize = (unsigned int)m_Meshes.size() - 1;
	return newMeshSize;
}

void CVis_Core::ClearRenderEntityList()
{
	for ( unsigned int i = 0; i < m_RenderEntitys.size(); i++ )
	{
		m_RenderEntitys[i]->SetActive(false);
	}
	for ( unsigned int i = 0; i < m_TriangleLists.size(); i++ )
	{
		if ( m_TriangleLists[i]->GetActive() )
		{
			m_TriangleLists[i]->ReleaseBuffers();
		}
		m_TriangleLists[i]->SetActive(false);
	}
}

void CVis_Core::ClearNonStaticRenderEntityList()
{
	for ( unsigned int i = 0; i < m_RenderEntitys.size(); i++ )
	{
		if ( !m_RenderEntitys[i]->GetStatic() )
		{
			m_RenderEntitys[i]->SetActive(false);
		}
	}
	for ( unsigned int i = 0; i < m_TriangleLists.size(); i++ )
	{
		if ( m_TriangleLists[i]->GetActive() )
		{
			m_TriangleLists[i]->ReleaseBuffers();
		}
		m_TriangleLists[i]->SetActive(false);	
	}
}

void CVis_Core::AddModelToRenderList(int p_ModelID,util_vector3d p_vecPosition,util_vector3d p_vecRotation, bool p_static)
{
	bool bFoundFreeSpace = false;
	unsigned int i = 0;
	while(!bFoundFreeSpace)
	{
		if ( i < m_RenderEntitys.size() )
		{
			if ( !m_RenderEntitys[i]->GetActive() )
			{
				//this index is not being used, hijack.
				m_RenderEntitys[i]->SetModelIndex(p_ModelID);
				m_RenderEntitys[i]->m_vecPosition = p_vecPosition;
				m_RenderEntitys[i]->m_vecRotation = p_vecRotation;
				m_RenderEntitys[i]->SetActive(true);
				m_RenderEntitys[i]->SetStatic(p_static);
				bFoundFreeSpace = true;
			}
		}
		else
		{
			bFoundFreeSpace = true;//This is a lie, we couldn't find any free spaces.
								   //Easy solution is to just not draw the model.
		}
		i++;
	}
}

void CVis_Core::PositionCamera(util_vector3d p_vecPosition,util_vector3d p_vecRotation)
{
	m_cMainCamera->SetPosition(p_vecPosition);
	m_cMainCamera->SetRotation(p_vecRotation);
}


void CVis_Core::RotateCamera(int Axis, float Degrees)
{
	util_vector3d temp(m_cMainCamera->GetRotationAngles().x,
						m_cMainCamera->GetRotationAngles().y,
						m_cMainCamera->GetRotationAngles().z);
	switch(Axis)
	{
		case 0://y
			temp.y += Degrees;
			m_cMainCamera->SetRotation(temp);
			break;
		case 1://x
			if ( (temp.x < 45 && Degrees > 0)|| (temp.x > -45 && Degrees < 0))
			{
				temp.x += Degrees;
				m_cMainCamera->SetRotation(temp);
			}
			break;
		case 2://z
			temp.z += Degrees;
			m_cMainCamera->SetRotation(temp);
			break;
	}
}

void CVis_Core::MoveCamera(int Axis, float Distance)
{
	D3DXVECTOR3* temp = new D3DXVECTOR3();
	D3DXVECTOR3* scaled = new D3DXVECTOR3();
	D3DXVECTOR3* location = new D3DXVECTOR3(m_cMainCamera->GetPosition().x,m_cMainCamera->GetPosition().y,m_cMainCamera->GetPosition().z);
	util_vector3d output;
	switch(Axis)
	{
		case 0://y
			D3DXVec3Scale(scaled,&m_cMainCamera->up,Distance);//Scale the movement we'll do
			D3DXVec3Add(temp,location,scaled);//add it to the current position
			output.x = temp->x;
			output.y = temp->y;
			output.z = temp->z;
			m_cMainCamera->SetPosition(output);
			break;
			
		case 1://x
			D3DXVec3Scale(scaled,&m_cMainCamera->right,Distance);//Scale the movement we'll do
			D3DXVec3Add(temp,location,scaled);//add it to the current position
			output.x = temp->x;
			output.y = temp->y;
			output.z = temp->z;
			m_cMainCamera->SetPosition(output);
			break;
		case 2://z
			D3DXVec3Scale(scaled,&m_cMainCamera->forward,Distance);//Scale the movement we'll do
			D3DXVec3Add(temp,location,scaled);//add it to the current position
			output.x = temp->x;
			output.y = temp->y;
			output.z = temp->z;
			m_cMainCamera->SetPosition(output);
			break;
	}
	delete temp;
	delete scaled;
	delete location;
}

void CVis_Core::AddTriangleListToRenderList(int p_xRes, int p_yRes,util_vector3d p_vecMin, util_vector3d p_vecMax, int* Array)
{
	D3DXVECTOR3 tempmin(p_vecMin.x,p_vecMin.y,p_vecMin.z);
	D3DXVECTOR3 tempmax(p_vecMax.x,p_vecMax.y,p_vecMax.z);
	bool bFoundFreeSpace = false;
	unsigned int i = 0;
	while(!bFoundFreeSpace)
	{
		if ( i < m_TriangleLists.size() )
		{
			if ( !m_TriangleLists[i]->GetActive() )
			{
				//this index is not being used, hijack.
				m_TriangleLists[i]->Create(d3dDevice,p_xRes,p_yRes,tempmin,tempmax,Array);
				bFoundFreeSpace = true;
			}
		}
		else
		{
			bFoundFreeSpace = true;//This is a lie, we couldn't find any free spaces.
								   //Easy solution is to just not draw the model.
		}
		i++;
	}
}

void CVis_Core::CreateBaseLight()
{
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type    = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.9f;
	light.Diffuse.a = 1.0f;
	light.Range   = 100000.0f;
	// Create a direction for our light - it must be normalized 
	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(-1.5f,0.3f,1.5);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	// Tell the device about the light and turn it on
	d3dDevice->SetLight( 0, &light );
	d3dDevice->LightEnable( 0, TRUE ); 
}