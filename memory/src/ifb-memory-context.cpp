#include "ifb-memory-internal.hpp"

/**********************************************************************************/
/* GLOBAL CONTEXT                                                                 */
/**********************************************************************************/

ifb_global IFBMemoryContext* _ptr_context;

/**********************************************************************************/
/* CREATE / DESTROY                                                               */
/**********************************************************************************/

const IFBB8
ifb_memory::context_create(
    const IFBByte* stack_memory,
    const IFBU32   stack_size) {

    //calculate the size of the memory struct
    const IFBU32 memory_struct_size = ifb_macro_align_size_struct(IFBMemoryContext);

    // make sure the stack is valid and large enough
    IFBB8 result = true;
    result &= stack_memory != NULL;
    result &= stack_size   >= memory_struct_size;

    //if everything isn't valid, we're done
    if (!result) return(false);

    //cast the stack memory to the memory struct
    _ptr_context = (IFBMemoryContext*)stack_memory;

    //cache the properties
    IFBMemoryContextStack&   ref_stack   = _ptr_context->system_info;
    IFBMemoryContextOffsets& ref_offsets = _ptr_context->stack;

    //calculate stack addresses
    const IFBAddr stack_start = (IFBAddr)stack_memory;
    const IFBAddr stack_end   = stack_start + stack_size; 

    //initialize the stack
    ref_stack.start    = stack_start;
    ref_stack.size     = stack_size;
    ref_stack.position = memory_struct_size;

    //calculate the sizes of the context properties 
    const IFBU32 size_array[] = {
        ifb_macro_align_size_struct(IFBMemoryContextInfo),
        ifb_macro_align_size_struct(IFBReservationList)
    };
    
    //calculate the count of the size array
    const IFBU32 size_array_count = ifb_macro_array_count(IFBU32,size_array);
    
    //cast the offsets as a u32 array
    IFBU32* ptr_offsets_array = (IFBU32*)&ref_offsets;

    for (
        IFBU32 index = 0;
        index < size_array_count;
        ++index) {

        //cache the current properties
        const IFBU32 current_position = ref_stack.position; 
        const IFBU32 current_size     = size_array[index];
        
        //calculate the new position
        const IFBU32 new_position = current_position + current_size;
        
        //this should fit in the stack, and the size should not be 0
        ifb_macro_assert(new_position < stack_size);
        ifb_macro_assert(new_position != current_position);

        //update the offset array
        ptr_offsets_array[index] = current_position;

        //update the stack position
        ref_stack.position += current_size;
    }

    //get the system information
    const IFBU32 system_page_size   = ifb_platform::system_page_size();
    const IFBU32 system_granularity = ifb_platform::system_allocation_granularity();
    ifb_macro_assert(system_page_size)
    ifb_macro_assert(system_granularity)

    //update the context info
    IFBMemoryContextInfo* ptr_context_info = ifb_memory::context_get_local_info();
    ifb_macro_assert(ptr_context_info);
    ptr_context_info->page_size   = system_page_size; 
    ptr_context_info->granularity = system_granularity; 

    //we're done
    return(true);
}

const IFBB8
ifb_memory::context_destroy(
    IFBVoid) {

    //sanity check
    ifb_macro_assert(_ptr_context);
    return(false);
}


/**********************************************************************************/
/* ALIGNMENT                                                                      */
/**********************************************************************************/

const IFBU32 
ifb_memory::context_align_size_to_page(
    const IFBU32 size) {

    //get the page size
    const IFBMemoryContextInfo* ptr_info = ifb_memory::context_get_local_info();
    const IFBU32 page_size = ptr_info->system_page_size;
    ifb_macro_assert(page_size);

    //align the size
    const IFBU32 size_aligned = ifb_macro_align_a_to_b(size,page_size); 

    //we're done
    return(size_aligned);
}

const IFBU32 
ifb_memory::context_align_size_to_granularity(
    const IFBU32 size) {

    //get the granularity
    const IFBMemoryContextInfo* ptr_info = ifb_memory::context_get_local_info();
    const IFBU32 granularity = ptr_info->system_granularity;
    ifb_macro_assert(granularity);

    //align the size
    const IFBU32 size_aligned = ifb_macro_align_a_to_b(size,granularity); 

    //we're done
    return(size_aligned);
}

/**********************************************************************************/
/* SIZE                                                                           */
/**********************************************************************************/

