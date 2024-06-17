#ifndef IFB_MATH_HPP
#define IFB_MATH_HPP

#include "ifb-math-vec2.hpp"
#include "ifb-math-mat2.hpp"
#include "ifb-math-mat3.hpp"
#include "ifb-math-trig.hpp"

#define IFB_MATH_KILOBYTES(value) ((long long)(value * 1024))
#define IFB_MATH_MEGABYTES(value) ((long long)(IFB_MATH_KILOBYTES(value) * 1024))
#define IFB_MATH_GIGABYTES(value) ((long long)(IFB_MATH_MEGABYTES(value) * 1024))

#endif //IFB_MATH_HPP