#include "stdafx.h"
#include "ObjectManager.h"
#include <dxtk/WICTextureLoader.h>
#include <dxtk/DDSTextureLoader.h>
ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

bool ObjectManager::LoadScene(ID3D11Device* device, ID3D11DeviceContext* context,ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader)
{
	std::vector<RenderableGameObject*> SceneObjectz;

	Cube* cube;
	Quad* quad;
	RenderableGameObject* hubRoom = new RenderableGameObject();




	//hubroom
	if (!hubRoom->Initialize("Resources\\Models\\Hub\\scene.gltf", device, context, cb_vs_vertexshader)) return false;
	hubRoom->SetInitialScale(4.0f, 4.0f, 4.0f);


	//this is just for testing. Put breakpoints around these. Objectz does nothing.
	SceneObjectz.push_back(hubRoom);
	SceneObjects.push_back(hubRoom);

	//AddObject(object);

	// primitives
	cube = new Cube();
	if (!cube->Initialize(context, device)) return false;
	cube->SetInitialPosition(0.0f, 10.0f, -6.0f);

	//AddObject(cube);
	//Sets textures for the cube. Currently each cube will hold its own textures. Possibly in the future make the object manager hold all textures instead of each cube reads from that
	cube->AddNewTexture(BoxType::Default, L"Resources\\Textures\\crates\\basic_crate.png",device);
	cube->AddNewTexture(BoxType::Bounce, L"Resources\\Textures\\crates\\bounce_crate.png", device);
	cube->AddNewTexture(BoxType::Arrow, L"Resources\\Textures\\crates\\arrow_crate.png", device);
	cube->AddNewTexture(BoxType::TNT, L"Resources\\Textures\\crates\\tnt_crate.png", device);

	quad = new Quad();
	if (!quad->Initialize(context, device)) return false;
	quad->SetInitialPosition(0.0f, 5.0f, 5.0f);
	quad->SetInitialRotation(quad->GetRotationFloat3().x + XM_PI, quad->GetRotationFloat3().y + XM_PI, quad->GetRotationFloat3().z);

	//AddObject(quad);

}

void ObjectManager::Draw(ConstantBuffer<CB_VS_matrix>& cb_vs_matrix)
{
	for (auto cube : Cubes)
	{
		cube->Draw(cb_vs_matrix);
	}

	for (auto object : SceneObjects)
	{
		object->Draw();
	}

}

void ObjectManager::Update(float dt)
{
	for (auto cube : Cubes)
	{
		// update cube scale multiplier
		if (cube->GetEditableProperties()->GetToolType() == ToolType::Resize)
			cube->SetScale(static_cast<float>(cube->GetEditableProperties()->GetSizeMultiplier()));
		cube->UpdatePhysics(dt);
	}
}


void ObjectManager::EmptyScene()
{
//	SceneObjects.clear();
	Cubes.clear();
}

void ObjectManager::AddObject(RenderableGameObject* object)
{

	switch (object->GetType())
	{
	case objectType::EditableCube:

		Cubes.push_back(dynamic_cast<Cube*>(object));
		break;

	case objectType::Default:
		SceneObjects.push_back(object);
		break;

	case objectType::Quad:
		SceneObjects.push_back(object);
		break;
	}
}