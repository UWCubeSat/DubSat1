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


#ifndef __promact_is_h__
#define __promact_is_h__

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
#define PS_APP_DEBUG 0


/*=========================================================================
| VERSION
 ========================================================================*/
#define PS_APP_HEADER_VERSION  0x0125   /* v1.37 */


/*=========================================================================
| STATUS CODES
 ========================================================================*/
/*
 * All API functions return an integer which is the result of the
 * transaction, or a status code if negative.  The status codes are
 * defined as follows:
 */
enum PromiraAppStatus {
    /* General codes (0 to -99) */
    PS_APP_OK                        =    0,
    PS_APP_UNABLE_TO_LOAD_LIBRARY    =   -1,
    PS_APP_UNABLE_TO_LOAD_DRIVER     =   -2,
    PS_APP_UNABLE_TO_LOAD_FUNCTION   =   -3,
    PS_APP_INCOMPATIBLE_LIBRARY      =   -4,
    PS_APP_INCOMPATIBLE_DEVICE       =   -5,
    PS_APP_COMMUNICATION_ERROR       =   -6,
    PS_APP_UNABLE_TO_OPEN            =   -7,
    PS_APP_UNABLE_TO_CLOSE           =   -8,
    PS_APP_INVALID_HANDLE            =   -9,
    PS_APP_CONFIG_ERROR              =  -10,
    PS_APP_MEMORY_ALLOC_ERROR        =  -11,
    PS_APP_UNABLE_TO_INIT_SUBSYSTEM  =  -12,
    PS_APP_INVALID_LICENSE           =  -13,

    /* Channel related code */
    PS_APP_PENDING_ASYNC_CMD         =  -30,
    PS_APP_TIMEOUT                   =  -31,
    PS_APP_CONNECTION_LOST           =  -32,
    PS_APP_CONNECTION_FULL           =  -33,

    /* Queue related code */
    PS_APP_QUEUE_FULL                =  -50,
    PS_APP_QUEUE_INVALID_CMD_TYPE    =  -51,
    PS_APP_QUEUE_EMPTY               =  -52,

    /* Collect related code */
    PS_APP_NO_MORE_CMDS_TO_COLLECT   =  -80,
    PS_APP_NO_MORE_QUEUES_TO_COLLECT =  -81,
    PS_APP_MISMATCHED_CMD            =  -82,
    PS_APP_UNKNOWN_CMD               =  -83,
    PS_APP_LOST_RESPONSE             =  -84,



    /* I2C codes (-100 to -199) */
    PS_I2C_NOT_AVAILABLE             = -100,
    PS_I2C_NOT_ENABLED               = -101,
    PS_I2C_READ_ERROR                = -102,
    PS_I2C_WRITE_ERROR               = -103,
    PS_I2C_SLAVE_BAD_CONFIG          = -104,
    PS_I2C_SLAVE_READ_ERROR          = -105,
    PS_I2C_SLAVE_TIMEOUT             = -106,
    PS_I2C_DROPPED_EXCESS_BYTES      = -107,
    PS_I2C_BUS_ALREADY_FREE          = -108,

    /* SPI codes (-200 to -299) */
    PS_SPI_NOT_AVAILABLE             = -200,
    PS_SPI_NOT_ENABLED               = -201,
    PS_SPI_WRITE_0_BYTES             = -202,
    PS_SPI_SLAVE_READ_ERROR          = -203,
    PS_SPI_SLAVE_TIMEOUT             = -204,
    PS_SPI_DROPPED_EXCESS_BYTES      = -205,
    PS_SPI_SLAVE_CMD_ERROR           = -206,
    PS_SPI_SLAVE_3WIRE               = -207,

    /* SPI code from Promira */
    PS_SPI_OUTPUT_NOT_ENABLED        = -250,
    PS_SPI_SLAVE_ENABLED             = -251,
    PS_SPI_OUTPUT_ENABLED            = -252
    /* GPIO codes (-300 to -399) */


    /* PHY Serial codes (-1000 to -1099) */

};
#ifndef __cplusplus
typedef enum PromiraAppStatus PromiraAppStatus;
#endif


