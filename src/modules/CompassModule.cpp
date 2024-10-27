#include "modules/CompassModule.h"
#include "modules/AHRSAlgorithms.h"

Adafruit_ICM20948 icm;
// For SPI mode, we need a CS pin
#define ICM_CS 38
// For software-SPI mode we need SCK/MOSI/MISO pins
#define ICM_SCK 9
#define ICM_MISO 11
#define ICM_MOSI 10
#define BOSTON_DECLINATION 8.25

CompassModule *compassModule;

float CompassModule::getBearing() {
    return 0.0f;
    MadgwickQuaternionUpdate(
        this->accel.acceleration.x, this->accel.acceleration.z, this->accel.acceleration.z,
        this->gyro.gyro.x, this->gyro.gyro.z, this->gyro.gyro.z,
        this->mag.magnetic.x, this->mag.magnetic.z, this->mag.magnetic.z,
        0.1f
    );
    return (getAHRSHeading() * RAD_TO_DEG) - BOSTON_DECLINATION;
}

int32_t CompassModule::runOnce() {
  icm.getEvent(&this->accel, &this->gyro, &this->temp, &this->mag);
  return 100;
}


CompassModule::CompassModule(): OSThread("compass_module") {
    /*
    while (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {
        delay(10);
    }
    icm.setMagDataRate(AK09916_MAG_DATARATE_100_HZ);
    icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
    icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
    icm.setAccelRateDivisor(4095);
    icm.setGyroRateDivisor(255);
    */
}

void compassInit() {
    compassModule = new CompassModule();
}