/*=========================================================================
| promact_is Interface Library
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
| To access promact_is through the API:
|
| 1) Use one of the following shared objects:
|      promact_is.so      --  Linux/Mac OSX shared object
|      promact_is.dll     --  Windows dynamic link library
|
| 2) Along with one of the following language modules:
|      promact_is.c/h     --  C/C++ API header file and interface module
|      promact_is_py.py   --  Python API
|      promact_is.cs      --  C# .NET source
 ========================================================================*/


/*=========================================================================
| INCLUDES
 ========================================================================*/
/* This #include can be customized to conform to the user's build paths. */
#include "promact_is.h"


/*=========================================================================
| VERSION CHECK
 ========================================================================*/
#define PS_APP_CFILE_VERSION   0x0125   /* v1.37 */
#define PS_APP_REQ_SW_VERSION  0x0125   /* v1.37 */

/*
 * Make sure that the header file was included and that
 * the version numbers match.
 */
#ifndef PS_APP_HEADER_VERSION
#  error Unable to include header file. Please check include path.

#elif PS_APP_HEADER_VERSION != PS_APP_CFILE_VERSION
#  error Version mismatch between source and header files.

#endif


/*=========================================================================
| DEFINES
 ========================================================================*/
#define API_NAME                     "promact_is"
#define API_DEBUG                    PS_APP_DEBUG
#define API_OK                       PS_APP_OK
#define API_UNABLE_TO_LOAD_LIBRARY   PS_APP_UNABLE_TO_LOAD_LIBRARY
#define API_INCOMPATIBLE_LIBRARY     PS_APP_INCOMPATIBLE_LIBRARY
#define API_UNABLE_TO_LOAD_FUNCTION  PS_APP_UNABLE_TO_LOAD_FUNCTION
#define API_HEADER_VERSION           PS_APP_HEADER_VERSION
#define API_REQ_SW_VERSION           PS_APP_REQ_SW_VERSION


/*=========================================================================
| LINUX AND DARWIN SUPPORT
 ========================================================================*/
#if defined(__APPLE_CC__) && !defined(DARWIN)
#define DARWIN
#endif

#if defined(linux) || defined(DARWIN)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#ifdef DARWIN
#define DLOPEN_NO_WARN
#endif

#include <dlfcn.h>

#define DLL_HANDLE  void *
#define MAX_SO_PATH 256

#if API_DEBUG
static char SO_NAME[MAX_SO_PATH+1] = API_NAME ".so";
#endif

#endif


/*=========================================================================
| WINDOWS SUPPORT
 ========================================================================*/
#if defined(WIN32) || defined(_WIN32)

#include <stdio.h>
#include <windows.h>

#define DLL_HANDLE           HINSTANCE
#define dlopen(name, flags)  LoadLibraryA(API_NAME ".dll")
#define dlsym(handle, name)  GetProcAddress(handle, name)
#define dlerror()            "Exiting program"
#if API_DEBUG
#define SO_NAME              API_NAME ".dll"
#endif

#endif


/*=========================================================================
| SHARED LIBRARY LOADER
 ========================================================================*/
/* The error conditions can be customized depending on the application. */
static void *_loadFunction (const char *name, int *result) {
    static DLL_HANDLE handle = 0;
    void * function = 0;

    /* Load the shared library if necessary */
    if (handle == 0) {
        u32 (*version) (void);
        u16 sw_version;
        u16 api_version_req;

        handle = dlopen(0, RTLD_LAZY);
        if (handle == 0) {
#if API_DEBUG
            fprintf(stderr, "Unable to load %s\n", SO_NAME);
            fprintf(stderr, "%s\n", dlerror());
#endif
            *result = API_UNABLE_TO_LOAD_LIBRARY;
            return 0;
        }

        version = (void *)dlsym(handle, "ps_app_c_version");
        if (version == 0) {
#if API_DEBUG
            fprintf(stderr, "Unable to bind ps_app_c_version() in %s\n",
                    SO_NAME);
            fprintf(stderr, "%s\n", dlerror());
#endif
            handle  = 0;
            *result = API_INCOMPATIBLE_LIBRARY;
            return 0;
        }

        sw_version      = (u16)((version() >>  0) & 0xffff);
        api_version_req = (u16)((version() >> 16) & 0xffff);
        if (sw_version  < API_REQ_SW_VERSION ||
            API_HEADER_VERSION < api_version_req)
        {
#if API_DEBUG
            fprintf(stderr, "\nIncompatible versions:\n");

            fprintf(stderr, "  Header version  = v%d.%02d  ",
                    (API_HEADER_VERSION >> 8) & 0xff, API_HEADER_VERSION & 0xff);

            if (sw_version < API_REQ_SW_VERSION)
                fprintf(stderr, "(requires library >= %d.%02d)\n",
                        (API_REQ_SW_VERSION >> 8) & 0xff,
                        API_REQ_SW_VERSION & 0xff);
            else
                fprintf(stderr, "(library version OK)\n");
                        
                   
            fprintf(stderr, "  Library version = v%d.%02d  ",
                    (sw_version >> 8) & 0xff,
                    (sw_version >> 0) & 0xff);

            if (API_HEADER_VERSION < api_version_req)
                fprintf(stderr, "(requires header >= %d.%02d)\n",
                        (api_version_req >> 8) & 0xff,
                        (api_version_req >> 0) & 0xff);
            else
                fprintf(stderr, "(header version OK)\n");
#endif
            handle  = 0;
            *result = API_INCOMPATIBLE_LIBRARY;
            return 0;
        }
    }

    /* Bind the requested function in the shared library */
    function = (void *)dlsym(handle, name);
    *result  = function ? API_OK : API_UNABLE_TO_LOAD_FUNCTION;
    return function;
}


