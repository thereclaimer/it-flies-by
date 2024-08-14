#pragma once

#include "ifb-win32.hpp"

global u64 bytes_read = 0;

void CALLBACK
ifb_win32_api_file_io_completion_routine(
    DWORD error_code,
    DWORD bytes_transferred,
    LPOVERLAPPED lpOverlapped) {
        
    bytes_read = bytes_transferred;
}


internal u64
ifb_win32_api_file_get_file_size_bytes(
    HANDLE file_handle,
    b8     terminate) {

    //get the initial file size
    u64 file_size = 
        (file_handle == INVALID_HANDLE_VALUE)
        ? 0
        : GetFileSize(file_handle, NULL);

    //if we dont care about termination, we're done
    if (!terminate) {
        return(file_size);
    }

    //if we do care, get the last byte
    OVERLAPPED overlapped = {0};

    overlapped.Offset = file_size - 1;
    u8 last_byte;
    bool read_result = 
        ReadFileEx(
            file_handle,
            &last_byte,
            1,
            &overlapped,
            ifb_win32_api_file_io_completion_routine
        );

    //if its not terminated, account for a null terminator
    file_size += last_byte == '\0' ? 0 : 1;

    return(file_size);
}

internal HANDLE
ifb_win32_api_open_file(
    const char* file_path,
          b8    create_new) {

    HANDLE file_handle = 
        CreateFileA(
            file_path,
            (GENERIC_READ | GENERIC_WRITE),
            (FILE_SHARE_READ | FILE_SHARE_WRITE),
            NULL,
            create_new ? CREATE_ALWAYS : OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
    );

    return (file_handle == INVALID_HANDLE_VALUE)
        ? NULL
        : file_handle;
}

internal void
ifb_win32_api_close_file(
    HANDLE file_handle) {

    CloseHandle(file_handle);
}


internal void
ifb_win32_api_read_file(
    HANDLE file_handle,
    u64    offset,
    u64    allocated_buffer_size,
    memory allocated_buffer,
    b8     terminate) {

    ifb_assert(file_handle);
    ifb_assert(allocated_buffer);

    OVERLAPPED overlapped = {0};
    overlapped.Offset = offset;

    u64 adjusted_size   = terminate ? allocated_buffer_size - 1 : allocated_buffer_size;
    u64 last_byte_index = adjusted_size - 1;

    bool read_result = 
        ReadFileEx(
            file_handle,
            allocated_buffer,
            adjusted_size,
            &overlapped,
            ifb_win32_api_file_io_completion_routine
    );

    DWORD error = GetLastError();
    
    if (!terminate || allocated_buffer[last_byte_index] == '\0') {
        return;
    }

    allocated_buffer[last_byte_index] = '\0';
}

internal void
ifb_win32_api_write_file(
    handle file_handle,
    u64    allocated_buffer_size,
    u64    offset,
    memory allocated_buffer) {

    OVERLAPPED overlapped = {0};

    overlapped.Offset = offset;

    bool read_result = WriteFileEx(
        file_handle,
        allocated_buffer,
        allocated_buffer_size,
        &overlapped,
        ifb_win32_api_file_io_completion_routine
    );
}

internal memory
ifb_win32_api_memory_reserve_small_pages(
    u64 reservation_size) {

    memory mem =
        (memory)VirtualAlloc(
            0,
            reservation_size,
            MEM_RESERVE,
            PAGE_NOACCESS);

    return(mem);
}


internal memory
ifb_win32_api_memory_reserve_large_pages(
    u64 reservation_size) {

    memory mem = 
        (memory)VirtualAlloc(
            0,
            reservation_size, 
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE);

    return(mem);
}

internal void
ifb_win32_api_memory_release(
    memory reserved_memory,
    u64    reserved_size) {

    VirtualFree(
        reserved_memory,
        reserved_size,
        MEM_RELEASE);
}

internal memory
ifb_win32_api_memory_commit(
    memory reserved_memory,
    u64    commit_size) {
    
    memory mem = (memory)VirtualAlloc(reserved_memory,commit_size,MEM_COMMIT,PAGE_READWRITE);

    return(mem);
}

internal void
ifb_win32_api_memory_decommit(
    memory committed_memory,
    u64    committed_size) {

    VirtualFree(
        committed_memory,
        committed_size,
        MEM_DECOMMIT);
}

internal u64
ifb_win32_api_memory_page_size_small() {

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);

    u64 page_size = system_info.dwPageSize;

    return(page_size);
}

internal u64
ifb_win32_api_memory_page_size_large() {

    u64 large_page_size = GetLargePageMinimum();

    return(large_page_size)
}

internal u64
ifb_win32_api_memory_allocation_granularity() {

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);

    u64 allocation_granularity = system_info.dwAllocationGranularity;
    
    return(allocation_granularity);
}

internal u64
ifb_win32_api_ticks() {

    LARGE_INTEGER win32_large_int = {0};
    QueryPerformanceCounter(&win32_large_int);
    u64 ticks = win32_large_int.QuadPart;

    return(ticks);
}

internal f64
ifb_win32_api_delta_time_ms(
    u64 ticks_before,
    u64 ticks_after) {

    //elapsed system ticks
    u64 ticks_elapsed = ticks_after - ticks_before;

    //get system frequency (Hz)
    LARGE_INTEGER win32_large_int = {0};
    QueryPerformanceFrequency(&win32_large_int);
    f64 frequency          = win32_large_int.QuadPart;

    //delta time in ms is the ticks divided by frequency times 1000
    f64 delta_time_seconds = (f64)ticks_elapsed / frequency;
    f64 delta_time_ms      = delta_time_seconds * 1000.0f;

    return(delta_time_ms);
}

internal void
ifb_win32_api_sleep(
    u64 time_ms) {

    Sleep(time_ms);
}

internal u64 
ifb_win32_api_process_id(
    void) {

    u64 process_id = GetCurrentProcessId();

    return(process_id);
}

internal u64 
ifb_win32_api_thread_id(
    void) {

    u64 thread_id = GetCurrentThreadId();

    return(thread_id);
}

IFBPlatformApi
ifb_win32_api_create_and_initialize() {

    IFBPlatformApi win32_api = {0};

    win32_api.file_size                     = ifb_win32_api_file_get_file_size_bytes; 
    win32_api.file_read                     = ifb_win32_api_read_file; 
    win32_api.file_write                    = ifb_win32_api_write_file; 
    win32_api.file_open                     = ifb_win32_api_open_file; 
    win32_api.file_close                    = ifb_win32_api_close_file; 
    win32_api.memory_reserve_small_pages    = ifb_win32_api_memory_reserve_small_pages;
    win32_api.memory_reserve_large_pages    = ifb_win32_api_memory_reserve_large_pages;
    win32_api.memory_release                = ifb_win32_api_memory_release;
    win32_api.memory_commit                 = ifb_win32_api_memory_commit;
    win32_api.memory_decommit               = ifb_win32_api_memory_decommit;
    win32_api.memory_page_size_small        = ifb_win32_api_memory_page_size_small;
    win32_api.memory_page_size_large        = ifb_win32_api_memory_page_size_large;
    win32_api.memory_allocation_granularity = ifb_win32_api_memory_allocation_granularity;
    win32_api.ticks                         = ifb_win32_api_ticks; 
    win32_api.delta_time_ms                 = ifb_win32_api_delta_time_ms; 
    win32_api.sleep                         = ifb_win32_api_sleep; 
    win32_api.process_id                    = ; 
    win32_api.thread_id                     = ; 

    return(win32_api);
}

