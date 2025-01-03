#pragma once

#include "ifb-win32.hpp"


ifb_internal const ifb_ptr 
ifb_win32::memory_reserve(
    const ifb_u32 reservation_size) {

    const ifb_ptr reservation_start = VirtualAlloc(
            NULL,
            reservation_size,
            MEM_RESERVE,
            PAGE_NOACCESS);

    return(reservation_start);
}

ifb_internal const ifb_b8 
ifb_win32::memory_release(
    const ifb_ptr reservation_start, 
    const ifb_u32 reservation_size) {

    const ifb_b8 result = VirtualFree(
        reservation_start,
        reservation_size,
        MEM_RELEASE);

    return(result);
}

ifb_internal const ifb_ptr 
ifb_win32::memory_commit(
    const ifb_ptr commit_start,
    const ifb_u32 commit_size) {

    //do the commit
    const ifb_ptr commit_result = VirtualAlloc(
        commit_start,
        commit_size,
        MEM_COMMIT,
        PAGE_READWRITE);

    //we're done
    return(commit_result);
}