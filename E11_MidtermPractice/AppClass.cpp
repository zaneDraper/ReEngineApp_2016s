#include "AppClass.h"
void AppClass::Update(void)
{
#pragma region DOES NOT NEED CHANGES
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma endregion
#pragma region FEEL FREE TO USE THIS CLOCK
	//Calculate delta and total times
	static float dTotalTime = 0.0; //Total time of the simulation
	float dDeltaTime = m_pSystem->LapClock(); //time difference between function calls
	dTotalTime += dDeltaTime; //Incrementing the time differences 
#pragma endregion
#pragma region YOUR CODE GOES HERE

	//interpelation for rotation and translation
	static float startLerp = 0.0;
	static float endLerp = 5.0;
	float fPercent = MapValue(dTotalTime, startLerp, endLerp, 0.0f, 1.0f);
	float rotation = MapValue(dTotalTime, startLerp, endLerp, 0.0f, 360.0f);

	//keep the translation from moving too far out
	if (fPercent >= 2) {
		fPercent = 0.0f;
		startLerp = dTotalTime;
		endLerp = startLerp + 5.0f;
	}
	//starts reducing the distance of the translation
	else if (fPercent >= 1) {
		float temp = fPercent - 1;
		fPercent = 1 - temp;
	}

	//set the rotation and translation matrix
	matrix4 rotMat = glm::rotate(IDENTITY_M4, rotation, vector3(0, 0, 1));
	matrix4 distance = glm::translate(0.0f, glm::lerp(0.0f, 5.0f, fPercent), 0.0f);

	m_m4Steve = rotMat * distance;
#pragma endregion
#pragma region DOES NOT NEED CHANGES
	//Set the model matrix
	m_pMeshMngr->SetModelMatrix(m_m4Steve, "Steve");
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine("\n" + m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Seconds:");
	m_pMeshMngr->PrintLine(std::to_string(dTotalTime), RERED);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}
