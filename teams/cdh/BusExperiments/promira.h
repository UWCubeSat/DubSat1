/*=========================================================================
| Promira Management Interface Library
|--------------------------------------------------------------------------
| Copyright (c) 2002-2014 Total Phase, Inc.
| All rights reserved.
| www.totalphase.com
|
| Redistribution and use in source and binary forms, with or without
| modification, are permitted provided that the following conditions
| are met:
|
| - Redistributions of source code must retain the above copyright
|   notice, this list of conditions and the following disclaimer.
|
| - Redistributions in binary form must reproduce the above copyright
|   notice, this list of conditions and the following disclaimer in the
|   documentation and/or other materials provided with the distribution.
|
| - Neither the name of Total Phase, Inc. nor the names of its
|   contributors may be used to endorse or promote products derived from
|   this software without specific prior written permission.
|
| THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
| "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
| LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
| FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
| COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
| INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
| BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
| LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
| CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
| LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
| ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
| POSSIBILITY OF SUCH DAMAGE.
|--------------------------------------------------------------------------
| To access Promira devices through the API:
|
| 1) Use one of the following shared objects:
|      promira.so       --  Linux shared object
|      promira.dll      --  Windows dynamic link library
|
| 2) Along with one of the following language modules:
|      promira.c/h      --  C/C++ API header file and interface module
|      promira_py.py    --  Python API
|      promira.cs       --  C# .NET source
 ========================================================================*/


#ifndef __promira_h__
#define __promira_h__

