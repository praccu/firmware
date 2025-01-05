#include "modules/CompassModule.h"
#include "modules/AHRSAlgorithms.h"

#include <Wire.h>

Adafruit_ICM20948 icm;
// For SPI mode, we need a CS pin
#define ICM_CS 4
// For software-SPI mode we need SCK/MOSI/MISO pins
#define ICM_SCK 5
#define ICM_MISO 7
#define ICM_MOSI 6
#define BOSTON_DECLINATION 8.25

CompassModule *compassModule;

float CompassModule::getBearing() {
    runOnce();
    Serial.print("Mag: ");
    Serial.print(this->mag.magnetic.x); Serial.print(", ");
    Serial.print(this->mag.magnetic.y); Serial.print(", ");
    Serial.print(this->mag.magnetic.z); Serial.println("");
    
    MadgwickQuaternionUpdate(
        this->accel.acceleration.x, this->accel.acceleration.y, this->accel.acceleration.z,
        this->gyro.gyro.x, this->gyro.gyro.y, this->gyro.gyro.z,
        this->mag.magnetic.x, this->mag.magnetic.y, this->mag.magnetic.z,
        0.1f
    );
    return (getAHRSHeading() * RAD_TO_DEG) - BOSTON_DECLINATION;
}

sensors_event_t  CompassModule::getAccel() {
    return this->accel;
}

sensors_event_t  CompassModule::getGyro() {
    return this->gyro;
}

sensors_event_t  CompassModule::getMag() {
    return this->mag;
}


int32_t CompassModule::runOnce() {
 icm.getEvent(&this->accel, &this->gyro, &this->temp, &this->mag);
  return 100;
}


CompassModule::CompassModule(): OSThread("compass_module") {
    //while (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {
    Wire.begin(ICM_MOSI, ICM_SCK);
    while(!icm.begin_I2C((uint8_t)105U, &Wire)) {
        delay(100);
    }
    delay(1000);
    this->icmOk = icm.setMagDataRate(AK09916_MAG_DATARATE_20_HZ);
    icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
    icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
    icm.setAccelRateDivisor(4095);
    icm.setGyroRateDivisor(255);
}

void compassInit() {
    Serial.begin(115200);
    compassModule = new CompassModule();
}