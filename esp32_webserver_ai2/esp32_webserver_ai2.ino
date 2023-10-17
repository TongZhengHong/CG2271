// Load Wi-Fi library
#include <stdlib.h>
#include <WiFi.h>

#define RXD2 16
#define TXD2 17

#define OFF_COMMAND 0b10001000

// Replace with your network credentials
const char *ssid = "mc";
const char *password = "123mc123";


WiFiServer server(80); // Set web server port number to 80
String response, ip_address; // Variable to store the HTTP request
String output26State = "off"; // Auxiliar variables to store the current output state
const int output26 = 2; // Assign output variables to GPIO pins


unsigned long currentTime = millis(); // Current time
unsigned long previousTime = 0; // Previous time
const long timeoutTime = 2000; // Define timeout time in milliseconds (example: 2000ms = 2s)
int wait30 = 30000; // time to reconnect when connection is lost.

// // This is your Static IP
// IPAddress local_IP(192, 168, 131, 50);
// // Gateway IP address
// IPAddress gateway(192, 168, 131, 50);
// IPAddress subnet(255, 255, 0, 0);
// IPAddress primaryDNS(8, 8, 8, 8);
// IPAddress secondaryDNS(8, 8, 4, 4);

int readBinaryString(char* s) {
  int result = 0;
  while (*s) {
    result <<= 1;
    if (*s++ == '1') result |= 1;
  }
  return result;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  pinMode(output26, OUTPUT); // Initialize the output variables as outputs
  digitalWrite(output26, LOW); // Set outputs to LOW

  // Configure Static IP
  // if(!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("Static IP failed to configure");
  // }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  ip_address = WiFi.localIP().toString();
  Serial.println(ip_address);
  server.begin();
}

void loop() {
  // If disconnected, try to reconnect every 30 seconds.
  if ((WiFi.status() != WL_CONNECTED) && (millis() > wait30)) {
    Serial.println("Trying to reconnect WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    wait30 = millis() + 30000;
  }

  // Check if a client has connected..
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.print("New client: ");
  Serial.println(client.remoteIP());

  // Espera hasta que el cliente envíe datos.
  // while(!client.available()){ delay(1); }

  /////////////////////////////////////////////////////
  // Read the information sent by the client.
  /////////////////////////////////////////////////////
  String req = client.readStringUntil('\r');
  Serial.println(req);

  // Check for off bit pattern and send OFF command
  if (req.indexOf("off") != -1) {
    Serial2.write(OFF_COMMAND);
    return;
  }

  String motor_ident = "motor=";
  int index = req.indexOf(motor_ident);
  if (index == -1) {
    return; // Motor command NOT found, abort
  }

  char charBuf[9];
  int start = index + motor_ident.length();
  String command = req.substring(start, start + 8);
  
  command.toCharArray(charBuf, 9);
  int value = readBinaryString(charBuf);
  
  Serial.println("User command: " + command);
  Serial.println("Sent value: " + value);
  Serial2.write(value);

  // if (req.indexOf("status") != -1) {
  //   response = "WiFi Connected: " + ip_address;
  // }
  if (req.indexOf("onRed") != -1) {
    digitalWrite(output26, HIGH);
    response = "RED LED ON";
    Serial.print("Red on");
  }
  if (req.indexOf("offRed") != -1) {
    digitalWrite(output26, LOW);
    response = "RED LED OFF";
    Serial.print("Red off");
  }

  // if (req.indexOf("onGreen") != -1) {
  //   digitalWrite(output26, HIGH);
  //   response = "GREEN LED ON";
  //   Serial2.write(0x33);
  // }
  // if (req.indexOf("offGreen") != -1) {
  //   digitalWrite(output26, LOW);
  //   response = "GREEN LED OFF";
  //   Serial2.write(0x32);
  // }
  // if (req.indexOf("onBlue") != -1) {
  //   digitalWrite(output26, HIGH);
  //   response = "BLUE LED ON";
  //   Serial2.write(0x35);
  // }
  // if (req.indexOf("offBlue") != -1) {
  //   digitalWrite(output26, LOW);
  //   response = "BLUE LED OFF";
  //   Serial2.write(0x34);
  // }

  /*
  if (req.indexOf("on12") != -1) {digitalWrite(LED12, HIGH); estado = "LED12 ON";}
  if (req.indexOf("off12") != -1){digitalWrite(LED12, LOW); estado = "LED12 OFF";}
  if (req.indexOf("on14") != -1) {digitalWrite(LED14, HIGH); estado = "LED14 ON";}
  if (req.indexOf("off14") != -1){digitalWrite(LED14, LOW); estado = "LED14 OFF";}
  if (req.indexOf("consulta") != -1) {
      estado ="";
      if (digitalRead(LED12) == HIGH) {estado = "LED12 ON,";} else {estado = "LED12 OFF,";}
      if (digitalRead(LED14) == HIGH) {estado = estado + "LED14 ON";} else {estado = estado + "LED14 OFF";}
  }*/

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println(response); //  Return status.

  client.flush();
  client.stop();
  Serial.println("Client disconnected.");
}