#ifdef __cplusplus
extern "C" {
#endif


/*=========================================================================
| TYPEDEFS
 ========================================================================*/
#ifndef TOTALPHASE_DATA_TYPES
#define TOTALPHASE_DATA_TYPES


#include <stdint.h>
typedef uint8_t   u08;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s08;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;
typedef float   f32;
typedef double  f64;

#endif /* TOTALPHASE_DATA_TYPES */


/*=========================================================================
| DEBUG
 ========================================================================*/
/* Set the following macro to '1' for debugging */
#define PM_DEBUG 0


/*=========================================================================
| VERSION
 ========================================================================*/
#define PM_HEADER_VERSION  0x0123   /* v1.35 */


/*=========================================================================
| STATUS CODES
 ========================================================================*/
/*
 * All API functions return an integer which is the result of the
 * transaction, or a status code if negative.  The status codes are
 * defined as follows:
 */
enum PromiraStatus {
    /* General codes (0 to -99) */
    PM_OK                        =    0,
    PM_UNABLE_TO_LOAD_LIBRARY    =   -1,
    PM_UNABLE_TO_LOAD_DRIVER     =   -2,
    PM_UNABLE_TO_LOAD_FUNCTION   =   -3,
    PM_INCOMPATIBLE_LIBRARY      =   -4,
    PM_INCOMPATIBLE_DEVICE       =   -5,
    PM_COMMUNICATION_ERROR       =   -6,
    PM_UNABLE_TO_OPEN            =   -7,
    PM_UNABLE_TO_CLOSE           =   -8,
    PM_INVALID_HANDLE            =   -9,
    PM_CONFIG_ERROR              =  -10,
    PM_SHORT_BUFFER              =  -11,
    PM_FUNCTION_NOT_AVAILABLE    =  -12,

    /* Load command error codes (-100 to -199) */
    PM_APP_NOT_FOUND             = -101,
    PM_INVALID_LICENSE           = -102,
    PM_UNABLE_TO_LOAD_APP        = -103,
    PM_INVALID_DEVICE            = -104,
    PM_INVALID_DATE              = -105,
    PM_NOT_LICENSED              = -106,
    PM_INVALID_APP               = -107,
    PM_INVALID_FEATURE           = -108,
    PM_UNLICENSED_APP            = -109,
    PM_APP_ALREADY_LOADED        = -110,

    /* Network info error codes (-200 to -299) */
    PM_NETCONFIG_ERROR           = -201,
    PM_INVALID_IPADDR            = -202,
    PM_INVALID_NETMASK           = -203,
    PM_INVALID_SUBNET            = -204,
    PM_NETCONFIG_UNSUPPORTED     = -205,
    PM_NETCONFIG_LOST_CONNECTION = -206
};
#ifndef __cplusplus
typedef enum PromiraStatus PromiraStatus;
#endif


/*=========================================================================
| GENERAL TYPE DEFINITIONS
 ========================================================================*/
/* Promira handle type definition */
typedef int Promira;

/*
 * Promira version matrix.
 *
 * This matrix describes the various version dependencies
 * of Promira components.  It can be used to determine
 * which component caused an incompatibility error.
 *
 * All version numbers are of the format:
 *   (major << 8) | minor
 *
 * ex. v1.20 would be encoded as:  0x0114
 */
struct PromiraVersion {
    /* Software and firmware versions. */
    u16 software;
    u16 firmware;
    u16 hardware;

    /* Firmware requires that software must be >= this version. */
    u16 sw_req_by_fw;

    /* Software requires that firmware must be >= this version. */
    u16 fw_req_by_sw;

    /* Software requires that the API interface must be >= this version. */
    u16 api_req_by_sw;

    /* (year << 24) | (month << 16) | (day << 8) | build */
    u32 build;
};
#ifndef __cplusplus
typedef struct PromiraVersion PromiraVersion;
#endif


/*=========================================================================
| MANAGEMENT API
 ========================================================================*/
/*
 * Get a list of Promira devices on the network.
 *
 * nelem   = maximum number of elements to return
 * devices = array into which the IP addresses are returned
 *
 * Each element of the array is written with the IP address.
 *
 * If the array is NULL, it is not filled with any values.
 * If there are more devices than the array size, only the
 * first nmemb IP addresses will be written into the array.
 *
 * Returns the number of devices found, regardless of the
 * array size.
 */
int c_pm_find_devices (
    int   num_devices,
    u32 * devices
);


/*
 * Get a list of Promira devices on the network.
 *
 * This function is the same as pm_find_devices() except that
 * it returns the unique ID and status of each Promira device.
 * The IDs are guaranteed to be non-zero if valid.
 *
 * The IDs are the unsigned integer representation of the 10-digit
 * serial numbers.
 *
 * If status is PM_DEVICE_NOT_FREE, the device is in-use by
 * another host and is not ready for connection.
 */
#define PM_DEVICE_NOT_FREE 1
int c_pm_find_devices_ext (
    int   num_devices,
    u32 * devices,
    int   num_ids,
    u32 * unique_ids,
    int   num_statuses,
    u32 * statuses
);


/*
 * Open the Promira device
 *
 * Returns a Promira handle, which is guaranteed to be
 * greater than zero if it is valid.
 *
 * This function is recommended for use in simple applications
 * where extended information is not required.  For more complex
 * applications, the use of pm_open_ext() is recommended.
 *   str net_addr
 */
Promira c_pm_open (
    const char * net_addr
);


/*
 * Return the version matrix for the device attached to the
 * given handle.  If the handle is 0 or invalid, only the
 * software and required api versions are set.
 */
int c_pm_version (
    Promira          promira,
    PromiraVersion * version
);


/*
 * Return the version matrix for the application to the
 * given application name. Only firmware is valid and other
 * will be set to 0.
 */
int c_pm_app_version (
    Promira          promira,
    const char *     app_name,
    PromiraVersion * app_version
);


/* Sleep for the specified number of milliseconds */
int c_pm_sleep_ms (
    u32 milliseconds
);


/* Network related commands */
enum PromiraNetCommand {
    PM_NET_ETH_ENABLE      = 0,
    PM_NET_ETH_IP          = 1,
    PM_NET_ETH_NETMASK     = 2,
    PM_NET_ETH_MAC         = 3,
    PM_NET_ETH_DHCP_ENABLE = 4,
    PM_NET_ETH_DHCP_RENEW  = 5,
    PM_NET_USB_IP          = 6,
    PM_NET_USB_NETMASK     = 7,
    PM_NET_USB_MAC         = 8
};
#ifndef __cplusplus
typedef enum PromiraNetCommand PromiraNetCommand;
#endif

/* Configure the network settings */
int c_pm_query_net (
    Promira           promira,
    PromiraNetCommand cmd,
    int               buf_size,
    u08 *             buf
);


int c_pm_config_net (
    Promira           promira,
    PromiraNetCommand cmd,
    const char *      data
);


/* Configure the preferences */
int c_pm_query_pref (
    Promira      promira,
    const char * key,
    int          buf_size,
    u08 *        buf
);


int c_pm_config_pref (
    Promira      promira,
    const char * key,
    const char * data
);


/* Get a list of apps installed on the device. */
int c_pm_apps (
    Promira promira,
    u16     apps_size,
    u08 *   apps
);


/* Get a list of licensed apps installed on the device. */
int c_pm_licensed_apps (
    Promira promira,
    u16     apps_size,
    u08 *   apps
);


/* Launch an app */
int c_pm_load (
    Promira      promira,
    const char * app_name
);


enum PromiraLoadFlags {
    PM_LOAD_NO_FLAGS = 0x00,
    PM_LOAD_UNLOAD   = 0x01
};
#ifndef __cplusplus
typedef enum PromiraLoadFlags PromiraLoadFlags;
#endif

/* Launch an app with the flags */
int c_pm_load_ext (
    Promira          promira,
    const char *     app_name,
    PromiraLoadFlags flags
);


/*
 * Retrieve the network address that was used to open the device.
 * Return NULL if the handle is invalid.
 *
 * C programmers should not free the string returned.  It should be
 * valid for as long as the device remains opens, but C callers are
 * advised to reference the value as soon as they can and not cache
 * it for later use.
 */
const char * c_pm_get_net_addr (
    Promira promira
);


/* Retreive the path of python binding file */
const char * c_pm_get_py_bind_path (
    Promira promira
);


/* Close the Promira device */
int c_pm_close (
    Promira promira
);


/*
 * Return the unique ID for this Promira host adapter.
 * IDs are guaranteed to be non-zero if valid.
 * The ID is the unsigned integer representation of the
 * 10-digit serial number.
 */
u32 c_pm_unique_id (
    Promira promira
);


/*
 * Return the status string for the given status code.
 * If the code is not valid or the library function cannot
 * be loaded, return a NULL string.
 */
const char * c_pm_status_string (
    int status
);


/*
 * Wipe the device in preparation for an OS update
 *
 * Use with caution!!!
 */
int c_pm_init_device (
    Promira promira
);


/*
 * Read currently installed license
 * Pass 0 as buf_size to get the required size of the buffer
 */
int c_pm_read_license (
    Promira promira,
    int     buf_size,
    u08 *   buf
);


/*
 * Generate a colon separated string containig the names of features
 * that are licensed for the supplied app.
 */
int c_pm_features (
    Promira      promira,
    const char * app,
    u16          features_size,
    u08 *        features
);


/*
 * Return the value for the specified feature.  The value returned is
 * the string representation of the value.
 */
int c_pm_feature_value (
    Promira      promira,
    const char * app,
    const char * feature,
    u16          value_size,
    u08 *        value
);


/*
 * Return the description for the specified feature.  The description
 * returned is a string.
 */
int c_pm_feature_description (
    Promira      promira,
    const char * app,
    const char * feature,
    u16          desc_size,
    u08 *        desc
);




#ifdef __cplusplus
}
#endif

#endif  /* __promira_h__ */
