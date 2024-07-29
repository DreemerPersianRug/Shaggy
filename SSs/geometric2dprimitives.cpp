#include "geometric2dprimitives.h"
#include <GLFW/glfw3.h>
#include <cmath>

Geometric2DPrimitives::Geometric2DPrimitives()
{
}

void Geometric2DPrimitives::circle(float radius, int num_segments, float x, float y, float z, float r, float g, float b) {
    float angle_step = 2.0f * M_PI / num_segments;

    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y, z);
    for (int i = 0; i < num_segments; ++i) {
        float angle = i * angle_step;
        float x_border = x + cosf(angle) * radius;
        float y_border = y + sinf(angle) * radius;
        glVertex3f(x_border, y_border, z);
    }
    glEnd();
}

void Geometric2DPrimitives::rectangle(float width, float height, float r, float g, float b, float alpha) {
    glColor4f(r, g, b, alpha);

    float x = width / 2.0f;
    float y = height / 2.0f;

    glBegin(GL_QUADS);
    glVertex3f(-x, -y, 0.0f);
    glVertex3f(x, -y, 0.0f);
    glVertex3f(x, y, 0.0f);
    glVertex3f(-x, y, 0.0f);
    glEnd();
}
