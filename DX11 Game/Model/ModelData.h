#pragma once
#ifndef MODELDATA_H
#define MODELDATA_H

// "Rickenbacker 4001" (https://skfb.ly/HYEN) by Yogensia is licensed under CC Attribution-NonCommercial-ShareAlike (http://creativecommons.org/licenses/by-nc-sa/4.0/).
// "Headphones" (https://skfb.ly/6ANxo) by Ren is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
// "Zelda - Breath Of The Wild" (https://skfb.ly/6QWNH) by theStoff is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

#include<Utility/JSON_Helper.h>
#include <fstream>
#include <filesystem>

#include "RenderableGameObject.h"



struct Drawable
{
    std::string modelName;
    std::string fileName;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;
};
std::vector<Drawable> drawables;
std::vector<JSON_LOADER::ModdleData> drawablesA;
class ModelData
{
public:
    //load model data
    static bool LoadModelData( const std::string& filePath )
    {
        //update to other json loading methord
        drawablesA= JSON_LOADER::LoadGameObjects(filePath);

        //put into old struct
        //for (unsigned int i = 0; i < drawablesA.size(); i++)
        //{
        //    Drawable drawable;
        //    drawable.fileName= drawablesA[i].FileName;
        //    drawable.modelName= drawablesA[i].ObjectName;
        //    drawable.position= drawablesA[i].Position;
        //    drawable.rotation = drawablesA[i].Rotation;
        //    drawable.scale = drawablesA[i].Scale;
        //    drawables.push_back(std::move(drawable));
        //}
      
        return true;
    }
    //set up game object transfromation data
    static bool InitializeModelData( ID3D11DeviceContext* context, ID3D11Device* device,
        ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, std::map<std::string, RenderableGameObject>& renderables )
    {
        for ( unsigned int i = 0; i < drawables.size(); i++ )
        {
            RenderableGameObject model;
            model.SetInitialScale(drawablesA[i].Scale.x, drawablesA[i].Scale.y, drawablesA[i].Scale.z);
            if (!model.Initialize("Resources\\Models\\" + drawablesA[i].FileName, device, context, cb_vs_matrix))
                return false;
            model.SetInitialPosition(drawablesA[i].Position);
            model.SetInitialRotation(drawablesA[i].Rotation);
            renderables.emplace(std::move(drawablesA[i].ObjectName), std::move(model));

            //useing old struct
          /*  model.SetInitialScale( drawables[i].scale.x, drawables[i].scale.y, drawables[i].scale.z );
            if ( !model.Initialize( "Resources\\Models\\" + drawables[i].fileName, device, context, cb_vs_matrix ) )
                return false;
            model.SetInitialPosition( drawables[i].position );
            model.SetInitialRotation( drawables[i].rotation );
            renderables.emplace( std::move( drawables[i].modelName ), std::move( model ) );*/
        }
        return true;
    }
};

#endif