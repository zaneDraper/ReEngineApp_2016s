#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");
	fDuration = 1.0f;

	//************************************
	//************************************
	//MY STUFF
	//setting my variables
	curTargetIndex = 0;
	startTime = 0;
	endTime = fDuration;

	//populating the list of values
	points[0] = vector3(-4.0f, -2.0f, 5.0f);
	points[1] = vector3(1.0f, -2.0f, 5.0f);
	points[2] = vector3(-3.0f, -1.0f, 3.0f);
	points[3] = vector3(2.0f, -1.0f, 3.0f);
	points[4] = vector3(-2.0f, 0.0f, 0.0f);
	points[5] = vector3(3.0f, 0.0f, 0.0f);
	points[6] = vector3(-1.0f, 1.0f, -3.0f);
	points[7] = vector3(4.0f, 1.0f, -3.0f);
	points[8] = vector3(0.0f, 2.0f, -5.0f);
	points[9] = vector3(5.0f, 2.0f, -5.0f);
	points[10] = vector3(1.0f, 3.0f, -5.0f);

	//populate the sphere meshes and their locations
	m_pSpheres = new PrimitiveClass[11];
	m_pMatrix = new matrix4[11];

	//set location values and set meshes up
	for (int i = 0; i < 11; i++) {
		m_pSpheres[i].GenerateSphere(.2, 5, RERED);
		m_pMatrix[i] = glm::translate(points[i]);
	}
	//***********************************
	//***********************************
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static float fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here

	//after setting startTime to current runtime, and then adding on fDuration for the endTime, Map the crrent fRunTime between them
	float percent = MapValue(fRunTime, startTime, endTime, 0.0f, 1.0f);

	//Loop the curTargetIndex back to start when it reaches the last circle
	if (percent >= 1.0f) {
		if (curTargetIndex != 10)
			curTargetIndex++;
		else curTargetIndex = 0;

		startTime = fRunTime;
		endTime = fRunTime + fDuration;
		percent = 0;
	}

	//set the creepers current spot based on the MapValue
	matrix4 m4Creeper;
	if (curTargetIndex != 10)
		m4Creeper = glm::translate(glm::lerp(points[curTargetIndex], points[curTargetIndex + 1], percent));
	else m4Creeper = glm::translate(glm::lerp(points[curTargetIndex], points[0], percent));

	m_pMeshMngr->SetModelMatrix(m4Creeper, "WallEye");

#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	for (int i = 0; i < 11; i++) {
		m_pSpheres[i].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_pMatrix[i]);
	}

	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}