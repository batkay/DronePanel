#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;           // your network SSID (name)
char pass[] = SECRET_OPTIONAL_PASS;  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                    // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library

WiFiClient client;
bool print = false;

// server address:
//char server[] = "example.org";
IPAddress server(172, 20, 10, 5);
int port = 3000;
unsigned long lastConnectionTime = 0;  // last time you connected to the server, in milliseconds

const unsigned long postingInterval = 10L * 1000L;  // delay between updates, in milliseconds

String message = "";

int colors[8*32];
int lenCol = 8*32;
bool set = false;

#define PIN_NEO_PIXEL  12   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     8*32  // The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 1
Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {

  //Initialize serial and wait for port to open:

  Serial.begin(9600);

  while (!Serial) {

    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    // status = WiFi.begin(ssid, pass);

    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:

    delay(10000);
  }

  // you're connected now, so print out the status:

  printWifiStatus();
  NeoPixel.begin();
  NeoPixel.setBrightness(50);
}

void loop() {
  StaticJsonDocument<4096> doc;
  // Serial.println(doc.capacity());

  // if there's incoming data from the net connection.

  // send it out the serial port.  This is for debugging

  // purposes only:

  print = false;
  while (client.available()) {

    char c = client.read();
    message += c;

    // Serial.write(c);
    print = true;
  }
  if (print) {
    // int len = message.substring(message.indexOf("{"), message.indexOf("}") + 2).length();
    // char buf [len];
    // message.substring(message.indexOf("{"), message.indexOf("}") + 2).toCharArray(buf, len);
    // Serial.println(buf);
    // //Serial.print(buf);
    // Serial.println(buf[0]);
    // Serial.println(buf[len-2]);
    // //char buffer[message.length() + 1];
    // // const char* buffer = message.c_str();
    // DeserializationError error = deserializeJson(doc, buf);

    // // Test if parsing succeeds.
    // if (error) {
    //   Serial.print(F("deserializeJson() failed: "));
    //   Serial.println(error.f_str());

    // } else {
    //   float color = doc["colors"][0];
    //   Serial.println(color);
    //   Serial.println(message);
    // }
    
    int len = message.substring(message.indexOf("[") + 1, message.indexOf("]")).length() + 1;
    char buf [len];
    message.substring(message.indexOf("[") + 1, message.indexOf("]")).toCharArray(buf, len);
    char* ptr = strtok(buf, ",");
    int i = 0;
    // Serial.println(message.substring(message.indexOf("[") + 1, message.indexOf("]")));
    Serial.println("Splitting");
    while(ptr != NULL) {
      colors[i] = atoi(ptr);
      // Serial.print(ptr);
      ++i;
      ptr = strtok(NULL, ",");

    }
    Serial.println("\nDone split");
    Serial.println(colors[0]);
    for (int i = 0; i < len; ++i) {
      // Serial.println(colors[i]);
    }
    NeoPixel.clear();
    if (!set) {
      for (int i = 0; i < NUM_PIXELS; ++i) {
        NeoPixel.setPixelColor(i, colors[i]);
        // Serial.println("yep");
      }
      NeoPixel.show();
      Serial.println(NeoPixel.Color(50, 0, 0));
      Serial.println("showing");
      set = true;
    }
    
  }
  // Serial.print(message);


  // if ten seconds have passed since your last connection,

  // then connect again and send data:

  if (millis() - lastConnectionTime > postingInterval) {

    httpRequest();
  }
}

// this method makes a HTTP connection to the server:
void httpRequest() {

  // close any connection before send a new request.

  // This will free the socket on the Nina module

  client.stop();

  // if there's a successful connection:

  if (client.connect(server, port)) {

    Serial.println("connecting...");

    // send the HTTP PUT request:

    client.println("GET /data HTTP/1.1");

    client.println("Host: example.org");
    client.println("Accept: application/json;charset=utf-8");
    client.println("User-Agent: ArduinoWiFi/1.1");

    client.println("Connection: close");

    client.println();

    // note the time that the connection was made:

    lastConnectionTime = millis();
    message = "";

  } else {

    // if you couldn't make a connection:

    Serial.println("connection failed");
  }
}

void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your board's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}