/*
 * mag_io.c
 *
 *  Created on: Mar 29, 2018
 *      Author: djdup
 */

#include "mag_io.h"
#include "adcs_sensorproc_ids.h"
#include "sensors/magnetometer.h"

#include "core/i2c.h"
#include "core/utils.h"
#include "core/dataArray.h"
#include "interfaces/canwrap.h"

#include "autocode/MSP_SP.h"
#include "autocode/rtwtypes.h"

// rollcall arrays
typedef struct {
    int16_t mag1xData[RC_BUFFER_SIZE];
    int16_t mag1yData[RC_BUFFER_SIZE];
    int16_t mag1zData[RC_BUFFER_SIZE];
    int16_t mag2xData[RC_BUFFER_SIZE];
    int16_t mag2yData[RC_BUFFER_SIZE];
    int16_t mag2zData[RC_BUFFER_SIZE];
    int16_t magpxData[RC_BUFFER_SIZE];
    int16_t magpyData[RC_BUFFER_SIZE];
    int16_t magpzData[RC_BUFFER_SIZE];
    uint8_t magValidData[RC_BUFFER_SIZE];
    uint16_t mag1xHandle;
    uint16_t mag1yHandle;
    uint16_t mag1zHandle;
    uint16_t mag2xHandle;
    uint16_t mag2yHandle;
    uint16_t mag2zHandle;
    uint16_t magpxHandle;
    uint16_t magpyHandle;
    uint16_t magpzHandle;
    uint16_t magValidHandle;
} mag_rc;

#pragma NOINIT(rc)
FILE_STATIC mag_rc rc;

FILE_STATIC hMag mag1;
FILE_STATIC hMag mag2;
FILE_STATIC MagnetometerData *data1;
FILE_STATIC MagnetometerData *data2;

#if !ENABLE_MAG1
    MagnetometerData mockData1;
#endif
#if !ENABLE_MAG2
    MagnetometerData mockData2;
#endif

void magioInit()
{
#if ENABLE_MAG1
    mag1 = magInit(MAG1_I2CBUS);
#endif
#if ENABLE_MAG2
    mag2 = magInit(MAG2_I2CBUS);
#endif
    rc.mag1xHandle = init_int16_t(rc.mag1xData, RC_BUFFER_SIZE);
    rc.mag1yHandle = init_int16_t(rc.mag1yData, RC_BUFFER_SIZE);
    rc.mag1zHandle = init_int16_t(rc.mag1zData, RC_BUFFER_SIZE);
    rc.mag2xHandle = init_int16_t(rc.mag2xData, RC_BUFFER_SIZE);
    rc.mag2yHandle = init_int16_t(rc.mag2yData, RC_BUFFER_SIZE);
    rc.mag2zHandle = init_int16_t(rc.mag2zData, RC_BUFFER_SIZE);
    rc.magpxHandle = init_int16_t(rc.magpxData, RC_BUFFER_SIZE);
    rc.magpyHandle = init_int16_t(rc.magpyData, RC_BUFFER_SIZE);
    rc.magpzHandle = init_int16_t(rc.magpzData, RC_BUFFER_SIZE);
    rc.magValidHandle = init_uint8_t(rc.magValidData, RC_BUFFER_SIZE);
}

FILE_STATIC uint8_t isValid(hMag handle)
{
    // TODO write a validity check
    return 1;
}

FILE_STATIC void setOutput(hMag mag, MagnetometerData *input, real32_T *output)
{
    // set autocode inputs
    output[0] = magConvertRawToTeslas(input->rawX);
    output[1] = magConvertRawToTeslas(input->rawY);
    output[2] = magConvertRawToTeslas(input->rawZ);
    output[3] = isValid(mag);
}

FILE_STATIC void update1()
{
#if ENABLE_MAG1
#ifdef __I2C_DONT_WRITE_MAG1__
    data1 = testing_magReadXYZData(mag1, ConvertToNone);
#else
    data1 = magReadXYZData(mag1, ConvertToNone);
#endif /* __I2C_DONT_WRITE_MAG1__ */
#else
    mockData1.rawX = 100;
    mockData1.rawY = 100;
    mockData1.rawZ = 100;
    data1 = &mockData1;
#endif

    setOutput(mag1, data1, rtU.mag1_vec_body_T);

    // update rollcall array
    addData_int16_t(rc.mag1xHandle, data1->rawX);
    addData_int16_t(rc.mag1yHandle, data1->rawY);
    addData_int16_t(rc.mag1zHandle, data1->rawZ);
}

FILE_STATIC void update2()
{
#if ENABLE_MAG2
#ifdef __I2C_DONT_WRITE_MAG2__
    data2 = testing_magReadXYZData(mag2, ConvertToNone);
#else
    data2 = magReadXYZData(mag2, ConvertToNone);
#endif /* __I2C_DONT_WRITE_MAG2__ */
#else
    mockData2.rawX = 300;
    mockData2.rawY = 300;
    mockData2.rawZ = 300;
    data2 = &mockData2;
#endif /* ENABLE_MAG2 */

    setOutput(mag2, data2, rtU.mag2_vec_body_T);

    // update rollcall array
    addData_int16_t(rc.mag2xHandle, data2->rawX);
    addData_int16_t(rc.mag2yHandle, data2->rawY);
    addData_int16_t(rc.mag2zHandle, data2->rawZ);
}

void magioUpdate()
{
    update1();
    update2();
}

void magioSendBackchannelVector()
{
    sensor_vector_segment s;
    s.x = rtY.mag_body_processed_T[0];
    s.y = rtY.mag_body_processed_T[1];
    s.z = rtY.mag_body_processed_T[2];
    s.valid = rtY.mag_body_processed_T[3];
    bcbinPopulateHeader(&s.header, TLM_ID_MAG_VECTOR, sizeof(s));
    bcbinSendPacket((uint8_t *) &s, sizeof(s));
}

