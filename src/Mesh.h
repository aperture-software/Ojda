/*
 * Ojda: OpenGL Viewer
 *
 * Copyright © 2019 Aperture Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *     Copyright (c) 2016 Robert Smith
 *
 *     Permission is hereby granted, free of charge, to any person
 *     obtaining a copy of this software and associated documentation
 *     files (the "Software"), to deal in the Software without
 *     restriction, including without limitation the rights to use,
 *     copy, modify, merge, publish, distribute, sublicense, and/or
 *     sell copies of the Software, and to permit persons to whom the
 *     Software is furnished to do so, subject to the following
 *     conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *     HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *     WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *     OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <vector>
#include <Eigen/Eigen>

using namespace Eigen;
using namespace std;

struct Vertex
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Vector3f Position;
    Vector3f Normal;
    Vector2f TextureCoordinate;
};

struct BoundingBox {
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Vector3f min;
    Vector3f max;
};

struct Material
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Material()
    {
        Ns = 0.0f;
        Ni = 0.0f;
        d = 0.0f;
        illum = 0;
    }

    string name;        // Material Name
    Vector3f Ka;        // Ambient Color
    Vector3f Kd;        // Diffuse Color
    Vector3f Ks;        // Specular Color
    float Ns;           // Specular Exponent
    float Ni;           // Optical Density
    float d;            // Dissolve
    int illum;          // Illumination
    string map_Ka;      // Ambient Texture Map
    string map_Kd;      // Diffuse Texture Map
    string map_Ks;      // Specular Texture Map
    string map_Ns;      // Specular Hightlight Map
    string map_d;       // Alpha Texture Map
    string map_bump;    // Bump Map
};

struct Mesh
{
    Mesh() { }
    Mesh(vector<Vertex>& _Vertices, vector<size_t>& _Indices)
    {
        Vertices = _Vertices;
        Indices = _Indices;
    }
    string MeshName;                // Mesh Name
    vector<Vertex> Vertices;        // Vertex List
    vector<size_t> Indices;         // Index List
    Material MeshMaterial;          // Material
};
