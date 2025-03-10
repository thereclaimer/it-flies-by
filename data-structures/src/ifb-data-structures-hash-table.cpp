#pragma once

#include "ifb-data-structures.hpp" 

namespace ifb_hash_table {

    inline IFBHash* get_hash_array(const IFBHashTable* ptr_hash_table);
};

/**********************************************************************************/
/* MEMORY                                                                         */
/**********************************************************************************/

const IFBU32
ifb_hash_table::memory_size(
    const IFBU32 element_count,
    const IFBU32 element_size) {

    //sanity check
    if (element_count == 0 || element_size == 0) return(0);

    //calculate sizes
    const IFBU32 size_array_hash    = ifb_macro_array_size(IFBHash, element_count);
    const IFBU32 size_array_element = element_count * element_size;
    const IFBU32 size_table         = ifb_macro_align_size_struct(IFBHashTable);
    const IFBU32 size_total         = size_array_hash + size_array_element + size_table;

    //we're done
    return(size_total);
}

const IFBU32
ifb_hash_table::commit_to_arena_relative(
          IFBMemoryArena* ptr_arena,
    const IFBU32          element_count,
    const IFBU32          element_size,
    const IFBU32          key_length_max) {

    //sanity check
    if (element_count == 0 || element_size == 0) return(0);

    //calculate sizes
    const IFBU32 size_table         = ifb_macro_align_size_struct(IFBHashTable);
    const IFBU32 size_array_hash    = ifb_macro_array_size(IFBHash, element_count);
    const IFBU32 size_array_element = element_count * element_size;
    const IFBU32 size_data          = size_array_hash + size_array_element; 
    const IFBU32 size_total         = size_data       + size_table;

    //do the commit, if it fails we're done
    const IFBU32 offset = ifb_memory::arena_commit_bytes_relative(ptr_arena, size_total);  

    //get the pointer, we should ALWAYS be able
    //to get a pointer to a commit we just made
    IFBHashTable* ptr_hash_table = (IFBHashTable*)ifb_memory::arena_get_pointer(ptr_arena,offset);
    ifb_macro_assert(ptr_hash_table);

    //calculate starting addresses
    const IFBAddr start_address_data     = ((IFBAddr)ptr_hash_table) + size_table;
    const IFBAddr start_address_elements = start_address_data + size_array_hash; 

    //initialize the table
    ptr_hash_table->data_start          = start_address_data;
    ptr_hash_table->data_size           = size_data;
    ptr_hash_table->element_array_start = start_address_elements;
    ptr_hash_table->element_size        = element_size;
    ptr_hash_table->element_count_max   = element_count;
    ptr_hash_table->key_length_max      = key_length_max;

    //we're done
    return(offset);
}

IFBHashTable*
ifb_hash_table::commit_to_arena_absolute(
          IFBMemoryArena* ptr_arena,
    const IFBU32          element_count,
    const IFBU32          element_size,
    const IFBU32          key_length_max) {

    //sanity check
    if (element_count == 0 || element_size == 0) return(0);

    //calculate sizes
    const IFBU32 size_table         = ifb_macro_align_size_struct(IFBHashTable);
    const IFBU32 size_array_hash    = ifb_macro_array_size(IFBHash, element_count);
    const IFBU32 size_array_element = element_count * element_size;
    const IFBU32 size_data          = size_array_hash + size_array_element; 
    const IFBU32 size_total         = size_data       + size_table;

    //do the commit, if it fails we're done
    IFBHashTable* ptr_hash_table = (IFBHashTable*)ifb_memory::arena_commit_bytes_absolute(ptr_arena, size_total);  
    if (!ptr_hash_table) return(NULL);

    //calculate starting addresses
    const IFBAddr start_address_data     = ((IFBAddr)ptr_hash_table) + size_table;
    const IFBAddr start_address_elements = start_address_data + size_array_hash; 

    //initialize the table
    ptr_hash_table->data_start          = start_address_data;
    ptr_hash_table->data_size           = size_data;
    ptr_hash_table->element_array_start = start_address_elements;
    ptr_hash_table->element_size        = element_size;
    ptr_hash_table->element_count_max   = element_count;
    ptr_hash_table->key_length_max      = key_length_max;

    //we're done
    return(ptr_hash_table);
}

IFBHashTable*
ifb_hash_table::load_from_arena(
          IFBMemoryArena* ptr_arena,
    const IFBU32          offset) {

    IFBHashTable* ptr_hash_table = (IFBHashTable*)ifb_memory::arena_get_pointer(ptr_arena,offset);
    return(ptr_hash_table);
}

/**********************************************************************************/
/* OPERATIONS                                                                     */
/**********************************************************************************/

