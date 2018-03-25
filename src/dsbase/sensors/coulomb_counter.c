#include "coulomb_counter.h"

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC deviceContextCoulombCounter sensor;

void LTC2943Init(bus_instance_i2c bus, float rShunt) {

    hDev hSensor = i2cInit(bus, LTC2943_ADDRESS);

    //store sensor information
    sensor.initialized = TRUE;
    sensor.bus = bus;
    sensor.slaveaddr = LTC2943_ADDRESS;
    sensor.hI2CDevice = hSensor;
    sensor.shuntResistance = rShunt;

    //Configure control register
    i2cBuff[0] = LTC2943_ADDR_CONTROL;
    i2cBuff[1] = ALERTDISABLED_PRESCALER256_ADCAUTOMATICMODE;
    i2cMasterWrite(hSensor, i2cBuff, 2);

//    //read set control register
//    i2cMasterRegisterRead(sensor.hI2CDevice, LTC2943_ADDR_CONTROL, i2cBuff, 1);
//    sensor.controlReg = i2cBuff[0];

    //calculate charge LSB based on shunt resistance and prescaler.
    sensor.chargeLSB = 0.34*(50.0/sensor.shuntResistance)*((float)LTC2943_PRESCALEFACTOR/4096.0);

    //calculate accumulated charge at full/empty battery. Assuming 2200mAh battery
    sensor.accumChargeFull = 0x7FFF+((2200.0/sensor.chargeLSB)/2);
    sensor.accumChargeEmpty = 0x7FFF-((2200.0/sensor.chargeLSB)/2);

}

float rawBusVoltageToFloat(int16_t raw) {
    return 23.6*((float)raw/(float)0xFFFF);
}

float rawCurrentToFloat(int16_t raw) {
    return ((60.0/sensor.shuntResistance)*((float)(raw-0x7FFF)/(float)0x7FFF));
}

BOOL checkForFullState(float voltage, float current) {
    //full state is defined as 7.2 volts with the LTC2943 limiting the current into the batteries to .1A
    // +- .02 volt margin with +-.1A margin
    if(voltage >= 7.18 || voltage <= 7.22 && current <= 0.11 || current >= 0.09) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void calibrate(hDev hSensor) {
    //reset the accumulated charge register to the full state
    i2cBuff[0] = LTC2943_ADDR_ACCUMULATED_CHARGE_MSB;
    i2cBuff[1] = sensor.accumChargeFull >> 8;
    i2cBuff[2] = sensor.accumChargeFull & 0x00FF;
    i2cMasterWrite(hSensor, i2cBuff, 3);
}

CoulombCounterData readCoulombCounter() {
    //read voltage registers with repeated start condition.
    i2cMasterRegisterRead(sensor.hI2CDevice, LTC2943_ADDR_VOLTAGE_MSB, i2cBuff, 2);
    sensor.sensorData.rawBusVoltage = (uint16_t)(i2cBuff[1] | ((uint16_t)i2cBuff[0]<<8));
    sensor.sensorData.busVoltageV = rawBusVoltageToFloat(sensor.sensorData.rawBusVoltage);

    //read Current registers with repeated start condition
    i2cMasterRegisterRead(sensor.hI2CDevice, LTC2943_ADDR_CURRENT_MSB, i2cBuff, 2);
    sensor.sensorData.rawCurrent = (uint16_t)(i2cBuff[1] | ((uint16_t)i2cBuff[0]<<8));
    sensor.sensorData.calcdCurrentA = rawCurrentToFloat(sensor.sensorData.rawCurrent);

    //read Charge registers with repeated start condition
    i2cMasterRegisterRead(sensor.hI2CDevice, LTC2943_ADDR_ACCUMULATED_CHARGE_MSB, i2cBuff, 2);
    sensor.sensorData.rawAccumCharge = ((uint16_t)i2cBuff[1] | ((uint16_t)i2cBuff[0]<<8));

    //if the batteries are full, then calibrate the mAh measurement.
    if(checkForFullState(sensor.sensorData.busVoltageV, sensor.sensorData.calcdCurrentA)){
        calibrate(sensor.hI2CDevice);
    }

    //State of charge calculation
    sensor.sensorData.SOC = sensor.chargeLSB*((float)(sensor.sensorData.rawAccumCharge - sensor.accumChargeEmpty)/(float)(sensor.accumChargeFull - sensor.accumChargeEmpty));

    //Accumulated charge calculation. Relative to the entire register I.E it is calculating the [mAh worth of 1 bit change] times the [integer value of the register].
    sensor.sensorData.totalAccumulatedCharge = sensor.chargeLSB*sensor.sensorData.rawAccumCharge;


    //Accumulated charge in batteries, assuming 2200mAh batteries.
        sensor.sensorData.battCharge = sensor.chargeLSB*(sensor.sensorData.rawAccumCharge-sensor.accumChargeEmpty);

    return sensor.sensorData;
}


