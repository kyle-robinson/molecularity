#pragma once
#ifndef MODELDATA_H
#define MODELDATA_H

#include <map>
#include <fstream>
#include <filesystem>
#include "JSON_Helper.h"
#include "RenderableGameObject.h"
static std::vector<JSON::ModelData> drawables;

/// <summary>
/// Used JSON file parsing to load and store model information.
/// Main use is to quickly setup models with predetermined positions and scales from a JSON file.
/// </summary>
class ModelData
{
public:
    static bool LoadModelData( const std::string& filePath )
    {
        // Update to other json loading method
        drawables = JSON::LoadGameObjects( filePath );
        return true;
    }
    // Set up game object transformation data
    static bool InitializeModelData( ID3D11DeviceContext* context, ID3D11Device* device,
        ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, std::unordered_map<std::string, RenderableGameObject>& renderables )
    {
        for ( uint32_t i = 0; i < drawables.size(); i++ )
        {
            RenderableGameObject model;
            model.SetInitialScale( drawables[i].Scale.x, drawables[i].Scale.y, drawables[i].Scale.z );
            if (!model.Initialize( "Resources\\Models\\" + drawables[i].FileName, device, context, cb_vs_matrix ) )
                return false;
            model.SetInitialPosition( drawables[i].Position );
            model.SetInitialRotation( drawables[i].Rotation );
            renderables.emplace( std::move( drawables[i].ObjectName ), std::move( model ) );
        }
        return true;
    }
};

#endif