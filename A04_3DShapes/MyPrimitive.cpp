#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
//This will make the triang A->B->C
void MyPrimitive::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTop)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTop);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();


	vector3 point0(0, a_fHeight / 2, 0); //Head
	vector3 point1(0, -a_fHeight / 2, 0); //Base

	vector<vector3> points;
	float curRadian = 0;

	for (int i = 0; i < a_nSubdivisions; i++) {
		points.push_back(vector3( a_fRadius * cos(curRadian), -a_fHeight / 2, a_fRadius * sin(curRadian)));
		curRadian += ((2 * PI) / a_nSubdivisions);
	}

	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		AddQuad(point1, points[i], points[i + 1], point0);
	}
	AddQuad(point1, points[a_nSubdivisions-1], points[0], point0);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	vector3 point0(0, a_fHeight / 2, 0); //Head
	vector3 point1(0, -a_fHeight / 2, 0); //Base

	vector<vector3> points;
	float curRadian = 0;

	for (int i = 0; i < a_nSubdivisions; i++) {
		points.push_back(vector3(a_fRadius * cos(curRadian), a_fHeight / 2, a_fRadius * sin(curRadian)));
		points.push_back(vector3(a_fRadius * cos(curRadian), -a_fHeight / 2, a_fRadius * sin(curRadian)));
		curRadian += ((2 * PI) / a_nSubdivisions);
	}

	//quads
	for (int i = 0; i < (a_nSubdivisions*2) - 2; i+=2) {
		AddQuad(points[i], points[i+2], points[i + 1], points[i+3]);
	}
	AddQuad(points[(2*a_nSubdivisions) - 2], points[0], points[(2 * a_nSubdivisions) - 1], points[1]);

	//tris
	for (int i = 0; i < (2 * a_nSubdivisions) - 2; i += 2) {
		AddTri(point0, points[i+2], points[i]);
	}
	AddTri(point0, points[0], points[(2 * a_nSubdivisions) - 2]);

	for (int i = 1; i < (2 * a_nSubdivisions) - 2; i += 2) {
		AddTri(point1, points[i], points[i + 2]);
	}
	AddTri(point1, points[(2 * a_nSubdivisions) - 1], points[1]);


	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	vector<vector3> points1;
	vector<vector3> points2;
	float curRadian = 0;

	for (int i = 0; i < a_nSubdivisions; i++) {
		points1.push_back(vector3(a_fOuterRadius * cos(curRadian), a_fHeight / 2, a_fOuterRadius * sin(curRadian)));
		points1.push_back(vector3(a_fInnerRadius * cos(curRadian), a_fHeight / 2, a_fInnerRadius * sin(curRadian)));
		curRadian += ((2 * PI) / a_nSubdivisions);
	}

	for (int i = 0; i < a_nSubdivisions; i++) {
		points2.push_back(vector3(a_fOuterRadius * cos(curRadian), -a_fHeight / 2, a_fOuterRadius * sin(curRadian)));
		points2.push_back(vector3(a_fInnerRadius * cos(curRadian), -a_fHeight / 2, a_fInnerRadius * sin(curRadian)));
		curRadian += ((2 * PI) / a_nSubdivisions);
	}

	//top bottom
	for (int i = 0; i < (2 * a_nSubdivisions) -2; i+=2) {
		AddQuad(points1[i], points1[i + 1], points1[i + 2], points1[i + 3]);
		AddQuad(points2[i], points2[i + 2], points2[i + 1], points2[i + 3]);
	}
	AddQuad(points1[(2 * a_nSubdivisions) - 2], points1[(2 * a_nSubdivisions) - 1], points1[0], points1[1]);
	AddQuad(points2[(2 * a_nSubdivisions) - 2], points2[0], points2[(2 * a_nSubdivisions) - 1], points2[1]);

	//inside outside
	for (int i = 0; i < (2 * a_nSubdivisions) - 2; i += 2) {
		AddQuad(points1[i], points1[i + 2], points2[i], points2[i + 2]);
		AddQuad(points1[i + 1], points2[i + 1], points1[i + 3], points2[i + 3]);
	}
	AddQuad(points1[(2 * a_nSubdivisions) - 2], points1[0], points2[(2 * a_nSubdivisions) - 2], points2[0]);
	AddQuad(points1[(2 * a_nSubdivisions) - 1], points2[(2 * a_nSubdivisions) - 1], points1[1], points2[1]);


	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	a_nSubdivisions *= 2;

	vector<vector3> points;

	vector3 point0(0, a_fRadius, 0); //Head
	vector3 point1(0, -a_fRadius, 0); //Base
	
	for (int i = 1; i < a_nSubdivisions; i++) {
		float angle1 = (PI/2) - i * (PI / a_nSubdivisions);
		float radius = a_fRadius * cos(angle1);
		float height = a_fRadius * sin(angle1);

		for (int j = 0; j < a_nSubdivisions; j++) {
			float angle2 = j * ((2*PI) / a_nSubdivisions);
			points.push_back(vector3(radius * cos(angle2), height, radius * sin(angle2)));
		}
	}

	//tris
	for (int i = 0; i < a_nSubdivisions - 1; i++) {
		AddTri(point0, points[i + 1], points[i]);
	}
	AddTri(point0, points[0], points[a_nSubdivisions - 1]);
	for (int i = points.size() - a_nSubdivisions; i < points.size() - 1; i++) {
		AddTri(point1, points[i], points[i + 1]);
	}
	AddTri(point1, points[points.size() - 1], points[points.size() - a_nSubdivisions]);
	
	//quads
	for (int i = 0; i < points.size() - a_nSubdivisions; i+=a_nSubdivisions) {
		for (int j = 0; j < a_nSubdivisions - 1; j++) {
			AddQuad(points[i + j], points[i + j + 1], points[i + j + a_nSubdivisions], points[i + j + a_nSubdivisions + 1]);
		}
	}
	for (int i = 0; i < points.size() - a_nSubdivisions - 1; i += a_nSubdivisions) {
		AddQuad(points[i], points[i + a_nSubdivisions], points[i + a_nSubdivisions - 1], points[i + (2*a_nSubdivisions) - 1]);
	}
	

	//Your code ends here
	CompileObject(a_v3Color);
}