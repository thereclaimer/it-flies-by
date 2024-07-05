#ifndef IFB_TAG_HPP
#define IFB_TAG_HPP

#include <string.h>

#include "ifb-types.hpp"

struct IFBTag {
    char value[32];
    u8   length;
};

inline const IFBTag
ifb_tag(
    const char* tag_value) {

    assert(strlen(tag_value) <= 32);

    IFBTag tag = {0};

    strcpy(
        tag.value,
        tag_value);  

    tag.length = strlen(tag_value);

    return(tag);
}

#endif //IFB_TAG_HPP