/*=========================================================================
| FUNCTIONS
 ========================================================================*/
static PromiraConnectionHandle (*c_ps_app_connect) (const char *) = 0;
PromiraConnectionHandle ps_app_connect (
    const char * net_addr
)
{
    if (c_ps_app_connect == 0) {
        int res = 0;
        if (!(c_ps_app_connect = _loadFunction("c_ps_app_connect", &res)))
            return res;
    }
    return c_ps_app_connect(net_addr);
}


static int (*c_ps_app_disconnect) (PromiraConnectionHandle) = 0;
int ps_app_disconnect (
    PromiraConnectionHandle conn
)
{
    if (c_ps_app_disconnect == 0) {
        int res = 0;
        if (!(c_ps_app_disconnect = _loadFunction("c_ps_app_disconnect", &res)))
            return res;
    }
    return c_ps_app_disconnect(conn);
}


static int (*c_ps_app_version) (PromiraChannelHandle, PromiraAppVersion *) = 0;
int ps_app_version (
    PromiraChannelHandle channel,
    PromiraAppVersion *  version
)
{
    if (c_ps_app_version == 0) {
        int res = 0;
        if (!(c_ps_app_version = _loadFunction("c_ps_app_version", &res)))
            return res;
    }
    return c_ps_app_version(channel, version);
}


static int (*c_ps_app_sleep_ms) (u32) = 0;
int ps_app_sleep_ms (
    u32 milliseconds
)
{
    if (c_ps_app_sleep_ms == 0) {
        int res = 0;
        if (!(c_ps_app_sleep_ms = _loadFunction("c_ps_app_sleep_ms", &res)))
            return res;
    }
    return c_ps_app_sleep_ms(milliseconds);
}


static const char * (*c_ps_app_status_string) (int) = 0;
const char * ps_app_status_string (
    int status
)
{
    if (c_ps_app_status_string == 0) {
        int res = 0;
        if (!(c_ps_app_status_string = _loadFunction("c_ps_app_status_string", &res)))
            return 0;
    }
    return c_ps_app_status_string(status);
}


static PromiraChannelHandle (*c_ps_channel_open) (PromiraConnectionHandle) = 0;
PromiraChannelHandle ps_channel_open (
    PromiraConnectionHandle conn
)
{
    if (c_ps_channel_open == 0) {
        int res = 0;
        if (!(c_ps_channel_open = _loadFunction("c_ps_channel_open", &res)))
            return res;
    }
    return c_ps_channel_open(conn);
}


static int (*c_ps_channel_close) (PromiraChannelHandle) = 0;
int ps_channel_close (
    PromiraChannelHandle channel
)
{
    if (c_ps_channel_close == 0) {
        int res = 0;
        if (!(c_ps_channel_close = _loadFunction("c_ps_channel_close", &res)))
            return res;
    }
    return c_ps_channel_close(channel);
}


static int (*c_ps_channel_submitted_count) (PromiraChannelHandle) = 0;
int ps_channel_submitted_count (
    PromiraChannelHandle channel
)
{
    if (c_ps_channel_submitted_count == 0) {
        int res = 0;
        if (!(c_ps_channel_submitted_count = _loadFunction("c_ps_channel_submitted_count", &res)))
            return res;
    }
    return c_ps_channel_submitted_count(channel);
}


static int (*c_ps_channel_uncollected_count) (PromiraChannelHandle) = 0;
int ps_channel_uncollected_count (
    PromiraChannelHandle channel
)
{
    if (c_ps_channel_uncollected_count == 0) {
        int res = 0;
        if (!(c_ps_channel_uncollected_count = _loadFunction("c_ps_channel_uncollected_count", &res)))
            return res;
    }
    return c_ps_channel_uncollected_count(channel);
}


static PromiraQueueHandle (*c_ps_queue_create) (PromiraConnectionHandle, u08) = 0;
PromiraQueueHandle ps_queue_create (
    PromiraConnectionHandle conn,
    u08                     queue_type
)
{
    if (c_ps_queue_create == 0) {
        int res = 0;
        if (!(c_ps_queue_create = _loadFunction("c_ps_queue_create", &res)))
            return res;
    }
    return c_ps_queue_create(conn, queue_type);
}


