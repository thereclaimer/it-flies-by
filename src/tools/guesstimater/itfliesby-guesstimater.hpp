#ifndef ITFLIESBY_GUESSTIMATER_HPP
#define ITFLIESBY_GUESSTIMATER_HPP

/**
 * Guesstimator is a tool to calculate theoretical vs actual performance
 * characteristics of a system. It will not be 100% accurate, but we 
 * can make a best guess (aka guesstimate) on how to make the best
 * use of a cache line and how many cycles per frame we can use
 * 
*/

#include <itfliesby.hpp>

#include <Windows.h>
#include <stdio.h>

#define itfliesby_guesstimater_main WINAPI wWinMain

struct ItfliesbyGuesstimaterProcessorInfo {
   s32 num_cores;

};

struct ItfliesbyGuesstimaterFrameCycles {
    s32 cycles_per_frame_030fps;
    s32 cycles_per_frame_060fps;
    s32 cycles_per_frame_120fps;
    s32 cycles_per_frame_240fps;
};

struct ItfliesbyGuesstimater {
    ItfliesbyGuesstimaterProcessorInfo processor_info;
    ItfliesbyGuesstimaterFrameCycles   frame_cycles_theoretical;
    ItfliesbyGuesstimaterFrameCycles   frame_cycles_actual;

};

#endif//ITFLIESBY_GUESSTIMATER_HPP