#include "mathfunc.h"
#include <cmath>

MathFunc::MathFunc()
{  
}

float MathFunc::norm(QVector<float> vec)
{
    float norm = 0;
    for(int i = 0; i < vec.size(); ++i) {
        norm += vec[i]*vec[i];
    }
    return sqrt(norm);
}
