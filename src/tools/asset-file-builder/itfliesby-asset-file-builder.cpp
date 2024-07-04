#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <strsafe.h>
 
#include "itfliesby-asset-file-builder.hpp"
                     
internal u64 bytes_read = 0;

#define DO_TERMINATE_FILE     true
#define DO_NOT_TERMINATE_FILE false


internal void
itfliesby_asset_file_builer_console_log(
    ItfliesbyAssetFileBuilder* asset_file_builder,
    const wchar_t *format, ...) {


    wchar_t log_buffer[256];
    
    va_list args;
    va_start(args,format);

    StringCchVPrintfW(log_buffer,256,format,args);
   
    va_end(args);

    DWORD bytes_wrriten = 0;

    WriteConsoleW(
        asset_file_builder->console_handle, 
        L"> ", 
        2, 
        &bytes_wrriten, 
        NULL
    );
    WriteConsoleW(
        asset_file_builder->console_handle, 
        log_buffer, 
        lstrlenW(log_buffer), 
        &bytes_wrriten, 
        NULL
    );
    WriteConsoleW(
        asset_file_builder->console_handle, 
        "\n", 
        1, 
        &bytes_wrriten, 
        NULL
    );
}

internal u32
itfliesby_asset_file_builder_asset_header_size(
    bool packed,
    u32  num_indexes) {

    u32 header_size = packed
        ?
            ITFLIESBY_ASSET_FILE_VERIFICATION_SIZE + 
            (ITFLIESBY_ASSET_FILE_INDEX_SIZE *  num_indexes)

        :
            (ITFLIESBY_ASSET_FILE_VERIFICATION_SIZE) + 
            (ITFLIESBY_ASSET_FILE_INDEX_SIZE *  num_indexes);


    return(header_size);
}

internal void
itfliesby_asset_file_builder_debug_output_line(
    ItfliesbyAssetFileBuilder* asset_file_builder,
    const wchar_t *format, ...) {

    wchar_t log_buffer[256];
    
    va_list args;
    va_start(args,format);

    StringCchVPrintfW(log_buffer,256,format,args);
   
    va_end(args);
   
    OutputDebugStringW(L"> ");
    OutputDebugStringW(log_buffer);
    OutputDebugStringW(L"\n");

}

