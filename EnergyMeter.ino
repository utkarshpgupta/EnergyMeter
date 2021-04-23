#include "EmonLib.h"             // Include Emon Library
#define CURRENT_CAL 30
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#define adcpin A0
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
EnergyMonitor emon1;
unsigned long timeFinishedSetup = 0;

const char* ssid = "Airtel_2.4G";
const char* pwd = "srgmpdnss2112";
WiFiServer server(80);
String device;
String stat;
float currentDraw,energy,power;

void setup()
{  
  Serial.begin(9600);
  emon1.current(adcpin, CURRENT_CAL);       // Current: input pin, calibration.

  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C after scanning I2C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  
  timeFinishedSetup = millis();
}
void loop()
{
  WiFiClient client = server.available();
  if (!client) {
  display.setCursor(0,0);
  display.clearDisplay();
  Serial.print("Current: ");
  Serial.println(currentDraw);
  
  Serial.print("Watts: ");
  Serial.println(power);
  
  display.println("Power:");
  display.print(power);
  display.println(" W");
  
  Serial.println("\n\n");
  Serial.print("Energy: ");
  Serial.print(energy);

  display.println("Energy:");
  display.print(energy);
  display.print(" Whr");
  display.display();
  }
  
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  String m;
  
  display.setCursor(0,0);
  display.clearDisplay();
  currentDraw = emon1.calcIrms(1480);
  float supplyVoltage = 230;
  unsigned long currentTime = (millis()-timeFinishedSetup);
  Serial.println(currentTime);
  
  energy = currentDraw * supplyVoltage * currentTime;
  energy=energy/3600000;
  power = currentDraw * supplyVoltage;

  if (req.indexOf("/Power") != -1){
    device="Power";
    stat = power;
  }
  else if (req.indexOf("/Energy") != -1){
    device="Energy";
    stat = energy;
  }
  else {
    Serial.println("invalid request");
    client.stop();
    m = "Invalid Request";
  }
  
  Serial.print("Current: ");
  Serial.println(currentDraw);
  Serial.println(currentTime);
  Serial.print("Watts: ");
  Serial.println(power);
  
  display.println("Power:");
  display.print(power);
  display.println(" W");
  
  Serial.println("\n\n");
  Serial.print("Energy: ");
  Serial.print(energy);

  display.println("Energy:");
  display.print(energy);
  display.print(" Whr");
  display.display();

  client.flush();

  // Prepare the response

  // Send the response to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *");
  client.println("");
   
   //send json response
  client.print("{\""+device+"\": \""+stat+"\"}");
  
  delay(1);
  Serial.println("Client disonnected");

}
