#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "quaternion.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    void quaternionToMatrix(const Quaternion& q, GLfloat* matrix);
    void updateQuaternion(const Quaternion& new_quatrenion);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    bool axes__;
    bool model__;
    bool basis__;

    Quaternion quaternion__;

public slots:
    void DisplayAxesSlot();
    void DisplayModelSlot();
    void DisplayBasisSlot();
};

#endif // GLWIDGET_H