/*=========================================================================
| GENERAL TYPE DEFINITIONS
 ========================================================================*/
/* Connection handle type definition */
typedef int PromiraConnectionHandle;

/* Channel handle type definition */
typedef int PromiraChannelHandle;

/* Queue handle type definition */
typedef int PromiraQueueHandle;

/* Collect handle type definition */
typedef int PromiraCollectHandle;

/*
 * Version matrix.
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
struct PromiraAppVersion {
    /* Software, firmware, and hardware versions. */
    u16 software;
    u16 firmware;
    u16 hardware;

    /* Firmware requires that software must be >= this version. */
    u16 sw_req_by_fw;

    /* Software requires that firmware must be >= this version. */
    u16 fw_req_by_sw;

    /* Software requires that the API interface must be >= this version. */
    u16 api_req_by_sw;
};
#ifndef __cplusplus
typedef struct PromiraAppVersion PromiraAppVersion;
#endif


/*=========================================================================
| GENERAL API
 ==========================================================================
| Connect to the application
|
| Returns a connection handle, which is guaranteed to be
| greater than zero if it is vali*/
PromiraConnectionHandle c_ps_app_connect (
    const char * net_addr
);


/* Disconnect from the application */
int c_ps_app_disconnect (
    PromiraConnectionHandle conn
);


/*
 * Return the version matrix for the device attached to the
 * given handle.  If the handle is 0 or invalid, only the
 * software and required api versions are set.
 */
int c_ps_app_version (
    PromiraChannelHandle channel,
    PromiraAppVersion *  version
);


/* Sleep for the specified number of milliseconds */
int c_ps_app_sleep_ms (
    u32 milliseconds
);


/*
 * Return the status string for the given status code.
 * If the code is not valid or the library function cannot
 * be loaded, return a NULL string.
 */
const char * c_ps_app_status_string (
    int status
);



/*=========================================================================
| CHANNEL API
 ==========================================================================
| Channel is a logical communication layer that talks to the application.
|
| Open a logical channel to communicate with the application.
|
| Returns a connection handle, which is guaranteed to be
| greater than zero if it is vali*/
PromiraChannelHandle c_ps_channel_open (
    PromiraConnectionHandle conn
);


/* Close a logical channel. */
int c_ps_channel_close (
    PromiraChannelHandle channel
);


/* Get the number of queues submitted, but not collected through the channel */
int c_ps_channel_submitted_count (
    PromiraChannelHandle channel
);


/* Get the number of queues completed and uncollected through the channel */
int c_ps_channel_uncollected_count (
    PromiraChannelHandle channel
);



/*=========================================================================
| QUEUE API
 ==========================================================================
| In order to use the Promira to send data across the bus at high
| speed, commands are accumulated in a queue until a call is made to
| submit all of the queued commands.  A queue can contain only commands
| with same type. For instance, any SPI command cannot be appended to
| I2C queue.
|
| Create the queue.  queue_type is one of the PS_MODULE_ID_* value*/
PromiraQueueHandle c_ps_queue_create (
    PromiraConnectionHandle conn,
    u08                     queue_type
);


/* Destroy the queue. */
int c_ps_queue_destroy (
    PromiraQueueHandle queue
);


/*
 * Clear all commands in the queue.
 * Please note that the queue is not cleared after it has been submitted.
 */
int c_ps_queue_clear (
    PromiraQueueHandle queue
);


/* Append a delay commmand into the queue. */
int c_ps_queue_delay_ms (
    PromiraQueueHandle queue,
    int                milliseconds
);


/*
 * Append a sync command into the queue.  Waits for all previous
 * commands to compelte entirely.
 */
int c_ps_queue_sync (
    PromiraQueueHandle queue
);


/* Return the number of commands in the queue. */
int c_ps_queue_size (
    PromiraQueueHandle queue
);


/*
 * Submit the queue.
 *
 * After the operation completes, the queue is untouched.  Therefore,
 * this function may be called repeatedly in rapid succession.  Waits
 * for the first command in the queue to complete.  Returns a
 * PromiraCollectHandle.
 */
