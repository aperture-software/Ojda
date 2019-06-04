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
 */

#include "ObjLoader.h"
#include "Cube.h"

Cube::Cube(const char* filename)
{
    ObjLoader::Loader Loader;
    if (Loader.LoadFile(filename)) {
        // Only load the first Mesh for now
        mMesh = Loader.LoadedMeshes[0];
    } else {
        cerr << "Could not load '" << filename << "'\n";
    }
}

Vector3f Cube::getNormal(const size_t index) const
{
    if (index > this->mMesh.Indices.size())
        return Vector3f();

    Vector3f v[3];
    for (size_t j = 0; j < 3; j++)
        v[j] = mMesh.Vertices[mMesh.Indices[3 * index + j]].Position;
    v[1] -= v[0];
    v[2] -= v[0];
    Vector3f n = v[1].cross(v[2]);
    n.normalize();

    return n;
}

void Cube::glDraw() const
{
    for (size_t i = 0; i < mMesh.Indices.size() / 3; i++) {
        glBegin(GL_TRIANGLES);
        Vector3f n = this->getNormal(i);
        glNormal3f(n.x(), n.y(), n.z());
        for (size_t j = 0; j < 3; j++) {
            Vector3f v = mMesh.Vertices[mMesh.Indices[3 * i + j]].Position;
            glVertex3f(v.x(), v.y(), v.z());
        }
        glEnd();
    }
}
