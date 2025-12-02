#ifndef SLD_OS_MONITOR_HPP
#define SLD_OS_MONITOR_HPP

#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr u32 OS_MONITOR_NAME_WIDTH = 32;

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct os_monitor_handle;
    struct os_monitor_info;
    struct os_monitor_working_area;
    struct os_monitor_screen_size;
    struct os_monitor_dimensions;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    SLD_API_OS u32               os_monitor_count            (void);
    SLD_API_OS void              os_monitor_get_working_area (os_monitor_working_area& monitor_working_area);
    SLD_API_OS void              os_monitor_get_info         (os_monitor_info&         monitor_info);
    SLD_API_OS os_monitor_handle os_monitor_get_primary      (void);
    SLD_API_OS os_monitor_handle os_monitor_from_point       (const u32 x, const u32 y);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct os_monitor_handle : vptr_t { };

    struct os_monitor_working_area {
        u32 virtual_pixel_width;
        u32 virtual_pixel_height;
    };

    struct os_monitor_dimensions {
        u32 pixel_width;
        u32 pixel_height;        
        u32 virtual_position_x;        
        u32 virtual_position_y;        
    };

    struct os_monitor_name {
        cchar cstr[OS_MONITOR_NAME_WIDTH];
    };

    struct os_monitor_info {
        os_monitor_handle     handle;
        os_monitor_dimensions dimensions;
        u32                   index;
        os_monitor_name       name;
    };
};

#endif //SLD_OS_MONITOR_HPP