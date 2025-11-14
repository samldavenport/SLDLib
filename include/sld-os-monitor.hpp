#ifndef SLD_OS_MONITOR_HPP
#define SLD_OS_MONITOR_HPP

#include "sld.hpp"

namespace sld {

    constexpr u32 OS_MONITOR_NAME_WIDTH = 32;

    struct os_monitor_t;
    struct os_monitor_info_t;
    struct os_monitor_working_area_t;
    
    using os_monitor_count_f        = u32          (*) (void);
    using os_monitor_working_area_f = void         (*) (os_monitor_working_area_t& monitor_working_area);
    using os_monitor_info_f         = void         (*) (os_monitor_info_t*         monitor_info);
    using os_monitor_primary_f      = os_monitor_t (*) (void);
    using os_monitor_from_point_f   = os_monitor_t (*) (const u32 x, const u32 y);

    struct os_monitor_t {
        vptr os_handle;
    };

    struct os_monitor_working_area_t {
        u32 virtual_pixel_width;
        u32 virtual_pixel_height;
    };

    struct os_monitor_screen_size_t {
        u32 pixel_width;
        u32 pixel_height;
    };

    struct os_monitor_info_t {
        vptr  os_handle;
        u32   index;
        u32   pixel_width;
        u32   pixel_height;
        u32   position_x;
        u32   position_y;
        cchar name_cstr[OS_MONITOR_NAME_WIDTH];
    };
};

#endif //SLD_OS_MONITOR_HPP