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

#include "autocode/MSP_SP0.h"
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

FILE_STATIC hMag magioInit(bus_instance_i2c bus)
{
    hMag handle = magInit(bus);
    return handle;
}

void magioInit1()
{
#if ENABLE_MAG1
    mag1 = magioInit(MAG1_I2CBUS);
#endif
}

void magioInit2()
{
#if ENABLE_MAG2
    mag2 = magioInit(MAG2_I2CBUS);
#endif
}

FILE_STATIC uint8_t isValid(hMag handle)
{
    // TODO write a validity check
    return 1;
}

void magioUpdate1()
{
#if ENABLE_MAG1
    data1 = magReadXYZData(mag1, ConvertToNone);
#else
    mockData1.rawX = 123;
    mockData1.rawY = 123;
    mockData1.rawZ = 123;
    data1 = &mockData1;
#endif

    // set autocode inputs
    // TODO verify units
    rtU.mag1_vec_body_T[0] = data1->rawX;
    rtU.mag1_vec_body_T[1] = data1->rawY;
    rtU.mag1_vec_body_T[2] = data1->rawZ;
    rtU.mag1_vec_body_T[3] = isValid(mag1);
}

void magioUpdate2()
{
#if ENABLE_MAG2
    data2 = magReadXYZData(mag2, ConvertToNone);
#else
    mockData2.rawX = 123;
    mockData2.rawY = 123;
    mockData2.rawZ = 123;
    data2 = &mockData2;
#endif

    // set autocode inputs
    // TODO verify units
    rtU.mag2_vec_body_T[0] = data2->rawX;
    rtU.mag2_vec_body_T[1] = data2->rawY;
    rtU.mag2_vec_body_T[2] = data2->rawZ;
    rtU.mag2_vec_body_T[3] = isValid(mag2);
}

FILE_STATIC void magioSendBackchannel(MagnetometerData *data, uint8_t tlmId)
{
    mag_segment seg;
    seg.x = data->rawX;
    seg.y = data->rawY;
    seg.z = data->rawZ;
    bcbinPopulateHeader(&seg.header, tlmId, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));
}

void magioSendBackchannelVector()
{
    // TODO fix units
    sensor_vector_segment s;
    s.x = rtY.mag_body_processed_T[0];
    s.y = rtY.mag_body_processed_T[1];
    s.z = rtY.mag_body_processed_T[2];
    s.valid = rtY.mag_body_processed_T[3];
    bcbinPopulateHeader(&s.header, TLM_ID_MAG_VECTOR, sizeof(s));
    bcbinSendPacket((uint8_t *) &s, sizeof(s));
}

void magioSendBackchannel1()
{
    magioSendBackchannel(data1, TLM_ID_MAG1_RAW);
}

void magioSendBackchannel2()
{
    magioSendBackchannel(data2, TLM_ID_MAG2_RAW);
}

void magioSendCAN()
{
    sensorproc_mag mag;
    mag.sensorproc_mag_x = rtY.mag_body_processed_T[0];
    mag.sensorproc_mag_y = rtY.mag_body_processed_T[1];
    mag.sensorproc_mag_z = rtY.mag_body_processed_T[2];
    mag.sensorproc_mag_valid = rtY.mag_body_processed_T[3];
    CANPacket packet;
    encodesensorproc_mag(&mag, &packet);
    canSendPacket(&packet);
}
