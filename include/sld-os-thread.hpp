#ifndef SLD_OS_THREAD_HPP
#define SLD_OS_THREAD_HPP

#include "sld.hpp"

namespace sld {


    struct os_thread;
    struct os_thread_context;
    struct os_thread_mutex;
    struct os_thread_condition;
    struct os_thread_callback_data;
    struct os_thread_error;

    using os_thread_callback_function_f   = void (*) (os_thread_context& context);

    using os_thread_create_f              = bool (os_thread*       thread);
    using os_thread_destroy_f             = bool (os_thread*       thread);
    using os_thread_exit_f                = bool (os_thread*       thread);
    using os_thread_sleep_f               = bool (os_thread*       thread);     
    using os_thread_yield_f               = bool (os_thread*       thread);
    using os_thread_join_f                = bool (os_thread*       thread);
    using os_thread_mutex_create_f        = bool (os_thread_mutex* mutex);
    using os_thread_mutex_destroy_f       = bool (os_thread_mutex* mutex);
    using os_thread_mutex_lock_f          = bool (os_thread_mutex* mutex);
    using os_thread_mutex_unlock_f        = bool (os_thread_mutex* mutex);
    using os_thread_mutex_lock_f          = bool (os_thread_mutex* mutex);
    using os_thread_condition_create_f    = bool (void);
    using os_thread_condition_destroy_f   = bool (void);
    using os_thread_condition_wait_f      = bool (void);
    using os_thread_condition_signal_f    = bool (void);
    using os_thread_condition_broadcast_f = bool (void);

    struct os_thread_error : s32_t { };

    struct os_thread {
        vptr os_handle;
    };

    struct os_thread_mutex {
        vptr os_handle;
    };

    struct os_thread_condition {
        vptr os_handle;
    };

    struct os_thread_callback_data {
        vptr ptr;
        u64  size;
    };

    struct os_thread_context {
        os_thread_callback_function_f function;
        os_thread_callback_data     data;
    };

    enum os_thread_error_e {
        os_thread_error_e_success =  1,
        os_thread_error_e_unknown = -1
    };
};

#endif //SLD_OS_THREAD_HPP