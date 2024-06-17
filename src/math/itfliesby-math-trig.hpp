#ifndef ITFLIESBY_MATH_TRIG_HPP
#define ITFLIESBY_MATH_TRIG_HPP

#include <ifb.hpp>
#include <math.h>

#define ITFLIESBY_MATH_TRIG_PI                  3.14159265
#define ITFLIESBY_MATH_TRIG_DEG_TO_RAD_CONSTANT 0.01745329 // (Pi / 180)

inline f32
itfliesby_math_trig_degrees_to_radians(
    f32 degrees) {
    
    f32 radians = degrees * ITFLIESBY_MATH_TRIG_DEG_TO_RAD_CONSTANT;
    return(radians);
}

#endif //ITFLIESBY_MATH_TRIG_HPP