PromiraCollectHandle c_ps_queue_submit (
    PromiraQueueHandle   queue,
    PromiraChannelHandle channel,
    u08                  ctrl_id,
    u08 *                queue_type
);


/*
 * Submit the queue asynchronously.
 *
 * After the operation completes, the queue is untouched.  Therefore,
 * this function may be called repeatedly in rapid succession.
 */
int c_ps_queue_async_submit (
    PromiraQueueHandle   queue,
    PromiraChannelHandle channel,
    u08                  ctrl_id
);


/*
 * Collect the next queue that was previously submitted asynchronously.
 * Waits for the first command in the queue to complete.  queue_type is
 * one of the PS_MODULE_ID_* values.  Returns a PromiraCollectHandle.
 */
PromiraCollectHandle c_ps_queue_async_collect (
    PromiraChannelHandle channel,
    u08 *                queue_type
);



/*=========================================================================
| Collect API
 ==========================================================================
| Collect the next command response.  Waits for the next command to
| complete or timeout. Returns the command type.  Get the result of
| the command and the length of data to be returned when the command
| specific collect function is calle*/
int c_ps_collect_resp (
    PromiraCollectHandle collect,
    int *                length,
    int *                result,
    int                  timeout
);



/*=========================================================================
| I2C Active API
 ========================================================================*/
#define PS_MODULE_ID_I2C_ACTIVE 0x00000001
enum PromiraI2cCommand {
    PS_I2C_CMD_WRITE    = 100,
    PS_I2C_CMD_READ     = 101,
    PS_I2C_CMD_DELAY_MS = 102
};
#ifndef __cplusplus
typedef enum PromiraI2cCommand PromiraI2cCommand;
#endif

/* Free the I2C bus. */
int c_ps_i2c_free_bus (
    PromiraChannelHandle channel
);


/*
 * Set the bus lock timeout.  If a zero is passed as the timeout,
 * the timeout is unchanged and the current timeout is returned.
 */
int c_ps_i2c_bus_timeout (
    PromiraChannelHandle channel,
    u16                  timeout_ms
);


/*
 * Set the I2C bit rate in kilohertz.  If a zero is passed as the
 * bitrate, the bitrate is unchanged and the current bitrate is
 * returned.
 */
int c_ps_i2c_bitrate (
    PromiraChannelHandle channel,
    u16                  bitrate_khz
);


enum PromiraI2cFlags {
    PS_I2C_NO_FLAGS          = 0x00,
    PS_I2C_10_BIT_ADDR       = 0x01,
    PS_I2C_COMBINED_FMT      = 0x02,
    PS_I2C_NO_STOP           = 0x04,
    PS_I2C_SIZED_READ        = 0x10,
    PS_I2C_SIZED_READ_EXTRA1 = 0x20
};
#ifndef __cplusplus
typedef enum PromiraI2cFlags PromiraI2cFlags;
#endif

enum PromiraI2cStatus {
    PS_I2C_STATUS_OK            = 0,
    PS_I2C_STATUS_BUS_ERROR     = 1,
    PS_I2C_STATUS_SLAVE_ACK     = 2,
    PS_I2C_STATUS_SLAVE_NACK    = 3,
    PS_I2C_STATUS_DATA_NACK     = 4,
    PS_I2C_STATUS_ARB_LOST      = 5,
    PS_I2C_STATUS_BUS_LOCKED    = 6,
    PS_I2C_STATUS_LAST_DATA_ACK = 7
};
#ifndef __cplusplus
typedef enum PromiraI2cStatus PromiraI2cStatus;
#endif

/*
 * Read a stream of bytes from the I2C slave device.  Fills the number
 * of bytes read into the num_read variable.  The return value of the
 * function is a status code.
 */
int c_ps_i2c_read (
    PromiraChannelHandle channel,
    u16                  slave_addr,
    PromiraI2cFlags      flags,
    u16                  num_bytes,
    u08 *                data_in,
    u16 *                num_read
);