FILE_STATIC void sendBackchannel(MagnetometerData *data, uint8_t tlmId)
{
    mag_segment seg;
    seg.x = data->rawX;
    seg.y = data->rawY;
    seg.z = data->rawZ;
    bcbinPopulateHeader(&seg.header, tlmId, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));
}

FILE_STATIC void magioSendBackchannel1()
{
    sendBackchannel(data1, TLM_ID_MAG1_RAW);
}

FILE_STATIC void magioSendBackchannel2()
{
    sendBackchannel(data2, TLM_ID_MAG2_RAW);
}

void magioSendBackchannel()
{
    magioSendBackchannel1();
    magioSendBackchannel2();
}

void magioSendCAN()
{
    // send packet
    sensorproc_mag mag;
    mag.sensorproc_mag_x = magConvertTeslasToRaw(rtY.mag_body_processed_T[0]);
    mag.sensorproc_mag_y = magConvertTeslasToRaw(rtY.mag_body_processed_T[1]);
    mag.sensorproc_mag_z = magConvertTeslasToRaw(rtY.mag_body_processed_T[2]);
    mag.sensorproc_mag_valid = rtY.mag_body_processed_T[3];
    CANPacket packet;
    encodesensorproc_mag(&mag, &packet);
    canSendPacket(&packet);

    // update rollcall arrays
    addData_int16_t(rc.magpxHandle, mag.sensorproc_mag_x);
    addData_int16_t(rc.magpyHandle, mag.sensorproc_mag_y);
    addData_int16_t(rc.magpzHandle, mag.sensorproc_mag_z);
    addData_uint8_t(rc.magValidHandle, mag.sensorproc_mag_valid);
}

void magioRcPopulate6(rc_adcs_sp_6 *r)
{
    r->rc_adcs_sp_6_magp_x_min = getMin_int16_t(rc.magpxHandle);
    r->rc_adcs_sp_6_magp_x_max = getMax_int16_t(rc.magpxHandle);
}

void magioRcPopulate7(rc_adcs_sp_7 *r)
{
    r->rc_adcs_sp_7_magp_x_avg = getAvg_int16_t(rc.magpxHandle);
    r->rc_adcs_sp_7_magp_y_min = getMin_int16_t(rc.magpyHandle);
    r->rc_adcs_sp_7_magp_y_max = getMax_int16_t(rc.magpyHandle);
    r->rc_adcs_sp_7_magp_y_avg = getAvg_int16_t(rc.magpyHandle);
}

void magioRcPopulate8(rc_adcs_sp_8 *r)
{
    r->rc_adcs_sp_8_magp_z_min = getMin_int16_t(rc.magpzHandle);
    r->rc_adcs_sp_8_magp_z_max = getMax_int16_t(rc.magpzHandle);
    r->rc_adcs_sp_8_magp_z_avg = getAvg_int16_t(rc.magpzHandle);
    r->rc_adcs_sp_8_mag1_x_min = getMin_int16_t(rc.mag1xHandle);
}

void magioRcPopulate9(rc_adcs_sp_9 *r)
{
    r->rc_adcs_sp_9_mag1_x_max = getMax_int16_t(rc.mag1xHandle);
    r->rc_adcs_sp_9_mag1_x_avg = getAvg_int16_t(rc.mag1xHandle);
    r->rc_adcs_sp_9_mag1_y_min = getMin_int16_t(rc.mag1yHandle);
    r->rc_adcs_sp_9_mag1_y_max = getMax_int16_t(rc.mag1yHandle);
}

void magioRcPopulate10(rc_adcs_sp_10 *r)
{
    r->rc_adcs_sp_10_mag1_y_avg = getAvg_int16_t(rc.mag1yHandle);
    r->rc_adcs_sp_10_mag1_z_min = getMin_int16_t(rc.mag1zHandle);
    r->rc_adcs_sp_10_mag1_z_max = getMax_int16_t(rc.mag1zHandle);
    r->rc_adcs_sp_10_mag1_z_avg = getAvg_int16_t(rc.mag1zHandle);
}

void magioRcPopulate11(rc_adcs_sp_11 *r)
{
    r->rc_adcs_sp_11_mag2_x_min = getMin_int16_t(rc.mag2xHandle);
    r->rc_adcs_sp_11_mag2_x_max = getMax_int16_t(rc.mag2xHandle);
    r->rc_adcs_sp_11_mag2_x_avg = getAvg_int16_t(rc.mag2xHandle);
    r->rc_adcs_sp_11_mag2_y_min = getMin_int16_t(rc.mag2yHandle);
}

void magioRcPopulate12(rc_adcs_sp_12 *r)
{
    r->rc_adcs_sp_12_mag2_y_max = getMax_int16_t(rc.mag2yHandle);
    r->rc_adcs_sp_12_mag2_y_avg = getAvg_int16_t(rc.mag2yHandle);
    r->rc_adcs_sp_12_mag2_z_min = getMin_int16_t(rc.mag2zHandle);
    r->rc_adcs_sp_12_mag2_z_max = getMax_int16_t(rc.mag2zHandle);
}

void magioRcPopulate13(rc_adcs_sp_13 *r)
{
    r->rc_adcs_sp_13_mag2_z_avg = getAvg_int16_t(rc.mag2zHandle);
}

void magioRcPopulate14(rc_adcs_sp_14 *r)
{
    r->rc_adcs_sp_14_magp_valid = getSum_uint8_t(rc.magValidHandle);
}
