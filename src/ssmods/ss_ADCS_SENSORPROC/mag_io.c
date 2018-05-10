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
#include "interfaces/canwrap.h"

#include "autocode/MSP_SP.h"
#include "autocode/rtwtypes.h"

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
}

FILE_STATIC uint8_t isValid(hMag handle)
{
    // TODO write a validity check
    return 1;
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

    // set autocode inputs
    rtU.mag1_vec_body_T[0] = magConvertRawToTeslas(data1->rawX);
    rtU.mag1_vec_body_T[1] = magConvertRawToTeslas(data1->rawY);
    rtU.mag1_vec_body_T[2] = magConvertRawToTeslas(data1->rawZ);
    rtU.mag1_vec_body_T[3] = isValid(mag1);
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

    // set autocode inputs
    rtU.mag2_vec_body_T[0] = magConvertRawToTeslas(data2->rawX);
    rtU.mag2_vec_body_T[1] = magConvertRawToTeslas(data2->rawY);
    rtU.mag2_vec_body_T[2] = magConvertRawToTeslas(data2->rawZ);
    rtU.mag2_vec_body_T[3] = isValid(mag2);
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
    sensorproc_mag mag;
    mag.sensorproc_mag_x = magConvertTeslasToRaw(rtY.mag_body_processed_T[0]);
    mag.sensorproc_mag_y = magConvertTeslasToRaw(rtY.mag_body_processed_T[1]);
    mag.sensorproc_mag_z = magConvertTeslasToRaw(rtY.mag_body_processed_T[2]);
    mag.sensorproc_mag_valid = rtY.mag_body_processed_T[3];
    CANPacket packet;
    encodesensorproc_mag(&mag, &packet);
    canSendPacket(&packet);
}
