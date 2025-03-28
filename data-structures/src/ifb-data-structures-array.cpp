#pragma once

#include "ifb-data-structures.hpp"
#include "ifb-data-structures-internal.cpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

struct IFBArray : IFBDataStructure {
    IFBU32 element_size;
    IFBU32 element_count_total;
    IFBU32 element_count_current;
};

namespace ifb_array {

    IFBArray* cast_and_assert_valid(const IFBDS64Array array_handle);
};

#define IFB_ARRAY_STRUCT_SIZE                    ifb_macro_align_size_struct(IFBArray)

#define ifb_array_macro_size_data(stride,count)  (stride * count)
#define ifb_array_macro_size_total(stride,count) IFB_ARRAY_STRUCT_SIZE + ifb_array_macro_size_data(stride,count) 

/**********************************************************************************/
/* MEMORY                                                                       */
/**********************************************************************************/

IFBVoid
ifb_array::size(
    IFBArrayArgs& args) {

    //cache args
    const IFBU32 element_size  = args.element_size;
    const IFBU32 element_count = args.element_count;

    //calculate the memory size
    args.memory_size = ifb_array_macro_size_total(args.element_size,args.element_count);
}

const IFBDS64Array
ifb_array::create(
    IFBArrayArgs& args) {

    //sanity check
    ifb_macro_assert(args.memory_ptr    != NULL);
    ifb_macro_assert(args.memory_size   != 0);
    ifb_macro_assert(args.element_size  != 0);
    ifb_macro_assert(args.element_count != 0);

    //get the addresses
    const IFBAddr array_start_struct = (IFBAddr)args.memory_ptr;
    const IFBAddr array_start_data   = array_start_struct + IFB_ARRAY_STRUCT_SIZE;

    //cast the pointer
    IFBArray* array_ptr = (IFBArray*)args.memory_ptr;
    
    //initialize the array
    array_ptr->start                 = array_start_data;
    array_ptr->size                  = ifb_array_macro_size_data(args.element_size, args.element_count);
    array_ptr->element_size          = args.element_size;
    array_ptr->element_count_total   = args.element_count;
    array_ptr->element_count_current = 0;

    //we're done, get the handle and return it
    IFBDS64Array array_handle;
    array_handle.h64 = array_start_struct;
    return(array_handle);
}

/**********************************************************************************/
/* INFO                                                                           */
/**********************************************************************************/

IFBVoid
ifb_array::info(
    const IFBDS64Array  array_handle,
          IFBArrayInfo& info) {

    //get the array
    IFBArray* array = ifb_array::cast_and_assert_valid(array_handle);

    //set the info
    info.element_size          = array->element_size;
    info.element_count_total   = array->element_count_total;
    info.element_count_current = array->element_count_current;
}

/**********************************************************************************/
/* OPERATIONS                                                                     */
/**********************************************************************************/

IFBVoid
ifb_array::clear(
    const IFBDS64Array array_handle) {

    //get the array
    IFBArray* array = ifb_array::cast_and_assert_valid(array_handle);
    array->element_count_current = 0;
}

const IFBB8
ifb_array::add(
    const IFBDS64Array array_handle,
    const IFBU32       count,
    const IFBPtr       element) {

    //sanity check
    IFBB8 result = true;
    result &= (count   >  0);
    result &= (element != NULL);
    if (!result) return(false);

    //get the array
    IFBArray* array = ifb_array::cast_and_assert_valid(array_handle);

    //make sure we can fit the element(s)
    const IFBU32 array_count_total   = array->element_count_total;
    const IFBU32 array_count_current = array->element_count_current;
    const IFBU32 array_count_new     = array_count_current + count;
    if (array_count_new > array_count_total) return(false);

    //calculate the end of the current array data
    const IFBU32  array_element_size = array->element_size;
    const IFBU32  array_size_current = array_element_size * array_count_current;
    const IFBAddr array_end          = array->start + array_size_current;
    
    //calculate the element source and destination
    IFBByte*       element_destination = (IFBByte*)array_end;
    const IFBU32   element_source_size = array_element_size * count;
    const IFBByte* element_source      = (const IFBByte*)element;

    //copy the data
    for (
        IFBU32 byte_index = 0;
               byte_index < element_source_size;
             ++byte_index) {

        element_destination[byte_index] = element_source[byte_index];        
    }

    //update the count and do a final sanity check
    array->element_count_current = array_count_new;
    ifb_macro_assert(array->element_count_current < array_count_total);

    //we're done
    return(true);
}

const IFBPtr
ifb_array::index(
    const IFBDS64Array array_handle,
    const IFBU32       index) {

    //get the array
    IFBArray* array = ifb_array::cast_and_assert_valid(array_handle);

    //check the index is valid
    if (index >= array->element_count_current) return(NULL);

    //calculate the element pointer
    const IFBU32  element_size    = array->element_size;
    const IFBU32  element_offset  = element_size * index; 
    const IFBAddr element_address = array->start + element_offset;
    const IFBPtr  element_pointer = (IFBPtr)element_address;

    //we're done
    return(element_pointer);
}

const IFBB8
ifb_array::iterate(
    const IFBDS64Array array_handle,
          IFBIterator& iterator) {

    //get the array
    IFBArray* array = ifb_array::cast_and_assert_valid(array_handle);

    //check the iterator
    const IFBU32 array_count_current = array->element_count_current;
    if (iterator.index >= array_count_current) {
        
        //if the iterator is greater than the current count, 
        //just set it to the current count and return false
        iterator.index = array_count_current;
        return(false);
    } 

    //get the pointer at the index
    iterator.pointer = ifb_array::index(array_handle,iterator.index);
    
    //update the index
    ++iterator.index;

    //we're done
    return(true);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

inline IFBArray*
ifb_array::cast_and_assert_valid(
    const IFBDS64Array array_handle) {

    //cast the handle
    IFBArray* array = (IFBArray*)array_handle.h64;

    //assert the properties are valid
    IFBB8 result = true;
    result &= (array                        != NULL);
    result &= (array->size                  != NULL);
    result &= (array->start                 != NULL);
    result &= (array->element_size          != NULL);
    result &= (array->element_count_total   != NULL);
    result &= (array->element_count_current <= array->element_count_total);
    ifb_macro_assert(result);

    //we're done
    return(array);
}