#pragma once 

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
ifb_win32_api_allocate_memory(u64 size) {

    memory mem = (memory)VirtualAlloc(0,size,MEM_COMMIT,PAGE_READWRITE);

    return(mem);
}

internal void
ifb_win32_api_free_memory(handle memory, u64 size) {

    VirtualFree(memory,size,MEM_RELEASE);
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