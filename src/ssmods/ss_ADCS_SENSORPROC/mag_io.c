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

FILE_STATIC hDev mag1;
FILE_STATIC hDev mag2;

FILE_STATIC hDev magioInit(bus_instance_i2c bus)
{
    hDev handle = magInit(bus);
    normalOperationConfig(bus);
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

FILE_STATIC void magioUpdate(hDev handle, uint8_t tlmId)
{
    MagnetometerData data;
    magReadXYZData(handle, ConvertToNanoTeslas, &data);

    // send backchannel data
    mag_segment seg;
    seg.x = data.rawX;
    seg.y = data.rawY;
    seg.z = data.rawZ;
    bcbinPopulateHeader(&seg.header, tlmId, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));

    // TODO send CAN data
}

void magioUpdate1()
{
    magioUpdate(mag1, TLM_ID_MAG1);
}

void magioUpdate2()
{
    magioUpdate(mag2, TLM_ID_MAG2);
}