static int (*c_ps_queue_destroy) (PromiraQueueHandle) = 0;
int ps_queue_destroy (
    PromiraQueueHandle queue
)
{
    if (c_ps_queue_destroy == 0) {
        int res = 0;
        if (!(c_ps_queue_destroy = _loadFunction("c_ps_queue_destroy", &res)))
            return res;
    }
    return c_ps_queue_destroy(queue);
}


static int (*c_ps_queue_clear) (PromiraQueueHandle) = 0;
int ps_queue_clear (
    PromiraQueueHandle queue
)
{
    if (c_ps_queue_clear == 0) {
        int res = 0;
        if (!(c_ps_queue_clear = _loadFunction("c_ps_queue_clear", &res)))
            return res;
    }
    return c_ps_queue_clear(queue);
}


static int (*c_ps_queue_delay_ms) (PromiraQueueHandle, int) = 0;
int ps_queue_delay_ms (
    PromiraQueueHandle queue,
    int                milliseconds
)
{
    if (c_ps_queue_delay_ms == 0) {
        int res = 0;
        if (!(c_ps_queue_delay_ms = _loadFunction("c_ps_queue_delay_ms", &res)))
            return res;
    }
    return c_ps_queue_delay_ms(queue, milliseconds);
}


static int (*c_ps_queue_sync) (PromiraQueueHandle) = 0;
int ps_queue_sync (
    PromiraQueueHandle queue
)
{
    if (c_ps_queue_sync == 0) {
        int res = 0;
        if (!(c_ps_queue_sync = _loadFunction("c_ps_queue_sync", &res)))
            return res;
    }
    return c_ps_queue_sync(queue);
}


static int (*c_ps_queue_size) (PromiraQueueHandle) = 0;
int ps_queue_size (
    PromiraQueueHandle queue
)
{
    if (c_ps_queue_size == 0) {
        int res = 0;
        if (!(c_ps_queue_size = _loadFunction("c_ps_queue_size", &res)))
            return res;
    }
    return c_ps_queue_size(queue);
}


static PromiraCollectHandle (*c_ps_queue_submit) (PromiraQueueHandle, PromiraChannelHandle, u08, u08 *) = 0;
PromiraCollectHandle ps_queue_submit (
    PromiraQueueHandle   queue,
    PromiraChannelHandle channel,
    u08                  ctrl_id,
    u08 *                queue_type
)
{
    if (c_ps_queue_submit == 0) {
        int res = 0;
        if (!(c_ps_queue_submit = _loadFunction("c_ps_queue_submit", &res)))
            return res;
    }
    return c_ps_queue_submit(queue, channel, ctrl_id, queue_type);
}


static int (*c_ps_queue_async_submit) (PromiraQueueHandle, PromiraChannelHandle, u08) = 0;
int ps_queue_async_submit (
    PromiraQueueHandle   queue,
    PromiraChannelHandle channel,
    u08                  ctrl_id
)
{
    if (c_ps_queue_async_submit == 0) {
        int res = 0;
        if (!(c_ps_queue_async_submit = _loadFunction("c_ps_queue_async_submit", &res)))
            return res;
    }
    return c_ps_queue_async_submit(queue, channel, ctrl_id);
}


static PromiraCollectHandle (*c_ps_queue_async_collect) (PromiraChannelHandle, u08 *) = 0;
PromiraCollectHandle ps_queue_async_collect (
    PromiraChannelHandle channel,
    u08 *                queue_type
)
{
    if (c_ps_queue_async_collect == 0) {
        int res = 0;
        if (!(c_ps_queue_async_collect = _loadFunction("c_ps_queue_async_collect", &res)))
            return res;
    }
    return c_ps_queue_async_collect(channel, queue_type);
}


static int (*c_ps_collect_resp) (PromiraCollectHandle, int *, int *, int) = 0;
int ps_collect_resp (
    PromiraCollectHandle collect,
    int *                length,
    int *                result,
    int                  timeout
)
{
    if (c_ps_collect_resp == 0) {
        int res = 0;
        if (!(c_ps_collect_resp = _loadFunction("c_ps_collect_resp", &res)))
            return res;
    }
    return c_ps_collect_resp(collect, length, result, timeout);
}


static int (*c_ps_i2c_free_bus) (PromiraChannelHandle) = 0;
int ps_i2c_free_bus (
    PromiraChannelHandle channel
)
{
    if (c_ps_i2c_free_bus == 0) {
        int res = 0;
        if (!(c_ps_i2c_free_bus = _loadFunction("c_ps_i2c_free_bus", &res)))
            return res;
    }
    return c_ps_i2c_free_bus(channel);
}


static int (*c_ps_i2c_bus_timeout) (PromiraChannelHandle, u16) = 0;
int ps_i2c_bus_timeout (
    PromiraChannelHandle channel,
    u16                  timeout_ms
)
{
    if (c_ps_i2c_bus_timeout == 0) {
        int res = 0;
        if (!(c_ps_i2c_bus_timeout = _loadFunction("c_ps_i2c_bus_timeout", &res)))
            return res;
    }
    return c_ps_i2c_bus_timeout(channel, timeout_ms);
}


