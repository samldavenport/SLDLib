#ifndef SLD_OS_THREAD_HPP
#define SLD_OS_THREAD_HPP

#include "sld.hpp"

namespace sld {

    using os_thread_handle_t           = vptr_t;
    using os_thread_mutex_handle_t     = vptr_t;
    using os_thread_condition_handle_t = vptr_t;
    using os_thread_error_t            = s32_t;

    struct os_thread_callback_data_t;
    struct os_thread_context_t;

    using os_thread_callback_function_f   = void (*) (os_thread_context_t& context);

    using os_thread_create_f              = const os_thread_error_t (*) (os_thread_handle_t&            thread_handle);
    using os_thread_destroy_f             = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_exit_f                = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_sleep_f               = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);     
    using os_thread_yield_f               = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_join_f                = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_mutex_create_f        = const os_thread_error_t (*) (os_thread_mutex_handle_t&      mutex_handle);
    using os_thread_mutex_destroy_f       = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_lock_f          = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_unlock_f        = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_try_lock_f      = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_condition_create_f    = const os_thread_error_t (*) (void);
    using os_thread_condition_destroy_f   = const os_thread_error_t (*) (void);
    using os_thread_condition_wait_f      = const os_thread_error_t (*) (void);
    using os_thread_condition_signal_f    = const os_thread_error_t (*) (void);
    using os_thread_condition_broadcast_f = const os_thread_error_t (*) (void);

    struct os_thread_callback_data_t {
        void* ptr;
        u64   size;
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