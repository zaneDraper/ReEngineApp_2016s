#pragma once

#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>

using namespace ReEng; //Using ReEng namespace to use all the classes in the dll

class MyCamera
{
	glm::quat rotation;
	vector3 position;
	vector3 forward;
	vector3 right;
	vector3 up;;
	vector3 center;

public:
	MyCamera();
	~MyCamera();

	matrix4 GetView();

	matrix4 GetProjection(bool bOrthographic);

	void SetPosition(vector3 v3Position);

	void SetTarget(vector3 v3Target);

	void IncTarget(vector3 v3Target);

	void SetUp(vector3 v3Up);

	void MoveForward(float fIncrement);

	void MoveSideways(float fIncrement);

	void MoveVertical(float fIncrement);

	void ChangePitch(float fIncrement);

	void ChangeRoll(float fIncrement);

	void ChangeYaw(float fIncrement);

	void UpdateDirectionVectors();
};

