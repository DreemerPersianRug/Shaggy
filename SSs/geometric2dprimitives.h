#ifndef GEOMETRIC2DPRIMITIVES_H
#define GEOMETRIC2DPRIMITIVES_H


class Geometric2DPrimitives
{
public:
    Geometric2DPrimitives();
    static void circle(float radius, int num_segments, float x, float y, float z, float r, float g, float b);
    static void rectangle(float width, float height, float r, float g, float b, float alpha);
};

#endif // GEOMETRIC2DPRIMITIVES_H