static int (*c_ps_i2c_bitrate) (PromiraChannelHandle, u16) = 0;
int ps_i2c_bitrate (
    PromiraChannelHandle channel,
    u16                  bitrate_khz
)
{
    if (c_ps_i2c_bitrate == 0) {
        int res = 0;
        if (!(c_ps_i2c_bitrate = _loadFunction("c_ps_i2c_bitrate", &res)))
            return res;
    }
    return c_ps_i2c_bitrate(channel, bitrate_khz);
}


static int (*c_ps_i2c_read) (PromiraChannelHandle, u16, PromiraI2cFlags, u16, u08 *, u16 *) = 0;
int ps_i2c_read (
    PromiraChannelHandle channel,
    u16                  slave_addr,
    PromiraI2cFlags      flags,
    u16                  num_bytes,
    u08 *                data_in,
    u16 *                num_read
)
{
    if (c_ps_i2c_read == 0) {
        int res = 0;
        if (!(c_ps_i2c_read = _loadFunction("c_ps_i2c_read", &res)))
            return res;
    }
    return c_ps_i2c_read(channel, slave_addr, flags, num_bytes, data_in, num_read);
}


static int (*c_ps_queue_i2c_read) (PromiraQueueHandle, u16, PromiraI2cFlags, u16) = 0;
int ps_queue_i2c_read (
    PromiraQueueHandle queue,
    u16                slave_addr,
    PromiraI2cFlags    flags,
    u16                num_bytes
)
{
    if (c_ps_queue_i2c_read == 0) {
        int res = 0;
        if (!(c_ps_queue_i2c_read = _loadFunction("c_ps_queue_i2c_read", &res)))
            return res;
    }
    return c_ps_queue_i2c_read(queue, slave_addr, flags, num_bytes);
}


static int (*c_ps_collect_i2c_read) (PromiraCollectHandle, u16, u08 *, u16 *) = 0;
int ps_collect_i2c_read (
    PromiraCollectHandle collect,
    u16                  num_bytes,
    u08 *                data_in,
    u16 *                num_read
)
{
    if (c_ps_collect_i2c_read == 0) {
        int res = 0;
        if (!(c_ps_collect_i2c_read = _loadFunction("c_ps_collect_i2c_read", &res)))
            return res;
    }
    return c_ps_collect_i2c_read(collect, num_bytes, data_in, num_read);
}


static int (*c_ps_i2c_write) (PromiraChannelHandle, u16, PromiraI2cFlags, u16, const u08 *, u16 *) = 0;
int ps_i2c_write (
    PromiraChannelHandle channel,
    u16                  slave_addr,
    PromiraI2cFlags      flags,
    u16                  num_bytes,
    const u08 *          data_out,
    u16 *                num_written
)
{
    if (c_ps_i2c_write == 0) {
        int res = 0;
        if (!(c_ps_i2c_write = _loadFunction("c_ps_i2c_write", &res)))
            return res;
    }
    return c_ps_i2c_write(channel, slave_addr, flags, num_bytes, data_out, num_written);
}


static int (*c_ps_queue_i2c_write) (PromiraQueueHandle, u16, PromiraI2cFlags, u16, const u08 *) = 0;
int ps_queue_i2c_write (
    PromiraQueueHandle queue,
    u16                slave_addr,
    PromiraI2cFlags    flags,
    u16                num_bytes,
    const u08 *        data_out
)
{
    if (c_ps_queue_i2c_write == 0) {
        int res = 0;
        if (!(c_ps_queue_i2c_write = _loadFunction("c_ps_queue_i2c_write", &res)))
            return res;
    }
    return c_ps_queue_i2c_write(queue, slave_addr, flags, num_bytes, data_out);
}


static int (*c_ps_collect_i2c_write) (PromiraCollectHandle, u16 *) = 0;
int ps_collect_i2c_write (
    PromiraCollectHandle collect,
    u16 *                num_written
)
{
    if (c_ps_collect_i2c_write == 0) {
        int res = 0;
        if (!(c_ps_collect_i2c_write = _loadFunction("c_ps_collect_i2c_write", &res)))
            return res;
    }
    return c_ps_collect_i2c_write(collect, num_written);
}


static int (*c_ps_i2c_slave_enable) (PromiraChannelHandle, u16, u16, u16) = 0;
int ps_i2c_slave_enable (
    PromiraChannelHandle channel,
    u16                  addr,
    u16                  maxTxBytes,
    u16                  maxRxBytes
)
{
    if (c_ps_i2c_slave_enable == 0) {
        int res = 0;
        if (!(c_ps_i2c_slave_enable = _loadFunction("c_ps_i2c_slave_enable", &res)))
            return res;
    }
    return c_ps_i2c_slave_enable(channel, addr, maxTxBytes, maxRxBytes);
}


