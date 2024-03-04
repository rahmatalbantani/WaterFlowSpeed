/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6prtIP41z"
#define BLYNK_TEMPLATE_NAME         "Laju Alir"
#define BLYNK_AUTH_TOKEN            "28gikxnYaqe8c8QA_FHaAMwcYzj5pqMu"

/* Comment this out to disable prints and save space */
#define BLYNK_PRImeNT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <VL53L1X.h>

#define RFM95_CS D8
#define RFM95_RST D0
#define RFM95_INT D2


VL53L1X sensor;
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "UNTIRTAKU";
char pass[] = "untirtajawara";

int period = 200;
unsigned long time_now = 0;
int data1,data2,data3;

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
  Serial.println(value);
}



// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, data3);

}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  if (!rf95.init()) {
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  Serial.println("LoRa initialization successful Receiver.");
  
  if (!rf95.setFrequency(433.0)) {
    Serial.println("Frequency set failed!");
    while (1);
  }


  delay(1000);  // give the ESC some time to initialize

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}
void getdata(){
    if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      String receivedData = (char*)buf;
      
      int startIndex = receivedData.indexOf("#");
      if (startIndex != -1) {
        // Data 1
        data1 = receivedData.substring(startIndex + 1, receivedData.indexOf("#", startIndex + 1)).toInt();
        
        startIndex = receivedData.indexOf("#", startIndex + 1);
        if (startIndex != -1) {
          // Data 2
          data2 = receivedData.substring(startIndex + 1, receivedData.indexOf("#", startIndex + 1)).toInt();
          
          startIndex = receivedData.indexOf("#", startIndex + 1);
          if (startIndex != -1) {
            // Data 3
            data3 = receivedData.substring(startIndex + 1, receivedData.indexOf("?", startIndex + 1)).toInt();
            
            Serial.print("Received data1: ");
            Serial.println(data1);
            Serial.print("Received data2: ");
            Serial.println(data2);
            Serial.print("Received data3: ");
            Serial.println(data3);
            int rssi = rf95.lastRssi();
            Serial.print("RSSI: ");
            Serial.println(rssi);
          } else {
            Serial.println("Invalid data format: Data 3 is missing.");
          }
        } else {
          Serial.println("Invalid data format: Data 2 is missing.");
        }
      } else {
        Serial.println("Invalid data format: Data 1 is missing.");
      }
    } else {
      Serial.println("Receive failed.");
}
}


}
void loop()
{
  Blynk.run();
  timer.run();
  if(millis() >= time_now + period){
        time_now += period;
        getdata();
    }
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
