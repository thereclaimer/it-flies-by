#ifndef IFB_ENGINE_PLATFORM_HPP
#define IFB_ENGINE_PLATFORM_HPP

#include <r-libs.hpp>
#include "ifb-engine-types.hpp"

/**********************************************************************************/
/* FILES                                                                          */
/**********************************************************************************/

typedef ifb_index IFBEnginePlatformFileIndex;

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_open_read_only) (
    const ifb_cstr                     in_file_path,
          IFBEnginePlatformFileIndex& out_file_index_ref);

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_open_read_write) (
    const ifb_cstr                     in_file_path,
          IFBEnginePlatformFileIndex& out_file_index_ref);

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_open_read_write) (
    const ifb_cstr                     in_file_path,
          IFBEnginePlatformFileIndex& out_file_index_ref);

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_close) (
    const IFBEnginePlatformFileIndex file_index);

typedef const ifb_size
(*funcptr_ifb_engine_platform_file_size) (
    const IFBEnginePlatformFileIndex file_index);

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_read)(
    const IFBEnginePlatformFileIndex in_file_index,
    const ifb_size                   in_file_read_start,
    const ifb_size                   in_file_read_size,
          ifb_memory                out_file_read_buffer);

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_write)(
    const IFBEnginePlatformFileIndex in_file_index,
    const ifb_size                   in_file_write_start,
    const ifb_size                   in_file_write_size,
          ifb_memory                 in_file_write_buffer);

struct IFBEnginePlatformFile {
    funcptr_ifb_engine_platform_file_open_read_only  open_read_only;
    funcptr_ifb_engine_platform_file_open_read_write open_read_write;
    funcptr_ifb_engine_platform_file_close           close;
    funcptr_ifb_engine_platform_file_size            size;
    funcptr_ifb_engine_platform_file_read            read;
    funcptr_ifb_engine_platform_file_write           write;
};

/**********************************************************************************/
/* FILE DIALOG                                                                    */
/**********************************************************************************/

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_dialog_open) (
    const ifb_cstr  file_dialog_starting_directory,
    const ifb_size  file_type_count,
    const ifb_cstr* file_type_name_cstr_ptr,
    const ifb_cstr* file_type_spec_cstr_ptr);

typedef const ifb_b8
(*funcptr_ifb_engine_platform_file_dialog_get_selection) (
    const ifb_size  in_file_path_size,
          ifb_cstr out_file_path_selection);

struct IFBEnginePlatformFileDialog {
    funcptr_ifb_engine_platform_file_dialog_open          open;
    funcptr_ifb_engine_platform_file_dialog_get_selection get_selection;
};

/**********************************************************************************/
/* PLATFORM API                                                                   */
/**********************************************************************************/

struct IFBEnginePlatformApi {
    IFBEnginePlatformFile       file;
    IFBEnginePlatformFileDialog file_dialog;
};

#endif //IFB_ENGINE_PLATFORM_HPP