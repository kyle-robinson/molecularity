#pragma once
#ifndef RENDERABLEGAMEOBJECT_H
#define RENDERABLEGAMEOBJECT_H

enum class objectType {
	EditableCube,
	Default,
	Quad,
	Player,

};

class Camera;
#include "GameObject3D.h"

/// <summary>
/// Allows for a model to be bound to a 3D game object.
/// Inherits from GameObject3D to allow for modification of position/rotation/scale data.
/// </summary>
class RenderableGameObject : public GameObject3D
{
public:
	RenderableGameObject();
	
	bool Initialize(
		const std::string& filePath,
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader );
	void Draw();
	objectType GetType() { return type; }
protected:
	Model model;
	void UpdateMatrix() override;
	
	XMMATRIX worldMatrix = XMMatrixIdentity();
	objectType type = objectType::Default;
};

#endif