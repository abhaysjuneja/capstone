#include <ESP8266WiFi.h>

char* host = "www.google.com";    // name address for Google (using DNS)

char* ssid = "ssid";
char* password = "password";

const int httpsPort = 80;

  WiFiClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print("Connecting to: ");
  Serial.print(ssid);   

  delay(100);

  WiFi.begin(ssid,password);

  delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WLAN connected"); 
  Serial.println("IPv4-address: ");
  Serial.println(WiFi.localIP());


 
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
 
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }

}

void loop() {

  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}