/* Queue I2C read. */
int c_ps_queue_i2c_read (
    PromiraQueueHandle queue,
    u16                slave_addr,
    PromiraI2cFlags    flags,
    u16                num_bytes
);


/* Collect I2C read. */
int c_ps_collect_i2c_read (
    PromiraCollectHandle collect,
    u16                  num_bytes,
    u08 *                data_in,
    u16 *                num_read
);


/*
 * Write a stream of bytes to the I2C slave device.  Fills the number
 * of bytes written into the num_written variable.  The return value of
 * the function is a status code.
 */
int c_ps_i2c_write (
    PromiraChannelHandle channel,
    u16                  slave_addr,
    PromiraI2cFlags      flags,
    u16                  num_bytes,
    const u08 *          data_out,
    u16 *                num_written
);


/* Queue I2C write. */
int c_ps_queue_i2c_write (
    PromiraQueueHandle queue,
    u16                slave_addr,
    PromiraI2cFlags    flags,
    u16                num_bytes,
    const u08 *        data_out
);


/* Collect I2C write. */
int c_ps_collect_i2c_write (
    PromiraCollectHandle collect,
    u16 *                num_written
);


/* Enable the Promira as an I2C slave */
int c_ps_i2c_slave_enable (
    PromiraChannelHandle channel,
    u16                  addr,
    u16                  maxTxBytes,
    u16                  maxRxBytes
);


/* Disable the Promira as an I2C slave */
int c_ps_i2c_slave_disable (
    PromiraChannelHandle channel
);


/* Set the slave response. */
int c_ps_i2c_slave_set_resp (
    PromiraChannelHandle channel,
    u08                  num_bytes,
    const u08 *          data_out
);


/*
 * Polling function to check if there are any asynchronous
 * messages pending for processing.  The function takes a timeout
 * value in units of milliseconds.  If the timeout is < 0, the
 * function will block until data is received.  If the timeout is 0,
 * the function will perform a non-blocking check.
 */
enum PromiraI2cSlaveStatus {
    PS_I2C_SLAVE_NO_DATA   = 0x00,
    PS_I2C_SLAVE_READ      = 0x01,
    PS_I2C_SLAVE_WRITE     = 0x02,
    PS_I2C_SLAVE_DATA_LOST = 0x80
};
#ifndef __cplusplus
typedef enum PromiraI2cSlaveStatus PromiraI2cSlaveStatus;
#endif

int c_ps_i2c_slave_poll (
    PromiraChannelHandle channel,
    int                  timeout
);


/*
 * Return number of bytes written and status code from a previous
 * Promira->I2C_master transmission.  Since the transmission is
 * happening asynchronously with respect to the PC host software, there
 * could be responses queued up from many previous write transactions.
 */
int c_ps_i2c_slave_write_stats (
    PromiraChannelHandle channel,
    u08 *                addr,
    u16 *                num_written
);


/* Read the bytes from an I2C slave reception */
int c_ps_i2c_slave_read (
    PromiraChannelHandle channel,
    u08 *                addr,
    u16                  num_bytes,
    u08 *                data_in,
    u16 *                num_read
);


/*
 * Return number of I2C transactions lost due to the overflow of the
 * internal Promira buffer.
 */
int c_ps_i2c_slave_data_lost_stats (
    PromiraChannelHandle channel
);



/*=========================================================================
| SPI Active API
 ========================================================================*/
#define PS_MODULE_ID_SPI_ACTIVE 0x00000002
enum PromiraSpiCommand {
    PS_SPI_CMD_OE           = 200,
    PS_SPI_CMD_SS           = 201,
    PS_SPI_CMD_DELAY_MS     = 202,
    PS_SPI_CMD_DELAY_CYCLES = 203,
    PS_SPI_CMD_DELAY_NS     = 204,
    PS_SPI_CMD_READ         = 205
};
#ifndef __cplusplus
typedef enum PromiraSpiCommand PromiraSpiCommand;
#endif

/*
 * Set the SPI bitrate in kilohertz.  If a zero is passed as the
 * bitrate, the bitrate is unchanged and the current bitrate is
 * returned.
 */
