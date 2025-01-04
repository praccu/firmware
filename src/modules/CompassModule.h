#pragma once
#include "concurrency/OSThread.h"
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


class CompassModule : concurrency::OSThread {
    public:
        CompassModule();
        float getBearing();
        sensors_event_t getAccel();
        sensors_event_t getGyro();
        sensors_event_t getMag();
        bool icmOk = false;
    protected:
        virtual int32_t runOnce() override;
    private:
        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t mag;
        sensors_event_t temp;
 };


void compassInit();

extern CompassModule *compassModule;