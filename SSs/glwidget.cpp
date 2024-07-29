#include "glwidget.h"
#include "complexobjects.h"
#include "geometric3dprimitives.h"
#include "ui_mainwindow.h"
#include "quaternion.h"

#include <GL/glu.h>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QtCore/qglobal.h>

#include<QCheckBox>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), axes__(true),  model__(true), basis__(true), quaternion__(1.0, 0.0, 0.0, 0.0)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&GLWidget::update));
    (*timer).start(16); // Approximately 60 FPS

}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width() / height(), 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 10.0f, 5.0f, // Camera
              0.0f, 0.0f, 0.0f, // Focus dot
              0.0f, 1.0f, 0.0f); // Up camera's vector

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -35.0f);

    GLfloat rotationMatrix[16];
    quaternionToMatrix(quaternion__, rotationMatrix);
    glMultMatrixf(rotationMatrix);

    if(axes__) {
        ComplexObjects::cortesian(1.0f,  1.0f);
    }
    if(basis__) {
        ComplexObjects::basis(0.2f, 9.0f, 0.4f, 1.0f, 32);
    }
    if(model__) {
        Geometric3DPrimitives::parallelepiped(10.0f, 10.0f, 20.0f, 0.2f);
    }
    glPopMatrix();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::quaternionToMatrix(const Quaternion &q, GLfloat *matrix)
{
    matrix[0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    matrix[1] = 2.0f * (q.x * q.y + q.z * q.w);
    matrix[2] = 2.0f * (q.x * q.z - q.y * q.w);
    matrix[3] = 0.0f;

    matrix[4] = 2.0f * (q.x * q.y - q.z * q.w);
    matrix[5] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    matrix[6] = 2.0f * (q.y * q.z + q.x * q.w);
    matrix[7] = 0.0f;

    matrix[8] = 2.0f * (q.x * q.z + q.y * q.w);
    matrix[9] = 2.0f * (q.y * q.z - q.x * q.w);
    matrix[10] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

void GLWidget::updateQuaternion(const Quaternion &new_quatrenion)
{
    quaternion__ = new_quatrenion;
    update();
}

void GLWidget::DisplayAxesSlot()
{
    axes__ = !axes__;
    update();
}

void GLWidget::DisplayModelSlot()
{
    model__ = !model__;
    update();
}

void GLWidget::DisplayBasisSlot()
{
    basis__ = !basis__;
    update();
}
