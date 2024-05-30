#ifndef ITFLIESBY_MATH_MAT2
#define ITFLIESBY_MATH_MAT2

#include <common/itfliesby-types.hpp>
#include <immintrin.h> 

struct ItfliesbyMathMat2 {
    //[collumn][row]
    f32 m[2][2];
};

inline void
itfliesby_math_mat2_multiply_mat2(
    const ItfliesbyMathMat2* mat2_in_a,
    const ItfliesbyMathMat2* mat2_in_b,
    const size               count,
    ItfliesbyMathMat2*       mat2_out) {

    __m128 mat_a;
    __m128 mat_b;
    __m128 mat_c;

    for (
        size mat_index = 0;
        mat_index < count;
        ++mat_index) {

        __m128 mat_a = _mm_loadu_ps((f32*)mat2_in_a[mat_index].m); // Load the first column of the first matrix
        __m128 mat_b = _mm_loadu_ps((f32*)mat2_in_b[mat_index].m); // Load the first column of the second matrix

        // Transpose the second matrix
        mat_b = _mm_shuffle_ps(mat_b, mat_b, _MM_SHUFFLE(0, 1, 0, 1));    
    
        // Perform the multiplication
        __m128 mat_c = _mm_dp_ps(mat_a, mat_b, 0xF1); // Dot product of the first row of 'a' and the first row of 'b'

        // Store the result back into the result matrix
        _mm_storeu_ps((f32*)mat2_out[mat_index].m, mat_c);
    }
}

#endif //ITFLIESBY_MATH_MAT2
