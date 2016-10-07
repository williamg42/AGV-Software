#include <FastCRC.h>
#include <FastCRC_cpu.h>
#include <FastCRC_tables.h>

FastCRC8 CRC8;
IntervalTimer Sampler;
#define WEIGHT 3
#define DEAD 12
#define HWSERIAL Serial1

int16_t value_X = 0;
int16_t value_Y = 0;
int16_t value_R = 0;
int16_t temp_X = 0;
int16_t temp_Y = 0;
int16_t temp_R = 0;
unsigned char packet[4];

void setup() {
  analogReadResolution(8);
  Serial.begin(57600);
  HWSERIAL.begin(19200);
  Sampler.begin(measure_Axis, 500);




}

void loop() {

  packet[0] = value_X;
  packet[1] = value_Y;
  packet[2] = value_R;
  packet[3] = CRC8.smbus(packet, 3);
  HWSERIAL.write(packet, 4);
  HWSERIAL.flush();
  delay(25);
}


//simple weighted filter
void measure_Axis(void) {

  temp_X = (analogRead(2) + temp_X * (WEIGHT - 1)) / WEIGHT;
  if (temp_X < 127 - DEAD )
    value_X = sqrt(127) * sqrt(temp_X);

  else if (temp_X > 127 + DEAD )
    value_X = -sqrt(127) * sqrt(-temp_X + 255) + 255;

  else
    value_X = 125;


  temp_Y = (analogRead(1) + temp_Y * (WEIGHT - 1)) / WEIGHT;
  if (temp_Y < 127 - DEAD )
    value_Y = sqrt(127) * sqrt(temp_Y);

  else if (temp_Y > 127 + DEAD )
    value_Y = -sqrt(127) * sqrt(-temp_Y + 255) + 255;

  else
    value_Y = 125;


  temp_R = (analogRead(0) + temp_R * (WEIGHT - 1)) / WEIGHT;
  if (temp_R < 127 - DEAD )
    value_R = sqrt(127) * sqrt(temp_R);

  else if (temp_R > 127 + DEAD )
    value_R = -sqrt(127) * sqrt(-temp_R + 255) + 255;

  else
    value_R = 125;

  value_X = map(value_X, 11, 239, 0, 255);
  value_Y = map(value_Y, 11, 239, 0, 255);
  value_R = map(value_R, 11, 239, 0, 255);
}





