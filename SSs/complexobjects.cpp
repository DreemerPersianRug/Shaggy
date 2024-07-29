#include "complexobjects.h"
#include <GLFW/glfw3.h>

ComplexObjects::ComplexObjects()
    : Geometric3DPrimitives()
{   
}

void ComplexObjects::cortesian(float cylinder_height, float cone_height)
{
    glBegin(GL_LINES);
    glLineWidth(0.2f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, cylinder_height + cone_height + 20, 0.0f);
    glVertex3f(0.0f, -(cylinder_height + cone_height + 20), 0.0f);

    glLineWidth(0.2f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -(cylinder_height + cone_height + 20));
    glVertex3f(0.0f, 0.0f, cylinder_height + cone_height + 20);

    glLineWidth(0.2f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(cylinder_height + cone_height + 20, 0.0f, 0.0f);
    glVertex3f(-(cylinder_height + cone_height + 20), 0.0f, 0.0f);
    glEnd();
}

void ComplexObjects::basis(float cylinder_radius, float cylinder_height, float cone_radius, float cone_height, int num_segments)
{
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    cylinder(cylinder_radius, cylinder_height, num_segments, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, cylinder_height);
    cone(cone_radius, cone_height, num_segments, 0.0f, 0.0f, 1.0f);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    cylinder(cylinder_radius, cylinder_height, num_segments, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, cylinder_height);
    cone(cone_radius, cone_height, num_segments, 0.0f, 1.0f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glRotatef(120.0f, 1.0f, 1.0f, 1.0f);
    cylinder(cylinder_radius, cylinder_height, num_segments, 1.0f, 0.0f, 0.0f);
    glTranslatef( 0.0f, 0.0f, cylinder_height);
    cone(cone_radius, cone_height, num_segments, 1.0f, 0.0f, 0.0f);
    glPopMatrix();
}