internal LPCWSTR
itfliesby_asset_file_builder_file_type_string(
    ItfliesbyAssetFileBuilderAssetFileType file_type) {

    LPCWSTR file_type_string;

    switch (file_type) {
        case ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_TEXT: {
            file_type_string = L"TEXT";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_IMAGE: {
            file_type_string = L"IMAGE";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_MODEL: {
            file_type_string = L"MODEL";
        } break;
    }
    
    return(file_type_string);
}

internal LPCWSTR
itfliesby_asset_file_builder_error_code_string(
    ItfliesbyAssetFileBuilderReturnCode error_code) {

    LPCWSTR error_code_string;

    switch (error_code) {

        case ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS: {
            error_code_string = L"ASSET FILE BUILT SUCCESSFULLY";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_MEMORY_ALLOCATION_ERROR: {
            error_code_string = L"ERROR: FAILED TO ALLOCATE MEMORY";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_INVALID_ARGUMENTS: {
            error_code_string = L"ERROR: ARGUMENTS INVALID";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_INVALID_INPUT_FILE: {
            error_code_string = L"ERROR: INPUT FILE INVALID";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_OUT_OF_MEMORY: {
            error_code_string = L"ERROR: OUT OF MEMORY";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_CSV_READ_FAILURE: {
            error_code_string = L"ERROR: COULD NOT READ CSV FILE";
        } break;
        case ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_ASSET_FILE_CREATE_FAILURE: {
            error_code_string = L"ERROR: FAILED TO CREATE ASSET FILE";
        } break;
    }
    
    return(error_code_string);
}

void CALLBACK
itfliesby_asset_file_builder_file_completion_routine(
    DWORD        error_code,
    DWORD        bytes_transferred,
    LPOVERLAPPED lpOverlapped) {
        
    bytes_read = bytes_transferred;
}

internal u64
itfliesby_asset_file_builder_file_size(
    HANDLE file_handle,
    b8     terminate) {

    u64 file_size = GetFileSize(file_handle,NULL);

    file_size += terminate ? 1 : 0;

    return(file_size);
}

internal bool
itfliesby_asset_file_builder_read_file(
    HANDLE   file_handle,
    u64      allocated_buffer_size,
    mem_data allocated_buffer,
    b8       terminate) {

    OVERLAPPED overlapped = {0};
    bool read_result = 
            ReadFileEx(file_handle,
            allocated_buffer,
            allocated_buffer_size,
            &overlapped,
            itfliesby_asset_file_builder_file_completion_routine
    );

    allocated_buffer[allocated_buffer_size - 1] = '\0';

    return(read_result);
}


internal bool 
itfliesby_asset_file_builder_write_file(
    HANDLE file_handle,
    u64 allocated_buffer_size,
    mem_data allocated_buffer) {

    OVERLAPPED overlapped = {0};

    SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);

    bool write_result = WriteFileEx(
        file_handle,
        allocated_buffer,
        allocated_buffer_size,
        &overlapped,
        itfliesby_asset_file_builder_file_completion_routine
    );

    return(write_result);
}

internal bool
itfliesby_asset_file_builder_append_file(
    HANDLE file_handle,
    u64 allocated_buffer_size,
    mem_data allocated_buffer) {

    OVERLAPPED overlapped = {0};
    
    //set the cursor to the end of the file
    SetFilePointer(file_handle, 0, NULL, FILE_END);
    
    bool write_result = WriteFileEx(
        file_handle,
        allocated_buffer,
        allocated_buffer_size,
        &overlapped,
        itfliesby_asset_file_builder_file_completion_routine
    );

    return(write_result);

}

internal u64
itfliesby_asset_file_builder_memory_used_size(
    ItfliesbyAssetFileBuilder* asset_file_builder) {

    u64 used_size = 0;

    for (
        ItfliesbyAssetFileBuilderMemoryBlock* block = asset_file_builder->allocated_memory;
        block != NULL;
        block = block->next
    ) {
        used_size += block->allocation_size;
    }

    return(used_size);
}

internal u64 
itfliesby_asset_file_builder_memory_available_space(
    ItfliesbyAssetFileBuilder* asset_file_builder) {

    u64 available_space = 
        ITFLIESBY_ASSET_FILE_BUILDER_MEMORY_SIZE - 
        itfliesby_asset_file_builder_memory_used_size(asset_file_builder);

    return(available_space);
}


internal ItfliesbyAssetFileBuilderMemoryBlock*
itfliesby_asset_file_builder_memory_block_push(
    ItfliesbyAssetFileBuilder* asset_file_builder,
    u64 size) {

    ItfliesbyAssetFileBuilderMemoryBlock* new_block = NULL;

    //calculate the allocation size
    u64 allocation_size = size + sizeof(ItfliesbyAssetFileBuilderMemoryBlock);

    //first we need to grab make sure we can allocate the space
    u64 used_space = itfliesby_asset_file_builder_memory_used_size(asset_file_builder);
    u64 available_space = (ITFLIESBY_ASSET_FILE_BUILDER_MEMORY_SIZE) - used_space; 

    if (available_space < allocation_size) {
        return(NULL);
    }

    //we have the space, so we need to get the next block
    u64 offset = used_space + 1;
    new_block  = (ItfliesbyAssetFileBuilderMemoryBlock*)&asset_file_builder->memory[offset];
    new_block->data            = (mem_data)new_block + sizeof(ItfliesbyAssetFileBuilderMemoryBlock);
    new_block->allocation_size = allocation_size;
    new_block->data_size       = size;
    new_block->next            = NULL;

    // put this new block at the end of the list
    if (asset_file_builder->allocated_memory == NULL) {
        asset_file_builder->allocated_memory = new_block;
    } 
    else {

        ItfliesbyAssetFileBuilderMemoryBlock* last_block = NULL;
        for (
            last_block = asset_file_builder->allocated_memory;
            last_block->next != NULL;
            last_block = last_block->next) {
        }

        last_block->next = new_block;
    }

    return(new_block);
}

internal void
itfliesby_asset_file_builder_memory_block_pop(
    ItfliesbyAssetFileBuilder* asset_file_builder) {

    //find the most recent block of allocated memory
    ItfliesbyAssetFileBuilderMemoryBlock* current = asset_file_builder->allocated_memory;
    ItfliesbyAssetFileBuilderMemoryBlock* previous;
    
    //if nothing's allocated, we're done
    if (current == NULL || current->next == NULL) {
        return;
    }

    //find the block that will become the new last block
    while(current->next != NULL) {
        previous = current;
        current = current->next;
    }

    //null out the last block and we're done
    previous->next = NULL;
}

internal void
itfliesby_asset_file_builder_memory_clear(
    ItfliesbyAssetFileBuilder* asset_file_builder) {

    memset(
        asset_file_builder->memory,
        0,
        asset_file_builder->allocated_memory->allocation_size);

    asset_file_builder->allocated_memory = NULL;
}

internal HANDLE
itfliesby_asset_file_builder_open_asset_file(
    char* file_path) {
    
    HANDLE file_handle = 
        CreateFile(
            file_path,
            FILE_APPEND_DATA,
            FILE_SHARE_READ,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
    );

    return (file_handle == INVALID_HANDLE_VALUE)
        ? NULL
        : file_handle;
}

internal HANDLE
itfliesby_asset_file_builder_file_open(
    char* file_path) {

    HANDLE file_handle = 
        CreateFile(
            file_path,
            (GENERIC_ALL),
            (FILE_SHARE_READ | FILE_SHARE_WRITE),
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
    );

    return (file_handle == INVALID_HANDLE_VALUE)
        ? NULL
        : file_handle;
}

internal void
itfliesby_asset_file_builder_file_close(
    HANDLE file_handle) {

    CloseHandle(file_handle);
}

internal s32
itfliesby_asset_file_builder_process_arguments(
    ItfliesbyAssetFileBuilder* asset_file_builder,
    PWSTR                      cmd_line) {

    s32     argc;
    LPWSTR* argv;

    itfliesby_asset_file_builder_debug_output_line(asset_file_builder,L"PROCESSING ARGUMENTS");
    
    //check arguments
    argv = CommandLineToArgvW(cmd_line,&argc);
    if (argc != 3) {
        return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_INVALID_ARGUMENTS);
    }

    ItfliesbyAssetFileBuilderArguments* arguments = &asset_file_builder->arguments;
    wcstombs(arguments->path_to_csv,        argv[0], 128);
    wcstombs(arguments->path_to_asset_file, argv[1], 128);

    //get the file type
    wchar_t* end_ptr;
    arguments->asset_file_type = wcstol(argv[2],&end_ptr,10);

    if (arguments->asset_file_type <=ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_INVALID ||
        arguments->asset_file_type >=ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_COUNT) {
        
        return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_INVALID_TYPE);
    }

    return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS);
}

internal ItfliesbyAssetFileBuilderAssetFileType
itfliesby_asset_file_builder_get_file_type(
    const char* file_path) {

    //if anything goes wrong here, by default we'll treat it as a text file
    const char* dot = strrchr(file_path,'.');
    const char* extension = NULL;

    if (dot && dot != file_path && dot[1]) {
        extension = dot + 1;
    }
    else {
        return(ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_TEXT);
    }

    if (strcmp(extension,"png") == 0) {
        return(ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_IMAGE);
    }

    if (strcmp(extension,"fbx") == 0) {
        return(ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_MODEL);
    }

    //we have an extension, so determine what we got
    return(ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_TEXT);
}

internal s32
itfliesby_asset_file_builder_process_input_file(
    ItfliesbyAssetFileBuilder* asset_file_builder,
    PWSTR                      cmd_line) {

    ItfliesbyAssetFileBuilderCsvFile csv_file = {0};

    s32     argc;
    LPWSTR* argv;

    itfliesby_asset_file_builder_debug_output_line(asset_file_builder,L"PROCESSING INPUT CSV FILE");

    //open the csv file that will be the table of files to process
    csv_file.file_handle = itfliesby_asset_file_builder_file_open(asset_file_builder->arguments.path_to_csv);
    if (!csv_file.file_handle) {
        u32 error = GetLastError();
        return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_INVALID_INPUT_FILE);
    }
    
    csv_file.file_size = itfliesby_asset_file_builder_file_size(csv_file.file_handle,DO_TERMINATE_FILE); 
    csv_file.memory    = itfliesby_asset_file_builder_memory_block_push(asset_file_builder,csv_file.file_size);
    if (!itfliesby_asset_file_builder_read_file(
        csv_file.file_handle,
        csv_file.file_size,
        csv_file.memory->data,
        DO_TERMINATE_FILE
    )) {
        return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_CSV_READ_FAILURE);
    }
    csv_file.buffer = (char*)csv_file.memory->data;
    itfliesby_asset_file_builder_debug_output_line(asset_file_builder,L"OPENED CSV FILE");

    //now we have the data, we need to determine how many entries are in it
    csv_file.num_entries = 1;
    for (
        u32 index = 0;
        index < csv_file.file_size;
        ++index) {
            
        char character = csv_file.buffer[index];
        if (character == '\n') {
            ++csv_file.num_entries;
        }
    }
    itfliesby_asset_file_builder_debug_output_line(asset_file_builder,L"CSV FILE CONTAINS [%d] ENTRIES",csv_file.num_entries);

    //we know the number of entries, now we can allocate space for them
    ItfliesbyAssetFileBuilderMemoryBlock* entries_memory_block = 
        itfliesby_asset_file_builder_memory_block_push(asset_file_builder,sizeof(ItfliesbyAssetFileBuilderCsvEntry) * csv_file.num_entries);
    if (!entries_memory_block) {
        return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_OUT_OF_MEMORY);
    }
    csv_file.entries = (ItfliesbyAssetFileBuilderCsvEntry*)entries_memory_block->data;


    //we have the lines allocated, we're ready to rock
    u32 entry_index = 0;
    char *line_token, *line_context, *comma_token, *comma_context;
    u32 comma_token_length = 0;

    for (
        line_token = strtok_s((char*)csv_file.buffer,"\n",&line_context);
        line_token != NULL;
        line_token = strtok_s(NULL,"\n",&line_context)
    ) {

        ItfliesbyAssetFileBuilderCsvEntry* entry = &csv_file.entries[entry_index]; 

        //asset path
        comma_token = strtok_s(line_token,",",&comma_context); 
        comma_token_length = strnlen_s(comma_token,128);
        memmove(entry->asset_path,comma_token,comma_token_length);
        entry->asset_path[comma_token_length] = '\0';

        //asset tag
        comma_token = strtok_s(NULL,",",&comma_context); 
        comma_token_length = strnlen_s(comma_token,128);
        memmove(entry->asset_tag,comma_token,comma_token_length);
        entry->asset_tag[comma_token_length] = '\0';

        entry->asset_type = itfliesby_asset_file_builder_get_file_type(entry->asset_path);

        ++entry_index;
    }

    asset_file_builder->csv_file = csv_file;
    itfliesby_asset_file_builder_debug_output_line(asset_file_builder,L"PROCESSED ALL CSV ENTRIES");

    return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS);
}



