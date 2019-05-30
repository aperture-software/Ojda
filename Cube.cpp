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

#include "Cube.h"

Cube::Cube()
{
    Matrix3f m;
    m << 1.0, -1.0, -1.0,
        1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0;
    this->mFace.push_back(m);
    m << 1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        1.0, 1.0, 1.0;
    this->mFace.push_back(m);
    m << 1.0, -1.0, -1.0,
        1.0, 1.0, -1.0,
        1.0, -1.0, 1.0;
    this->mFace.push_back(m);
    m << 1.0, -1.0, 1.0,
        1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0;
    this->mFace.push_back(m);
    m << -1.0, -1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, -1.0;
    this->mFace.push_back(m);
    m << 1.0, 1.0, -1.0,
        1.0, -1.0, -1.0,
        -1.0, 1.0, -1.0;
    this->mFace.push_back(m);
    m << 1.0, -1.0, 1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0;
    this->mFace.push_back(m);
    m << -1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0,
        1.0, 1.0, 1.0;
    this->mFace.push_back(m);
    m << 1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        1.0, -1.0, 1.0;
    this->mFace.push_back(m);
    m << 1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0;
    this->mFace.push_back(m);
    m << -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0;
    this->mFace.push_back(m);
    m << 1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0, 1.0, -1.0;
    this->mFace.push_back(m);
}

Vector3f Cube::getNormal(const int index) const
{
    if (index > this->mFace.size())
        return Vector3f();

    Matrix3f m = this->mFace[index];
    Vector3f v[3] = { m.row(0), m.row(1), m.row(2) };
    v[1] -= v[0];
    v[2] -= v[0];
    Vector3f n = v[1].cross(v[2]);
    n.normalize();

    return n;
}

void Cube::glDraw() const
{
    for (int i = 0; i < mFace.size(); i++) {
        glBegin(GL_TRIANGLES);
        Vector3f n = this->getNormal(i);
        glNormal3f(n.x(), n.y(), n.z());
        for (int j = 0; j < 3; j++)
            glVertex3f(this->mFace[i].row(j).x(), this->mFace[i].row(j).y(), this->mFace[i].row(j).z());
        glEnd();
    }
}
