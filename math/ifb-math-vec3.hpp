#ifndef IFB_MATH_VEC3_HPP
#define IFB_MATH_VEC3_HPP

#include <ifb.hpp>

namespace ifb_math {

    //----------------------
    // instance operations
    //----------------------

    void vec3_a_add_b           (const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_a_sub_b           (const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_a_dot_b           (const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_a_cross_b         (const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_a_project_b       (const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_a_reject_b        (const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);

    void vec3_scalar            (const ifb_v3* v3, const ifb_f32* scalar, ifb_v3* v3_scalar);

    void vec3_magnitude         (const ifb_v3* v3, ifb_f32* v3_magnitude);
    void vec3_normalize         (const ifb_v3* v3, ifb_v3*  v3_normalized);

    //----------------------
    // batch operations
    //----------------------

    void vec3_batch_a_add_b     (const ifb_u32 count, const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_batch_a_sub_b     (const ifb_u32 count, const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_batch_a_dot_b     (const ifb_u32 count, const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_batch_a_cross_b   (const ifb_u32 count, const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_batch_a_project_b (const ifb_u32 count, const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);
    void vec3_batch_a_reject_b  (const ifb_u32 count, const ifb_v3* v3_a, const ifb_v3* v3_b, ifb_v3* v3_c);

    void vec3_batch_scalar      (const ifb_u32 count, const ifb_v3* v3, const ifb_f32* scalar, ifb_v3* v3_scalar);

    void vec3_batch_magnitude   (const ifb_u32 count, const ifb_v3* v3, ifb_f32* v3_magnitude);
    void vec3_batch_normalize   (const ifb_u32 count, const ifb_v3* v3, ifb_v3*  v3_normalized);
};


#endif //IFB_MATH_VEC3_HPP