internal s32
 itfliesby_asset_file_builder_asset_file_create(
    ItfliesbyAssetFileBuilder* asset_file_builder) {
    
    //init stbi
    stbi_set_flip_vertically_on_load(true);

    //open the file
    HANDLE file_handle = 
        CreateFile(
            asset_file_builder->arguments.path_to_asset_file,
            FILE_APPEND_DATA,
            FILE_SHARE_READ, 
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
    );

    if (!file_handle) {
        return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_ASSET_FILE_CREATE_FAILURE);
    }

    itfliesby_asset_file_builder_debug_output_line(asset_file_builder,L"CREATING ASSET FILE");

    //build the header and indexes
    ItfliesbyAssetsGameAssetFile* asset_file = &asset_file_builder->asset_file;
    asset_file->file_handle = file_handle;
    asset_file->file_header.verification[0] = 'I';
    asset_file->file_header.verification[1] = 'F';
    asset_file->file_header.verification[2] = 'B';
    asset_file->file_header.num_indexs = asset_file_builder->csv_file.num_entries;

    asset_file_builder->index_memory = 
        itfliesby_asset_file_builder_memory_block_push(
            asset_file_builder,
            sizeof(ItfliesbyAssetFileindex) * asset_file->file_header.num_indexs
    );

    asset_file->file_header.indexes = (ItfliesbyAssetFileindex*)asset_file_builder->index_memory->data;

    u32 asset_index_offset = ITFLIESBY_ASSET_FILE_HEADER_SIZE(asset_file->file_header.num_indexs);

    HANDLE tmp_file_handle;
    for (
        u32 index = 0;
        index  < asset_file->file_header.num_indexs;
        ++index
    ) {

        //get the next index
        ItfliesbyAssetFileBuilderCsvEntry csv_entry = asset_file_builder->csv_file.entries[index];
        ItfliesbyAssetFileindex* asset_index = &asset_file->file_header.indexes[index];

        tmp_file_handle = itfliesby_asset_file_builder_file_open(csv_entry.asset_path);

        if (tmp_file_handle == NULL) {
            itfliesby_asset_file_builder_debug_output_line(
                asset_file_builder,
                L"FAILED TO CREATE INDEX FOR [%s]",
                csv_entry.asset_path
            );
            
            continue;
        }

        *asset_index = {0};
        asset_index->file_size = GetFileSize(tmp_file_handle,NULL);
        asset_index->file_size = itfliesby_asset_file_builder_file_size(tmp_file_handle,DO_NOT_TERMINATE_FILE);
        strcpy(asset_index->tag,csv_entry.asset_tag);
    

        LPCWSTR file_type_string = itfliesby_asset_file_builder_file_type_string(csv_entry.asset_type);

        itfliesby_asset_file_builder_debug_output_line(
            asset_file_builder,
            L"CREATING INDEX [%d] - SIZE:[%d], TAG:[%hs], TYPE:[%s]",
            index,
            asset_index->file_size,
            asset_index->tag,
            file_type_string
        );

        switch (csv_entry.asset_type) {

            case ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_IMAGE: {

                auto image_memory_block = 
                    itfliesby_asset_file_builder_memory_block_push(
                        asset_file_builder,
                        asset_index->file_size);

                itfliesby_asset_file_builder_read_file(
                    tmp_file_handle,
                    asset_index->file_size,
                    image_memory_block->data,
                    DO_NOT_TERMINATE_FILE
                );

                s32 width_pixels   = 0;
                s32 height_pixels  = 0;
                s32 channels_count = 0;

                stbi_info_from_memory(
                    image_memory_block->data,
                    image_memory_block->data_size,
                    &width_pixels,
                    &height_pixels,
                    &channels_count);

                asset_index->allocation_size =
                    itfliesby_asset_file_builder_image_asset_allocation_size_bytes(
                        width_pixels,
                        height_pixels);

                itfliesby_asset_file_builder_memory_block_pop(
                    asset_file_builder);

            } break;

            case ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_TEXT: 
            default: {

                asset_index->allocation_size = asset_index->file_size + 1;

            } break;
        }

        itfliesby_asset_file_builder_file_close(tmp_file_handle);

        //TODO: we need to account for other types of file allocations
        asset_index->offset = asset_index_offset;
        asset_index_offset += asset_index->allocation_size;
    }

    u32 total_header_size = ITFLIESBY_ASSET_FILE_HEADER_SIZE(asset_file->file_header.num_indexs);

    //pack the header info
    ItfliesbyAssetFileHeaderPacked packed_header = {0};
    packed_header.num_indexs   = asset_file->file_header.num_indexs; 
    packed_header.verification[0] = 'I';
    packed_header.verification[1] = 'F';
    packed_header.verification[2] = 'B';

    //allocate space for the header
    u32 header_size  = ITFLIESBY_ASSET_FILE_HEADER_SIZE(asset_file->file_header.num_indexs); 
    u32 header_start = ITFLIESBY_ASSET_FILE_VERIFICATION_SIZE; 

    ItfliesbyAssetFileBuilderMemoryBlock* header_memory_block = 
        itfliesby_asset_file_builder_memory_block_push(
            asset_file_builder,
            header_size
        );

    asset_file_builder->header_data = header_memory_block->data;

    //write the header to the file
    memmove(
        asset_file_builder->header_data,
        &packed_header,
        header_start
    );

    u32 offset = header_start;
    for (
        u32 index = 0;
        index < packed_header.num_indexs;
        ++index) {

        ItfliesbyAssetFileindex file_index = asset_file->file_header.indexes[index];

        //copy the tag
        memmove(
            &asset_file_builder->header_data[offset],
            file_index.tag,
            32
        );
        offset += 32;

        //copy the rest of the index info
        memmove(
            &asset_file_builder->header_data[offset],
            &file_index.file_size,
            sizeof(u32) * 3
        );
        offset += sizeof(u32) * 3;
    }

    if (!itfliesby_asset_file_builder_write_file(
        asset_file->file_handle,
        header_size,
        asset_file_builder->header_data)) {

        return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_ASSET_FILE_CREATE_FAILURE);
    }

    //now, we are going to write the actual header data
    itfliesby_asset_file_builder_debug_output_line(
        asset_file_builder,
        L"HEADER CONSTRUCTED, WRITING ASSET DATA"
    );

    ItfliesbyAssetFileBuilderMemoryBlock* file_contents_block = NULL;
    ItfliesbyAssetFileBuilderMemoryBlock* asset_memory_block  = NULL;
    asset_handle asset_file_handle;



    for (
        u32 index = 0;
        index < asset_file->file_header.num_indexs;
        ++index
    ) {

        //get info for the next asset
        ItfliesbyAssetFileindex* header_index = &asset_file->file_header.indexes[index];
        ItfliesbyAssetFileBuilderCsvEntry* csv_entry = &asset_file_builder->csv_file.entries[index];

        //open the asset file
        asset_file_handle = itfliesby_asset_file_builder_file_open(csv_entry->asset_path);
        if (!asset_file_handle) {

            itfliesby_asset_file_builder_debug_output_line(
                asset_file_builder,
                L"CANNOT OPEN ASSET FILE [%hs]",
                csv_entry->asset_path
            );

            continue;
        }

        u32 asset_file_size = itfliesby_asset_file_builder_file_size(asset_file_handle,true);
        
        //allocate space for the asset
        file_contents_block = 
            itfliesby_asset_file_builder_memory_block_push(
                asset_file_builder,
                asset_file_size
            );

        if (!file_contents_block) {
            
            itfliesby_asset_file_builder_debug_output_line(
                asset_file_builder,
                L"CANNOT ALLOCATE MEMORY FOR FILE [%hs]",
                csv_entry->asset_path
            );

            itfliesby_asset_file_builder_file_close(asset_file_handle);

            continue;
        }

        //read the asset file
        if (!itfliesby_asset_file_builder_read_file(
            asset_file_handle,
            file_contents_block->data_size,
            file_contents_block->data,
            DO_TERMINATE_FILE)) {
            
            itfliesby_asset_file_builder_debug_output_line(
                asset_file_builder,
                L"CANNOT READ ASSET FILE [%hs]",
                csv_entry->asset_path
            );

            itfliesby_asset_file_builder_file_close(asset_file_handle);

            continue;
        }

        //process the asset data
        switch (csv_entry->asset_type) {
            
            case ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_IMAGE: {

                s32 width_pixels   = 0; 
                s32 height_pixels  = 0;
                s32 channels_count = 0;

                //get the information on our image
                stbi_info_from_memory(
                    file_contents_block->data,
                    file_contents_block->data_size,
                    &width_pixels,
                    &height_pixels,
                    &channels_count);

                //calculate the actual in-memory size of the image
                u32 image_size_bytes = 
                    itfliesby_asset_file_builder_image_size_bytes(
                        width_pixels,
                        height_pixels);

                //this is the size of the image plus width and height information
                u32 image_asset_allocation_size_bytes = 
                    itfliesby_asset_file_builder_image_asset_allocation_size_bytes(
                        width_pixels,
                        height_pixels);

                //allocate a block for the image data
                asset_memory_block =
                    itfliesby_asset_file_builder_memory_block_push(
                        asset_file_builder,
                        image_asset_allocation_size_bytes);

                //load the image
                memmove(
                    &asset_memory_block->data[0],
                    &width_pixels,
                    sizeof(u32)
                );
                memmove(
                    &asset_memory_block->data[4],
                    &height_pixels,
                    sizeof(u32)
                );

                auto image_block = itfliesby_asset_file_builder_memory_block_push(
                    asset_file_builder,
                    image_size_bytes
                );

                image_block->data =
                    stbi_load_from_memory(
                        file_contents_block->data,
                        file_contents_block->data_size,
                        &width_pixels,
                        &height_pixels,
                        &channels_count,
                        ITFLIESBY_ASSET_FILE_BUILDER_IMAGE_CHANNEL_COUNT);

                memmove(
                    &asset_memory_block->data[ITFLIESBY_ASSET_FILE_BUILDER_IMAGE_DATA_OFFSET],
                    image_block->data,
                    image_size_bytes
                );

                itfliesby_asset_file_builder_memory_block_pop(asset_file_builder);

                ifb_nop();

            } break;

            case ITFLIESBY_ASSET_FILE_BUILDER_ASSET_FILE_TYPE_TEXT:
            default: {
                //by default we treat this as a text file
                //to do that, just make the asset memory the same as the file memory
                asset_memory_block            = file_contents_block;
                header_index->allocation_size = asset_memory_block->data_size;
            } break;
        }

        itfliesby_asset_file_builder_debug_output_line(
            asset_file_builder,
            L"WRITING ASSET [%hs] TO OUTPUT FILE SIZE: [%d]",
            csv_entry->asset_path,
            //TODO: not sure where this extra size is coming from
            asset_memory_block->data_size
        );

        //write the asset memory to the file
        itfliesby_asset_file_builder_append_file(
            asset_file_builder->asset_file.file_handle,
            asset_memory_block->data_size,
            asset_memory_block->data
        );

        //reset everything for the next asset
        itfliesby_asset_file_builder_memory_block_pop(asset_file_builder);
        
        
        itfliesby_asset_file_builder_file_close(asset_file_handle);

    }

    return(ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS);
}

