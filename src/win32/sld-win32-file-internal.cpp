#pragma once

#include "sld-win32.hpp"

namespace sld {

    SLD_API_OS_INTERNAL void
    win32_file_set_last_error(
        void) {

        const DWORD win32_error = GetLastError();

        switch (win32_error) {
            case (ERROR_SUCCESS):              { _last_error_file = os_file_error_success;             }
            case (ERROR_INVALID_PARAMETER):    { _last_error_file = os_file_error_invalid_args;        }
            case (ERROR_INVALID_HANDLE):       { _last_error_file = os_file_error_invalid_handle;      }
            case (ERROR_SECTOR_NOT_FOUND):     { _last_error_file = os_file_error_invalid_disk;        }
            case (ERROR_DEVICE_NOT_CONNECTED): { _last_error_file = os_file_error_invalid_device;      }
            case (ERROR_INVALID_USER_BUFFER):  { _last_error_file = os_file_error_invalid_buffer;      }
            case (ERROR_FILE_INVALID):         { _last_error_file = os_file_error_invalid_file;        }
            case (ERROR_SHARING_VIOLATION):    { _last_error_file = os_file_error_sharing_violation;   }
            case (ERROR_ALREADY_EXISTS):       { _last_error_file = os_file_error_already_exists;      }
            case (ERROR_FILE_EXISTS):          { _last_error_file = os_file_error_already_exists;      }
            case (ERROR_FILE_NOT_FOUND):       { _last_error_file = os_file_error_not_found;           }
            case (ERROR_ACCESS_DENIED):        { _last_error_file = os_file_error_access_denied;       }
            case (ERROR_PIPE_BUSY):            { _last_error_file = os_file_error_pipe_busy;           }
            case (ERROR_HANDLE_EOF):           { _last_error_file = os_file_error_reached_end_of_file; }
            case (ERROR_BROKEN_PIPE):          { _last_error_file = os_file_error_broken_pipe;         }
            case (ERROR_NO_DATA):              { _last_error_file = os_file_error_no_data;             }
            case (ERROR_MORE_DATA):            { _last_error_file = os_file_error_more_data;           }
            case (ERROR_IO_INCOMPLETE):        { _last_error_file = os_file_error_io_incomplete;       }
            case (ERROR_IO_PENDING):           { _last_error_file = os_file_error_io_pending;          }
            case (ERROR_OPERATION_ABORTED):    { _last_error_file = os_file_error_operation_aborted;   }
            case (ERROR_CRC):                  { _last_error_file = os_file_error_disk_io_failure;     }
            case (ERROR_DISK_CORRUPT):         { _last_error_file = os_file_error_disk_corrupt;        }
            case (ERROR_NOT_READY):            { _last_error_file = os_file_error_device_not_ready;    }
            case (ERROR_GEN_FAILURE):          { _last_error_file = os_file_error_device_failure;      }
            case (ERROR_NOT_ENOUGH_MEMORY):    { _last_error_file = os_file_error_out_of_memory;       }
            default:                           { _last_error_file = os_file_error_unknown;             }
        }
    }

    SLD_API_OS_INTERNAL void
    win32_file_clear_last_error(
        void) {
        
        _last_error_file = os_file_error_success;
    }

    SLD_API_OS_INTERNAL const u64
    win32_file_get_buffer_granularity(
        void) {

        static u64 granularity = 0;

        if (granularity == 0) {
            SYSTEM_INFO sys_info;
            GetSystemInfo(&sys_info);
            granularity = sys_info.dwAllocationGranularity;
        }

        assert(granularity != 0);
        return(granularity);
    }

    SLD_API_OS_INTERNAL LPOVERLAPPED
    win32_file_get_overlapped(
        os_file_async* async) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)async->data;
        assert(overlapped);
        return(overlapped);
    }
};