#pragma once

#include "ifb-graphics.hpp"

const IFBB8
ifb_graphics::monitor_table_initialize(
          IFBMonitorTable* monitor_table_ptr,
    const IFBHNDArena      arena_handle) {

    //sanity check
    ifb_macro_assert(monitor_table_ptr);
    
    //get the monitor count
    const IFBU32 monitor_count = ifb_platform::monitor_count();
    
    //this should NEVER happen
    ifb_macro_assert(monitor_count > 0);
    
    //commit monitor array
    const IFBU32 monitor_array_size = ifb_macro_array_size(IFBMonitor, monitor_count);
    IFBMonitor*  monitor_array_ptr  = (IFBMonitor*)ifb_memory::arena_commit_bytes_absolute(arena_handle, monitor_array_size);

    //sanity check
    IFBB8 result = (monitor_array_ptr != NULL);

    //set the table values
    monitor_table_ptr->monitor_primary = 0; //index 0 until otherwise
    monitor_table_ptr->monitor_count   = monitor_count;
    monitor_table_ptr->monitor_array   = monitor_array_ptr; 

    //while we're at it, update the table
    result &= ifb_graphics::monitor_table_update(monitor_table_ptr);

    //we're done
    return(result);
}


const IFBB8
ifb_graphics::monitor_table_update(
    IFBMonitorTable* monitor_table_ptr) {
        
    //sanity check
    ifb_macro_assert(monitor_table_ptr);
    ifb_macro_assert(monitor_table_ptr->monitor_count);
    ifb_macro_assert(monitor_table_ptr->monitor_array);

    //get the monitor info from the platform
    const IFBB8 result = ifb_platform::monitor_info(
        monitor_table_ptr->monitor_count,
        monitor_table_ptr->monitor_array);

    //we're done
    return(result);
}

const IFBB8
ifb_graphics::monitor_table_get_monitor(
    const IFBMonitorTable* monitor_table_ptr,
          IFBMonitor*      monitor_ptr) {

    //sanity check
    ifb_macro_assert(monitor_table_ptr);

    //we can proceed if our monitor pointer is valid, and the index 
    //is within the table
    IFBB8 result = true;
    result &= (monitor_ptr != NULL);
    result &= (monitor_ptr->index < monitor_table_ptr->monitor_count);
    if (!result) return(false);

    //get the monitor info
    const IFBU32 monitor_index = monitor_ptr->index;
    *monitor_ptr = monitor_table_ptr->monitor_array[monitor_index];

    //this is valid if the dimensions and
    //refresh rate are non-zero
    result &= (monitor_ptr->refresh_hz        != 0);
    result &= (monitor_ptr->dimensions.width  != 0);
    result &= (monitor_ptr->dimensions.height != 0);

    //we're done
    return(false);
}

const IFBB8
ifb_graphics::monitor_table_get_monitor_primary(
    const IFBMonitorTable* monitor_table_ptr,
    IFBMonitor*      monitor_ptr) {
        
    //sanity check
    ifb_macro_assert(monitor_table_ptr);
    
    return(false);
}
