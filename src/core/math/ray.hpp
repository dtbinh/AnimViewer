#pragma once
#ifndef RAY_TRACER_LINE_H
#define RAY_TRACER_LINE_H

#include "math/vector.hpp"

namespace Luc {


class Ray
{
public:
    Ray(const Vector3 & pnt, const Vector3 & dir);
    Ray(const Ray& line);
    ~Ray() {};

    Vector3 Point()  const { return m_pnt; };
    Vector3 Direction() const { return m_dir; };

private:
    Vector3 m_pnt;
    Vector3 m_dir;
};

}
#endif //RAY_TRACER_LINE_H
