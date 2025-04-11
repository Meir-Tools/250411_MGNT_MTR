// https://github.com/Meir-Tools/250411_MGNT_MTR
// by GPT , GGL , Meir-Tools
// MGNT_MTR_3RA_KLMN.ino | 3 Resolution Accuracy
#include <Wire.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// Kalman variables
float kalman_x = 0;
float kalman_P = 1;
const float Q = 0.01;   // Process noise
const float R = 0.6;    // Measurement noise

float kalmanFilter(float measurement) {
  kalman_P = kalman_P + Q;
  float K = kalman_P / (kalman_P + R);
  kalman_x = kalman_x + K * (measurement - kalman_x);
  kalman_P = (1 - K) * kalman_P;
  return kalman_x;
}

void setup() {
  Serial.begin(9600);
  if (!mag.begin()) {
    Serial.println("HMC5883 not detected!");
    while (1);
  }
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);

  float heading = atan2(event.magnetic.y, event.magnetic.x);
  heading = heading * 180.0 / PI;
  if (heading < 0) heading += 360.0;

  float filtered_heading = kalmanFilter(heading);

  Serial.print("Raw Heading: ");
  Serial.print(heading, 3);
  Serial.print("°, Filtered: ");
  Serial.print(filtered_heading, 3);
  Serial.println("°");

  delay(100);
}
