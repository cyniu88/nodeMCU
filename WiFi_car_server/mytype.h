#ifndef MYTYPE_H
#define MYTYPE_H
#include <Arduino.h>
#include "Engine.h"
#include "Light.h"

#define P0 0
#define P1 1
#define P2 2
#define P3 3
#define P4 4
#define P5 5
#define P6 6
#define P7 7

#define LED         D4
#define FRONT_LED   D5
#define BACK_LED    D6
#define SERVO_PIN   D7      // GPIO 13

#define IN1         P7      // GPIO 5   2 silnik P4 /P5   1 P7
#define IN2         P6      // GPIO 4                       P6
#define PWMa        D8      // GPIO 0    2 silnik  D3      1 D8

#define EXTERNAL_IN1 P4
#define EXTERNAL_IN2 P5
#define EXTERNAL_PWM D3

#define i2c_SCL     D1      //PINES esp826612e D1-->SCL, D2-->SDA
#define i2c_SDA     D2

#define STOP_LED_TIMER  60

struct Wifi_data {
  String ssid;
  String password;
};
#endif
