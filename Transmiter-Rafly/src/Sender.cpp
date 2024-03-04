#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <VL53L1X.h>

#define RFM95_CS D8
#define RFM95_RST D0
#define RFM95_INT D4
int jarak;
int count=0;
int pinswipe = A0;
int swipe;
int KY;
int KX;
VL53L1X sensor;


RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  while (!Serial) {}
  Serial.begin(115200);
  // pinMode(RFM95_INT, INPUT); // Update pinMode sesuai pin baru
  // attachInterrupt(digitalPinToInterrupt(RFM95_INT), NULL, RISING);
  if (!rf95.init()) {
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  Serial.println("LoRa initialization successful.");
  
  if (!rf95.setFrequency(433.0)) {
    Serial.println("Frequency set failed!");
    while (1);
  }
    Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(100000);
  sensor.startContinuous(100);

delay(1000);

}

void getdata(){
  jarak = sensor.read();
  KY = 900;
  KX = 900;

}

void loop() {
  getdata();


  String message = "#"+String(KX)+"#"+String(KY)+"#"+String(jarak)+"?"; // String yang berisi beberapa angka dengan tanda
  rf95.send((uint8_t*)message.c_str(), message.length());
  rf95.waitPacketSent();
  
  Serial.print("Sent data: ");
  Serial.println(message);
  
  delay(200);
}