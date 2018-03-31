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

void magioInit()
{
    magInit(MAG_I2CBUS);
    normalOperationConfig();
}

void magioUpdate()
{
    MagnetometerData *data = magReadXYZData(ConvertToNanoTeslas);

    // send backchannel data
    mag_segment seg;
    seg.x = data->rawX;
    seg.y = data->rawY;
    seg.z = data->rawZ;
    bcbinPopulateHeader(&seg.header, TLM_ID_MAG, sizeof(seg));
    bcbinSendPacket((uint8_t *) &seg, sizeof(seg));

    // TODO send CAN data
}