int c_ps_spi_bitrate (
    PromiraChannelHandle channel,
    u32                  bitrate_khz
);


/*
 * These configuration parameters specify how to clock the bits that
 * are sent and received on SPI interface.  See the the Promira User
 * Manual for more details.
 */
enum PromiraSpiMode {
    PS_SPI_MODE_0 = 0,
    PS_SPI_MODE_1 = 1,
    PS_SPI_MODE_2 = 2,
    PS_SPI_MODE_3 = 3
};
#ifndef __cplusplus
typedef enum PromiraSpiMode PromiraSpiMode;
#endif

enum PromiraSpiBitorder {
    PS_SPI_BITORDER_MSB = 0,
    PS_SPI_BITORDER_LSB = 1
};
#ifndef __cplusplus
typedef enum PromiraSpiBitorder PromiraSpiBitorder;
#endif

/*
 * Configure the SPI master and slave interface.  Each of bits of
 * ss_polarity corresponds to each of the SS lines.
 */
int c_ps_spi_configure (
    PromiraChannelHandle channel,
    PromiraSpiMode       mode,
    PromiraSpiBitorder   bitorder,
    u08                  ss_polarity
);


/*
 * Configure the delays. word_delay is the the number of clock cycles
 * between words.  word_delay can not be 1.  There is no gap after the
 * last word.
 */
int c_ps_spi_configure_delays (
    PromiraChannelHandle channel,
    u08                  word_delay
);


/*
 * Enable/disable slave select lines.  When a slave select line is
 * enabled, the corresponding GPIO is disabled.
 */
int c_ps_spi_enable_ss (
    PromiraChannelHandle channel,
    u08                  ss_enable
);


/* Enable/disable driving the SPI data and clock signals */
int c_ps_queue_spi_oe (
    PromiraQueueHandle queue,
    u08                oe
);


/*
 * Assert the slave select lines.  Set the bit value for a given SS
 * line to 1 to assert the line or 0 to deassert the line.  The
 * polarity is determined by ps_spi_configure.
 */
int c_ps_queue_spi_ss (
    PromiraQueueHandle queue,
    u08                ss_assert
);


/* Queue a delay in clock cycles. */
int c_ps_queue_spi_delay_cycles (
    PromiraQueueHandle queue,
    u32                cycles
);


/* Queue a delay in nanoseconds. Maximum delay is 2 secs. */
int c_ps_queue_spi_delay_ns (
    PromiraQueueHandle queue,
    u32                nanoseconds
);


enum PromiraSpiIOMode {
    PS_SPI_IO_STANDARD = 0,
    PS_SPI_IO_DUAL     = 2,
    PS_SPI_IO_QUAD     = 4
};
#ifndef __cplusplus
typedef enum PromiraSpiIOMode PromiraSpiIOMode;
#endif

/*
 * Queue an array to be shifted.  data_out is a byte array containing
 * packed words.  word_size is the number of bits in a word and is
 * between 2 and 32.
 */
int c_ps_queue_spi_write (
    PromiraQueueHandle queue,
    PromiraSpiIOMode   io,
    u08                word_size,
    u32                out_num_words,
    const u08 *        data_out
);


/*
 * Queue a single word out_num_word times.  word_size is the number of
 * bits in a word and is between 2 and 32.
 */
int c_ps_queue_spi_write_word (
    PromiraQueueHandle queue,
    PromiraSpiIOMode   io,
    u08                word_size,
    u32                out_num_words,
    u32                word
);


/*
 * Queue an SPI read command.  Equivalent to ps_queue_spi_write_word
 * with word equal to 0 when io is PS_SPI_IO_STANDARD.  When io is
 * PS_SPI_IO_DUAL or PS_SPI_IO_QUAD, the clock is generated and the
 * data lines are set to inputs.
 */
int c_ps_queue_spi_read (
    PromiraQueueHandle queue,
    PromiraSpiIOMode   io,
    u08                word_size,
    u32                in_num_words
);


