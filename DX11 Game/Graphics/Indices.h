#pragma once
#ifndef INDICES_H
#define INDICES_H

/// <summary>
/// Contains a list of index arrays for some primitive objects.
/// Arrays are contained within the 'Idx' namespace.
/// </summary>

typedef unsigned short WORD;

namespace Idx
{
    static WORD indicesCube[] =
    {
        23, 22, 20,
        22, 21, 20,
        19, 18, 16,
        18, 17, 16,
        15, 14, 12,
        14, 13, 12,
        11, 10, 8,
        10, 9, 8,
        7, 6, 4,
        6, 5, 4,
        3, 2, 0,
        2, 1, 0
    };

    static WORD indicesCube_Tex[] =
    {
        0, 1, 2,
        2, 1, 3,
        4, 0, 6,
        6, 0, 2,
        7, 5, 6,
        6, 5, 4,
        3, 1, 7,
        7, 1, 5,
        4, 5, 0,
        0, 5, 1,
        3, 7, 2,
        2, 7, 6
    };

    static WORD indicesQuad[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    static WORD indicesFullscreen[] =
    {
        0, 1, 2,
        1, 3, 2
    };
}

#endif