#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 7); // RX, TX
uint8_t data_1 = 0;
uint8_t data_2 = 0;

#define ARDUINO_5V_ACTUAL 4.64

#define OP_SENSOR_PIN A1
#define OT_SENSOR_PIN A4
#define WT_SENSOR_PIN A3
#define BAT_SENSOR_PIN A2

#define OP_SENS_V_MIN 0.5
#define OP_SENS_V_MAX 4.5
#define OP_SENS_P_MIN 0.0
#define OP_SENS_P_MAX 10.0

struct SensorData
{
  float battery_voltage;
  float oil_pressure;
  float oil_temperature;
  float water_temperature;
  
  const static uint8_t data_packet_size = 16;
  char data_packet[data_packet_size];

  const char* getDataPacket()
  {
    memcpy(data_packet,      &battery_voltage, sizeof(float));
    memcpy(data_packet + 4,  &oil_pressure, sizeof(float));
    memcpy(data_packet + 8,  &oil_temperature, sizeof(float));
    memcpy(data_packet + 12, &water_temperature, sizeof(float));
    return data_packet;
  }
  
  void fromDataPacket(const char* data_packet)
  {
    memcpy(&battery_voltage,   data_packet, sizeof(float));
    memcpy(&oil_pressure,      data_packet + 4, sizeof(float));
    memcpy(&oil_temperature,   data_packet + 8, sizeof(float));
    memcpy(&water_temperature, data_packet + 12, sizeof(float));
  }
};

SensorData sensor_data;
SensorData sensor_data_test;

float readVoltageAverage(int pin, uint8_t nr_of_samples)
{
  int value = 0;
  for (int i=0; i<nr_of_samples; i++)
  {
    value += analogRead(pin);
  }
  value /= nr_of_samples;
  return ARDUINO_5V_ACTUAL*value/1024; // 10bit ADC
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float readOilPressure()
{
  // Get the raw voltage readings
  float oil_pressure_v = readVoltageAverage(OP_SENSOR_PIN, 10);

  // Convert to pressure units (bar)
  float oil_pressure_p = mapf(oil_pressure_v, OP_SENS_V_MIN, OP_SENS_V_MAX, OP_SENS_P_MIN, OP_SENS_P_MAX);

  // TODO: maybe check the max/min limits
  return oil_pressure_p;
}

float readOilTemperature()
{
  // Get the raw voltage readings
  float oil_temperature_v = readVoltageAverage(OT_SENSOR_PIN, 10);

  // Convert to pressure units (bar)
  float oil_temperature_t = mapf(oil_temperature_v, OP_SENS_V_MIN, OP_SENS_V_MAX, OP_SENS_P_MIN, OP_SENS_P_MAX);

  // TODO: maybe check the max/min limits
  return oil_temperature_t;
}

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600); // Used for communicating with the display module
}

void loop()
{
  data_2 = 2*(++data_1);

  sensor_data.battery_voltage = 14.3;
  sensor_data.oil_pressure = readOilPressure();
  sensor_data.oil_temperature = readOilTemperature();
  sensor_data.water_temperature = 92.2;

  sensor_data_test.fromDataPacket(sensor_data.getDataPacket());
  
  Serial.print(sensor_data_test.oil_pressure);
  Serial.print(" ");
  Serial.println(sensor_data_test.oil_temperature);
  
  char data[16];
  sprintf(data, "%d;%d;tere\n", data_1, data_2);
  mySerial.write(data);
  delay(100);
}
