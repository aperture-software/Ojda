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
    Eigen::Matrix3d m;
    m << 1.0, -1.0, -1.0,
        1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0;
    this->m_face.push_back(m);
    m << 1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        1.0, 1.0, 1.0;
    this->m_face.push_back(m);
    m << 1.0, -1.0, -1.0,
        1.0, 1.0, -1.0,
        1.0, -1.0, 1.0;
    this->m_face.push_back(m);
    m << 1.0, -1.0, 1.0,
        1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0;
    this->m_face.push_back(m);
    m << -1.0, -1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, -1.0;
    this->m_face.push_back(m);
    m << 1.0, 1.0, -1.0,
        1.0, -1.0, -1.0,
        -1.0, 1.0, 1.0;
    this->m_face.push_back(m);
    m << 1.0, -1.0, 1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0;
    this->m_face.push_back(m);
    m << -1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0,
        1.0, 1.0, 1.0;
    this->m_face.push_back(m);
    m << 1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        1.0, -1.0, 1.0;
    this->m_face.push_back(m);
    m << 1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0;
    this->m_face.push_back(m);
    m << -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0;
    this->m_face.push_back(m);
    m << 1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0, 1.0, -1.0;
    this->m_face.push_back(m);
}

Eigen::Vector3d Cube::getNormal(const int index) const
{
    if (index > this->m_face.size())
        return Eigen::Vector3d();

    Eigen::Matrix3d m = this->m_face[index];
    Eigen::Vector3d v[3] = { m.row(0), m.row(1), m.row(2) };
    v[1] -= v[0];
    v[2] -= v[0];
    Eigen::Vector3d n = v[1].cross(v[2]);
    n.normalize();

    return n;
}

void Cube::glDraw() const
{
    for (int i = 0; i < m_face.size(); i++) {
        glBegin(GL_TRIANGLES);
        Eigen::Vector3d n = this->getNormal(i);
        glNormal3d(n.x(), n.y(), n.z());
        for (int j = 0; j < 3; j++)
            glVertex3d(this->m_face[i].row(j).x(), this->m_face[i].row(j).y(), this->m_face[i].row(j).z());
        glEnd();
    }
}
