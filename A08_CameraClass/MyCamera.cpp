//Author: Zane T Draper
//
//Class: DSA2 w/ Luis Bobadillo Sotelo
//
//Assistance: Instructors original code and engine

#include "MyCamera.h"


//constructor - sets up position and rotation and then the direction vectors
MyCamera::MyCamera()
{
	rotation = glm::quat(vector3(0, 0, 0));
	position = vector3(0, 0, 0);

	UpdateDirectionVectors();

	center = position + rotation * vector3(0, 0, -1);
}

//destructor - no pointers or variables to delete
MyCamera::~MyCamera()
{
}

//returns the view of the camera
matrix4 MyCamera::GetView()
{
	//recalculated every frame using the cameras rotation orientation
	center = position + rotation * vector3(0, 0, -1);

	return glm::lookAt(position, center, up);
}

//returns the projection of the camera, with a choice of style
matrix4 MyCamera::GetProjection(bool bOrthographic)
{
	//switches between orthographic and perspective
	if (bOrthographic) {
		return glm::ortho(position.x * (1080.0f / 768.0f), 1.5f * (1080.0f / 768.0f), -1.5f, 1.5f, 0.01f, 1000.0f);
	}
	else return glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
}

//directly sets the position of the camera
void MyCamera::SetPosition(vector3 v3Position)
{
	position = v3Position;
}

//sets the target of the camera by setting the rotation to face the target
//**currently contains a bug**
void MyCamera::SetTarget(vector3 v3Target)
{
	//gets a direction vector from the current position to the targets location
	vector3 dirVector = v3Target - position;
	//gets the magnitude to find help with getting the unit vector value
	float magnitude = glm::length(dirVector);

	//gets the unit vector values
	dirVector.x /= magnitude;
	dirVector.y /= magnitude;
	dirVector.z /= magnitude;

	//acos should return the correct orientations around each axis
	//had some trouble figuring out why the orientation weren't coming out correct
	vector3 newRot(acos(dirVector.x), acos(dirVector.y), acos(dirVector.z));

	//applies the new rotation
	rotation = glm::quat(newRot);

	//resets all the direction vectors to represent the new rotation
	UpdateDirectionVectors();
}

//sets the input value of up
void MyCamera::SetUp(vector3 v3Up)
{
	up = v3Up;
}

//moves the position forward along the current forward vector
void MyCamera::MoveForward(float fIncrement)
{
	position += (forward * -fIncrement);
}

//moves the position sideways along the current right vector
void MyCamera::MoveSideways(float fIncrement)
{
	position += (right * fIncrement);
}

//moves the position vertical along the current up value
void MyCamera::MoveVertical(float fIncrement)
{
	position += (up * fIncrement);
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
