#ifndef GEOMETRIC3DPRIMITIVES_H
#define GEOMETRIC3DPRIMITIVES_H

#include "geometric2dprimitives.h"

class Geometric3DPrimitives : public Geometric2DPrimitives
{
public:
    Geometric3DPrimitives();
    static void cylinder(float radius, float height, int num_segments, float r, float g, float b);
    static void cone(float radius, float height, int num_segments, float r, float g, float b);
    static void parallelepiped(float width, float height, float depth, float alpha);
};

#endif // GEOMETRIC3DPRIMITIVES_H
