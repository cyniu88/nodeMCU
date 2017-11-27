#include <Servo.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "mytype.h"
#include "functions.h"
#include "light.h"
#include "engine.h"
#include "driver.h"
#include "stopLight.h"
#include "PCF8574.h"

int kat;
int counter = 0;
int speed_  = 0;
String speed_s;
String kat_s;
constexpr int port = 8833;

const int  servoLeft = 0;
const int  servoRight = 86;
//int stopLedTimer     = STOP_LED_TIMER;

PCF8574 mainPCF8574(i2c_SDA, i2c_SCL, 0x20);

Engine mainMotor(&mainPCF8574);
Driver mainDriver(&mainMotor);
Engine externalMotor(&mainPCF8574);
Driver externalDriver(&externalMotor);
Light lightFront(FRONT_LED);
Light lightBack (BACK_LED);
StopLight mainStopLight(&lightBack, STOP_LED_TIMER);
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(port);
WiFiClient client;
Servo servomotor;

void setup() {
  Serial.begin(74880);
  while (!Serial) {
  }
  servomotor.attach(SERVO_PIN);
  servomotor.write(60);
  mainMotor.init(PWMa, IN1, IN2);
  externalMotor.init(EXTERNAL_PWM, EXTERNAL_IN1, EXTERNAL_IN2);
  lightBack.turnOFF();
  lightFront.turnOFF();

  digitalWrite(LED, 0);
  //analogWriteFreq(20);
  // Connect to WiFi network
  setupWiFi();

  OTA_update_setup();

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void wait_for_client() {
  digitalWrite(LED, 0);
  while (1) {
    if (counter == 0) {
      OTA_handle();
    }
    ++counter;
    if (counter == 90000) {
      counter = 0;
    }
    // Check if a client has connected
    client = server.available();
    if (!client) {
      // Serial.print(".");
    }
    else
    {
      break;
    }
  }

}  // end wait_for_clietn

void working() {
  digitalWrite(LED, 1);
  counter = 0;
  String req;
  String s;
  int externalSpeed_            = 0;
  String externalSpeed_s;
  while (1) {
    // Serial.println("wait");
    while (!client.available()) {
      ++counter;
      // Serial.println("czekam");
      // Serial.println(counter,DEC);
      mainMotor.hard_stop();
      externalMotor.hard_stop();
    }

    // Read the first line of the request
    req = client.readStringUntil('#');

    client.flush();
    //Serial.println("odebralem: " + req);
    if (req == "DISCONNECT") {
      Serial.println("disconnect");
      break;
    }

    if (req == "SLEEP") {
      ESP.deepSleep(10000000);
    }
    //////////////////////////////////// steering wheel ////////////////////////////////
    kat_s = req.substring(10, 14);
    //Serial.print("kat str: ");
    //Serial.println(kat_s);
    kat = kat_s.toInt();
    kat = map(kat, -100, 100, servoLeft, servoRight);
    servomotor.write(kat);

    /////////////////////////////////////// low beam /////////////////////////////////
    if (1 == req.substring(20, 21).toInt()) {

      lightFront.turnON();
      lightBack.turnON();
    }
    else {
      lightFront.turnOFF();
      lightBack.turnOFF();
    }
    ///////////////////////////////////////// high beam ///////////////////////////////
    if (1 == req.substring(22, 23).toInt()) {
      lightFront.maximal();
    }
    else {
      lightFront.maximalEnd();
    }
    ////////////////////////////////////////  motor /////////////////////////////////
    speed_s = req.substring(5, 9);
    speed_ = speed_s.toInt();
    mainDriver.runMotor(speed_);
    mainStopLight.handle(speed_);

    //////////////////////////////////////// trailer mechanism  /////////////////////////////////
    if (1 == req.substring(24, 25).toInt()) {
      externalSpeed_s = req.substring(15, 19);
      externalSpeed_ = externalSpeed_s.toInt();
      externalDriver.runMotor(externalSpeed_);
    }
    s = String(externalSpeed_, DEC) + " volt " + analogRead(A0) ;

   /////////////////////////////////////// Send the response to the client  /////////////////////
    client.print(s);
    delay(1);
  }
}  // end working
//////////////////////////////////////////// main() /////////////////////////////////////////////
void loop() {
  wait_for_client();
  // Wait until the client sends some data
  working();
  mainMotor.hard_stop();
  externalMotor.hard_stop();
  lightFront.turnOFF();
  lightBack.turnOFF();
}

