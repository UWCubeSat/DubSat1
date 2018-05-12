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
#include "core/agglib.h"
#include "interfaces/canwrap.h"

#include "autocode/MSP_SP.h"
#include "autocode/rtwtypes.h"

// rollcall
FILE_STATIC aggVec_i rc_mag1x;
FILE_STATIC aggVec_i rc_mag1y;
FILE_STATIC aggVec_i rc_mag1z;
FILE_STATIC aggVec_i rc_mag2x;
FILE_STATIC aggVec_i rc_mag2y;
FILE_STATIC aggVec_i rc_mag2z;
FILE_STATIC aggVec_i rc_magpx;
FILE_STATIC aggVec_i rc_magpy;
FILE_STATIC aggVec_i rc_magpz;
FILE_STATIC aggVec_i rc_magValid;

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
    aggVec_init_i(&rc_mag1x);
    aggVec_init_i(&rc_mag1y);
    aggVec_init_i(&rc_mag1z);
    aggVec_init_i(&rc_mag2x);
    aggVec_init_i(&rc_mag2y);
    aggVec_init_i(&rc_mag2z);
    aggVec_init_i(&rc_magpx);
    aggVec_init_i(&rc_magpy);
    aggVec_init_i(&rc_magpz);
    aggVec_init_i(&rc_magValid);
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

    // update rollcall data
    aggVec_push_i(&rc_mag1x, data1->rawX);
    aggVec_push_i(&rc_mag1y, data1->rawY);
    aggVec_push_i(&rc_mag1z, data1->rawZ);
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
    aggVec_push_i(&rc_mag2x, data2->rawX);
    aggVec_push_i(&rc_mag2y, data2->rawY);
    aggVec_push_i(&rc_mag2z, data2->rawZ);
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
    aggVec_push_i(&rc_magpx, mag.sensorproc_mag_x);
    aggVec_push_i(&rc_magpy, mag.sensorproc_mag_y);
    aggVec_push_i(&rc_magpz, mag.sensorproc_mag_z);
    aggVec_push_i(&rc_magValid, mag.sensorproc_mag_valid);
}

void magioRcPopulate6(rc_adcs_sp_6 *r)
{
    r->rc_adcs_sp_6_magp_x_min = aggVec_min_i(&rc_magpx);
    r->rc_adcs_sp_6_magp_x_max = aggVec_max_i(&rc_magpx);
    aggVec_mm_reset((aggVec *) &rc_magpx);
}

void magioRcPopulate7(rc_adcs_sp_7 *r)
{
    r->rc_adcs_sp_7_magp_x_avg = aggVec_avg_i_i(&rc_magpx);
    aggVec_as_reset((aggVec *) &rc_magpx);
    r->rc_adcs_sp_7_magp_y_min = aggVec_min_i(&rc_magpy);
    r->rc_adcs_sp_7_magp_y_max = aggVec_max_i(&rc_magpy);
    r->rc_adcs_sp_7_magp_y_avg = aggVec_avg_i_i(&rc_magpy);
    aggVec_reset((aggVec *) &rc_magpx);
}

void magioRcPopulate8(rc_adcs_sp_8 *r)
{
    r->rc_adcs_sp_8_magp_z_min = aggVec_min_i(&rc_magpz);
    r->rc_adcs_sp_8_magp_z_max = aggVec_max_i(&rc_magpz);
    r->rc_adcs_sp_8_magp_z_avg = aggVec_avg_i_i(&rc_magpz);
    aggVec_reset((aggVec *) &rc_magpz);
    r->rc_adcs_sp_8_mag1_x_min = aggVec_min_i(&rc_mag1x);
    aggVec_min_reset((aggVec *) &rc_mag1x);
}

void magioRcPopulate9(rc_adcs_sp_9 *r)
{
    r->rc_adcs_sp_9_mag1_x_max = aggVec_max_i(&rc_mag1x);
    aggVec_max_reset((aggVec *) &rc_mag1x);
    r->rc_adcs_sp_9_mag1_x_avg = aggVec_avg_i_i(&rc_mag1x);
    aggVec_as_reset((aggVec *) &rc_mag1x);
    r->rc_adcs_sp_9_mag1_y_min = aggVec_min_i(&rc_mag1y);
    r->rc_adcs_sp_9_mag1_y_max = aggVec_max_i(&rc_mag1y);
    aggVec_mm_reset((aggVec *) &rc_mag1y);
}

void magioRcPopulate10(rc_adcs_sp_10 *r)
{
    r->rc_adcs_sp_10_mag1_y_avg = aggVec_avg_i_i(&rc_mag1y);
    aggVec_as_reset((aggVec *) &rc_mag1y);
    r->rc_adcs_sp_10_mag1_z_min = aggVec_min_i(&rc_mag1z);
    r->rc_adcs_sp_10_mag1_z_max = aggVec_max_i(&rc_mag1z);
    r->rc_adcs_sp_10_mag1_z_avg = aggVec_avg_i_i(&rc_mag1z);
    aggVec_reset((aggVec *) &rc_mag1z);
}

void magioRcPopulate11(rc_adcs_sp_11 *r)
{
    r->rc_adcs_sp_11_mag2_x_min = aggVec_min_i(&rc_mag2x);
    r->rc_adcs_sp_11_mag2_x_max = aggVec_max_i(&rc_mag2x);
    r->rc_adcs_sp_11_mag2_x_avg = aggVec_avg_i_i(&rc_mag2x);
    aggVec_reset((aggVec *) &rc_mag2x);
    r->rc_adcs_sp_11_mag2_y_min = aggVec_min_i(&rc_mag2y);
    aggVec_min_reset((aggVec *) &rc_mag2y);
}

void magioRcPopulate12(rc_adcs_sp_12 *r)
{
    r->rc_adcs_sp_12_mag2_y_max = aggVec_max_i(&rc_mag2y);
    aggVec_max_reset((aggVec *) &rc_mag2y);
    r->rc_adcs_sp_12_mag2_y_avg = aggVec_avg_i_i(&rc_mag2y);
    aggVec_as_reset((aggVec *) &rc_mag2y);
    r->rc_adcs_sp_12_mag2_z_min = aggVec_min_i(&rc_mag2z);
    r->rc_adcs_sp_12_mag2_z_max = aggVec_max_i(&rc_mag2z);
    aggVec_mm_reset((aggVec *) &rc_mag2z);
}

void magioRcPopulate13(rc_adcs_sp_13 *r)
{
    r->rc_adcs_sp_13_mag2_z_avg = aggVec_avg_i_i(&rc_mag2z);
    aggVec_as_reset((aggVec *) &rc_mag2z);
}

void magioRcPopulate14(rc_adcs_sp_14 *r)
{
    r->rc_adcs_sp_14_magp_valid = aggVec_sum_i(&rc_magValid);
    aggVec_reset((aggVec *) &rc_magValid);
}
