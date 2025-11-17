#ifndef SLD_OS_THREAD_HPP
#define SLD_OS_THREAD_HPP

#include "sld.hpp"

namespace sld {

    using os_thread_error_s32 = s32;

    struct os_thread_t;
    struct os_thread_context_t;
    struct os_thread_mutex_t;
    struct os_thread_condition_t;
    struct os_thread_callback_data_t;

    using os_thread_callback_function_f   = void (*) (os_thread_context_t& context);

    using os_thread_create_f              = bool (os_thread_t*       thread);
    using os_thread_destroy_f             = bool (os_thread_t*       thread);
    using os_thread_exit_f                = bool (os_thread_t*       thread);
    using os_thread_sleep_f               = bool (os_thread_t*       thread);     
    using os_thread_yield_f               = bool (os_thread_t*       thread);
    using os_thread_join_f                = bool (os_thread_t*       thread);
    using os_thread_mutex_create_f        = bool (os_thread_mutex_t* mutex);
    using os_thread_mutex_destroy_f       = bool (os_thread_mutex_t* mutex);
    using os_thread_mutex_lock_f          = bool (os_thread_mutex_t* mutex);
    using os_thread_mutex_unlock_f        = bool (os_thread_mutex_t* mutex);
    using os_thread_mutex_try_lock_f      = bool (os_thread_mutex_t* mutex);
    using os_thread_condition_create_f    = bool (void);
    using os_thread_condition_destroy_f   = bool (void);
    using os_thread_condition_wait_f      = bool (void);
    using os_thread_condition_signal_f    = bool (void);
    using os_thread_condition_broadcast_f = bool (void);

    struct os_thread_t {
        vptr os_handle;
    };

    struct os_thread_mutex_t {
        vptr os_handle;
    };

    struct os_thread_condition_t {
        vptr os_handle;
    };

    struct os_thread_callback_data_t {
        vptr ptr;
        u64  size;
    };

    struct os_thread_context_t {
        os_thread_callback_function_f function;
        os_thread_callback_data_t     data;
    };

    enum os_thread_error_e {
        os_thread_error_e_success =  1,
        os_thread_error_e_unknown = -1
    };
};

#endif //SLD_OS_THREAD_HPP