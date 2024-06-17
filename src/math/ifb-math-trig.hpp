#ifndef IFB_MATH_TRIG_HPP
#define IFB_MATH_TRIG_HPP

#include <ifb.hpp>
#include <math.h>

#define IFB_MATH_TRIG_PI                  3.14159265
#define IFB_MATH_TRIG_DEG_TO_RAD_CONSTANT 0.01745329 // (Pi / 180)

inline f32
ifb_math_trig_degrees_to_radians(
    f32 degrees) {
    
    f32 radians = degrees * IFB_MATH_TRIG_DEG_TO_RAD_CONSTANT;
    return(radians);
}

#endif //IFB_MATH_TRIG_HPP