#include "modules/CompassModule.h"
#include "modules/AHRSAlgorithms.h"

Adafruit_ICM20948 icm;
// TODO: set the correct pins.
// For SPI mode, we need a CS pin
#define ICM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define ICM_SCK 13
#define ICM_MISO 12
#define ICM_MOSI 11

CompassModule *compassModule;

float CompassModule::getBearing() {
    return MadgwickQuaternionUpdate(
        this->accel.acceleration.x, this->accel.acceleration.z, this->accel.acceleration.z,
        this->gyro.gyro.x, this->gyro.gyro.z, this->gyro.gyro.z,
        this->mag.magnetic.x, this->mag.magnetic.z, this->mag.magnetic.z,
        0.1f
    )
}

int32_t CompassModule::runOnce() {
  icm.getEvent(&this->accel, &this->gyro, &this->temp, &this->mag);
  return 100;
}


CompassModule::CompassModule(): OSThread("compass_module") {
    if (!icm.begin_I2C()) {
    // if (!icm.begin_SPI(ICM_CS)) {
    // if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {
        while (1) { delay(10); }
    }
    icm.setMagDataRate(AK09916_MAG_DATARATE_100_HZ);
}