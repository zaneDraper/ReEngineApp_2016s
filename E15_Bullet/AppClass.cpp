#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
	super::InitWindow("Bullet integration"); // Window Name
}

void AppClass::InitVariables(void)
{
	//Set the camera at a position other than the default
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 10.0f, 25.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	std::vector<vector3> vVectors;
	m_nCubes = 100;
	for (int n = 0; n < m_nCubes; n++)
	{
		vVectors.push_back(vector3(0.0f, 2 * n + 2, -0.1 * n));
		m_pMeshMngr->LoadModel("Sorted\\Polycube.obj", "Cube", false, glm::translate(matrix4(1.0f), vVectors[n]));
	}

	//Setup world, Broadphase, collision configuration, solver
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	//Ground
	groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	//Cube
	fallShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	for (int n = 0; n < m_nCubes; n++)
	{
		fallMotionState.push_back(
			new btDefaultMotionState(
				btTransform(
					btQuaternion(0, 0, 0, 1),
					btVector3(
						vVectors[n].x,
						vVectors[n].y,
						vVectors[n].z)))
		);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState[n], fallShape, fallInertia);
		fallRigidBody.push_back(new btRigidBody(fallRigidBodyCI));
		dynamicsWorld->addRigidBody(fallRigidBody[n]);
	}
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

	dynamicsWorld->stepSimulation(1 / 60.0f, 1);

	btTransform trans;
	btVector3 position;
	btQuaternion orientation;
	vector3 v3Position;
	quaternion qOrientation;
	matrix4 mToWorld;
	for (int n = 0; n < m_nCubes; n++)
	{
		fallRigidBody[n]->getMotionState()->getWorldTransform(trans);
		position = trans.getOrigin();
		orientation = trans.getRotation();
		v3Position = vector3(position.getX(), position.getY(), position.getZ());
		qOrientation = quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ());
		mToWorld = glm::translate(matrix4(1.0f), v3Position) * glm::mat4_cast(qOrientation);
		m_pMeshMngr->SetModelMatrix(mToWorld, n);
	}

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	m_pMeshMngr->AddPlaneToRenderList(glm::rotate(IDENTITY_M4, 90.0f, REAXISX) * glm::scale(vector3(100.0f)), REBLACK);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

														//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY);
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

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	const int nRigidBodies = static_cast<int>(fallRigidBody.size());
	for (int nRigidBody = 0; nRigidBody < nRigidBodies; nRigidBody++)
	{
		dynamicsWorld->removeRigidBody(fallRigidBody[nRigidBody]);
		if (fallRigidBody[nRigidBody]->getMotionState() != nullptr)
			delete fallRigidBody[nRigidBody]->getMotionState();
	}
	fallRigidBody.clear();

	dynamicsWorld->removeRigidBody(groundRigidBody);
	if (groundRigidBody->getMotionState() != nullptr)
		delete groundRigidBody->getMotionState();
	SafeDelete(groundRigidBody);

	SafeDelete(fallShape);
	SafeDelete(groundShape);

	SafeDelete(dynamicsWorld);
	SafeDelete(solver);
	SafeDelete(collisionConfiguration);
	SafeDelete(dispatcher);
	SafeDelete(broadphase);

	super::Release(); //release the memory of the inherited fields
}