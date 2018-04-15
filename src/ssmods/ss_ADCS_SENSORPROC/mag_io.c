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

void magioUpdate1()
{
    data1 = magReadXYZData(mag1, ConvertToNone);

    // TODO set autocode inputs
}

void magioUpdate2()
{
    data2 = magReadXYZData(mag2, ConvertToNone);

    // TODO set autocode inputs
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

void magioSendCAN1()
{
    // TODO
}

void magioSendCAN2()
{
    // TODO
}
