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

#include <iostream>
#include "ObjLoader.h"
#include "Model.h"

Model::Model(const char* filename)
{
    ObjLoader::Loader Loader;
    if (Loader.LoadFile(filename)) {
        for (Mesh m : Loader.LoadedMeshes)
            mMesh.push_back(m);
    } else {
        cerr << "Could not load '" << filename << "'\n";
    }
}

BoundingBox Model::getBoundingBox() const
{
    BoundingBox bb = { {-1.0f, -1.0f, -1.0f} , {1.0f, 1.0f, 1.0f} };
    for (Mesh m : mMesh) {
        for (Vertex v : m.Vertices) {
            bb.min.x() = (std::min)(bb.min.x(), v.Position.x());
            bb.min.y() = (std::min)(bb.min.y(), v.Position.y());
            bb.min.z() = (std::min)(bb.min.z(), v.Position.z());
            bb.max.x() = (std::max)(bb.max.x(), v.Position.x());
            bb.max.y() = (std::max)(bb.max.y(), v.Position.y());
            bb.max.z() = (std::max)(bb.max.z(), v.Position.z());
        }
    }
    return bb;
}

void Model::glDraw() const
{
    for (Mesh m : mMesh) {
        for (size_t i = 0; i < m.Indices.size() / 3; i++) {
            glBegin(GL_TRIANGLES);
            Vector3f n = m.getNormal(i);
            glNormal3f(n.x(), n.y(), n.z());
            for (size_t j = 0; j < 3; j++) {
                Vector3f v = m.Vertices[m.Indices[3 * i + j]].Position;
                glVertex3f(v.x(), v.y(), v.z());
            }
            glEnd();
        }
    }
}
