#pragma once

#include "ifb-memory.hpp"

/**********************************************************************************/
/* RESET                                                                          */
/**********************************************************************************/

IFBVoid
ifb_memory::arena_reset_all(
    IFBMemoryArena* ptr_arena) {

    ifb_macro_assert(ptr_arena);
    ptr_arena->position_committed = 0;
    ptr_arena->position_reserved  = 0;
}

IFBVoid
ifb_memory::arena_reset_committed_space(
    IFBMemoryArena* ptr_arena) {

    ifb_macro_assert(ptr_arena);
    ptr_arena->position_committed = 0;
}

IFBVoid
ifb_memory::arena_reset_reserved_space(
    IFBMemoryArena* ptr_arena) {

    ifb_macro_assert(ptr_arena);
    ptr_arena->position_reserved = 0;
}

/**********************************************************************************/
/* POINTERS                                                                       */
/**********************************************************************************/

const IFBPtr
ifb_memory::arena_get_pointer(
    const IFBMemoryArena* ptr_arena,
    const IFBU32          offset) {

    ifb_macro_assert(ptr_arena);

    //calculate the pointer, if its valid
    const IFBAddr address = ptr_arena->start + offset;
    const IFBPtr  pointer = offset < ptr_arena->size
        ? (IFBPtr)address
        : NULL;

    //we're done
    return(pointer);
}

const IFBAddr
ifb_memory::arena_get_start(
    const IFBMemoryArena* ptr_arena) {

    ifb_macro_assert(ptr_arena);
    
    //get the start address
    const IFBAddr start = ptr_arena->start;

    //we're done
    return(start);
}

/**********************************************************************************/
/* RESERVE/RELEASE (BOTTOM -> TOP)                                                */
/**********************************************************************************/

const IFBPtr
ifb_memory::arena_reserve_bytes_absolute(
          IFBMemoryArena* ptr_arena,
    const IFBU32          size,
    const IFBU32          alignment) {

    IFBPtr ptr_result = NULL;
    
    //sanity check
    ifb_macro_assert(ptr_arena);

    //sanity check
    if (size == 0) return(NULL);

    //calculate the new reserved position
    const IFBU32 reservation_size_aligned     = ifb_macro_align_a_to_b(size,alignment);
    const IFBU32 reservation_position_current = ptr_arena->position_reserved;
    const IFBU32 reservation_position_new     = reservation_position_current + reservation_size_aligned;
        
    //calculate the address
    const IFBAddr reservation_address = ptr_arena->start + reservation_position_current;
    
    //this reservation is valid IF...
    IFBB8 is_valid = true;
    is_valid &= (reservation_position_new < ptr_arena->size);               // it doesn't overflow the arena
    is_valid &= (reservation_position_new < ptr_arena->position_committed); // it doesn't overflow into commit space

    //if its valid, update the position and calculate the pointer
    if (is_valid) {
        ptr_arena->position_reserved = reservation_position_new;
        ptr_result = (IFBPtr)reservation_address;
    }

    //we're done
    return(ptr_result);
}

const IFBU32
ifb_memory::arena_reserve_bytes_relative(
          IFBMemoryArena* ptr_arena,
    const IFBU32            size,
    const IFBU32            alignment) {
    
    //sanity check
    ifb_macro_assert(ptr_arena);
    
    //we use the arena size as an invalid offset since using it will return null
    IFBU32 offset_result = ptr_arena->size;
    
    //sanity check
    if (size == 0) return(offset_result);
    
    //calculate the new reserved position
    const IFBU32 reservation_size_aligned     = ifb_macro_align_a_to_b(size,alignment);
    const IFBU32 reservation_position_current = ptr_arena->position_reserved;
    const IFBU32 reservation_position_new     = reservation_position_current + reservation_size_aligned;
    
    //this reservation is valid IF...
    IFBB8 is_valid = true;
    is_valid &= (reservation_position_new < ptr_arena->size);               // it doesn't overflow the arena
    is_valid &= (reservation_position_new < ptr_arena->position_committed); // it doesn't overflow into commit space

    //if its valid, update the position and offset
    //the offset is the position prior to the update
    if (is_valid) {
        ptr_arena->position_reserved = reservation_position_new;
        offset_result = reservation_position_current; 
    }

    //we're done
    return(offset_result);
}

