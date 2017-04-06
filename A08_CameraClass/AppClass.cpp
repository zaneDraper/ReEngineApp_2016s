#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Zane - A08"); // Window Name

	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
	m_pSystem->SetWindowResolution(RESOLUTIONS::C_1280x720_16x9_HD);
}

void AppClass::InitVariables(void)
{
	//Set the position of the camera
	camera.SetPosition(vector3(0.f, 0.f, 0.f));

	//set the target **bug**
	//camera.SetTarget(vector3(0.f, 0.f, 0.f));

	//Generate the Cone
	m_pCone = new PrimitiveClass();
	m_pCone->GenerateCone(1.0f, 1.0f, 10, RERED);

	//Generate the Cylinder
	m_pCylinder = new PrimitiveClass();
	m_pCylinder->GenerateCylinder(1.0f, 1.0f, 10, REGREEN);

	//Generate the Cube
	m_pCube = new PrimitiveClass();
	m_pCube->GenerateCube(1.0f, REYELLOW);

	//Generate the Sphere
	m_pSphere = new PrimitiveClass();
	m_pSphere->GenerateSphere(1.0f, 4, REBLUE);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();
	
	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), 0);
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the cone
	m_pCone->Render(camera.GetProjection(false), camera.GetView(), glm::translate(IDENTITY_M4, REAXISX * 10.0f));

	//Render the cylinder
	m_pCylinder->Render(camera.GetProjection(false), camera.GetView(), glm::translate(IDENTITY_M4, REAXISZ * -10.0f));

	//Render the cylinder
	m_pCube->Render(camera.GetProjection(false), camera.GetView(), glm::translate(IDENTITY_M4, REAXISZ * 10.0f));

	//Render the cylinder
	m_pSphere->Render(camera.GetProjection(false), camera.GetView(), glm::translate(IDENTITY_M4, REAXISX * -10.0f));

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	//delete objects from environment
	delete m_pCone;
	delete m_pCylinder;
	delete m_pSphere;
	delete m_pCube;

	super::Release(); //release the memory of the inherited fields
}