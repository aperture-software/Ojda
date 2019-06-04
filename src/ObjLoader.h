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

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Mesh.h"

//#define OBJL_CONSOLE_OUTPUT

namespace ObjLoader
{
    namespace math
    {
        // Angle between 2 Vector3 Objects
        float AngleBetweenV3(const Vector3f a, const Vector3f b)
        {
            float angle = a.dot(b);
            angle /= a.norm() * b.norm();
            return acosf(angle);
        }

        // Projection Calculation of a onto b
        Vector3f ProjV3(const Vector3f a, const Vector3f b)
        {
            Vector3f bn = b / b.norm();
            return bn * a.dot(bn);
        }
    }

    namespace algorithm
    {
        // A test to see if P1 is on the same side as P2 of a line segment ab
        bool SameSide(Vector3f p1, Vector3f p2, Vector3f a, Vector3f b)
        {
            Vector3f cp1 = (b - a).cross(p1 - a);
            Vector3f cp2 = (b - a).cross(p2 - a);
            return (cp1.dot(cp2) >= 0);
        }

        // Generate a cross product normal for a triangle
        Vector3f GenTriNormal(Vector3f t1, Vector3f t2, Vector3f t3)
        {
            Vector3f u = t2 - t1;
            Vector3f v = t3 - t1;
            return u.cross(v);
        }

        // Check to see if a Vector3 Point is within a 3 Vector3 Triangle
        bool inTriangle(Vector3f point, Vector3f tri1, Vector3f tri2, Vector3f tri3)
        {
            // Test to see if it is within an infinite prism that the triangle outlines.
            bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
                && SameSide(point, tri3, tri1, tri2);

            // If it isn't it will never be on the triangle
            if (!within_tri_prisim)
                return false;

            // Calulate Triangle's Normal
            Vector3f n = GenTriNormal(tri1, tri2, tri3);

            // Project the point onto this normal
            Vector3f proj = math::ProjV3(point, n);

            // If the distance from the triangle to the point is 0
            // it lies on the triangle
            return (proj.norm() == 0);
        }

        // Split a String into a string array at a given token
        inline void split(const string &in, vector<string> &out, string token)
        {
            out.clear();

            string temp;

            for (size_t i = 0; i < in.size(); i++) {
                string test = in.substr(i, token.size());

                if (test == token) {
                    if (!temp.empty()) {
                        out.push_back(temp);
                        temp.clear();
                        i += token.size() - 1;
                    } else {
                        out.push_back("");
                    }
                } else if (i + token.size() >= in.size()) {
                    temp += in.substr(i, token.size());
                    out.push_back(temp);
                    break;
                } else {
                    temp += in[i];
                }
            }
        }

        // Get tail of string after first token and possibly following spaces
        inline string tail(const string &in)
        {
            size_t token_start = in.find_first_not_of(" \t");
            size_t space_start = in.find_first_of(" \t", token_start);
            size_t tail_start = in.find_first_not_of(" \t", space_start);
            size_t tail_end = in.find_last_not_of(" \t");
            if (tail_start != string::npos && tail_end != string::npos) {
                return in.substr(tail_start, tail_end - tail_start + 1);
            } else if (tail_start != string::npos) {
                return in.substr(tail_start);
            }
            return "";
        }

        // Get first token of string
        inline string firstToken(const string &in)
        {
            if (!in.empty()) {
                size_t token_start = in.find_first_not_of(" \t");
                size_t token_end = in.find_first_of(" \t", token_start);
                if (token_start != string::npos && token_end != string::npos) {
                    return in.substr(token_start, token_end - token_start);
                } else if (token_start != string::npos) {
                    return in.substr(token_start);
                }
            }
            return "";
        }

        // Get element at given index position
        template <class T>
        inline const T & getElement(const vector<T> &elements, string &index)
        {
            int idx = stoi(index);
            if (idx < 0)
                idx = int(elements.size()) + idx;
            else
                idx--;
            return elements[idx];
        }
    }

    class Loader
    {
    public:
        Loader() { }
        ~Loader()
        {
            LoadedMeshes.clear();
        }

