#include <Keyboard.h>

// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>

// You need to create an driver instance
RPLidar lidar;
uint8_t keys[] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};
int lastKey;
#define RPLIDAR_MOTOR 3 // The PWM pin   for   ontrol the speed of RPLIDAR's motor.
// This pin should connected with the RPLIDAR's MOTOCTRL signal


void setup() {
  // bind the RPLIDAR driver to the arduino hardware serial
  Serial.begin(115200);
  Serial1.begin(115200);
  Keyboard.begin();
  lidar.begin(Serial1);

  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);
}

void loop() {
  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance; //distance value in mm unit
    float angle    = lidar.getCurrentPoint().angle; //anglue value in degree
    bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement

    //perform data processing here...
    if (distance > 0 && distance < 6000 && ((angle >= 0 && angle <= 1) || (angle >= 359 && angle <= 360))) {
      Serial.print("distance:");
      Serial.print(distance);
      Serial.print("/");
      Serial.print("angle:");
      Serial.print(angle);
      Serial.print("/");
      Serial.print("startBit:");
      Serial.print(startBit);
      Serial.print("/");
      Serial.print("quality:");
      Serial.println(quality);
      int i = map(distance, 150, 5800, 0, 8);
      if (i != lastKey) {
        Keyboard.write(keys[i]);
        delay(100);
        lastKey = i;
      }
    }
  } else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor

    // try to detect RPLIDAR...
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      // detected...
      lidar.startScan();

      // start motor rotating at max allowed speed
      analogWrite(RPLIDAR_MOTOR, 190);
      delay(1000);
    }
  }
}
