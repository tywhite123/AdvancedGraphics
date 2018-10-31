#include "CubeRobot.h"

Mesh* CubeRobot::cube = NULL;

CubeRobot::CubeRobot()
{

	s = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");

	if (!s->LinkProgram()) {
		return;
	}


	SceneNode* body = new SceneNode(cube, s, Vector4(1, 0, 0, 1)); //RED
	body->SetScale(Vector3(10, 15, 5));
	body->SetModelMatrix(Matrix4::Translation(Vector3(0, 35, 0)));
	body->SetBoundingRadius(15.0f);
	AddChild(body);

	head = new SceneNode(cube, s, Vector4(0, 1, 0, 1));
	head->SetScale(Vector3(5, 5, 5));
	head->SetModelMatrix(Matrix4::Translation(Vector3(0,30,0)));
	head->SetBoundingRadius(5.0f);
	body->AddChild(head);

	leftArm = new SceneNode(cube, s, Vector4(0, 0, 1, 1));
	leftArm->SetScale(Vector3(3, -18, 3));
	leftArm->SetModelMatrix(Matrix4::Translation(Vector3(-12, 30, -1)));
	leftArm->SetBoundingRadius(18.0f);
	body->AddChild(leftArm);

	rightArm = new SceneNode(cube, s, Vector4(0, 0, 1, 1));
	rightArm->SetScale(Vector3(3, -18, 3));
	rightArm->SetModelMatrix(Matrix4::Translation(Vector3(12, 30, -1)));
	rightArm->SetBoundingRadius(18.0f);
	body->AddChild(rightArm);

	SceneNode* leftLeg = new SceneNode(cube, s, Vector4(0, 0, 1, 1));
	leftLeg->SetScale(Vector3(3, -17.5f, 3));
	leftLeg->SetModelMatrix(Matrix4::Translation(Vector3(-8, 0, 0)));
	leftLeg->SetBoundingRadius(18.0f);
	body->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(cube, s, Vector4(0, 0, 1, 1));
	rightLeg->SetScale(Vector3(3, -17.5f, 3));
	rightLeg->SetModelMatrix(Matrix4::Translation(Vector3(8, 0, 0)));
	rightLeg->SetBoundingRadius(18.0f);
	body->AddChild(rightLeg);

	/*leftFoot = new SceneNode(cube, s, Vector4(0, 1, 1, 1));
	leftFoot->SetScale(Vector3(3, -8.5f, 3));
	leftFoot->SetModelMatrix(Matrix4::Translation(Vector3(0, -16.5, 0)));
	leftFoot->SetBoundingRadius(8.0f);
	leftLeg->AddChild(leftFoot);

	rightFoot = new SceneNode(cube, s, Vector4(0, 1, 1, 1));
	rightFoot->SetScale(Vector3(3, -8.5f, 3));
	rightFoot->SetModelMatrix(Matrix4::Translation(Vector3(0, -16.5, 0)));
	rightFoot->SetBoundingRadius(8.0f);
	rightLeg->AddChild(rightFoot);*/
}


CubeRobot::~CubeRobot()
{
	delete s;
}

void CubeRobot::Update(float msec)
{
	modelMatrix = modelMatrix * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));

	head->SetModelMatrix(head->GetModelMatrix() * Matrix4::Rotation(-msec / 10.0f, Vector3(0, 1, 0)));

	leftArm->SetModelMatrix(leftArm->GetModelMatrix() * Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));

	rightArm->SetModelMatrix(rightArm->GetModelMatrix() * Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));
	

	/*leftFoot->SetModelMatrix(leftFoot->GetModelMatrix() * Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));

	rightFoot->SetModelMatrix(rightFoot->GetModelMatrix() * Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));*/

	SceneNode::Update(msec);

}
