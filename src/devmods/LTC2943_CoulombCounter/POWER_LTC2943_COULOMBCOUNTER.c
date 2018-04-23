#include <POWER_LTC2943_COULOMBCOUNTER.h>

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

    //calculate charge LSB based on shunt resistance and prescaler.
    sensor.chargeLSB = 0.34*(50/sensor.shuntResistance)*(LTC2943_PRESCALEFACTOR/4096.0);

    //calculate accumulated charge at full/empty battery. Assuming 2200mAh battery
    sensor.accumChargeFull = 0x7FFF+((2200/sensor.chargeLSB)/2);
    sensor.accumChargeEmpty = 0x7FFF-((2200/sensor.chargeLSB)/2);
}

float rawBusVoltageToFloat(int16_t raw) {
    return 23.6*(raw/0xFFFF);
}

float rawCurrentToFloat(int16_t raw) {
    return ((60.0/sensor.shuntResistance)*((raw-0x7FFF)/0x7FFF));
}

CoulombCounterData readCoulombCounter() {
    //read voltage registers with repeated start condition.
    i2cMasterRegisterRead(sensor.hI2CDevice, LTC2943_ADDR_VOLTAGE_MSB, i2cBuff, 2);
    sensor.sensorData.rawBusVoltage = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0]<<8));
    sensor.sensorData.busVoltageV = rawBusVoltageToFloat(sensor.sensorData.rawBusVoltage);

    //read Current registers with repeated start condition
    i2cMasterRegisterRead(sensor.hI2CDevice, LTC2943_ADDR_CURRENT_MSB, i2cBuff, 2);
    sensor.sensorData.rawCurrent = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0]<<8));
    sensor.sensorData.calcdCurrentA = rawCurrentToFloat(sensor.sensorData.rawCurrent);

    //read Charge registers with repeated start condition
    i2cMasterRegisterRead(sensor.hI2CDevice, LTC2943_ADDR_ACCUMULATED_CHARGE_MSB, i2cBuff, 2);
    sensor.sensorData.rawAccumCharge = (int16_t)(i2cBuff[1] | ((int16_t)i2cBuff[0]<<8));

    //State of charge calculation
    sensor.sensorData.SOC = sensor.chargeLSB*((sensor.sensorData.rawAccumCharge - sensor.accumChargeEmpty)/(sensor.accumChargeFull - sensor.accumChargeEmpty));

    return sensor.sensorData;
}



