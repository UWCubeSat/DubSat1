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

#include "autocode/MSP_SP0.h"
#include "autocode/rtwtypes.h"

FILE_STATIC hMag mag1;
FILE_STATIC hMag mag2;
FILE_STATIC MagnetometerData *data1;
FILE_STATIC MagnetometerData *data2;

FILE_STATIC hMag magioInit(bus_instance_i2c bus)
{
    hMag handle = magInit(bus);
    return handle;
}

void magioInit1()
{
    mag1 = magioInit(MAG1_I2CBUS);
}

void magioInit2()
{
    mag2 = magioInit(MAG2_I2CBUS);
}

FILE_STATIC uint8_t isValid(hMag handle)
{
    // TODO write a validity check
    return 1;
}

FILE_STATIC void magioUpdate(hMag handle, MagnetometerData **output, real32_T *autoOutput)
{
    MagnetometerData *data = magReadXYZData(handle, ConvertToNone);
    *output = data;

    // set autocode inputs
    // TODO verify units
    autoOutput[0] = data->rawX;
    autoOutput[1] = data->rawY;
    autoOutput[2] = data->rawZ;
    autoOutput[3] = isValid(handle);
}

void magioUpdate1()
{
    magioUpdate(mag1, &data1, rtU.mag1_vec_body_T);
}

void magioUpdate2()
{
    magioUpdate(mag2, &data2, rtU.mag2_vec_body_T);
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

void magioSendBackchannel1()
{
    magioSendBackchannel(data1, TLM_ID_MAG1);
}

void magioSendBackchannel2()
{
    magioSendBackchannel(data2, TLM_ID_MAG2);
}

void magioSendCAN()
{
    // TODO send CAN packet when implemented
//    rtY.mag_body_processed_T...
}
