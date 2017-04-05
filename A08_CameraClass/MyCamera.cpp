#include "MyCamera.h"



MyCamera::MyCamera()
{
	rotation = glm::quat(vector3(0, 0, 0));
	position = vector3(0, 0, 0);

	up = rotation * vector3(0, 1, 0);
	center = position + rotation * vector3(0, 0, -1);

}


MyCamera::~MyCamera()
{
}

matrix4 MyCamera::GetView()
{
	center = position + rotation * vector3(0, 0, -1);

	std::cout << "X: " << rotation.x << " Y: " << rotation.y << " Z: " << rotation.z << std::endl;

	return glm::lookAt(position, center, up);
}

matrix4 MyCamera::GetProjection(bool bOrthographic)
{
	if (bOrthographic) {
		return glm::ortho(position.x * (1080.0f / 768.0f), 1.5f * (1080.0f / 768.0f), -1.5f, 1.5f, 0.01f, 1000.0f);
	}
	else return glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
}

void MyCamera::SetPosition(vector3 v3Position)
{
	position = v3Position;
}

void MyCamera::SetTarget(vector3 v3Target)
{
	center = v3Target;

	vector3 dirVector = center - position;
	float magnitude = glm::length(dirVector);

	std::cout << "Mag: " << magnitude << std::endl;

	dirVector.x /= magnitude;
	dirVector.y /= magnitude;
	dirVector.z /= magnitude;

	//float convert = 180 / PI;

	vector3 newRot(sin(dirVector.x), sin(dirVector.y), cos(dirVector.z));

	std::cout << "X: " << newRot.x << " Y: " << newRot.y << " Z: " << newRot.z << std::endl;

	rotation = glm::quat(newRot);

	UpdateDirectionVectors();
}

void MyCamera::IncTarget(vector3 v3Target)
{
	vector3 dirVector = v3Target - position;
	float magnitude = glm::length(dirVector);

	std::cout << "Mag: " << magnitude << std::endl;

	dirVector.x /= magnitude;
	dirVector.y /= magnitude;
	dirVector.z /= magnitude;

	//float convert = 180 / PI;

	vector3 newRot(sin(dirVector.x), sin(dirVector.y), cos(dirVector.z));

	std::cout << "X: " << newRot.x << " Y: " << newRot.y << " Z: " << newRot.z << std::endl;

	rotation = glm::quat(newRot);

	UpdateDirectionVectors();
}

void MyCamera::SetUp(vector3 v3Up)
{
	up = v3Up;
}

void MyCamera::MoveForward(float fIncrement)
{
	position += (forward * -fIncrement);
}

void MyCamera::MoveSideways(float fIncrement)
{
	position += (right * fIncrement);
}

void MyCamera::MoveVertical(float fIncrement)
{
	position += (up * fIncrement);
}

void MyCamera::ChangePitch(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(1, 0, 0));

	UpdateDirectionVectors();

	//std::cout << "X: " << rotation.x << " Y: " << rotation.y << " Z: " << rotation.z << std::endl;
}

void MyCamera::ChangeRoll(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 0, 1));

	UpdateDirectionVectors();

	//std::cout << "X: " << rotation.x << " Y: " << rotation.y << " Z: " << rotation.z << std::endl;
}

void MyCamera::ChangeYaw(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 1, 0));

	UpdateDirectionVectors();

	//std::cout << "X: " << rotation.x << " Y: " << rotation.y << " Z: " << rotation.z << std::endl;
}

void MyCamera::UpdateDirectionVectors()
{
	up = rotation * vector3(0, 1, 0);
	right = rotation * vector3(1, 0, 0);
	forward = rotation * vector3(0, 0, 1);
}
