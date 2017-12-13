/*
 *                   Copyright 2016 by KVASER AB, SWEDEN
 *                        WWW: http://www.kvaser.com
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license.
 */

/**
* \file kvlclib.h
* \brief   Library for converting Kvaser Memorator log files.
*
*/

/**
 * \defgroup grp_kvlc kvlclib
 * \brief The kvlclib is used for converting Kvaser Memorator log files.
 * \defgroup kvlc_converter Converter
 * \ingroup grp_kvlc
 * \defgroup kvlc_output Output Formats
 * \ingroup grp_kvlc
 * \defgroup kvlc_memorator Memorator
 * \ingroup grp_kvlc
 * \defgroup kvlc_database Database
 * \ingroup grp_kvlc
 */


#ifndef KVLCLIB_H
#define KVLCLIB_H

#if defined(WIN32)
#include <windows.h>
#else
#include <stdint.h>
#include <stddef.h>
#define WINAPI
#endif

#include "kvaDbLib.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * Type definitions for converter
 * @{
 */
typedef int KvlcStatus;
typedef void *KvlcHandle;
#if defined(WIN32)
typedef signed   __int64 time_int64;
typedef unsigned __int64 time_uint64;
#else
typedef uint64_t time_uint64;
typedef int64_t  time_int64;
#endif
 /** @} */

/**
* \name KvlcStatus
* \anchor kvlcERR_xxx
*
* Generally, a return code greater than or equal to zero means success. A
* value less than zero means failure.
*
* @{
*/
#define kvlcOK                          ((KvlcStatus) 0)  ///< OK.
#define kvlcFail                        ((KvlcStatus)-1)  ///< Generic error.
#define kvlcERR_PARAM                   ((KvlcStatus)-2)  ///< Error in supplied parameters.
#define kvlcEOF                         ((KvlcStatus)-3)  ///< End of input file reached.
#define kvlcERR_NOT_IMPLEMENTED         ((KvlcStatus)-4)  ///< Not implemented.
#define kvlcERR_FILE_ERROR              ((KvlcStatus)-5)  ///< File I/O error.
#define kvlcERR_FILE_EXISTS             ((KvlcStatus)-6)  ///< Output file already exists.
#define kvlcERR_INTERNAL_ERROR          ((KvlcStatus)-7)  ///< Unhandled internal error.
#define kvlcERR_NULL_POINTER            ((KvlcStatus)-8)  ///< Unexpected null pointer.
#define kvlcERR_FILE_TOO_LARGE          ((KvlcStatus)-10) ///< File size too large for specified format.
#define kvlcERR_TYPE_MISMATCH           ((KvlcStatus)-11) ///< Supplied parameter has incorrect type.
#define kvlcERR_NO_FREE_HANDLES         ((KvlcStatus)-12) ///< Too many open KvlcHandle handles.
#define kvlcERR_NO_INPUT_SELECTED       ((KvlcStatus)-13) ///< Missing call to kvlcSetInputFile or kvlcFeedSelectFormat.
#define kvlcERR_CONVERTING              ((KvlcStatus)-14) ///< Call failed since conversion is running.
#define kvlcERR_BUFFER_SIZE             ((KvlcStatus)-15) ///< The supplied buffer is too small to hold the result.

#define kvlcERR_INVALID_LOG_EVENT       ((KvlcStatus)-30) ///< Event is unknown to converter.
#define kvlcERR_NO_TIME_REFERENCE       ((KvlcStatus)-31) ///< Required timestamp missing.
#define kvlcERR_TIME_DECREASING         ((KvlcStatus)-32) ///< Decreasing time between files.
#define kvlcERR_MIXED_ENDIANNESS        ((KvlcStatus)-33) ///< Wrong data type in MDF.

/** @} */

/**
 * \name FILE_FORMAT_xxx
 * \anchor FILE_FORMAT_xxx
 *
 * Format used for input and output, used in kvlcSetInputFile(),
 * kvlcFeedSelectFormat().
 *
 * \note Not all formats are valid as both output and input format.
 *
 * @{
 */
#define KVLC_FILE_FORMAT_KME24       1 ///< Input and output file format.
#define KVLC_FILE_FORMAT_KME25       2 ///< Input and output file format.
#define KVLC_FILE_FORMAT_VECTOR_ASC  3 ///< Output file format.
#define KVLC_FILE_FORMAT_CSV         4 ///< Output file format.
#define KVLC_FILE_FORMAT_PLAIN_ASC   5 ///< Output file format.
#define KVLC_FILE_FORMAT_MEMO_LOG    6 ///< Input (internal device logfile format).
#define KVLC_FILE_FORMAT_KME40       7 ///< Input and output file format.
#define KVLC_FILE_FORMAT_VECTOR_BLF  8 ///< Output file format.
#define KVLC_FILE_FORMAT_KME50       9 ///< Input and output file format.

#define KVLC_FILE_FORMAT_CSV_SIGNAL    100 ///< Output file format.
#define KVLC_FILE_FORMAT_MDF           101 ///< Output file format.
#define KVLC_FILE_FORMAT_MATLAB        102 ///< Output file format.
#define KVLC_FILE_FORMAT_J1587         103 ///< Output file format.
#define KVLC_FILE_FORMAT_J1587_ALT     104 ///< Obsolete.
#define KVLC_FILE_FORMAT_FAMOS         105 ///< Output file format.
#define KVLC_FILE_FORMAT_MDF_SIGNAL    106 ///< Output file format.
#define KVLC_FILE_FORMAT_MDF_4X        107 ///< Output file format.
#define KVLC_FILE_FORMAT_MDF_4X_SIGNAL 108 ///< Output file format.

#define KVLC_FILE_FORMAT_XCP         200 ///< Not supported.
#define KVLC_FILE_FORMAT_FAMOS_XCP   201 ///< Not supported.

#define KVLC_FILE_FORMAT_DEBUG       1000 ///< Reserved for debug.
/** @} */


/**
 * \name PROPERTY_xxx
 * \anchor PROPERTY_xxx
 *
 * Converter properties used in the output format. These are used in
 * kvlcSetProperty(), kvlcGetProperty(), kvlcGetWriterPropertyDefault() and
 * kvlcIsPropertySupported().
 *
 * @{
 */

#define KVLC_PROPERTY_START_OF_MEASUREMENT          1     ///< (int) Use start of measurement as time reference.
#define KVLC_PROPERTY_FIRST_TRIGGER                 2     ///< (int) Use first trigger as time reference.
#define KVLC_PROPERTY_USE_OFFSET                    3     ///< (int) Use offset as time reference.
#define KVLC_PROPERTY_OFFSET                        4     ///< (time_int64) Time reference offset.
#define KVLC_PROPERTY_CHANNEL_MASK                  5     ///< (unsigned int) Bitmask of the channels that should be used during conversion.

#define KVLC_PROPERTY_HLP_J1939                     6     ///< (int) Interpret events as J1939.
#define KVLC_PROPERTY_CALENDAR_TIME_STAMPS          7     ///< (int) Write calendar time stamps.
#define KVLC_PROPERTY_WRITE_HEADER                  8     ///< (int) Write informational header.
#define KVLC_PROPERTY_SEPARATOR_CHAR                9     ///< (char) Use token as separator.
#define KVLC_PROPERTY_DECIMAL_CHAR                  10    ///< (char) Use token as decimal separator.

#define KVLC_PROPERTY_ID_IN_HEX                     11    ///< (int) Write id in hexadecimal format.
#define KVLC_PROPERTY_DATA_IN_HEX                   12    ///< (int) Write data in hexadecimal format.
#define KVLC_PROPERTY_NUMBER_OF_TIME_DECIMALS       13    ///< (int) Number of time decimals (0-9).
#define KVLC_PROPERTY_NAME_MANGLING                 14    ///< (int) Make signal names safe for use in Matlab.
#define KVLC_PROPERTY_FILL_BLANKS                   15    ///< (int) Propagate values down to next row in csv-files.

#define KVLC_PROPERTY_SHOW_UNITS                    16    ///< (int) Show units on their own row.
#define KVLC_PROPERTY_ISO8601_DECIMALS              17    ///< (int) Number of time decimals (0-9) to print in the calendar timestamps using ISO8601.
#define KVLC_PROPERTY_MERGE_LINES                   18    ///< (int) Merge two lines if their signal values are equal.
#define KVLC_PROPERTY_RESAMPLE_COLUMN               19    ///< (int) Only print a line when the given column has been accessed.
#define KVLC_PROPERTY_VERSION                       20    ///< (int) File format version.

#define KVLC_PROPERTY_SHOW_COUNTER                  21    ///< (int) Add a counter to the output.
#define KVLC_PROPERTY_CROP_PRETRIGGER               22    ///< (int) Crop pre-triggers.
#define KVLC_PROPERTY_ENUM_VALUES                   23    ///< (int) Replace integer values in signals with strings from database.
#define KVLC_PROPERTY_SIZE_LIMIT                    24    ///< (unsigned int) Maximum file size in megabytes before starting a new output file.
#define KVLC_PROPERTY_TIME_LIMIT                    25    ///< (unsigned int) Maximum delta time in seconds between first and last event before starting a new output file.

#define KVLC_PROPERTY_LIMIT_DATA_BYTES              26    ///< (int) Number of data bytes that a converter will write.
#define KVLC_PROPERTY_CREATION_DATE                 27    ///< (time_int64) File creation date/time as seconds in standard UNIX format. Used in file headers if not zero
#define KVLC_PROPERTY_OVERWRITE                     28    ///< (int) Overwrite existing output files
#define KVLC_PROPERTY_TIMEZONE                      29    ///< (int) Timezone for absolute timestamps

#define KVLC_PROPERTY_SIGNAL_BASED                  1001  ///< Writes signals and not data frames. Used only with kvlcIsPropertySupported().
#define KVLC_PROPERTY_SHOW_SIGNAL_SELECT            1002  ///< Requires a database. Used only with kvlcIsPropertySupported().
#define KVLC_PROPERTY_ATTACHMENTS                   1003  ///< Can attach files to converted data. Use kvlcAttachFile() to add a file. Used only with kvlcIsPropertySupported().

/** @} */


/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status GetErrorText(Status error, out String^ buf);</b>
 * \source_end
 *
 * Convert a \ref KvlcStatus error code to a text.
 *
 * \param[in]  error         The error code to convert.
 * \param[out]  buffer       Buffer to receive error text, a pointer
 *                           to a \c NULL terminated array of chars.
 * \param[in]  buffer_size   Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 */
KvlcStatus WINAPI kvlcGetErrorText(KvlcStatus error, char *buffer, unsigned int buffer_size);


/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status GetVersion(out Int32 major, out Int32 minor, out Int32 build);</b>
 * \source_end
 *
 * Returns the version of the kvcnvlib DLL.
 *
 * \param[out]  major         Major version number.
 * \param[out]  minor         Minor version number.
 * \param[out]  build         Build version number.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 */
KvlcStatus WINAPI kvlcGetVersion(unsigned int *major, unsigned int *minor, unsigned int *build);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status CreateConverter(out Handle^ hnd, String^ filename, FileFormat format);</b>
 * \source_end
 *
 * Create a new converter.
 *
 * \param[out]  handle         An open handle to a converter.
 * \param[in]   filename       Output filename, a pointer
 *                             to a \c NULL terminated array of chars.
 * \param[in]   format         Output format, use \ref FILE_FORMAT_xxx.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \note No more than 128 converter handles can be open at the same time.
 * \sa kvlcDeleteConverter()
 */
KvlcStatus WINAPI kvlcCreateConverter(KvlcHandle *handle, const char *filename, int format);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status DeleteConverter(Handle^ hnd);</b>
 * \source_end
 *
 * Delete a converter and close all files.
 *
 * \param[in]  handle         An open handle to a converter.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcCreateConverter()
 */
KvlcStatus WINAPI kvlcDeleteConverter(KvlcHandle handle);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status EventCount(Handle^ hnd, out UInt32 count);</b>
 * \source_end
 *
 * Get the estimated number of remaining events in the input file. This can be
 * useful for displaying progress during conversion.
 *
 * \param[in]  handle         An open handle to a converter.
 * \param[out]  count         The estimated number of remaining events in the input file.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 */
KvlcStatus WINAPI kvlcEventCount(KvlcHandle handle, unsigned int *count);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status ConvertEvent(Handle^ hnd);</b>
 * \source_end
 *
 * Convert one event from input file and write it to output file.
 *
 * \param[in]  handle         An open handle to a converter.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 */
KvlcStatus WINAPI kvlcConvertEvent(KvlcHandle handle);


/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status SetInputFile(Handle^ hnd, String^ filename, FileFormat format);</b>
 * \source_end
 *
 * Select input file.
 *
 * \param[in]  handle       An open handle to a converter.
 * \param[in]  filename     Input filename, a pointer
 *                          to a \c NULL terminated array of chars.
 * \param[in]  format       Input format, use \ref FILE_FORMAT_xxx.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcNextInputFile()
 */
KvlcStatus WINAPI kvlcSetInputFile(KvlcHandle handle, const char *filename, int format);


/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status NextInputFile(Handle^ hnd, String^ filename);</b>
 * \source_end
 *
 * Select next input file.
 *
 * \note It is the responsibility of the caller to arrange multiple files in
 * correct time stamp order. Overlapping time stamps are not handled.
 *
 * \param[in]  handle       An open handle to a converter.
 * \param[in]  filename     Next input filename, a pointer
 *                          to a \c NULL terminated array of chars.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcSetInputFile()
 */
KvlcStatus WINAPI kvlcNextInputFile(KvlcHandle handle, const char *filename);

/**
 * \ingroup kvlc_memorator
 *
 * \source_cs <b>static Kvlclib.Status FeedSelectFormat(Handle^ hnd, FileFormat format);</b>
 * \source_end
 *
 * Select feed format. Used when reading log files directly from device.
 *
 * E.g. use this function with KVLC_FILE_FORMAT_MEMO_LOG when using
 * KVMLIB to read events from a Kvaser Memorator connected to USB.
 *
 * \param[in]  handle       An open handle to a converter.
 * \param[in]  format       Input format, use \ref FILE_FORMAT_xxx.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcFeedLogEvent(), kvlcFeedNextFile()
 */
KvlcStatus WINAPI kvlcFeedSelectFormat(KvlcHandle handle, int format);

/**
 * \ingroup kvlc_memorator
 *
 * \source_cs <b>static Kvlclib.Status FeedLogEvent(Handle^ hnd, Kvmlib::Log^ event);</b>
 * \source_end
 *
 * Feed one event to the converter. Used when reading log files directly from device.
 *
 * \note kvlcFeedLogEvent cannot be used along side a set input file,  see \ref kvlcSetInputFile.
 *
 * \param[in]  handle       An open handle to a converter.
 * \param[in]  event        Pointer to an event that matches the selected
 *                          input format.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcFail (-1) if an input file has been set, see \ref kvlcSetInputFile.
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcFeedSelectFormat(), kvlcFeedNextFile(), kvlcIsOverrunActive()
 */
KvlcStatus WINAPI kvlcFeedLogEvent(KvlcHandle handle, void *event);

/**
 * \ingroup kvlc_memorator
 *
 * \source_cs <b>static Kvlclib.Status FeedNextFile(Handle^ hnd);</b>
 * \source_end
 *
 * Notify the converter that next event in kvlcFeedLogEvent() will come
 * from another file. Used when reading log files directly from device.
 *
 * E.g. use this function with KVLC_FILE_FORMAT_MEMO_LOG when using
 * KVMLIB to read events from a Kvaser Memorator connected to USB.
 *
 * \param[in]  handle       An open handle to a converter.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcFeedLogEvent(), kvlcFeedSelectFormat()
 */
KvlcStatus WINAPI kvlcFeedNextFile(KvlcHandle handle);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status IsOutputFilenameNew(Handle^ hnd, out Int32 updated);</b>
 * \source_end
 *
 * Check if the converter has created a new file. It is only true once after
 * a new file has been created. Used when splitting output into multiple files.
 *
 * \param[in]  handle       An open handle to a converter.
 * \param[out] updated      Not zero if a new file has been created.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetOutputFilename()
 */
KvlcStatus WINAPI kvlcIsOutputFilenameNew(KvlcHandle handle, int *updated);


/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status GetOutputFilename(Handle^ hnd, out String^ filename);</b>
 * \source_end
 *
 * Get the filename of the current output file.
 *
 * This function can be used in conjunction with kvlcIsOutputFilenameNew() to
 * report current filename when splitting output into multiple files.
 *
 * \param[in]  handle       An open handle to a converter.
 * \param[out] filename     Buffer to receive output filename, a pointer
 *                          to a \c NULL terminated array of chars.
 * \param[in]  len          Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcIsOutputFilenameNew()
 */
KvlcStatus WINAPI kvlcGetOutputFilename(KvlcHandle handle, char *filename, int len);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs
 * <b>static Kvlclib.Status GetWriterPropertyDefaultInt(FileFormat format, Property property, out Int32 value);</b><br>
 * <b>static Kvlclib.Status GetWriterPropertyDefaultUInt32(FileFormat format, Property property, out UInt32 value);</b><br>
 * <b>static Kvlclib.Status GetWriterPropertyDefaultChar(FileFormat format, Property property, out Char value);</b><br>
 * <b>static Kvlclib.Status GetWriterPropertyDefaultTime64(FileFormat format, Property property, out Int64 value);</b>
 * \source_end
 *
 * Get the default value for a property. The buffer pointer and length of the
 * data type must match the property specified in \ref PROPERTY_xxx, e.g.
 *
 * time_int64 offset;
 * kvlcGetWriterPropertyDefault(KVLC_PROPERTY_OFFSET, &offset, sizeof(offset));
 *
 * \param[in]   format       Output format, use \ref FILE_FORMAT_xxx.
 * \param[in]   property     Property, use PROPERTY_xxx.
 * \param[out]  buf          Buffer to receive property value.
 * \param[in]   len          Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetFirstWriterFormat(), kvlcIsPropertySupported(), kvlcGetProperty()
 */
KvlcStatus WINAPI kvlcGetWriterPropertyDefault(int format, unsigned int property, void *buf, unsigned int len);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs
 * <b>static Kvlclib.Status SetPropertyInt(Handle^ hnd, Property property, Int32 value);</b><br>
 * <b>static Kvlclib.Status SetPropertyUInt32(Handle^ hnd, Property property, UInt32 value);</b><br>
 * <b>static Kvlclib.Status SetPropertyChar(Handle^ hnd, Property property, Char value);</b><br>
 * <b>static Kvlclib.Status SetPropertyTime64(Handle^ hnd, Property property, Int64 value);</b>
 * \source_end
 *
 * Set a property value. The buffer pointer and length of the
 * data type must match the property specified in \ref PROPERTY_xxx, e.g.
 *
 * int use_offset = 1;
 * kvlcSetProperty(hnd, PROPERTY_USE_OFFSET, &use_offset, sizeof(use_offset));
 *
 * \param[in]   handle       An open handle to a converter.
 * \param[in]   property     Property, use PROPERTY_xxx.
 * \param[in]   buf          Buffer containing property value.
 * \param[in]   len          Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcIsPropertySupported(), kvlcGetWriterPropertyDefault(), kvlcGetProperty()
 */
KvlcStatus WINAPI kvlcSetProperty(KvlcHandle handle, unsigned int property, void *buf, unsigned int len);


/**
 * \ingroup kvlc_converter
 *
 * Get a property value. The buffer pointer and length of the
 * data type must match the property specified in \ref PROPERTY_xxx, e.g.
 *
 * \source_cs
 * <b>static Kvlclib.Status GetPropertyInt(Handle^ hnd, Property property, out Int32 value);</b><br>
 * <b>static Kvlclib.Status GetPropertyUInt32(Handle^ hnd, Property property, out UInt32 value);</b><br>
 * <b>static Kvlclib.Status GetPropertyChar(Handle^ hnd, Property property, out Char value);</b><br>
 * <b>static Kvlclib.Status GetPropertyTime64(Handle^ hnd, Property property, out Int64 value);</b>
 * \source_end
 *
 * char sep;
 * kvlcGetProperty(hnd, PROPERTY_SEPARATOR_CHAR, &sep, sizeof(sep));
 *
 * \param[in]   handle       An open handle to a converter.
 * \param[in]   property     Property, use PROPERTY_xxx.
 * \param[out]  buf          Buffer to receive property value.
 * \param[in]   len          Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcIsPropertySupported(), kvlcSetProperty(), kvlcGetWriterPropertyDefault()
 */
KvlcStatus WINAPI kvlcGetProperty(KvlcHandle handle, unsigned int property, void *buf, unsigned int len);


/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status IsOverrunActive(Handle^ hnd, out Int32 overrun);</b>
 * \source_end
 *
 * Get overrun status.
 *
 * Overruns can occur during logging with a Memorator if the bus load exceeds
 * the logging capacity. This is very unusual, but can occur if a Memorator
 * runs complex scripts and triggers.
 *
 * \param[in]   handle       An open handle to a converter.
 * \param[out]  overrun      Not zero if an overrun occurred.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcIsDataTruncated()
 */
KvlcStatus WINAPI kvlcIsOverrunActive(KvlcHandle handle, int *overrun);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status ResetOverrunActive(Handle^ hnd);</b>
 * \source_end
 *
 * Reset overrun status.
 *
 * \param[in]   handle       An open handle to a converter.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcIsOverrunActive()
 */
KvlcStatus WINAPI kvlcResetOverrunActive(KvlcHandle handle);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status IsDataTruncated(Handle^ hnd, out Int32 truncated);</b>
 * \source_end
 *
 * Get truncation status.
 *
 * Truncation occurs when the selected output converter can't write the all bytes
 * in a data frame to file. This can happen if CAN FD data is extracted to a format
 * that only supports up to 8 data bytes, e.g. KVLC_FILE_FORMAT_KME40.
 *
 * It can also happen if PROPERTY_LIMIT_DATA_BYTES is set to limit the number
 * of data bytes in output.
 *
 * \param[in]   handle       An open handle to a converter.
 * \param[out]  truncated    Not zero if data is truncated.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcIsOverrunActive(), kvlcResetDataTruncated()
 */
KvlcStatus WINAPI kvlcIsDataTruncated (KvlcHandle handle,  int *truncated);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status ResetDataTruncated(Handle^ hnd);</b>
 * \source_end
 *
 * Reset truncation status.
 *
 * \param[in]   handle       An open handle to a converter.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcIsDataTruncated()
 */
KvlcStatus WINAPI kvlcResetDataTruncated (KvlcHandle handle);

/**
 * \ingroup kvlc_converter
 *
 * \source_cs <b>static Kvlclib.Status AttachFile(Handle^ hnd, String^ filename);</b>
 * \source_end
 *
 * Attach a file to be included in the output file, e.g. used to add a database
 * or a movie to the output.
 *
 * \note The output format must support the property PROPERTY_ATTACHMENTS.
 *
 * \param[in]  handle       An open handle to a converter.
 * \param[out] filename     File to be included in output, a pointer
 *                          to a \c NULL terminated array of chars.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx  (negative) if failure
 *
 * \sa kvlcIsPropertySupported()
 */
KvlcStatus WINAPI kvlcAttachFile(KvlcHandle handle, const char *filename);


/**
 * \ingroup kvlc_output
 *
 * \source_cs <b>static Kvlclib.Status GetFirstWriterFormat(out FileFormat format);</b>
 * \source_end
 *
 * Get the first supported output format.
 *
 * \param[out]  format       First output format.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetNextWriterFormat()
 */
KvlcStatus WINAPI kvlcGetFirstWriterFormat(int *format);


/**
 * \ingroup kvlc_output
 *
 * \source_cs <b>static Kvlclib.Status GetNextWriterFormat(FileFormat currentFormat, out FileFormat nextFormat);</b>
 * \source_end
 *
 * Get the next supported output format.
 *
 * \param[out]  currentFormat    Current output format.
 * \param[out]  nextFormat       Next output format. Zero if no more formats are
 *                               to be found.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetFirstWriterFormat()
 */
KvlcStatus WINAPI kvlcGetNextWriterFormat(int currentFormat, int *nextFormat);


/**
 * \ingroup kvlc_output
 *
 * \source_cs <b>static Kvlclib.Status GetWriterName(FileFormat format, out String^ str);</b>
 * \source_end
 *
 * Get output converter name.
 *
 * \param[in]   format       Output format.
 * \param[out]  str          Buffer to receive name, a pointer
 *                           to a \c NULL terminated array of chars.
 * \param[in]   len          Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetFirstWriterFormat()
 */
KvlcStatus WINAPI kvlcGetWriterName(int format, char *str, unsigned int len);


/**
 * \ingroup kvlc_output
 *
 * \source_cs <b>static Kvlclib.Status GetWriterExtension(FileFormat format, out String^ str);</b>
 * \source_end
 *
 * Get output converter file extension.
 *
 * \param[in]   format       Output format.
 * \param[out]  str          Buffer to receive extension, a pointer
 *                           to a \c NULL terminated array of chars.
 * \param[in]   len          Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetFirstWriterFormat()
 */
KvlcStatus WINAPI kvlcGetWriterExtension(int format, char *str, unsigned int len);


/**
 * \ingroup kvlc_output
 *
 * \source_cs <b>static Kvlclib.Status GetWriterDescription(FileFormat format, out String^ str);</b>
 * \source_end
 *
 * Get output converter file description.
 *
 * \param[in]   format       Output format.
 * \param[out]  str          Buffer to receive description, a pointer
 *                           to a \c NULL terminated array of chars.
 * \param[in]   len          Buffer size in bytes.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetFirstWriterFormat()
 */
KvlcStatus WINAPI kvlcGetWriterDescription(int format, char *str, unsigned int len);


/**
 * \ingroup kvlc_output
 *
 * \source_cs <b>static Kvlclib.Status IsPropertySupported(FileFormat format, Property property, out Int32 supported);</b>
 * \source_end
 *
 * Check if property is supported by output format.
 *
 * \param[in]   format       Output format, use \ref FILE_FORMAT_xxx.
 * \param[out]  property     Property, use \ref PROPERTY_xxx.
 * \param[in]   supported    Not zero if format supports property.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcGetFirstWriterFormat()
 */
KvlcStatus WINAPI kvlcIsPropertySupported(int format, unsigned int property, int *supported);

/**
 * \ingroup kvlc_database
 *
 * \source_cs <b>static Kvlclib.Status AddDatabaseFile(Handle^ hnd, String^ filename, UInt32 channelMask);</b>
 * \source_end
 *
 * Add a database file. Output formats with  the property PROPERTY_SIGNAL_BASED
 * will match events against all entries in the database and write signals to
 * the output file.
 *
 * \note Data conversion is faster with fewer signals and fewer active channels.
 *
 * \param[in]   handle       An open handle to a converter.
 * \param[in]   filename     Buffer containing database name, a pointer
 *                           to a \c NULL terminated array of chars.
 * \param[in]   channelMask  Bitmask with active channels.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcAddDatabase()
 */
KvlcStatus WINAPI kvlcAddDatabaseFile(KvlcHandle handle, const char *filename, unsigned int channelMask);

/**
 * \ingroup kvlc_database
 *
 * \source_cs <b>static Kvlclib.Status AddDatabase(Handle^ hnd, Kvadblib::Hnd^ dbHandle, UInt32 channelMask);</b>
 * \source_end
 *
 * Add a database handle. Output formats with the property PROPERTY_SIGNAL_BASED
 * will match events against all entries in the database and write signals to
 * the output file.
 *
 * \note Data conversion is faster with fewer signals and fewer active channels.
 *
 * \param[in]   handle       An open handle to a converter.
 * \param[in]   dbHandle     A database handle created with kvadblib.
 * \param[in]   channelMask  Bitmask with active channels.
 *
 * \return \ref kvlcOK (zero) if success
 * \return \ref kvlcERR_xxx (negative) if failure
 *
 * \sa kvlcAddDatabaseFile(), kvlcDeleteConverter()
 */
KvlcStatus WINAPI kvlcAddDatabase(KvlcHandle handle, KvaDbHnd dbHandle, unsigned int channelMask);

/**
 * \page page_kvlclib Converter API (kvlclib)
 *
 * \tableofcontents
 *
 * \section section_user_guide_kvlclib_1 Description
 *  Library for converting Kvaser Memorator log files.
 *
 * \section section_user_guide_kvlclib_2 Naming convention
 *  The function calls in the API all have names starting with
 *  <b>kvlc</b>, e.g \ref kvlcAddDatabase ().
 *
 * \section section_user_guide_kvlclib_3 Build an application
 *     You will need to
 *     \code
 *       #include kvlclib.h
 *     \endcode
 *
 *     in your programs and link with kvlclib.dll on Windows and with libkvlclib.so on Linux.
 *
 *
 *    \note
 *     On Windows kvlclib.dll is dependent on kvaDbLib.dll.\n
 *     On Linux libkvlclib.so is dependent on libkvadblib.so.
 */

#ifdef __cplusplus
};
#endif

#endif // KVLCLIB_H