        bool LoadFile(string Path)
        {
            if (Path.substr(Path.size() - 4, 4) != ".obj")
                return false;


            ifstream file(Path);

            if (!file.is_open())
                return false;

            LoadedMeshes.clear();
            LoadedVertices.clear();
            LoadedIndices.clear();

            vector<Vector3f> Positions;
            vector<Vector2f> TCoords;
            vector<Vector3f> Normals;

            vector<Vertex> Vertices;
            vector<size_t> Indices;

            vector<string> MeshMatNames;

            bool listening = false;
            string meshname;

            Mesh tempMesh;

#ifdef OBJL_CONSOLE_OUTPUT
            const unsigned int outputEveryNth = 1000;
            unsigned int outputIndicator = outputEveryNth;
#endif

            string curline;
            while (getline(file, curline)) {
                if ((curline.size() > 0) && (curline[curline.size() - 1] == '\r')) {
                    curline.resize(curline.size() - 1);
                }

#ifdef OBJL_CONSOLE_OUTPUT
                if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 1) {
                    if (!meshname.empty()) {
                        cout
                            << "\r- " << meshname
                            << "\t| vertices > " << Positions.size()
                            << "\t| texcoords > " << TCoords.size()
                            << "\t| normals > " << Normals.size()
                            << "\t| triangles > " << (Vertices.size() / 3)
                            << (!MeshMatNames.empty() ? "\t| material: " + MeshMatNames.back() : "");
                    }
                }
#endif

                // Generate a Mesh Object or Prepare for an object to be created
                if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g" || curline[0] == 'g') {
                    if (!listening) {
                        listening = true;

                        if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g") {
                            meshname = algorithm::tail(curline);
                        } else {
                            meshname = "unnamed";
                        }
                    } else {
                        // Generate the mesh to put into the array

                        if (!Indices.empty() && !Vertices.empty()) {
                            // Create Mesh
                            tempMesh = Mesh(Vertices, Indices);
                            tempMesh.MeshName = meshname;

                            // Insert Mesh
                            LoadedMeshes.push_back(tempMesh);

                            // Cleanup
                            Vertices.clear();
                            Indices.clear();
                            meshname.clear();

                            meshname = algorithm::tail(curline);
                        } else {
                            if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g") {
                                meshname = algorithm::tail(curline);
                            } else {
                                meshname = "unnamed";
                            }
                        }
                    }
#ifdef OBJL_CONSOLE_OUTPUT
                    cout << endl;
                    outputIndicator = 0;
#endif
                }
                // Generate a Vertex Position
                if (algorithm::firstToken(curline) == "v") {
                    vector<string> spos;
                    algorithm::split(algorithm::tail(curline), spos, " ");
                    Positions.push_back(Vector3f(stof(spos[0]), stof(spos[1]), stof(spos[2])));
                }
                // Generate a Vertex Texture Coordinate
                if (algorithm::firstToken(curline) == "vt") {
                    vector<string> stex;
                    algorithm::split(algorithm::tail(curline), stex, " ");
                    TCoords.push_back(Vector2f(stof(stex[0]), stof(stex[1])));
                }
                // Generate a Vertex Normal;
                if (algorithm::firstToken(curline) == "vn") {
                    vector<string> snor;
                    algorithm::split(algorithm::tail(curline), snor, " ");
                    Normals.push_back(Vector3f(stof(snor[0]), stof(snor[1]), stof(snor[2])));
                }
                // Generate a Face (vertices & indices)
                if (algorithm::firstToken(curline) == "f") {
                    // Generate the vertices
                    vector<Vertex> vVerts;
                    GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

                    // Add Vertices
                    for (size_t i = 0; i < vVerts.size(); i++) {
                        Vertices.push_back(vVerts[i]);
                        LoadedVertices.push_back(vVerts[i]);
                    }

                    vector<size_t> iIndices;
                    VertexTriangluation(iIndices, vVerts);

                    // Add Indices
                    for (size_t i = 0; i < iIndices.size(); i++) {
                        size_t indnum = ((Vertices.size()) - vVerts.size()) + iIndices[i];
                        Indices.push_back(indnum);

                        indnum = ((LoadedVertices.size()) - vVerts.size()) + iIndices[i];
                        LoadedIndices.push_back(indnum);
                    }
                }
                // Get Mesh Material Name
                if (algorithm::firstToken(curline) == "usemtl") {
                    MeshMatNames.push_back(algorithm::tail(curline));

                    // Create new Mesh, if Material changes within a group
                    if (!Indices.empty() && !Vertices.empty()) {
                        // Create Mesh
                        tempMesh = Mesh(Vertices, Indices);
                        tempMesh.MeshName = meshname;
                        int i = 2;
                        while (1) {
                            tempMesh.MeshName = meshname + "_" + to_string(i);

                            for (auto &m : LoadedMeshes)
                                if (m.MeshName == tempMesh.MeshName)
                                    continue;
                            break;
                        }

                        // Insert Mesh
                        LoadedMeshes.push_back(tempMesh);

                        // Cleanup
                        Vertices.clear();
                        Indices.clear();
                    }

#ifdef OBJL_CONSOLE_OUTPUT
                    outputIndicator = 0;
#endif
                }
                // Load Materials
                if (algorithm::firstToken(curline) == "mtllib") {
                    // Generate LoadedMaterial

                    // Generate a path to the material file
                    vector<string> temp;
                    algorithm::split(Path, temp, "/");

                    string pathtomat = "";

                    if (temp.size() > 1) {
                        for (size_t i = 0; i < temp.size() - 1; i++) {
                            pathtomat += temp[i] + "/";
                        }
                    }


                    pathtomat += algorithm::tail(curline);

#ifdef OBJL_CONSOLE_OUTPUT
                    cout << endl << "- find materials in: " << pathtomat << endl;
#endif

                    // Load Materials
                    LoadMaterials(pathtomat);
                }
            }

