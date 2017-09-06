#include <ESP8266WiFi.h>

const char* ssid = "******"; // SSID of you wi-fi network
const char* password = "*******";// password for your wi-fi network
int led = LED_BUILTIN; // will control the buildin led

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare pin
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
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
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  String req_val;
  int ind=req.indexOf("msg?gpio="); //if request have a string like "msg?gpio=" then put it to 'ind'
  Serial.println(ind);
  
  // if 'ind' is not empty get the char from right side of '=' and try to convert it to integer
  if (ind != -1){
    req_val = req.charAt(ind+9);
    Serial.println(req_val);
    val = req_val.toInt();
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set pin according to the request
  digitalWrite(led, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