const IFBU64
ifb_memory::context_get_size_from_page_count(
    const IFBU32 page_count) {

    //get the page size
    const IFBMemoryContextInfo* ptr_info = ifb_memory::context_get_local_info();
    const IFBU32 page_size = ptr_info->system_page_size;
    ifb_macro_assert(page_size);

    //calculate the size
    const IFBU64 size = page_count * page_size;

    //we're done
    return(size);
}

const IFBU32
ifb_memory::context_get_page_count_from_size(
    const IFBU64 size) {

    //get the page size
    const IFBMemoryContextInfo* ptr_info = ifb_memory::context_get_local_info();
    const IFBU32 page_size = ptr_info->system_page_size;
    ifb_macro_assert(page_size);

    //calculate the page count
    const IFBU32 page_count = (IFBU32)(size / page_size);
    
    //we're done
    return(page_count);  
}

/**********************************************************************************/
/* STACK                                                                          */
/**********************************************************************************/

const IFBU32
ifb_memory::context_stack_commit_relative(
    const IFBU32 size,
    const IFBU32 alignment) {

    //get the stack
    IFBMemoryContextStack& ref_stack = ifb_memory::context_get_stack();

    //calculate the aligned size
    const IFBU32 size_aligned = ifb_macro_align_a_to_b(size,alignment);

    //calculate the new position
    const IFBU32 offset       = ref_stack.position;
    const IFBU32 position_new = offset + size_aligned;

    //make sure we can fit the commit
    ifb_macro_assert(position_new < ref_stack.size);

    //update the position
    ref_stack.position = position_new;
    
    //we're done
    return(offset);
}

const IFBPtr
ifb_memory::context_stack_commit_absolute(
    const IFBU32 size,
    const IFBU32 alignment) {

    //get the stack
    IFBMemoryContextStack& ref_stack = ifb_memory::context_get_stack();

    //calculate the aligned size
    const IFBU32 size_aligned = ifb_macro_align_a_to_b(size,alignment);

    //calculate the new position
    const IFBU32 offset       = ref_stack.position;
    const IFBU32 position_new = offset + size_aligned;

    //make sure we can fit the commit
    ifb_macro_assert(position_new < ref_stack.size);

    //update the position
    ref_stack.position = position_new;
    
    //calculate the pointer
    const IFBAddress stack_start  = ref_stack.start;
    const IFBAddress stack_offset = stack_start + offset;
    const IFBPtr     pointer      = (IFBPtr)stack_offset;

    //we're done
    return(pointer);
}

const IFBPtr
ifb_memory::context_stack_get_pointer(
    const IFBU32 offset) {

    //get the stack
    IFBMemoryContextStack& ref_stack = ifb_memory::context_get_stack();

    //make sure this offset is within the stack
    ifb_macro_assert(offset < ref_stack.size);
        
    //calculate the pointer
    const IFBAddress stack_start  = ref_stack.start;
    const IFBAddress stack_offset = stack_start + offset;
    const IFBPtr     pointer      = (IFBPtr)stack_offset;

    //we're done
    return(pointer);
}

/**********************************************************************************/
/* RESERVATIONS                                                                   */
/**********************************************************************************/