static int (*c_ps_i2c_slave_disable) (PromiraChannelHandle) = 0;
int ps_i2c_slave_disable (
    PromiraChannelHandle channel
)
{
    if (c_ps_i2c_slave_disable == 0) {
        int res = 0;
        if (!(c_ps_i2c_slave_disable = _loadFunction("c_ps_i2c_slave_disable", &res)))
            return res;
    }
    return c_ps_i2c_slave_disable(channel);
}


static int (*c_ps_i2c_slave_set_resp) (PromiraChannelHandle, u08, const u08 *) = 0;
int ps_i2c_slave_set_resp (
    PromiraChannelHandle channel,
    u08                  num_bytes,
    const u08 *          data_out
)
{
    if (c_ps_i2c_slave_set_resp == 0) {
        int res = 0;
        if (!(c_ps_i2c_slave_set_resp = _loadFunction("c_ps_i2c_slave_set_resp", &res)))
            return res;
    }
    return c_ps_i2c_slave_set_resp(channel, num_bytes, data_out);
}


static int (*c_ps_i2c_slave_poll) (PromiraChannelHandle, int) = 0;
int ps_i2c_slave_poll (
    PromiraChannelHandle channel,
    int                  timeout
)
{
    if (c_ps_i2c_slave_poll == 0) {
        int res = 0;
        if (!(c_ps_i2c_slave_poll = _loadFunction("c_ps_i2c_slave_poll", &res)))
            return res;
    }
    return c_ps_i2c_slave_poll(channel, timeout);
}


static int (*c_ps_i2c_slave_write_stats) (PromiraChannelHandle, u08 *, u16 *) = 0;
int ps_i2c_slave_write_stats (
    PromiraChannelHandle channel,
    u08 *                addr,
    u16 *                num_written
)
{
    if (c_ps_i2c_slave_write_stats == 0) {
        int res = 0;
        if (!(c_ps_i2c_slave_write_stats = _loadFunction("c_ps_i2c_slave_write_stats", &res)))
            return res;
    }
    return c_ps_i2c_slave_write_stats(channel, addr, num_written);
}


static int (*c_ps_i2c_slave_read) (PromiraChannelHandle, u08 *, u16, u08 *, u16 *) = 0;
int ps_i2c_slave_read (
    PromiraChannelHandle channel,
    u08 *                addr,
    u16                  num_bytes,
    u08 *                data_in,
    u16 *                num_read
)
{
    if (c_ps_i2c_slave_read == 0) {
        int res = 0;
        if (!(c_ps_i2c_slave_read = _loadFunction("c_ps_i2c_slave_read", &res)))
            return res;
    }
    return c_ps_i2c_slave_read(channel, addr, num_bytes, data_in, num_read);
}


static int (*c_ps_i2c_slave_data_lost_stats) (PromiraChannelHandle) = 0;
int ps_i2c_slave_data_lost_stats (
    PromiraChannelHandle channel
)
{
    if (c_ps_i2c_slave_data_lost_stats == 0) {
        int res = 0;
        if (!(c_ps_i2c_slave_data_lost_stats = _loadFunction("c_ps_i2c_slave_data_lost_stats", &res)))
            return res;
    }
    return c_ps_i2c_slave_data_lost_stats(channel);
}


static int (*c_ps_spi_bitrate) (PromiraChannelHandle, u32) = 0;
int ps_spi_bitrate (
    PromiraChannelHandle channel,
    u32                  bitrate_khz
)
{
    if (c_ps_spi_bitrate == 0) {
        int res = 0;
        if (!(c_ps_spi_bitrate = _loadFunction("c_ps_spi_bitrate", &res)))
            return res;
    }
    return c_ps_spi_bitrate(channel, bitrate_khz);
}


static int (*c_ps_spi_configure) (PromiraChannelHandle, PromiraSpiMode, PromiraSpiBitorder, u08) = 0;
int ps_spi_configure (
    PromiraChannelHandle channel,
    PromiraSpiMode       mode,
    PromiraSpiBitorder   bitorder,
    u08                  ss_polarity
)
{
    if (c_ps_spi_configure == 0) {
        int res = 0;
        if (!(c_ps_spi_configure = _loadFunction("c_ps_spi_configure", &res)))
            return res;
    }
    return c_ps_spi_configure(channel, mode, bitorder, ss_polarity);
}


static int (*c_ps_spi_configure_delays) (PromiraChannelHandle, u08) = 0;
int ps_spi_configure_delays (
    PromiraChannelHandle channel,
    u08                  word_delay
)
{
    if (c_ps_spi_configure_delays == 0) {
        int res = 0;
        if (!(c_ps_spi_configure_delays = _loadFunction("c_ps_spi_configure_delays", &res)))
            return res;
    }
    return c_ps_spi_configure_delays(channel, word_delay);
}


static int (*c_ps_spi_enable_ss) (PromiraChannelHandle, u08) = 0;
int ps_spi_enable_ss (
    PromiraChannelHandle channel,
    u08                  ss_enable
)
{
    if (c_ps_spi_enable_ss == 0) {
        int res = 0;
        if (!(c_ps_spi_enable_ss = _loadFunction("c_ps_spi_enable_ss", &res)))
            return res;
    }
    return c_ps_spi_enable_ss(channel, ss_enable);
}