const IFBB8
ifb_hash_table::insert(
          IFBHashTable* ptr_hash_table,
    const IFBChar*      key,
    const IFBByte*      element,
          IFBU32*       ptr_element_index) {

    //sanity check
    ifb_macro_assert(ptr_hash_table);
    IFBB8 result = true;
    result &= (key     != NULL);
    result &= (element != NULL);
    if (!result) return(false);

    //hash the key
    const IFBU32  key_lengh_max = ptr_hash_table->key_length_max;
    const IFBHash key_hash      = ifb_hash::get_hash(key,key_lengh_max);

    //if the hash doesn't have a value, we're done
    result &= !ifb_hash::hash_is_clear(key_hash);
    if (!result) return(false);

    //get the hash array info
    IFBU32 hash_index = 0;
    const IFBU32 hash_array_size = ptr_hash_table->element_count_max;
    IFBHash*     hash_array_ptr  = ifb_hash_table::get_hash_array(ptr_hash_table);
    
    //TODO(SAM): we can probably consolidate these loops

    //search for a clear value
    result &= ifb_hash::find_next_clear_value(
        hash_array_ptr,
        hash_array_size,
        hash_index);

    //if we don't have one, we're done
    return(false);

    //check for collisions
    const IFBB8 collision = ifb_hash::collision_check(
        hash_array_ptr,
        hash_array_size,  
        key_hash);

    //this should NEVER happen
    ifb_macro_assert(!collision);

    //set the hash at the index
    hash_array_ptr[hash_index] = key_hash;

    //get the element destination
    const IFBU32  element_size   = ptr_hash_table->element_size;
    const IFBU32  element_offset = element_size * hash_index;
    const IFBAddr element_start  = ptr_hash_table->element_array_start + element_offset;
    IFBByte* element_destination = (IFBByte*)element_start; 

    //copy the element data
    for (
        IFBU32 byte_index = 0;
               byte_index < element_size;
             ++byte_index) {
    
        element_destination[byte_index] = element[byte_index];
    }

    //set the hash index if the pointer is valid
    if (ptr_element_index) {
        *ptr_element_index = hash_index;
    }

    //we're done
    return(result);
}

const IFBB8
ifb_hash_table::remove(
    const IFBHashTable* ptr_hash_table,
    const IFBChar*      key) {

    //sanity check
    ifb_macro_assert(ptr_hash_table);
    IFBB8 result = (key != NULL);
    if (!result) return(NULL);
    
    //cache hash array properties
    IFBHash*       hash_array          = ifb_hash_table::get_hash_array(ptr_hash_table);
    const IFBU32   hash_array_count     = ptr_hash_table->element_count_max;
    const IFBU32   hash_key_length_max = ptr_hash_table->key_length_max;

    //hash the key
    const IFBHash hash_key = ifb_hash::get_hash(key,hash_key_length_max);

    //if the hash doesn't have a value, we're done
    result &= !ifb_hash::hash_is_clear(hash_key);
    if (!result) return(NULL);

    //search for the key
    IFBU32 hash_index = 0;
    result &= ifb_hash::search(
        hash_array,
        hash_array_count,
        hash_key,
        hash_index);

    //if we don't have a match, we're done
    if (!result) return(NULL);

    //clear the hash
    IFBHash& hash_to_clear_ref = hash_array[hash_index];
    hash_to_clear_ref.h1 = 0;    
    hash_to_clear_ref.h2 = 0;    
    hash_to_clear_ref.h3 = 0;    
    hash_to_clear_ref.h4 = 0;    

    //we're done
    return(true);
}

const IFBByte*
ifb_hash_table::lookup(
    const IFBHashTable* ptr_hash_table,
    const IFBChar*      key) {

    //sanity check
    ifb_macro_assert(ptr_hash_table);
    IFBB8 result = (key != NULL);
    if (!result) return(NULL);
    
    //cache hash array properties
    const IFBHash* hash_array          = ifb_hash_table::get_hash_array(ptr_hash_table);
    const IFBU32   hash_array_count     = ptr_hash_table->element_count_max;
    const IFBU32   hash_key_length_max = ptr_hash_table->key_length_max;

    //hash the key
    const IFBHash hash_key = ifb_hash::get_hash(key,hash_key_length_max);

    //if the hash doesn't have a value, we're done
    result &= !ifb_hash::hash_is_clear(hash_key);
    if (!result) return(NULL);

    //search for the key
    IFBU32 hash_index = 0;
    result &= ifb_hash::search(
        hash_array,
        hash_array_count,
        hash_key,
        hash_index);

    //if we don't have a match, we're done
    if (!result) return(NULL);

    //get the element at the index
    const IFBU32   element_size   = ptr_hash_table->element_size;
    const IFBU32   element_offset = hash_index * element_size;  
    const IFBAddr  element_start  = ptr_hash_table->element_array_start + element_offset;
    const IFBByte* element_data   = (IFBByte*)element_start;

    //we're done
    return(element_data); 
}

