#define vibsensorPin  5
#define ledPin  13
#include <ESP8266WiFi.h>

const char* ssid = "turkmen";
const char* password = "omer1234";
boolean bite = false;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  pinMode(vibsensorPin, INPUT);
  digitalWrite(ledPin, LOW);
  
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
  
  long fishAlarm = pulseIn(vibsensorPin, HIGH); 
  if (fishAlarm > 100) 
  {
    bite = true;
    Serial.println(fishAlarm);
    Serial.println(bite);
  }
 
   
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
  if (req.indexOf("/stop/0") != -1)
    val = 0;
  else if (req.indexOf("/start/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO according to the request
  digitalWrite(ledPin, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\n Content-Type: text/html\r\n\r\n\r\n";
  if (bite == true)
   {
    s += "!!!BALIK GELDİ!!!";
    bite= false;
   }

  else
  {
    s += "Balık yok";
  }

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
}