static int (*c_ps_queue_spi_oe) (PromiraQueueHandle, u08) = 0;
int ps_queue_spi_oe (
    PromiraQueueHandle queue,
    u08                oe
)
{
    if (c_ps_queue_spi_oe == 0) {
        int res = 0;
        if (!(c_ps_queue_spi_oe = _loadFunction("c_ps_queue_spi_oe", &res)))
            return res;
    }
    return c_ps_queue_spi_oe(queue, oe);
}


static int (*c_ps_queue_spi_ss) (PromiraQueueHandle, u08) = 0;
int ps_queue_spi_ss (
    PromiraQueueHandle queue,
    u08                ss_assert
)
{
    if (c_ps_queue_spi_ss == 0) {
        int res = 0;
        if (!(c_ps_queue_spi_ss = _loadFunction("c_ps_queue_spi_ss", &res)))
            return res;
    }
    return c_ps_queue_spi_ss(queue, ss_assert);
}


static int (*c_ps_queue_spi_delay_cycles) (PromiraQueueHandle, u32) = 0;
int ps_queue_spi_delay_cycles (
    PromiraQueueHandle queue,
    u32                cycles
)
{
    if (c_ps_queue_spi_delay_cycles == 0) {
        int res = 0;
        if (!(c_ps_queue_spi_delay_cycles = _loadFunction("c_ps_queue_spi_delay_cycles", &res)))
            return res;
    }
    return c_ps_queue_spi_delay_cycles(queue, cycles);
}


static int (*c_ps_queue_spi_delay_ns) (PromiraQueueHandle, u32) = 0;
int ps_queue_spi_delay_ns (
    PromiraQueueHandle queue,
    u32                nanoseconds
)
{
    if (c_ps_queue_spi_delay_ns == 0) {
        int res = 0;
        if (!(c_ps_queue_spi_delay_ns = _loadFunction("c_ps_queue_spi_delay_ns", &res)))
            return res;
    }
    return c_ps_queue_spi_delay_ns(queue, nanoseconds);
}


static int (*c_ps_queue_spi_write) (PromiraQueueHandle, PromiraSpiIOMode, u08, u32, const u08 *) = 0;
int ps_queue_spi_write (
    PromiraQueueHandle queue,
    PromiraSpiIOMode   io,
    u08                word_size,
    u32                out_num_words,
    const u08 *        data_out
)
{
    if (c_ps_queue_spi_write == 0) {
        int res = 0;
        if (!(c_ps_queue_spi_write = _loadFunction("c_ps_queue_spi_write", &res)))
            return res;
    }
    return c_ps_queue_spi_write(queue, io, word_size, out_num_words, data_out);
}


static int (*c_ps_queue_spi_write_word) (PromiraQueueHandle, PromiraSpiIOMode, u08, u32, u32) = 0;
int ps_queue_spi_write_word (
    PromiraQueueHandle queue,
    PromiraSpiIOMode   io,
    u08                word_size,
    u32                out_num_words,
    u32                word
)
{
    if (c_ps_queue_spi_write_word == 0) {
        int res = 0;
        if (!(c_ps_queue_spi_write_word = _loadFunction("c_ps_queue_spi_write_word", &res)))
            return res;
    }
    return c_ps_queue_spi_write_word(queue, io, word_size, out_num_words, word);
}


static int (*c_ps_queue_spi_read) (PromiraQueueHandle, PromiraSpiIOMode, u08, u32) = 0;
int ps_queue_spi_read (
    PromiraQueueHandle queue,
    PromiraSpiIOMode   io,
    u08                word_size,
    u32                in_num_words
)
{
    if (c_ps_queue_spi_read == 0) {
        int res = 0;
        if (!(c_ps_queue_spi_read = _loadFunction("c_ps_queue_spi_read", &res)))
            return res;
    }
    return c_ps_queue_spi_read(queue, io, word_size, in_num_words);
}


static int (*c_ps_collect_spi_read) (PromiraCollectHandle, u08 *, u32, u08 *) = 0;
int ps_collect_spi_read (
    PromiraCollectHandle collect,
    u08 *                word_size,
    u32                  in_num_bytes,
    u08 *                data_in
)
{
    if (c_ps_collect_spi_read == 0) {
        int res = 0;
        if (!(c_ps_collect_spi_read = _loadFunction("c_ps_collect_spi_read", &res)))
            return res;
    }
    return c_ps_collect_spi_read(collect, word_size, in_num_bytes, data_in);
}


static int (*c_ps_spi_slave_enable) (PromiraChannelHandle, PromiraSlaveMode) = 0;
int ps_spi_slave_enable (
    PromiraChannelHandle channel,
    PromiraSlaveMode     mode
)
{
    if (c_ps_spi_slave_enable == 0) {
        int res = 0;
        if (!(c_ps_spi_slave_enable = _loadFunction("c_ps_spi_slave_enable", &res)))
            return res;
    }
    return c_ps_spi_slave_enable(channel, mode);
}


