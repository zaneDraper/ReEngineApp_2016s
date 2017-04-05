#include "CameraCustom.h"



CameraCustom::CameraCustom()
{
	rotation = glm::quat(vector3(0, 0, 0));
	position = vector3(0, 0, 0);

	up = rotation * vector3(0, 1, 0);
	center = position + rotation * vector3(0, 0, -1);
}


CameraCustom::~CameraCustom()
{
}

matrix4 CameraCustom::GetView()
{
	//vector3 center = position + rotation * vector3(0, 0, -1);

	return glm::lookAt(position, center, up);
}

matrix4 CameraCustom::GetProjection(bool bOrthographic)
{
	if (bOrthographic) {
		return glm::ortho(position.x * (1080.0f / 768.0f), 1.5f * (1080.0f / 768.0f), -1.5f, 1.5f, 0.01f, 1000.0f);
	}
	else return glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
}

void CameraCustom::SetPosition(vector3 v3Position)
{
	position = v3Position;
}

void CameraCustom::SetTarget(vector3 v3Target)
{
	center = v3Target;

	rotation = glm::quat(glm::normalize(center - position));

	UpdateDirectionVectors();
}

void CameraCustom::SetUp(vector3 v3Up)
{
	up = v3Up;
}

void CameraCustom::MoveForward(float fIncrement)
{

	position += (forward * -fIncrement);
}

void CameraCustom::MoveSideways(float fIncrement)
{

	position += (right * fIncrement);
}

void CameraCustom::MoveVertical(float fIncrement)
{

	position += (up * fIncrement);
}

void CameraCustom::ChangePitch(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(1, 0, 0));

	UpdateDirectionVectors();
}

void CameraCustom::ChangeRoll(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 0, 1));

	UpdateDirectionVectors();
}

void CameraCustom::ChangeYaw(float fIncrement)
{
	rotation = glm::rotate(rotation, fIncrement, vector3(0, 1, 0));

	UpdateDirectionVectors();
}

void CameraCustom::UpdateDirectionVectors()
{
	up = rotation * vector3(0, 1, 0);
	right = rotation * vector3(1, 0, 0);
	forward = rotation * vector3(0, 0, 1);

	//center = position + rotation * forward;
}