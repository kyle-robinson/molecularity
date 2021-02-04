#pragma once
#ifndef VERTICES_H
#define VERTICES_H

#include "VertexType.h"

Vertex_Pos_Col verticesCube[] =
{
    { { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    { {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    { { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
    { {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    { { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
    { {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
    { {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }
};

Vertex_Pos_Tex verticesQuad_Tex[] =
{
    { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } },
    { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } },
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } }
};

Vertex_Pos_Col verticesQuad_Col[] =
{
    { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
    { { -0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
    { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }
};

#endif