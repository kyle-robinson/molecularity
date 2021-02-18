#pragma once
#ifndef MODELDATA_H
#define MODELDATA_H

/// <summary>
/// Used JSON file parsing to load and store model information.
/// Main use is to quickly setup models with predetermined positions and scales from a JSON file.
/// </summary>

// "Rickenbacker 4001" (https://skfb.ly/HYEN) by Yogensia is licensed under CC Attribution-NonCommercial-ShareAlike (http://creativecommons.org/licenses/by-nc-sa/4.0/).
// "Headphones" (https://skfb.ly/6ANxo) by Ren is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
// "Zelda - Breath Of The Wild" (https://skfb.ly/6QWNH) by theStoff is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

#include <fstream>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "RenderableGameObject.h"
using json = nlohmann::json;

struct Drawable
{
    std::string modelName = "";
    std::string fileName = "";
    XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 scale = { 0.0f, 0.0f, 0.0f };
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
        size_t size = objects.size();

        for ( size_t i = 0; i < size; i++ )
        {
            Drawable drawable;
            json objectDesc = objects[i];
            drawable.modelName = objectDesc["Name"];
            drawable.fileName = objectDesc["File"];
            drawable.position = { objectDesc["PosX"], objectDesc["PosY"], objectDesc["PosZ"] };
            drawable.rotation = { objectDesc["RotX"], objectDesc["RotY"], objectDesc["RotZ"] };
            drawable.scale = { objectDesc["ScaleX"], objectDesc["ScaleY"], objectDesc["ScaleZ"] };
            drawables.push_back( std::move( drawable ) );
        }
        return true;
    }
    static bool InitializeModelData( ID3D11DeviceContext* context, ID3D11Device* device,
        ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, std::map<std::string, RenderableGameObject>& renderables )
    {
        for ( unsigned int i = 0; i < drawables.size(); i++ )
        {
            RenderableGameObject model;
            model.SetInitialScale( drawables[i].scale.x, drawables[i].scale.y, drawables[i].scale.z );
            if ( !model.Initialize( "Resources\\Models\\" + drawables[i].fileName, device, context, cb_vs_matrix ) )
                return false;
            model.SetInitialPosition( drawables[i].position );
            model.SetInitialRotation( drawables[i].rotation );
            renderables.emplace( std::move( drawables[i].modelName ), std::move( model ) );
        }
        return true;
    }
};

#endif