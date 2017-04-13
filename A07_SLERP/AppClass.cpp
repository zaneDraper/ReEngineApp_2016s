#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - YOUR USER NAME GOES HERE"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
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

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static float fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	float fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	vector3 sunPosition(x, y, z);
	vector3 sunRotationV(0.f, 0.f, 0.f);
	matrix4 sunScale = glm::scale(5.936f, 5.936f, 5.936f);
	matrix4 sunTransform = glm::translate(sunPosition);

	vector3 earthPosition(0.f, 11.f, 0.f);
	matrix4 earthScale = glm::scale(.524f, .524f, .524f);
	matrix4 earthTransform = glm::translate(earthPosition);

	vector3 moonPosition(0.f, 2.f, 0.f);
	matrix4 moonScale = glm::scale(.27f, .27f, .27f);
	matrix4 moonTransform = glm::translate(moonPosition);

	float earthOrbPercent = MapValue(fRunTime, 0.f, fEarthHalfOrbTime, 0.f, 1.f);
	float earthRotPercent = MapValue(fRunTime, 0.f, fEarthHalfRevTime, 0.f, 1.f);
	float moonOrbPercent = MapValue(fRunTime, 0.f, fMoonHalfOrbTime, 0.f, 1.f);

	matrix4 earthRot = glm::mat4_cast(glm::mix(glm::quat(vector3(0.f, 0.f, 90.f)), glm::quat(vector3(0.f, 179.5f, 90.f)), earthRotPercent));
	matrix4 earthOrb = glm::mat4_cast(glm::mix(glm::quat(vector3(0.f, 0.f, 0.f)), glm::quat(vector3(0.f, 0.f, 179.5f)), earthOrbPercent));
	matrix4 moonOrb = glm::mat4_cast(glm::mix(glm::quat(vector3(0.f, 0.f, 0.f)), glm::quat(vector3(0.f, 0.f, 179.5f)), moonOrbPercent));

	matrix4 sunTran = sunTransform;
	matrix4 earthTran = sunTran * earthOrb * earthTransform;
	matrix4 moonTran = earthTran * moonOrb * moonTransform;

	matrix4 m_mSunMat = sunTran * sunScale;
	matrix4 m_mEarthMat = earthTran * earthRot * earthScale;
	matrix4 m_mMoonMat = moonTran * moonScale;
	
	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m_mSunMat, "Sun");
	m_pMeshMngr->SetModelMatrix(m_mEarthMat, "Earth");
	m_pMeshMngr->SetModelMatrix(m_mMoonMat, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	int nEarthOrbits = fRunTime / (fEarthHalfOrbTime * 2);
	int nEarthRevolutions = fRunTime / (fEarthHalfRevTime * 2);
	int nMoonOrbits = fRunTime / (fMoonHalfOrbTime * 2);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pMeshMngr->ClearRenderList();

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}