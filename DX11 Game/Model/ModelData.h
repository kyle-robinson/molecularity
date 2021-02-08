#pragma once
#ifndef MODELDATA_H
#define MODELDATA_H

#include <fstream>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "RenderableGameObject.h"
using json = nlohmann::json;

struct Drawable
{
    std::string modelName;
    std::string fileName;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;
};
std::vector<Drawable> drawables;

class ModelData
{
public:
    static bool LoadModelData( const std::string& filePath )
    {
        if ( !std::filesystem::exists( filePath ) )
            return false;

        json jFile;
        std::ifstream fileOpen( filePath.c_str() );
        fileOpen >> jFile;
        std::string version = jFile["version"].get<std::string>();

        json objects = jFile["GameObjects"];
        int size = objects.size();

        for ( unsigned int i = 0; i < size; i++ )
        {
            Drawable drawable;
            json objectDesc = objects[i];
            drawable.modelName = objectDesc["Name"];
            drawable.fileName = objectDesc["File"];
            drawable.position = { objectDesc["PosX"], objectDesc["PosY"], objectDesc["PosZ"] };
            drawable.rotation = { objectDesc["RotX"], objectDesc["RotY"], objectDesc["RotZ"] };
            drawable.scale = { objectDesc["ScaleX"], objectDesc["ScaleY"], objectDesc["ScaleZ"] };
            drawables.push_back( drawable );
        }
        return true;
    }
    static bool InitializeModelData( ID3D11DeviceContext* context, ID3D11Device* device,
        ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, std::vector<RenderableGameObject>& renderables )
    {
        for ( unsigned int i = 0; i < drawables.size(); i++ )
        {
            RenderableGameObject model;
            model.SetInitialScale( drawables[i].scale.x, drawables[i].scale.y, drawables[i].scale.z );
            if ( !model.Initialize( "Resources\\Models\\" + drawables[i].fileName, device, context, cb_vs_matrix ) )
                return false;
            model.SetInitialPosition( drawables[i].position );
            model.SetInitialRotation( drawables[i].rotation );
            model.SetModelName( drawables[i].modelName );
            renderables.push_back( model );
        }
        return true;
    }
};

#endif