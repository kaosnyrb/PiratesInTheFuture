//--------------------------------------------------------
// A sea change. The mesh representing the sea, handles water movement
// Bryn Stringer
//--------------------------------------------------------

#include "Sea.h"

CSea::CSea()
{
	Width = 100;
	Height = 100;
	for ( int i = 0; i < Width; i++ )
	{
		for ( int j = 0; j < Height; j++ )
		{
			Buffer1[i][j] = 0;
			Buffer2[i][j] = 0;
		}
	}
	tick = 0;
	m_vecMin = util_vector3d(0,0,0);
	m_vecMax = util_vector3d(400,0,400);
}

CSea::~CSea()
{

}

//--------------------------------------------------------------
//	The Sea main function
// The sea is a 3d plane which is redrawn every frame.
// Using a smoothing algorthm and two integer buffers
// It creates the effect of ripples in a pool.
//--------------------------------------------------------------
void CSea::Think(CVis_Core* p_Vis_Core)
{
	for ( int i=1; i<Width-1; i++)
	{
		for ( int j = 1; j < Height-1; j++ )
		{
			//For each point take the 8 sorrounding points smooth this one based on it
			//Buffer2 holds what the current frame should look like, buffer1 is the last frame.
			Buffer2[i][j] = (Buffer1[i-1][j] + Buffer1[i+1][j] + 
							 Buffer1[i][j+1] + Buffer1[i][j-1] +
							 Buffer1[i-1][j+1] + Buffer1[i-1][j-1] + 
							 Buffer1[i+1][j+1] + Buffer1[i+1][j-1]) / 4 - Buffer2[i][j];//4 gives smoothest result
			Buffer2[i][j] -= (Buffer2[i][j] >> 12);//needed to prevent the waves lasting forever
		}
	}
	//Tell the visualisation to render a plane and pass it the pointer to our current buffer.
	p_Vis_Core->AddTriangleListToRenderList(Width,Height,m_vecMin,m_vecMax,*Buffer2);
	//Swap the buffers, making the current frame the previous frame.
	//We still need the frame before the previous for the smoothing effect, so store it in 2.
	int temp = 0;
	for ( int i=1; i<Width; i++)
	{
		for ( int j = 1; j < Height; j++ )
		{
			temp = Buffer1[i][j];
			Buffer1[i][j] = Buffer2[i][j];
			Buffer2[i][j] = temp;
		}
	}
}

//------------------------------------------------
// Allows the creation of ripples.
// Simply a process of setting some hieghts in the
// buffer.
//------------------------------------------------
void CSea::AddRipple(util_vector3d p_vecPosition,int p_iMagnitude)
{
	float deltaPointX = (m_vecMax.x - m_vecMin.x)/100;
	float deltaPointZ = (m_vecMax.z - m_vecMin.z)/100;
	int x = (int)((p_vecPosition.x - m_vecMin.x)/deltaPointX);
	int z = (int)(100 - (p_vecPosition.z - m_vecMin.z)/deltaPointZ);
	//Make a cross like pattern around the selected point.
	//This gives smoother results than a single point.
	if ( z > 2 && z < 100 && x > 2 && x < 100 )
	{
		Buffer1[x][z] = p_iMagnitude;
		Buffer1[x + 1][z- 1] = p_iMagnitude;
		Buffer1[x - 1][z - 1] = p_iMagnitude;
		Buffer1[x + 1][z + 1] = p_iMagnitude;
		Buffer1[x - 1][z + 1] = p_iMagnitude;
	}
}

float CSea::GetHeightAtPoint(util_vector3d p_vecPosition)
{
	//As we've the heights all stored in the sea already, 
	//we can simply access them from the current buffer.
	//Works as long as the points are close enough together.
	float deltaPointX = (m_vecMax.x - m_vecMin.x)/100;
	float deltaPointZ = (m_vecMax.z - m_vecMin.z)/100;
	int x = (int)((p_vecPosition.x - m_vecMin.x)/deltaPointX);
	int z = (int)(100 - (p_vecPosition.z - m_vecMin.z)/deltaPointZ);
	if ( z > 2 && z < 100 && x > 2 && x < 100 )
	{
		float tempfloat = (float)Buffer1[x][z];
		return tempfloat / 14;
			
	}
	return 1;
}
