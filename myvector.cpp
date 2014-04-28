#include "myvector.h"
#include <math.h>

MyVector::MyVector(float x, float y) :
    QPointF(x,y)
{
}

float MyVector::norm()
{
    return sqrt( pow(this->x(),2) + pow(this->y(),2) );
}
