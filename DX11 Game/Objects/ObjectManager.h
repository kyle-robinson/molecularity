#pragma once
#include "RenderableGameObject.h"
#include "Cube.h"
#include "Quad.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void Draw(ConstantBuffer<CB_VS_matrix>& cb_vs_matrix);
	void Update(float dt);

	std::vector<RenderableGameObject*> GetSceneObjects() { return SceneObjects; }
	std::vector<Cube*> GetCubes() { return Cubes; }

	bool LoadScene(ID3D11Device* device, ID3D11DeviceContext* context,
		ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader);

private:
	void AddObject(RenderableGameObject* object);
	void EmptyScene(); //cleans out all vectors


private:
	std::vector<RenderableGameObject*> SceneObjects; //contains all the sceneobjects available to draw
	std::vector<Cube*> Cubes; //Contains all the editable cube objects
};

