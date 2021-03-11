#pragma once
#ifndef RENDERABLEGAMEOBJECT_H
#define RENDERABLEGAMEOBJECT_H

class Camera;
class PhysicsModel;
#include "GameObject3D.h"

/// <summary>
/// Allows for a model to be bound to a 3D game object.
/// Inherits from GameObject3D to allow for modification of position/rotation/scale data.
/// </summary>
class RenderableGameObject : public GameObject3D
{
public:
	bool Initialize(
		const std::string& filePath,
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader );
	void Draw();
	std::shared_ptr<PhysicsModel> GetPhysicsModel() const noexcept { return physicsModel; }
protected:
	void UpdateMatrix() override;

	Model model;
	XMMATRIX worldMatrix = XMMatrixIdentity();
	std::shared_ptr<PhysicsModel> physicsModel;
};

#endif