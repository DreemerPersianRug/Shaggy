#include "geometric3dprimitives.h"
#include <GLFW/glfw3.h>
#include <cmath>

Geometric3DPrimitives::Geometric3DPrimitives()
    : Geometric2DPrimitives()
{
}

void Geometric3DPrimitives::cylinder(float radius, float height, int num_segments, float r, float g, float b)
{
    float angle_step = 2.0f * M_PI / num_segments;

    glColor3f(r, g, b);
    glBegin(GL_QUAD_STRIP); // Side
    for (int i = 0; i < num_segments; ++i) {
        float angle = i * angle_step;
        float x = cosf(angle) * radius;
        float y = sinf(angle) * radius;

        glVertex3f(x, y, 0.0f);
        glVertex3f(x, y, height);
    }
    glEnd();

    circle(radius, num_segments, 0.0f, 0.0f, 0.0f, r, g, b); // Lower
    circle(radius, num_segments, 0.0f, 0.0f, height, r, g, b); // Upper
}

void Geometric3DPrimitives::cone(float radius, float height, int num_segments, float r, float g, float b)
{
    float angle_step = 2.0f * M_PI / num_segments;

    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN); // Side
    glVertex3f(0.0f, 0.0f, height); // Top of the cone
    for (int i = 0; i < num_segments; ++i) {
        float angle = i * angle_step;
        float x = cosf(angle) * radius;
        float y = sinf(angle) * radius;
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Base
    circle(radius, num_segments, 0.0f, 0.0f, 0.0f, r, g, b);
}

void Geometric3DPrimitives::parallelepiped(float width, float height, float depth, float alpha)
{
    float x = width / 2.0f;
    float y = height / 2.0f;
    float z = depth / 2.0f;

    // Front
    glColor4f(1.0f, 0.0f, 0.0f, alpha);
    glBegin(GL_QUADS);
    glVertex3f(-x, -y, z);
    glVertex3f(x, -y, z);
    glVertex3f(x, y, z);
    glVertex3f(-x, y, z);
    glEnd();

    // Back
    glColor4f(1.0f, 0.0f, 0.0f, alpha);
    glBegin(GL_QUADS);
    glVertex3f(-x, -y, -z);
    glVertex3f(x, -y, -z);
    glVertex3f(x, y, -z);
    glVertex3f(-x, y, -z);
    glEnd();

    // Top
    glColor4f(1.0f, 1.0f, 0.0f, alpha);
    glBegin(GL_QUADS);
    glVertex3f(-x, y, -z);
    glVertex3f(x, y, -z);
    glVertex3f(x, y, z);
    glVertex3f(-x, y, z);
    glEnd();

    // Bottom
    glColor4f(1.0f, 1.0f, 0.0f, alpha);
    glBegin(GL_QUADS);
    glVertex3f(-x, -y, -z);
    glVertex3f(x, -y, -z);
    glVertex3f(x, -y, z);
    glVertex3f(-x, -y, z);
    glEnd();

    // Left
    glColor4f(0.0f, 1.0f, 1.0f, alpha);
    glBegin(GL_QUADS);
    glVertex3f(-x, -y, -z);
    glVertex3f(-x, -y, z);
    glVertex3f(-x, y, z);
    glVertex3f(-x, y, -z);
    glEnd();

    // Right
    glColor4f(0.0f, 1.0f, 1.0f, alpha);
    glBegin(GL_QUADS);
    glVertex3f(x, -y, -z);
    glVertex3f(x, -y, z);
    glVertex3f(x, y, z);
    glVertex3f(x, y, -z);
    glEnd();
}
