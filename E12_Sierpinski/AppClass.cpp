#include "AppClass.h"

void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("E12 - Instance Rendering");
	m_v4ClearColor = vector4(REBLACK, 1.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
	
	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(-1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3( 1.0f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3(0.0f,  1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);

	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();

	//at this point we have to know how many triangles will be in the number of rows desired: 3 rows = 9 triangles, 5 rows = 15 triangles, 7 rows = 27 triangles
	m_nObjects = 27;

	//declare the array and a beginner vector
	m_fMatrixArray = new float[m_nObjects * 16];
	std::vector<int> triangleArray;
	
	//call the recursive Triangle function (creates the Sierpinski triangle)
	RecursiveTriangles(7, triangleArray);
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	ClearScreen();
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//m_pMesh->Render(m4Projection, m4View, IDENTITY_M4);//Rendering nObject(s)
	m_pMesh->RenderList(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_fMatrixArray, m_nObjects);//Rendering nObjects


	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}

//This function recursive creates the triangles
void AppClass::RecursiveTriangles(int numRecursions, std::vector<int> rowValues)
{
	//creates variables that increment between recursions
	static int row = 0;
	static int indexNum = 0;

	//creates this iterations vector array
	std::vector<int> newArray;
	newArray.push_back(1);

	//if this is the first row, add the triangle to the render list and move to the next iteration
	if (rowValues.empty()) {
		//makes sure the static variables arent carrying over from a previous use
		indexNum = 0; row = 0;

		//add the first triangle to the list
		const float* m4MVP = glm::value_ptr(
			glm::translate(vector3(0.f, 0.f, 0.f))
		);
		memcpy(&m_fMatrixArray[indexNum * 16], m4MVP, 16 * sizeof(float));
		indexNum = 1;

		//calls the next recursion
		RecursiveTriangles( numRecursions - 1, newArray);

		//end recursion on return
		return;
	}

	//increment the row number
	row++;

	//create values for new array (row)
	for (int i = 0; i < rowValues.size() - 1; i++) {
		newArray.push_back(rowValues[i] + rowValues[i + 1]);
	}
	newArray.push_back(1);

	//starting location of the row on the x axis
	float xPos = -1.f * row;

	//add only the odd value locations to the triangle render list
	for (int i = 0; i < newArray.size(); i++) {
		if (newArray[i] % 2 != 0) {
			const float* m4MVP = glm::value_ptr(
				glm::translate(vector3(xPos + (2.f * i), 2.0f * -row, 0.f))
			);
			memcpy(&m_fMatrixArray[indexNum * 16], m4MVP, 16 * sizeof(float));
			indexNum++;
		}
	}

	//keep iterating unless this is the last row
	if (numRecursions == 0) return;
	else RecursiveTriangles(numRecursions - 1, newArray);
}