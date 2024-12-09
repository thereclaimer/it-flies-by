#ifndef IFB_COMMON_HASH_HPP
#define IFB_COMMON_HASH_HPP

#include <string.h>

#include "ifb-common-types.hpp"
#include "ifb-common-scopes.hpp"

union IFBHashValue {
    struct {
        ifb_u32 h1;
        ifb_u32 h2;
        ifb_u32 h3;
        ifb_u32 h4;
    };
    ifb_u32 h[4];
};

namespace ifb_common {

    inline const ifb_u32 
    hash_cstr(
        const ifb_cstr       in_cstr,
        const ifb_u32        in_cstr_length_max,
              IFBHashValue& out_cstr_hash_value_ref) {

        //sanity check
        if (!in_cstr) {
            return(0);
        }

        //get the safe length of the string
        const ifb_u32 in_cstr_length = strnlen_s(in_cstr,in_cstr_length_max - 1) + 1;

        //do the hash
        meow_u128 meow_hash_value = MeowHash(MeowDefaultSeed,in_cstr_length,in_cstr);

        //load the value
        _mm_storeu_epi32(out_cstr_hash_value_ref.h,meow_hash_value);

        //we're done
        return(in_cstr_length);
    }

    inline const ifb_b8
    hash_collision_check(
              IFBHashValue& hash_value_ref,
        const IFBHashValue* hash_value_array_ptr,
        const ifb_u32       hash_value_array_count) {

        if (
            hash_value_array_ptr   == NULL ||  
            hash_value_array_count == 0) {
            
            return(false);
        }

        //load the hash value
        __m128i xmm_reg_0 = _mm_loadu_epi32(hash_value_ref.h);

        for (
            ifb_u32 hash_index = 0;
                    hash_index < hash_value_array_count;
                ++hash_index) {

            //load the current value to compare            
            __m128i xmm_reg_1 = _mm_loadu_epi32(hash_value_array_ptr[hash_index].h); 

            //compare the values
            if (MeowHashesAreEqual(xmm_reg_0,xmm_reg_1)) {
                //if they are equal, we're done
                return(true);
            }
        }

        //no collisions, we're done
        return(false); 
    }
    
    inline const ifb_b8
    hash_next_clear_value(
        const IFBHashValue* in_hash_value_array_ptr,
        const ifb_u32       in_hash_value_array_count,
            ifb_u32&       out_hash_index_ref) {

        __m128i xmm_reg;

        for (
            out_hash_index_ref = 0;
            out_hash_index_ref < in_hash_value_array_count;
            ++out_hash_index_ref) {

            xmm_reg = _mm_loadu_epi32(in_hash_value_array_ptr[out_hash_index_ref].h);

            if (_mm_testz_si128(xmm_reg,xmm_reg) != 0) {
                return(true);
            }
        }

        return(false);
    }

    inline const ifb_b8
    hash_search(
              IFBHashValue& in_hash_value_ref,
        const IFBHashValue* in_hash_value_array_ptr,
        const ifb_u32       in_hash_value_array_count,
              ifb_u32&     out_hash_index_ref) {

        if (
            in_hash_value_array_ptr   == NULL ||  
            in_hash_value_array_count == 0) {
            
            return(false);
        }

        //load the hash value
        __m128i xmm_reg_0 = _mm_loadu_epi32(in_hash_value_ref.h);

        for (
            ifb_u32 out_hash_index_ref = 0;
                    out_hash_index_ref < in_hash_value_array_count;
                ++out_hash_index_ref) {

            //load the current value to compare            
            __m128i xmm_reg_1 = _mm_loadu_epi32(in_hash_value_array_ptr[out_hash_index_ref].h); 

            //compare the values
            if (MeowHashesAreEqual(xmm_reg_0,xmm_reg_1)) {
                //if they are equal, we're done
                return(true);
            }
        }

        //no collisions, we're done
        return(false); 
    }
};


#endif //IFB_COMMON_HASH_HPP