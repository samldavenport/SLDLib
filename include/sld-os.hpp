#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"
#include "sld-os-window.hpp"
#include "sld-os-file.hpp"
#include "sld-os-system.hpp"
#include "sld-os-monitor.hpp"
#include "sld-os-memory.hpp"
#include "sld-os-thread.hpp"

/**********************************************************************************/
/* OS                                                                             */
/**********************************************************************************/

namespace sld {

    //-------------------------------------------------------------------
    // THREADS
    //-------------------------------------------------------------------

    SLD_API_OS os_thread_create_f               os_thread_create;
    SLD_API_OS os_thread_destroy_f              os_thread_destroy;
    SLD_API_OS os_thread_exit_f                 os_thread_exit;
    SLD_API_OS os_thread_sleep_f                os_thread_sleep;
    SLD_API_OS os_thread_yield_f                os_thread_yield;
    SLD_API_OS os_thread_join_f                 os_thread_join;
    SLD_API_OS os_thread_mutex_create_f         os_thread_mutex_create;
    SLD_API_OS os_thread_mutex_destroy_f        os_thread_mutex_destroy;
    SLD_API_OS os_thread_mutex_lock_f           os_thread_mutex_lock;
    SLD_API_OS os_thread_mutex_unlock_f         os_thread_mutex_unlock;
    SLD_API_OS os_thread_mutex_lock_f           os_thread_mutex_lock;
    SLD_API_OS os_thread_condition_create_f     os_thread_condition_create;
    SLD_API_OS os_thread_condition_destroy_f    os_thread_condition_destroy;
    SLD_API_OS os_thread_condition_wait_f       os_thread_condition_wait;
    SLD_API_OS os_thread_condition_signal_f     os_thread_condition_signal;
    SLD_API_OS os_thread_condition_broadcast_f  os_thread_condition_broadcast;
};

#endif //SLD_OS_HPP