/*
 * Collect the actual SPI data received.  Returns number of words
 * received.  This should be called after ps_collect_resp returns
 * PS_SPI_CMD_READ.
 */
int c_ps_collect_spi_read (
    PromiraCollectHandle collect,
    u08 *                word_size,
    u32                  in_num_bytes,
    u08 *                data_in
);


enum PromiraSlaveMode {
    PS_SPI_SLAVE_MODE_STD = 0
};
#ifndef __cplusplus
typedef enum PromiraSlaveMode PromiraSlaveMode;
#endif

/* Enable the Promira as an SPI slave device */
int c_ps_spi_slave_enable (
    PromiraChannelHandle channel,
    PromiraSlaveMode     mode
);


/* Disable the Promira as an SPI slave device */
int c_ps_spi_slave_disable (
    PromiraChannelHandle channel
);


/* Configure the slave parameters */
#define PS_SPI_SLAVE_NO_SS 0x00000001
#define PS_SPI_MULTI_IO_WRITE 0x00000000
#define PS_SPI_MULTI_IO_READ 0x00000002
int c_ps_spi_std_slave_configure (
    PromiraChannelHandle channel,
    PromiraSpiIOMode     io,
    u08                  flags
);


/*
 * When PS_SPI_SLAVE_NO_SS is configured, this timeout determines a SPI
 * transaction.  Returns actual ns to be used.  Minimum is 1us.
 */
int c_ps_spi_slave_timeout (
    PromiraChannelHandle channel,
    u32                  timeout_ns
);


/* Allow collecting partial slave data. */
int c_ps_spi_slave_host_read_size (
    PromiraChannelHandle channel,
    u32                  read_size
);


/* Set the slave response. */
int c_ps_spi_std_slave_set_resp (
    PromiraChannelHandle channel,
    u16                  num_bytes,
    const u08 *          resp
);


/*
 * Polling function to check if there are any asynchronous messages
 * pending for processing.  The function takes a timeout value in units
 * of milliseconds.  If the timeout is < 0, the function will block
 * until data is received.  If the timeout is 0, the function will
 * perform a non-blocking check.
 */
enum PromiraSpiSlaveStatus {
    PS_SPI_SLAVE_NO_DATA   = 0x00,
    PS_SPI_SLAVE_DATA      = 0x01,
    PS_SPI_SLAVE_DATA_LOST = 0x80
};
#ifndef __cplusplus
typedef enum PromiraSpiSlaveStatus PromiraSpiSlaveStatus;
#endif

int c_ps_spi_slave_poll (
    PromiraChannelHandle channel,
    int                  timeout
);


struct PromiraSpiSlaveReadInfo {
    u32 in_data_bits;
    u32 out_data_bits;
    u08 header_bits;
    u08 resp_id;
    u08 ss_mask;
    u08 is_last;
};
#ifndef __cplusplus
typedef struct PromiraSpiSlaveReadInfo PromiraSpiSlaveReadInfo;
#endif

/*
 * Read the data from an SPI slave reception.  Returns number of bytes
 * put into data_in.
 */
int c_ps_spi_slave_read (
    PromiraChannelHandle      channel,
    PromiraSpiSlaveReadInfo * read_info,
    u32                       in_num_bytes,
    u08 *                     data_in
);


/*
 * Return number of SPI transactions lost due to the overflow of the
 * internal Promira buffer.
 */
int c_ps_spi_slave_data_lost_stats (
    PromiraChannelHandle channel
);



/*=========================================================================
| GPIO API
 ========================================================================*/
#define PS_MODULE_ID_GPIO 0x00000003
enum PromiraGpioCommand {
    PS_GPIO_CMD_DIRECTION = 300,
    PS_GPIO_CMD_GET       = 301,
    PS_GPIO_CMD_SET       = 302,
    PS_GPIO_CMD_CHANGE    = 303,
    PS_GPIO_CMD_DELAY_MS  = 304
};
#ifndef __cplusplus
typedef enum PromiraGpioCommand PromiraGpioCommand;
#endif

