#ifndef IFB_DATA_STRUCTURES_HPP
#define IFB_DATA_STRUCTURES_HPP

#include <ifb.hpp>
#include <ifb-memory.hpp>

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

struct IFBArray;
struct IFBArrayList;
struct IFBStack;
struct IFBQueue;
struct IFBHashTable;

/**********************************************************************************/
/* ARRAY                                                                          */
/**********************************************************************************/

struct IFBArray : IFBMemoryBlock {
    IFBU32 element_size;
    IFBU32 element_count;
};

namespace ifb_array {

    //allocate
    const IFBU32 allocation_size_structure (IFBVoid);
    const IFBU32 allocation_size_data      (const IFBU32 element_size,const IFBU32 element_count);
    const IFBU32 allocation_size_total     (const IFBU32 element_size,const IFBU32 element_count);
    
    //create/initialize
    IFBArray*
    create(
        const IFBPtr memory,
        const IFBU32 element_size,
        const IFBU32 element_count);

    const IFBB8
    initialize(
              IFBArray* array_ptr,
        const IFBPtr    data,
        const IFBU32    element_size,
        const IFBU32    element_count);

    //pointers
    const IFBPtr get_pointer_indexed (const IFBArray* array_ptr, const IFBU32 index);
    const IFBPtr get_pointer_start   (const IFBArray* array_ptr);
    
    //size / count
    const IFBU32 get_element_size    (const IFBArray* array_ptr);
    const IFBU32 get_element_count   (const IFBArray* array_ptr);
    const IFBU32 get_array_size      (const IFBArray* array_ptr);
};


/**********************************************************************************/
/* ARRAY LIST                                                                      */
/**********************************************************************************/

struct IFBArrayList : IFBMemoryBlock {
    IFBU64 element_size;
    IFBU32 element_count_total;
    IFBU32 element_count_current;
};

namespace ifb_array_list {

    //memory
    const IFBU32   memory_allocation_size  (const IFBU32 element_size, const IFBU32 element_count);
    IFBArrayList*  memory_initialize       (const IFBU32 element_size, const IFBU32 element_count, const IFBPtr memory);   

    //operations
    const IFBB8 reset                 (IFBArrayList* array_list_ptr);
    const IFBB8 remove                (IFBArrayList* array_list_ptr, const IFBU32 index);
    const IFBB8 add_to_front          (IFBArrayList* array_list_ptr, const IFBPtr element_ptr);
    const IFBB8 add_to_end            (IFBArrayList* array_list_ptr, const IFBPtr element_ptr);
    const IFBB8 insert                (IFBArrayList* array_list_ptr, const IFBPtr element_ptr, const IFBU32 index);
    const IFBB8 is_empty              (IFBArrayList* array_list_ptr);

    //size
    const IFBU32 get_size_total       (const IFBArrayList* array_list_ptr);
    const IFBU32 get_size_used        (const IFBArrayList* array_list_ptr);
    
    //pointers
    const IFBPtr get_element_first    (const IFBArrayList* array_list_ptr);
    const IFBPtr get_element_last     (const IFBArrayList* array_list_ptr);
    const IFBPtr get_element_at_index (const IFBArrayList* array_list_ptr, const IFBU32 index);
};
/**********************************************************************************/
/* STACK                                                                          */
/**********************************************************************************/

struct IFBStack : IFBMemoryBlock {
    IFBU64 position;
};

namespace ifb_stack {

    IFBStack*    arena_load_pointer    (IFBMemoryArena* ptr_arena, const IFBU32 arena_offset);
    IFBStack*    arena_commit_absolute (IFBMemoryArena* ptr_arena, const IFBU32 stack_size);
    const IFBU32 arena_commit_relative (IFBMemoryArena* ptr_arena, const IFBU32 stack_size);

    //operations
    const IFBU32 push_relative         (IFBStack* stack, const IFBU32 size);
    const IFBPtr push_absolute         (IFBStack* stack, const IFBU32 size);
    const IFBB8  pull                  (IFBStack* stack, const IFBU32 size);
    const IFBB8  reset                 (IFBStack* stack);
    
    //pointers
    const IFBPtr get_pointer           (const IFBStack* stack, const IFBU32 position);
    
    //size
    const IFBU32 get_size_total        (const IFBStack* stack);
    const IFBU32 get_size_free         (const IFBStack* stack);
    const IFBU32 get_size_used         (const IFBStack* stack);
};

/**********************************************************************************/
/* QUEUE                                                                          */
/**********************************************************************************/

struct IFBQueue : IFBMemoryBlock {
    IFBU32 position;
};

namespace ifb_queue {

    const IFBB8  reset          (const IFBQueue* queue_ptr);
    
    const IFBU32 peek           (const IFBQueue* queue_ptr, const IFBU32 size);
    const IFBPtr enqueue        (IFBQueue*       queue_ptr, const IFBU32 size);
    const IFBPtr dequeue        (IFBQueue*       queue_ptr, const IFBU32 size);
    
    const IFBU32 get_size_total (const IFBQueue* queue_ptr);
    const IFBU32 get_size_free  (const IFBQueue* queue_ptr);
    const IFBU32 get_size_used  (const IFBQueue* queue_ptr);
};

/**********************************************************************************/
/* LINKED LIST                                                                    */
/**********************************************************************************/

namespace ifb_linked_list {


};

/**********************************************************************************/
/* HASH TABLE                                                                     */
/**********************************************************************************/

struct IFBHashTable : IFBMemoryBlock {
    IFBU32 element_array_start;
    IFBU32 element_size;
    IFBU32 element_count_max;
    IFBU32 key_length_max;
};

namespace ifb_hash_table {

    //memory
    const IFBU32   memory_size              (const IFBU32 element_count,const IFBU32 element_size);
    const IFBU32   commit_to_arena_relative (IFBMemoryArena* ptr_arena, const IFBU32 element_count, const IFBU32 element_size, const IFBU32 key_length_max);
    IFBHashTable*  commit_to_arena_absolute (IFBMemoryArena* ptr_arena, const IFBU32 element_count, const IFBU32 element_size, const IFBU32 key_length_max);
    IFBHashTable*  load_from_arena          (IFBMemoryArena* ptr_arena, const IFBU32 offset);

    //operations
    const IFBB8    insert                   (IFBHashTable*       ptr_hash_table, const IFBChar* key, const IFBByte* element, IFBU32* ptr_element_index = NULL);
    const IFBB8    remove                   (const IFBHashTable* ptr_hash_table, const IFBChar* key);
    const IFBByte* lookup                   (const IFBHashTable* ptr_hash_table, const IFBChar* key);

    //indexing
    const IFBU32   index_of                 (const IFBHashTable* ptr_hash_table, const IFBChar* key);
    const IFBByte* get_element_at_index     (const IFBHashTable* ptr_hash_table, const IFBU32   index);

    //count
    const IFBU32   get_element_count_total  (const IFBHashTable* ptr_hash_table);
    const IFBU32   get_element_count_free   (const IFBHashTable* ptr_hash_table);
    const IFBU32   get_element_count_used   (const IFBHashTable* ptr_hash_table);
};

#endif //IFB_DATA_STRUCTURES_HPP