static int (*c_ps_spi_slave_disable) (PromiraChannelHandle) = 0;
int ps_spi_slave_disable (
    PromiraChannelHandle channel
)
{
    if (c_ps_spi_slave_disable == 0) {
        int res = 0;
        if (!(c_ps_spi_slave_disable = _loadFunction("c_ps_spi_slave_disable", &res)))
            return res;
    }
    return c_ps_spi_slave_disable(channel);
}


static int (*c_ps_spi_std_slave_configure) (PromiraChannelHandle, PromiraSpiIOMode, u08) = 0;
int ps_spi_std_slave_configure (
    PromiraChannelHandle channel,
    PromiraSpiIOMode     io,
    u08                  flags
)
{
    if (c_ps_spi_std_slave_configure == 0) {
        int res = 0;
        if (!(c_ps_spi_std_slave_configure = _loadFunction("c_ps_spi_std_slave_configure", &res)))
            return res;
    }
    return c_ps_spi_std_slave_configure(channel, io, flags);
}


static int (*c_ps_spi_slave_timeout) (PromiraChannelHandle, u32) = 0;
int ps_spi_slave_timeout (
    PromiraChannelHandle channel,
    u32                  timeout_ns
)
{
    if (c_ps_spi_slave_timeout == 0) {
        int res = 0;
        if (!(c_ps_spi_slave_timeout = _loadFunction("c_ps_spi_slave_timeout", &res)))
            return res;
    }
    return c_ps_spi_slave_timeout(channel, timeout_ns);
}


static int (*c_ps_spi_slave_host_read_size) (PromiraChannelHandle, u32) = 0;
int ps_spi_slave_host_read_size (
    PromiraChannelHandle channel,
    u32                  read_size
)
{
    if (c_ps_spi_slave_host_read_size == 0) {
        int res = 0;
        if (!(c_ps_spi_slave_host_read_size = _loadFunction("c_ps_spi_slave_host_read_size", &res)))
            return res;
    }
    return c_ps_spi_slave_host_read_size(channel, read_size);
}


static int (*c_ps_spi_std_slave_set_resp) (PromiraChannelHandle, u16, const u08 *) = 0;
int ps_spi_std_slave_set_resp (
    PromiraChannelHandle channel,
    u16                  num_bytes,
    const u08 *          resp
)
{
    if (c_ps_spi_std_slave_set_resp == 0) {
        int res = 0;
        if (!(c_ps_spi_std_slave_set_resp = _loadFunction("c_ps_spi_std_slave_set_resp", &res)))
            return res;
    }
    return c_ps_spi_std_slave_set_resp(channel, num_bytes, resp);
}


static int (*c_ps_spi_slave_poll) (PromiraChannelHandle, int) = 0;
int ps_spi_slave_poll (
    PromiraChannelHandle channel,
    int                  timeout
)
{
    if (c_ps_spi_slave_poll == 0) {
        int res = 0;
        if (!(c_ps_spi_slave_poll = _loadFunction("c_ps_spi_slave_poll", &res)))
            return res;
    }
    return c_ps_spi_slave_poll(channel, timeout);
}


static int (*c_ps_spi_slave_read) (PromiraChannelHandle, PromiraSpiSlaveReadInfo *, u32, u08 *) = 0;
int ps_spi_slave_read (
    PromiraChannelHandle      channel,
    PromiraSpiSlaveReadInfo * read_info,
    u32                       in_num_bytes,
    u08 *                     data_in
)
{
    if (c_ps_spi_slave_read == 0) {
        int res = 0;
        if (!(c_ps_spi_slave_read = _loadFunction("c_ps_spi_slave_read", &res)))
            return res;
    }
    return c_ps_spi_slave_read(channel, read_info, in_num_bytes, data_in);
}


static int (*c_ps_spi_slave_data_lost_stats) (PromiraChannelHandle) = 0;
int ps_spi_slave_data_lost_stats (
    PromiraChannelHandle channel
)
{
    if (c_ps_spi_slave_data_lost_stats == 0) {
        int res = 0;
        if (!(c_ps_spi_slave_data_lost_stats = _loadFunction("c_ps_spi_slave_data_lost_stats", &res)))
            return res;
    }
    return c_ps_spi_slave_data_lost_stats(channel);
}


static int (*c_ps_gpio_query) (PromiraChannelHandle) = 0;
int ps_gpio_query (
    PromiraChannelHandle channel
)
{
    if (c_ps_gpio_query == 0) {
        int res = 0;
        if (!(c_ps_gpio_query = _loadFunction("c_ps_gpio_query", &res)))
            return res;
    }
    return c_ps_gpio_query(channel);
}


static int (*c_ps_gpio_direction) (PromiraChannelHandle, u32) = 0;
int ps_gpio_direction (
    PromiraChannelHandle channel,
    u32                  direction_mask
)
{
    if (c_ps_gpio_direction == 0) {
        int res = 0;
        if (!(c_ps_gpio_direction = _loadFunction("c_ps_gpio_direction", &res)))
            return res;
    }
    return c_ps_gpio_direction(channel, direction_mask);
}