/* Returns which GPIO pins are currently active. */
int c_ps_gpio_query (
    PromiraChannelHandle channel
);


/*
 * Configure the GPIO, specifying the direction of each bit.
 *
 * A call to this function will not change the value of the pullup
 * mask in the Promira.  This is illustrated by the following
 * example:
 *   (1) Direction mask is first set to 0x00
 *   (2) Pullup is set to 0x01
 *   (3) Direction mask is set to 0x01
 *   (4) Direction mask is later set back to 0x00.
 *
 * The pullup will be active after (4).
 *
 * On Promira power-up, the default value of the direction
 * mask is 0x00.
 */
#define PS_GPIO_DIR_INPUT 0
#define PS_GPIO_DIR_OUTPUT 1
int c_ps_gpio_direction (
    PromiraChannelHandle channel,
    u32                  direction_mask
);


int c_ps_queue_gpio_direction (
    PromiraQueueHandle queue,
    u32                direction_mask
);


/*
 * Read the current digital values on the GPIO input lines.
 *
 * If a line is configured as an output, its corresponding bit
 * position in the mask will be undefined.
 */
int c_ps_gpio_get (
    PromiraChannelHandle channel
);


int c_ps_queue_gpio_get (
    PromiraQueueHandle queue
);


/*
 * Set the outputs on the GPIO lines.
 *
 * Note: If a line is configured as an input, it will not be
 * affected by this call, but the output value for that line
 * will be cached in the event that the line is later
 * configured as an output.
 */
int c_ps_gpio_set (
    PromiraChannelHandle channel,
    u32                  value
);


int c_ps_queue_gpio_set (
    PromiraQueueHandle queue,
    u32                value
);


/*
 * Block until there is a change on the GPIO input lines.
 * Pins configured as outputs will be ignored.
 *
 * The function will return either when a change has occurred or
 * the timeout expires.  If the timeout expires, this function
 * will return the current state of the GPIO lines.
 *
 * If the function ps_gpio_get is called before calling
 * ps_gpio_change, ps_gpio_change will only register any changes
 * from the value last returned by ps_gpio_get.
 */
int c_ps_gpio_change (
    PromiraChannelHandle channel,
    int                  timeout
);


int c_ps_queue_gpio_change (
    PromiraQueueHandle queue,
    int                timeout
);



/*=========================================================================
| Serial PHY Module API
 ========================================================================*/
#define PS_MODULE_ID_PHY 0x000000ff
/*
 * Configure the device by enabling/disabling I2C, SPI, and
 * GPIO functions.
 */
#define PS_APP_CONFIG_GPIO 0x00000000
#define PS_APP_CONFIG_SPI 0x00000001
#define PS_APP_CONFIG_I2C 0x00000010
#define PS_APP_CONFIG_QUERY -1
int c_ps_app_configure (
    PromiraChannelHandle channel,
    int                  config
);


/* Configure the I2C pullup resistors. */
#define PS_I2C_PULLUP_NONE 0x00
#define PS_I2C_PULLUP_BOTH 0x03
#define PS_I2C_PULLUP_QUERY 0x80
int c_ps_i2c_pullup (
    PromiraChannelHandle channel,
    u08                  pullup_mask
);


/* Configure the target power pins. */
#define PS_PHY_TARGET_POWER_NONE 0x00
#define PS_PHY_TARGET_POWER_TARGET1_5V 0x01
#define PS_PHY_TARGET_POWER_TARGET1_3V 0x05
#define PS_PHY_TARGET_POWER_TARGET2 0x02
#define PS_PHY_TARGET_POWER_BOTH 0x03
#define PS_PHY_TARGET_POWER_QUERY 0x80
int c_ps_phy_target_power (
    PromiraChannelHandle channel,
    u08                  power_mask
);


/* Configure the power of output signal. */
#define PS_PHY_LEVEL_SHIFT_QUERY -1
f32 c_ps_phy_level_shift (
    PromiraChannelHandle channel,
    f32                  level
);




#ifdef __cplusplus
}
#endif

#endif  /* __promact_is_h__ */
