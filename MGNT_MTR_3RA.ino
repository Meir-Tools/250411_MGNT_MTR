// https://github.com/Meir-Tools/250411_MGNT_MTR
// by GPT , GGL , Meir-Tools
// MGNT_MTR_3RA.ino | 3 Resolution Accuracy
#include <Wire.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// הגדר ממוצע נע
#define AVG_WINDOW 10
float angle_buffer[AVG_WINDOW];
int buffer_index = 0;
bool buffer_filled = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!mag.begin()) {
    Serial.println("חיישן לא נמצא");
    while (1);
  }

  Serial.println("מגנטומטר מוכן");
}

float compute_heading(float x, float y) {
  float heading = atan2(y, x);
  heading *= 180.0 / PI;
  if (heading < 0) heading += 360.0;
  return heading;
}

float moving_average(float new_angle) {
  angle_buffer[buffer_index++] = new_angle;
  if (buffer_index >= AVG_WINDOW) {
    buffer_index = 0;
    buffer_filled = true;
  }

  float sum = 0;
  int count = buffer_filled ? AVG_WINDOW : buffer_index;

  for (int i = 0; i < count; i++) {
    sum += angle_buffer[i];
  }

  return sum / count;
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);

  float heading = compute_heading(event.magnetic.x, event.magnetic.y);
  float stable_heading = moving_average(heading);

  Serial.print("זווית מיוצבת: ");
  Serial.println(stable_heading, 3); // 3 ספרות אחרי הנקודה

  delay(100);
}
