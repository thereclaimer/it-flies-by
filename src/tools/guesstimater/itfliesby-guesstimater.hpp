#ifndef ITFLIESBY_GUESSTIMATER_HPP
#define ITFLIESBY_GUESSTIMATER_HPP

#define IFB_NO_API

/**
 * Guesstimator is a tool to calculate theoretical vs actual performance
 * characteristics of a system. It will not be 100% accurate, but we 
 * can make a best guess (aka guesstimate) on how to make the best
 * use of a cache line and how many cycles per frame we can use
 * 
*/

#define IFB_DEPENDENCIES_EXCLUDE_ALL

#include <ifb.hpp>

#include <Windows.h>
#include <stdio.h>
#include <powerbase.h>


#define itfliesby_guesstimater_main WINAPI wWinMain

// Processor Power Informatio
struct Win32ProcessorPowerInformation {
	ULONG number;
	ULONG max_mhz;
	ULONG current_mhz;
	ULONG mhz_limit;
	ULONG max_idle_state;
	ULONG current_idle_state;
};

struct ItfliesbyGuesstimaterProcessorInfo {
   u32 cores_count;
   u32 cache_line_size_bytes;
   u32 speed_mhz;
};

struct ItfliesbyGuesstimaterFrameCycles {
    u32 cycles_per_frame_fps_030;
    u32 cycles_per_frame_fps_060;
    u32 cycles_per_frame_fps_120;
    u32 cycles_per_frame_fps_240;
};

struct ItfliesbyGuesstimater {
    ItfliesbyGuesstimaterProcessorInfo processor_info;
    ItfliesbyGuesstimaterFrameCycles   frame_cycles_theoretical;
    ItfliesbyGuesstimaterFrameCycles   frame_cycles_actual;

};

#endif//ITFLIESBY_GUESSTIMATER_HPP