static int (*c_ps_queue_gpio_direction) (PromiraQueueHandle, u32) = 0;
int ps_queue_gpio_direction (
    PromiraQueueHandle queue,
    u32                direction_mask
)
{
    if (c_ps_queue_gpio_direction == 0) {
        int res = 0;
        if (!(c_ps_queue_gpio_direction = _loadFunction("c_ps_queue_gpio_direction", &res)))
            return res;
    }
    return c_ps_queue_gpio_direction(queue, direction_mask);
}


static int (*c_ps_gpio_get) (PromiraChannelHandle) = 0;
int ps_gpio_get (
    PromiraChannelHandle channel
)
{
    if (c_ps_gpio_get == 0) {
        int res = 0;
        if (!(c_ps_gpio_get = _loadFunction("c_ps_gpio_get", &res)))
            return res;
    }
    return c_ps_gpio_get(channel);
}


static int (*c_ps_queue_gpio_get) (PromiraQueueHandle) = 0;
int ps_queue_gpio_get (
    PromiraQueueHandle queue
)
{
    if (c_ps_queue_gpio_get == 0) {
        int res = 0;
        if (!(c_ps_queue_gpio_get = _loadFunction("c_ps_queue_gpio_get", &res)))
            return res;
    }
    return c_ps_queue_gpio_get(queue);
}


static int (*c_ps_gpio_set) (PromiraChannelHandle, u32) = 0;
int ps_gpio_set (
    PromiraChannelHandle channel,
    u32                  value
)
{
    if (c_ps_gpio_set == 0) {
        int res = 0;
        if (!(c_ps_gpio_set = _loadFunction("c_ps_gpio_set", &res)))
            return res;
    }
    return c_ps_gpio_set(channel, value);
}


static int (*c_ps_queue_gpio_set) (PromiraQueueHandle, u32) = 0;
int ps_queue_gpio_set (
    PromiraQueueHandle queue,
    u32                value
)
{
    if (c_ps_queue_gpio_set == 0) {
        int res = 0;
        if (!(c_ps_queue_gpio_set = _loadFunction("c_ps_queue_gpio_set", &res)))
            return res;
    }
    return c_ps_queue_gpio_set(queue, value);
}


static int (*c_ps_gpio_change) (PromiraChannelHandle, int) = 0;
int ps_gpio_change (
    PromiraChannelHandle channel,
    int                  timeout
)
{
    if (c_ps_gpio_change == 0) {
        int res = 0;
        if (!(c_ps_gpio_change = _loadFunction("c_ps_gpio_change", &res)))
            return res;
    }
    return c_ps_gpio_change(channel, timeout);
}


static int (*c_ps_queue_gpio_change) (PromiraQueueHandle, int) = 0;
int ps_queue_gpio_change (
    PromiraQueueHandle queue,
    int                timeout
)
{
    if (c_ps_queue_gpio_change == 0) {
        int res = 0;
        if (!(c_ps_queue_gpio_change = _loadFunction("c_ps_queue_gpio_change", &res)))
            return res;
    }
    return c_ps_queue_gpio_change(queue, timeout);
}


static int (*c_ps_app_configure) (PromiraChannelHandle, int) = 0;
int ps_app_configure (
    PromiraChannelHandle channel,
    int                  config
)
{
    if (c_ps_app_configure == 0) {
        int res = 0;
        if (!(c_ps_app_configure = _loadFunction("c_ps_app_configure", &res)))
            return res;
    }
    return c_ps_app_configure(channel, config);
}


static int (*c_ps_i2c_pullup) (PromiraChannelHandle, u08) = 0;
int ps_i2c_pullup (
    PromiraChannelHandle channel,
    u08                  pullup_mask
)
{
    if (c_ps_i2c_pullup == 0) {
        int res = 0;
        if (!(c_ps_i2c_pullup = _loadFunction("c_ps_i2c_pullup", &res)))
            return res;
    }
    return c_ps_i2c_pullup(channel, pullup_mask);
}


static int (*c_ps_phy_target_power) (PromiraChannelHandle, u08) = 0;
int ps_phy_target_power (
    PromiraChannelHandle channel,
    u08                  power_mask
)
{
    if (c_ps_phy_target_power == 0) {
        int res = 0;
        if (!(c_ps_phy_target_power = _loadFunction("c_ps_phy_target_power", &res)))
            return res;
    }
    return c_ps_phy_target_power(channel, power_mask);
}


static f32 (*c_ps_phy_level_shift) (PromiraChannelHandle, f32) = 0;
f32 ps_phy_level_shift (
    PromiraChannelHandle channel,
    f32                  level
)
{
    if (c_ps_phy_level_shift == 0) {
        int res = 0;
        if (!(c_ps_phy_level_shift = _loadFunction("c_ps_phy_level_shift", &res)))
            return res;
    }
    return c_ps_phy_level_shift(channel, level);
}


