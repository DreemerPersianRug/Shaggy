#ifndef COMPLEXOBJECTS_H
#define COMPLEXOBJECTS_H

#include "geometric3dprimitives.h"

class ComplexObjects : public Geometric3DPrimitives
{
public:
    ComplexObjects();
    static void cortesian(float cylinder_height, float cone_height);
    static void basis(float cylinder_radius, float cylinder_height, float cone_radius, float cone_height, int num_segments);
};

#endif // COMPLEXOBJECTS_H