internal s32
itfliesby_asset_file_builder_main(
    HINSTANCE instance,
    HINSTANCE prev_isntance,
    PWSTR     cmd_line,
    int       cmd_show) {

    OutputDebugStringW(L"********************************\n");
    OutputDebugStringW(L"* ITFLIESBY ASSET FILE BUILDER *\n");
    OutputDebugStringW(L"********************************\n");

    ItfliesbyAssetFileBuilder asset_file_builder   = {0};
    asset_file_builder.console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    s32 return_code = ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS;

    itfliesby_asset_file_builer_console_log(
        &asset_file_builder,
        L"TEST"
    );

    //get command arguments
    return_code = itfliesby_asset_file_builder_process_arguments(&asset_file_builder,cmd_line);
    if (return_code != ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS) {
        goto cleanup;
    }

    //allocate memory that will be used to store file data temporarily
    asset_file_builder.memory  = 
        (mem_data)VirtualAlloc(
            0,
            ITFLIESBY_ASSET_FILE_BUILDER_MEMORY_SIZE,
            MEM_COMMIT,
            PAGE_READWRITE
    );

    //process the input file
    return_code = itfliesby_asset_file_builder_process_input_file(&asset_file_builder,cmd_line);
    if (return_code != ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS) {
        goto cleanup;
    }

    //create the output asset file
    return_code = itfliesby_asset_file_builder_asset_file_create(&asset_file_builder);
    if (return_code != ITFLIESBY_ASSET_FILE_BUILDER_RETURN_CODE_SUCCESS) {
        goto cleanup;
    }

cleanup:

    //log the return code
    LPCWSTR return_code_string = itfliesby_asset_file_builder_error_code_string((ItfliesbyAssetFileBuilderReturnCode)return_code);
    itfliesby_asset_file_builder_debug_output_line(
        &asset_file_builder,
        L"%s",return_code_string
    ); 

    //close any files that are open
    if (asset_file_builder.csv_file.file_handle) {
        itfliesby_asset_file_builder_file_close(asset_file_builder.csv_file.file_handle);
    }
    if (asset_file_builder.asset_file.file_handle) {
        itfliesby_asset_file_builder_file_close(asset_file_builder.asset_file.file_handle);
    }

    //deallocate our memory
    VirtualFree(
        asset_file_builder.memory, 
        ITFLIESBY_ASSET_FILE_BUILDER_MEMORY_SIZE,
        MEM_RELEASE
    );
    
    return(return_code);
}