//reservations
const IFBHNDReservation
ifb_memory::context_reserve_platform_memory(
    const IFBU32 size_minimum) {

    // TODO(SAM):
    // make sure we search the list for released
    // reservations to recycle

    IFBHNDReservation reservation_handle;
    reservation_handle.offset = 0;

    //sanity check
    if (size_minimum == 0) return(reservation_handle);

    //get the granularity and page size 
    const IFBMemoryContextInfo* ptr_info = ifb_memory::context_get_local_info();
    const IFBU32 granularity = ptr_info->system_granularity;
    const IFBU32 page_size   = ptr_info->system_page_size;
    ifb_macro_assert(granularity);
    ifb_macro_assert(page_size);
    
    //align the size
    const IFBU32 size_aligned = ifb_macro_align_a_to_b(size_minimum,granularity);

    //make the reservation
    const IFBPtr ptr_platform_memory = ifb_platform::memory_reserve(size_aligned);
    if (!ptr_reservation) return(reservation_handle);

    //commit reservation structure
    const IFBU32 reservation_struct_size = ifb_macro_align_size_struct(IFBReservation); 
    reservation_handle.offset            = ifb_memory::context_stack_commit_relative(reservation_struct_size);
     
    //get the struct pointer
    IFBReservation* ptr_reservation = ifb_memory::context_stack_get_pointer(reservation_handle.offset);
    ifb_macro_assert(ptr_reservation);

    //calculate page count
    const IFBU32 page_count = size_aligned / page_size;

    //initialize the reservation
    ptr_reservation->next                 = NULL;
    ptr_reservation->start                = (IFBAddr)ptr_platform_memory;
    ptr_reservation->handle               = reservation_handle;
    ptr_reservation->page_count_total     = page_count;
    ptr_reservation->page_count_committed = 0;

    //add reservation to list
    IFBReservationList* ptr_reservation_list = ifb_memory::context_get_reservation_list();
    
    //if this is the first one, we need to initialize the list
    if (ptr_reservation_list->count == 0) {

        //pointers should be null
        ifb_macro_assert(ptr_reservation_list->first == NULL);
        ifb_macro_assert(ptr_reservation_list->last  == NULL);

        //initialize the list
        ptr_reservation_list->first = ptr_reservation_list;
        ptr_reservation_list->last  = ptr_reservation_list;
        ptr_reservation_list->count = 1;
    }
    else {

        //first and last elements should NOT be null
        ifb_macro_assert(ptr_reservation_list->first);
        ifb_macro_assert(ptr_reservation_list->last);
        
        //cache first and last elements
        IFBReservation* first_element = ptr_reservation_list->first; 
        IFBReservation* last_element  = ptr_reservation_list->last; 
        
        //if we are adding a second element, make sure
        //we update the next pointer for the first element
        if (ptr_reservation_list->count == 1) {
            ifb_macro_assert(first_element->next == NULL);
            first_element->next = ptr_reservation;
        }
        
        //update the end of the list
        ifb_macro_assert(first_element->next);
        last_element->next = ptr_reservation;
        ptr_reservation_list->last = ptr_reservation;

        //update the count
        ++ptr_reservation_list->count;                        
    }
    
    //we're done
    return(reservation_handle);
}

const IFBB8
ifb_memory::context_release_platform_memory(
    const IFBHNDReservation reservation_handle) {

    //get the reservation
    IFBReservation* ptr_reservation = ifb_memory::context_get_reservation(reservation_handle);

    //get the granularity and page size 
    const IFBMemoryContextInfo* ptr_info = ifb_memory::context_get_local_info();
    const IFBU32 page_size = ptr_info->system_page_size;
    ifb_macro_assert(page_size);

    //calculate reservation size
    const IFBPtr reservation_start = (IFBPtr)ptr_reservation->start;
    const IFBU64 reservation_size  = ptr_reservation->page_count_total * page_size; 

    //release memory
    const IFBB8 result = ifb_platform::memory_release(
        reservation_start,
        reservation_size);

    //update the reservation
    ptr_reservation->start                = 0;
    ptr_reservation->page_count_total     = 0;
    ptr_reservation->page_count_committed = 0;

    //we're done
    return(result);
}

/**********************************************************************************/
/* INTERNAL                                                                       */
/**********************************************************************************/

IFBMemoryContextStack&
ifb_memory::context_get_stack(
    IFBVoid) {

    ifb_macro_assert(_ptr_context);
    IFBMemoryContextStack& ref_stack = _ptr_context->stack;
    return(ref_stack);
}

IFBMemoryContextOffsets&
ifb_memory::context_get_offsets(
    IFBVoid) {

    ifb_macro_assert(_ptr_context);

    IFBMemoryContextOffsets& ref_offsets = _ptr_context->offsets;
    return(ref_offsets);
}

IFBMemoryContextInfo*
ifb_memory::context_get_local_info(
    IFBVoid) {
    
    //get the offsets
    IFBMemoryContextOffsets& ref_offsets = ifb_memory::context_get_offsets();

    //get the pointer
    IFBMemoryContextInfo* ptr_info = (IFBMemoryContextInfo*)ifb_memory::context_stack_get_pointer(ref_offsets.info);
    ifb_macro_assert(ptr_info);
    
    //we're done
    return(ptr_info);
}

IFBReservationList*
ifb_memory::context_get_reservation_list(
    IFBVoid) {

    //get the offsets
    IFBMemoryContextOffsets& ref_offsets = ifb_memory::context_get_offsets();

    //get the pointer
    IFBReservationList* ptr_list = (IFBReservationList*)ifb_memory::context_stack_get_pointer(ref_offsets.reservation_list);
    ifb_macro_assert(ptr_list);
    
    //we're done
    return(ptr_list);
}

IFBReservation*
ifb_memory::context_get_reservation(
    const IFBHNDReservation reservation_handle) {

    //get the pointer
    IFBReservation* ptr_reservation = (IFBReservation*)ifb_memory::context_stack_get_pointer(reservation_handle.offset);
    ifb_macro_assert(ptr_reservation);

    //we're done
    return(ptr_reservation);
}
