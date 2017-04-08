//Author: Zane T Draper
//
//Class: DSA2 w/ Luis Bobadillo Sotelo
//
//Assistance: Instructors original code and engine

#include "MyCamera.h"


//constructor - sets up position and rotation and then the direction vectors
MyCamera::MyCamera()
{
	//initial variables
	rotation = glm::quat(vector3(0, 0, 0));

	position = vector3(0, 0, 0);
	target = position + vector3(0, 0, 1);
	top = position + vector3(0, 1, 0);

	up = vector3(0, 1, 0);
	forward = vector3(1, 0, 0);
	right = vector3(0, 0, 1);
}

//destructor - no pointers or variables to delete
MyCamera::~MyCamera()
{
}

//returns the view of the camera
matrix4 MyCamera::GetView()
{
	//updates the target and top positions every frame
	target = position + forward;
	top = position + up;

	//returns the view
	return glm::lookAt(position, target, up);
}

//returns the projection of the camera, with a choice of style
matrix4 MyCamera::GetProjection(bool bOrthographic)
{
	if (bOrthographic) {
		return glm::ortho(6.f * (1080.0f / 768.0f), 1.5f * (1080.0f / 768.0f), -1.5f, 1.5f, 0.01f, 1000.0f);
	}
	else return glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
}

//directly sets the position of the camera
void MyCamera::SetPosition(vector3 v3Position)
{
	position = v3Position;
}

//sets the target of the camera by setting the rotation to face the target
void MyCamera::SetTarget(vector3 v3Target)
{
	//update the target
	target = v3Target;

	//recalculate the directional vectors
	forward = glm::normalize(target - position);
	up = glm::normalize(glm::cross(forward, vector3(forward.z, forward.y, forward.x)));
	right = glm::normalize(glm::cross(forward, up));

	//determines the rotation based off of the new target
	matrix4 lookMat = glm::lookAt(position, v3Target, position + up);
	rotation = glm::conjugate(glm::toQuat(lookMat));
	rotation = glm::rotate(rotation, 180.f, vector3(0, 1, 0));

	//rotation.x = abs(rotation.x);
	//rotation.y = abs(rotation.y);
	//rotation.z = abs(rotation.z);
}

//sets the input value of up
void MyCamera::SetUp(vector3 v3Up)
{
	up = v3Up;
}

//moves the position forward along the current forward vector
void MyCamera::MoveForward(float fIncrement)
{
	position += (forward * fIncrement);
	target += (forward * fIncrement);
	top += (forward * fIncrement);
}

//moves the position sideways along the current right vector
void MyCamera::MoveSideways(float fIncrement)
{
	position += (right * -fIncrement);
	target += (right * -fIncrement);
	top += (right * -fIncrement);
}

//moves the position vertical along the current up value
void MyCamera::MoveVertical(float fIncrement)
{
	position += (up * fIncrement);
	target += (up * fIncrement);
	top += (up * fIncrement);
}

//rotates the quat rotation around the current x axis
void MyCamera::ChangePitch(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(1, 0, 0));

	//resets all the direction vectors to represent the new rotation
	UpdateDirectionVectors();
}

//rotates the quat rotation around the current z axis 
void MyCamera::ChangeRoll(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 0, 1));

	//resets all the direction vectors to represent the new rotation
	UpdateDirectionVectors();
}

//rotates the quat rotation around the current y axis ('up' in this environment)
void MyCamera::ChangeYaw(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 1, 0));

	//resets all the direction vectors to represent the new rotation
	UpdateDirectionVectors();
}

//easy function for updating all the direction vectors when needed
void MyCamera::UpdateDirectionVectors()
{
	up = rotation * vector3(0, 1, 0);
	right = rotation * vector3(1, 0, 0);
	forward = rotation * vector3(0, 0, 1);
}