const IFBB8
ifb_memory::arena_release_bytes(
          IFBMemoryArena* ptr_arena,
    const IFBU32            size,
    const IFBU32            alignment) {
    
    //sanity check
    ifb_macro_assert(ptr_arena);
    
    //determine if we can release the bytes
    //we can release if we aren't releasing more than we have reserved
    const IFBU32 release_size_aligned = ifb_macro_align_a_to_b(size,alignment);
    IFBB8        release_is_valid     = release_size_aligned < ptr_arena->position_reserved;

    //if we can release, update the arena
    if (release_is_valid) {
        ptr_arena->position_reserved -= release_size_aligned;
    }

    //we're done
    return(release_is_valid);
}

/**********************************************************************************/
/* COMMIT (BOTTOM <- TOP)                                                         */
/**********************************************************************************/

const IFBPtr 
ifb_memory::arena_commit_bytes_absolute(
          IFBMemoryArena* ptr_arena,
    const IFBU32            size,
    const IFBU32            alignment) {

    IFBPtr ptr_commit_result = NULL;
    
    //sanity check
    ifb_macro_assert(ptr_arena);

    //determine if we can commit
    //we can commit if the commit size is smaller than the difference
    //between the committed and reserved space
    const IFBU32 commit_size_aligned      = ifb_macro_align_a_to_b(size,alignment);
    const IFBU32 commit_position_current  = ptr_arena->position_committed;
    const IFBU32 reserve_position_current = ptr_arena->position_reserved;
    const IFBU32 position_difference      = commit_position_current - reserve_position_current; 
    const IFBU32 commit_is_valid          = commit_size_aligned < position_difference;

    //if we can do the commit, 
    //update the position and pointer
    if (commit_is_valid) {

        //calculate the address
        const IFBU32  commit_position_new = commit_position_current - commit_size_aligned; 
        const IFBAddr commit_address      = commit_position_new     + ptr_arena->start; 
        ptr_commit_result = (IFBPtr)commit_address;

        //update the arena
        ptr_arena->position_committed = commit_position_new;
    }

    //we're done
    return(ptr_commit_result);
}

const IFBU32 
ifb_memory::arena_commit_bytes_relative(
          IFBMemoryArena* ptr_arena,
    const IFBU32            size,
    const IFBU32            alignment) {
    
    //sanity check
    ifb_macro_assert(ptr_arena);
    
    //we use the arena size as an invalid offset since using it will return null
    IFBU32 offset_result = ptr_arena->size;

    //determine if we can commit
    //we can commit if the commit size is smaller than the difference
    //between the committed and reserved space
    const IFBU32 commit_size_aligned      = ifb_macro_align_a_to_b(size,alignment);
    const IFBU32 commit_position_current  = ptr_arena->position_committed;
    const IFBU32 reserve_position_current = ptr_arena->position_reserved;
    const IFBU32 position_difference      = commit_position_current - reserve_position_current; 
    const IFBU32 commit_is_valid          = commit_size_aligned < position_difference;

    //if we can do the commit, 
    //update the position and pointer
    if (commit_is_valid) {

        //calculate the offset
        offset_result = commit_position_current - commit_size_aligned; 

        //update the arena
        ptr_arena->position_committed = offset_result;
    }

    //we're done
    return(offset_result);
}

const IFBChar*
ifb_memory::arena_commit_string(
          IFBMemoryArena* ptr_arena,
    const IFBChar*          c_string,
    const IFBU32            max_length) {

    //get the actual length of the string
    const IFBU32 length_actual = strnlen_s(
        c_string,
        max_length);

    //commit the bytes
    //add one to account for a null terminator
    IFBChar* result_c_str = (IFBChar*)ifb_memory::arena_commit_bytes_absolute(
        ptr_arena,
        length_actual + 1);

    //copy the string
    for (
        IFBU32 char_index = 0;
        char_index < length_actual;
        ++char_index) {

        result_c_str[char_index] = c_string[char_index];
    }

    //terminate the string
    result_c_str[length_actual] = '\0';

    //we're done
    return(result_c_str);
}