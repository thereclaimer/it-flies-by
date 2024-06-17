#ifndef ITFLIESBY_MATH_VEC_HPP
#define ITFLIESBY_MATH_VEC_HPP

#include <ifb.hpp>
#include <math.h>

struct ItfliesbyMathVec2 {
    f32 x;
    f32 y;
};

inline void
itfliesby_math_vec2_multiply_scalar(
    const ItfliesbyMathVec2* vec_in,
    ItfliesbyMathVec2*       vec_out,
    const f32                scalar,
    const u64                count) {

    for (
        u64 vec_index = 0;
        vec_index < count;
        ++vec_index) {

        vec_out[vec_index].x = vec_in[vec_index].x * scalar;
        vec_out[vec_index].y = vec_in[vec_index].y * scalar;
    }
}

inline void
itfliesby_math_vec2_multiply_scalars(
    const ItfliesbyMathVec2* vec_in,
    ItfliesbyMathVec2*       vec_out,
    const f32*               scalars,
    const u64                count) {

    for (
        u64 vec_index = 0;
        vec_index < count;
        ++vec_index) {

        vec_out[vec_index].x = vec_in[vec_index].x * scalars[vec_index];
        vec_out[vec_index].y = vec_in[vec_index].y * scalars[vec_index];
    }
}

inline void
itfliesby_math_vec2_divide_scalar(
    const ItfliesbyMathVec2* vec_in,
    ItfliesbyMathVec2*       vec_out,
    const f32                scalar,
    const u64              count) {

    f32 scalar_inverse = 1.0f / scalar;

    for (
        u64 vec_index = 0;
        vec_index < count;
        ++vec_index) {

        vec_out[vec_index].x = vec_in[vec_index].x * scalar_inverse;
        vec_out[vec_index].y = vec_in[vec_index].y * scalar_inverse;
    }
}

inline void
itfliesby_math_vec2_divide_scalars(
    const ItfliesbyMathVec2* vec_in,
    ItfliesbyMathVec2*       vec_out,
    const f32*               scalars,
    const u64              count) {


    for (
        u64 vec_index = 0;
        vec_index < count;
        ++vec_index) {

        vec_out[vec_index].x = vec_in[vec_index].x / scalars[vec_index];
        vec_out[vec_index].y = vec_in[vec_index].y / scalars[vec_index];
    }
}

inline void
itfliesby_math_vec2_add(
    const ItfliesbyMathVec2* vec_in_a,
    const ItfliesbyMathVec2* vec_in_b,
    const u64                count,
    ItfliesbyMathVec2*       vec_out) {

    for (
        u64 vec_index = 0;
        vec_index < count;
        ++vec_index) {

        vec_out[vec_index].x = vec_in_a[vec_index].x + vec_in_b[vec_index].x;
        vec_out[vec_index].y = vec_in_a[vec_index].y + vec_in_b[vec_index].y;
    }
}

inline void
itfliesby_math_vec2_subtract(
    const ItfliesbyMathVec2* vec_in_a,
    const ItfliesbyMathVec2* vec_in_b,
    const u64                count,
    ItfliesbyMathVec2*       vec_out) {

    for (
        u64 vec_index = 0;
        vec_index < count;
        ++vec_index) {

        vec_out[vec_index].x = vec_in_a[vec_index].x - vec_in_b[vec_index].x;
        vec_out[vec_index].y = vec_in_a[vec_index].y - vec_in_b[vec_index].y;
    }
}


inline void
itfliesby_math_vec2_magnitude(
    const ItfliesbyMathVec2* vec_in,
    f32*                     magnitude_out,
    const u64                count) {

    for (
        u64 vec_index = 0;
        vec_index < count;
        ++vec_index) {

        magnitude_out[vec_index] = 
            sqrtf(
                vec_in[vec_index].x * vec_in[vec_index].x + 
                vec_in[vec_index].y * vec_in[vec_index].y 
            );
    }

}

inline void
itfliesby_math_vec2_normalize(
    const ItfliesbyMathVec2* vec_in,
    ItfliesbyMathVec2*       vec_out,
    f32*                     magnitude_out,
    const u64                count) {


    itfliesby_math_vec2_magnitude(
        vec_in,
        magnitude_out,
        count
    );

    const f32* magnitude_const = (const f32*)magnitude_out;

    itfliesby_math_vec2_divide_scalars(
        vec_in,
        vec_out,
        magnitude_const,
        count
    );
}


#endif //ITFLIESBY_MATH_VEC_HPP