/**********************************************************************************/
/* INDEXING                                                                       */
/**********************************************************************************/

const IFBU32
ifb_hash_table::index_of(
    const IFBHashTable* ptr_hash_table,
    const IFBChar*      key) {

    IFBB8 result = true;

    //sanity check
    ifb_macro_assert(ptr_hash_table);
    
    //cache hash array properties
    const IFBHash* hash_array          = ifb_hash_table::get_hash_array(ptr_hash_table);
    const IFBU32   hash_array_count    = ptr_hash_table->element_count_max;
    const IFBU32   hash_key_length_max = ptr_hash_table->key_length_max;
    
    //the invalid index is the array count, since using that will return NULL
    const IFBU32 hash_index_invalid = hash_array_count;

    //hash the key
    const IFBHash hash_key = ifb_hash::get_hash(key,hash_key_length_max);

    //if the hash doesn't have a value, we're done
    result &= !ifb_hash::hash_is_clear(hash_key);
    if (!result) return(hash_index_invalid);

    //search for the key
    IFBU32 hash_index = 0;
    result &= ifb_hash::search(
        hash_array,
        hash_array_count,
        hash_key,
        hash_index);

    //if we have a valid result, set the index
    //if not, return the invalid index
    const IFBU32 hash_index_result = result 
        ? hash_index
        : hash_index_invalid;

    //we're done
    return(hash_index_result);
}

const IFBByte*
ifb_hash_table::get_element_at_index(
    const IFBHashTable* ptr_hash_table,
    const IFBU32        index) {

    //sanity check
    ifb_macro_assert(ptr_hash_table);

    //table properties
    const IFBU32  element_count      = ptr_hash_table->element_count_max;
    const IFBU32  element_size       = ptr_hash_table->element_size;
    const IFBAddr element_data_start = ptr_hash_table->element_array_start;

    //if the index is invalid, we're done
    if (index >= element_count) return(NULL);
 
    //calculate the pointer
    const IFBU32   element_offset  = index * element_size;
    const IFBAddr  element_address = element_data_start + element_offset;
    const IFBByte* element_pointer = (IFBByte*)element_address;

    //we're done
    return(element_pointer);
}

/**********************************************************************************/
/* COUNT                                                                          */
/**********************************************************************************/

const IFBU32
ifb_hash_table::get_element_count_total(
    const IFBHashTable* ptr_hash_table) {

    //sanity check
    ifb_macro_assert(ptr_hash_table);

    //get the count
    const IFBU32 count_total = ptr_hash_table->element_count_max;

    //we're done
    return(count_total);
}

const IFBU32
ifb_hash_table::get_element_count_free(
    const IFBHashTable* ptr_hash_table) {

    //sanity check
    ifb_macro_assert(ptr_hash_table);

    //hash array properties
    const IFBU32   hash_count = ptr_hash_table->element_count_max;
    const IFBHash* hash_array = ifb_hash_table::get_hash_array(ptr_hash_table); 

    //get the count free
    IFBU32 count_free = 0;
    for (
        IFBU32 hash_index = 0;
               hash_index < hash_count;
             ++hash_index) {

        //update the count if the hash is clear
        const IFBHash& hash_ref = hash_array[hash_index];
        if (ifb_hash::hash_is_clear(hash_ref)) {
            ++count_free;
        }
    }

    return(count_free);
}

const IFBU32
ifb_hash_table::get_element_count_used(
    const IFBHashTable* ptr_hash_table) {

    //sanity check
    ifb_macro_assert(ptr_hash_table);

    //hash array properties
    const IFBU32   hash_count = ptr_hash_table->element_count_max;
    const IFBHash* hash_array = ifb_hash_table::get_hash_array(ptr_hash_table); 

    //get the count used
    IFBU32 count_used = 0;
    for (
        IFBU32 hash_index = 0;
               hash_index < hash_count;
             ++hash_index) {

        //update the count if the hash is not clear
        const IFBHash& hash_ref = hash_array[hash_index];
        if (!ifb_hash::hash_is_clear(hash_ref)) {
            ++count_used;
        }
    }

    return(count_used);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

inline IFBHash*
ifb_hash_table::get_hash_array(
    const IFBHashTable* ptr_hash_table) {

    IFBHash* hash_array = (IFBHash*)ptr_hash_table->data_start;
    ifb_macro_assert(hash_array);
    return(hash_array);
}