#ifdef OBJL_CONSOLE_OUTPUT
            cout << endl;
#endif

            // Deal with last mesh
            if (!Indices.empty() && !Vertices.empty()) {
                // Create Mesh
                tempMesh = Mesh(Vertices, Indices);
                tempMesh.MeshName = meshname;

                // Insert Mesh
                LoadedMeshes.push_back(tempMesh);
            }

            file.close();

            // Set Materials for each Mesh
            for (size_t i = 0; i < MeshMatNames.size(); i++) {
                string matname = MeshMatNames[i];

                // Find corresponding material name in loaded materials
                // when found copy material variables into mesh material
                for (size_t j = 0; j < LoadedMaterials.size(); j++) {
                    if (LoadedMaterials[j].name == matname) {
                        LoadedMeshes[i].MeshMaterial = LoadedMaterials[j];
                        break;
                    }
                }
            }

            return (!LoadedMeshes.empty() && !LoadedVertices.empty() && !LoadedIndices.empty());
        }

        vector<Mesh> LoadedMeshes;
        vector<Vertex> LoadedVertices;
        vector<size_t> LoadedIndices;
        vector<Material> LoadedMaterials;

    private:
        // Generate vertices from a list of positions,
        // tcoords, normals and a face line
        void GenVerticesFromRawOBJ(vector<Vertex>& oVerts,
            const vector<Vector3f>& iPositions,
            const vector<Vector2f>& iTCoords,
            const vector<Vector3f>& iNormals,
            string icurline)
        {
            vector<string> sface, svert;
            Vertex vVert;
            algorithm::split(algorithm::tail(icurline), sface, " ");

            bool noNormal = false;

            // For every given vertex do this
            for (size_t i = 0; i < sface.size(); i++) {
                // See What type the vertex is.
                int vtype = 0;

                algorithm::split(sface[i], svert, "/");

                // Check for just position - v1
                if (svert.size() == 1) {
                    // Only position
                    vtype = 1;
                }

                // Check for position & texture - v1/vt1
                if (svert.size() == 2) {
                    // Position & Texture
                    vtype = 2;
                }

                // Check for Position, Texture and Normal - v1/vt1/vn1
                // or if Position and Normal - v1//vn1
                if (svert.size() == 3) {
                    if (svert[1] != "") {
                        // Position, Texture, and Normal
                        vtype = 4;
                    } else {
                        // Position & Normal
                        vtype = 3;
                    }
                }

                // Calculate and store the vertex
                switch (vtype) {
                case 1: // P
                    vVert.Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.TextureCoordinate = Vector2f(0, 0);
                    noNormal = true;
                    oVerts.push_back(vVert);
                    break;
                case 2: // P/T
                    vVert.Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
                    noNormal = true;
                    oVerts.push_back(vVert);
                    break;
                case 3: // P//N
                    vVert.Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.TextureCoordinate = Vector2f(0, 0);
                    vVert.Normal = algorithm::getElement(iNormals, svert[2]);
                    oVerts.push_back(vVert);
                    break;
                case 4: // P/T/N
                    vVert.Position = algorithm::getElement(iPositions, svert[0]);
                    vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
                    vVert.Normal = algorithm::getElement(iNormals, svert[2]);
                    oVerts.push_back(vVert);
                    break;
                default:
                    break;
                }
            }

            // Take care of missing normals.
            // These may not be truly acurate but it is the best we
            // can get for a mesh that wasn't compiled with normals.
            if (noNormal) {
                Vector3f A = oVerts[0].Position - oVerts[1].Position;
                Vector3f B = oVerts[2].Position - oVerts[1].Position;

                Vector3f normal = A.cross(B);

                for (size_t i = 0; i < oVerts.size(); i++) {
                    oVerts[i].Normal = normal;
                }
            }
        }

        // Triangulate a list of vertices into a face by printing
        // indices corresponding with triangles within it.
        void VertexTriangluation(vector<size_t>& oIndices,
            const vector<Vertex>& iVerts)
        {
            // If there are 2 or less verts,
            // no triangle can be created,
            // so exit
            if (iVerts.size() < 3) {
                return;
            }
            // If it is a triangle no need to calculate it
            if (iVerts.size() == 3) {
                oIndices.push_back(0);
                oIndices.push_back(1);
                oIndices.push_back(2);
                return;
            }

            // Create a list of vertices
            vector<Vertex> tVerts = iVerts;

            while (true) {
                // For every vertex
                for (size_t i = 0; i < tVerts.size(); i++) {
                    // pPrev = the previous vertex in the list
                    Vertex pPrev;
                    if (i == 0) {
                        pPrev = tVerts[tVerts.size() - 1];
                    } else {
                        pPrev = tVerts[i - 1];
                    }

                    // pCur = the current vertex;
                    Vertex pCur = tVerts[i];

                    // pNext = the next vertex in the list
                    Vertex pNext;
                    if (i == tVerts.size() - 1) {
                        pNext = tVerts[0];
                    } else {
                        pNext = tVerts[i + 1];
                    }

                    // Check to see if there are only 3 verts left
                    // if so this is the last triangle
                    if (tVerts.size() == 3) {
                        // Create a triangle from pCur, pPrev, pNext
                        for (size_t j = 0; j < tVerts.size(); j++) {
                            if (iVerts[j].Position == pCur.Position)
                                oIndices.push_back(j);
                            if (iVerts[j].Position == pPrev.Position)
                                oIndices.push_back(j);
                            if (iVerts[j].Position == pNext.Position)
                                oIndices.push_back(j);
                        }

                        tVerts.clear();
                        break;
                    }
                    if (tVerts.size() == 4) {
                        // Create a triangle from pCur, pPrev, pNext
                        for (size_t j = 0; j < iVerts.size(); j++) {
                            if (iVerts[j].Position == pCur.Position)
                                oIndices.push_back(j);
                            if (iVerts[j].Position == pPrev.Position)
                                oIndices.push_back(j);
                            if (iVerts[j].Position == pNext.Position)
                                oIndices.push_back(j);
                        }

                        Vector3f tempVec(0.0f, 0.0f, 0.0f);
                        for (size_t j = 0; j < tVerts.size(); j++) {
                            if (tVerts[j].Position != pCur.Position
                                && tVerts[j].Position != pPrev.Position
                                && tVerts[j].Position != pNext.Position) {
                                tempVec = tVerts[j].Position;
                                break;
                            }
                        }

                        // Create a triangle from pCur, pPrev, pNext
                        for (size_t j = 0; j < iVerts.size(); j++) {
                            if (iVerts[j].Position == pPrev.Position)
                                oIndices.push_back(j);
                            if (iVerts[j].Position == pNext.Position)
                                oIndices.push_back(j);
                            if (iVerts[j].Position == tempVec)
                                oIndices.push_back(j);
                        }

                        tVerts.clear();
                        break;
                    }

                    // If Vertex is not an interior vertex
                    float angle = math::AngleBetweenV3(pPrev.Position - pCur.Position,
                        pNext.Position - pCur.Position) * (180.0f / (float)M_PI);
                    if (angle <= 0 && angle >= 180)
                        continue;

                    // If any vertices are within this triangle
                    bool inTri = false;
                    for (size_t j = 0; j < iVerts.size(); j++) {
                        if (algorithm::inTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position)
                            && iVerts[j].Position != pPrev.Position
                            && iVerts[j].Position != pCur.Position
                            && iVerts[j].Position != pNext.Position) {
                            inTri = true;
                            break;
                        }
                    }
                    if (inTri)
                        continue;

                    // Create a triangle from pCur, pPrev, pNext
                    for (size_t j = 0; j < iVerts.size(); j++) {
                        if (iVerts[j].Position == pCur.Position)
                            oIndices.push_back(j);
                        if (iVerts[j].Position == pPrev.Position)
                            oIndices.push_back(j);
                        if (iVerts[j].Position == pNext.Position)
                            oIndices.push_back(j);
                    }

                    // Delete pCur from the list
                    for (size_t j = 0; j < tVerts.size(); j++) {
                        if (tVerts[j].Position == pCur.Position) {
                            tVerts.erase(tVerts.begin() + j);
                            break;
                        }
                    }

                    // reset i to the start
                    // -1 since loop will add 1 to it
                    i = -1;
                }

                // if no triangles were created
                if (oIndices.size() == 0)
                    break;

                // if no more vertices
                if (tVerts.size() == 0)
                    break;
            }
        }

        // Load Materials from .mtl file
        bool LoadMaterials(string path)
        {
            // If the file is not a material file return false
            if (path.substr(path.size() - 4, path.size()) != ".mtl")
                return false;

            ifstream file(path);

            // If the file is not found return false
            if (!file.is_open())
                return false;

            Material tempMaterial;

            bool listening = false;

            // Go through each line looking for material variables
            string curline;
            while (getline(file, curline)) {
                // new material and material name
                if (algorithm::firstToken(curline) == "newmtl") {
                    if (!listening) {
                        listening = true;

                        if (curline.size() > 7) {
                            tempMaterial.name = algorithm::tail(curline);
                        } else {
                            tempMaterial.name = "none";
                        }
                    } else {
                        // Generate the material

                        // Push Back loaded Material
                        LoadedMaterials.push_back(tempMaterial);

                        // Clear Loaded Material
                        tempMaterial = Material();

                        if (curline.size() > 7) {
                            tempMaterial.name = algorithm::tail(curline);
                        } else {
                            tempMaterial.name = "none";
                        }
                    }
                }
                // Ambient Color
                if (algorithm::firstToken(curline) == "Ka") {
                    vector<string> temp;
                    algorithm::split(algorithm::tail(curline), temp, " ");

                    if (temp.size() != 3)
                        continue;

                    tempMaterial.Ka = { stof(temp[0]), stof(temp[1]), stof(temp[2]) };
                }
                // Diffuse Color
                if (algorithm::firstToken(curline) == "Kd") {
                    vector<string> temp;
                    algorithm::split(algorithm::tail(curline), temp, " ");

                    if (temp.size() != 3)
                        continue;

                    tempMaterial.Kd = { stof(temp[0]), stof(temp[1]), stof(temp[2]) };
                }
                // Specular Color
                if (algorithm::firstToken(curline) == "Ks") {
                    vector<string> temp;
                    algorithm::split(algorithm::tail(curline), temp, " ");

                    if (temp.size() != 3)
                        continue;

                    tempMaterial.Ks = { stof(temp[0]), stof(temp[1]), stof(temp[2]) };
                }
                // Specular Exponent
                if (algorithm::firstToken(curline) == "Ns") {
                    tempMaterial.Ns = stof(algorithm::tail(curline));
                }
                // Optical Density
                if (algorithm::firstToken(curline) == "Ni") {
                    tempMaterial.Ni = stof(algorithm::tail(curline));
                }
                // Dissolve
                if (algorithm::firstToken(curline) == "d") {
                    tempMaterial.d = stof(algorithm::tail(curline));
                }
                // Illumination
                if (algorithm::firstToken(curline) == "illum") {
                    tempMaterial.illum = stoi(algorithm::tail(curline));
                }
                // Ambient Texture Map
                if (algorithm::firstToken(curline) == "map_Ka") {
                    tempMaterial.map_Ka = algorithm::tail(curline);
                }
                // Diffuse Texture Map
                if (algorithm::firstToken(curline) == "map_Kd") {
                    tempMaterial.map_Kd = algorithm::tail(curline);
                }
                // Specular Texture Map
                if (algorithm::firstToken(curline) == "map_Ks") {
                    tempMaterial.map_Ks = algorithm::tail(curline);
                }
                // Specular Hightlight Map
                if (algorithm::firstToken(curline) == "map_Ns") {
                    tempMaterial.map_Ns = algorithm::tail(curline);
                }
                // Alpha Texture Map
                if (algorithm::firstToken(curline) == "map_d") {
                    tempMaterial.map_d = algorithm::tail(curline);
                }
                // Bump Map
                if (algorithm::firstToken(curline) == "map_Bump" ||
                    algorithm::firstToken(curline) == "map_bump" ||
                    algorithm::firstToken(curline) == "bump") {
                    tempMaterial.map_bump = algorithm::tail(curline);
                }
            }

            // Deal with last material

            // Push Back loaded Material
            LoadedMaterials.push_back(tempMaterial);

            // Test to see if anything was loaded
            // If not return false
            return (!LoadedMaterials.empty());
        }
    };
}
