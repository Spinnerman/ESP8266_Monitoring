#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/****************************************************/
//Def
#define myPeriodic 15 //in sec | Thingspeak pub is 15sec
#define ONE_WIRE_BUS 2  // DS18B20 on arduino pin2 corresponds to D4 on physical board
//#define mySSR 0  // Solid State Relay on pin 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float prevTemp = 0;

 const char* ssid     = "our_ssID";
 const char* password = "our_Pass";
 const char* host = "f0167723.xsph.ru";
 int sent = 0;
 int value = 0;
/****************************************************/

void setup() {
  Serial.begin(115200);
 // connectWifi();
  delay(10);
  // We start by connecting to a WiFi network
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// int value = 0;
}
/****************************************************/
void loop() {
  
  delay(5000);
  ++value;
  
/******************          DS18B20       **********************************/
 float temp;
  //char buffer[10];
  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempCByIndex(0);
  //String tempC = dtostrf(temp, 4, 1, buffer);//handled in sendTemp()
  Serial.print(String(sent)+" Temperature: ");
  Serial.println(temp);

 // sendTeperatureTS(temp);
  int count = myPeriodic;
  while(count--)
  delay(1000);
/****************************************************/
 
  Serial.print("connecting to ");
  Serial.println(host);
    // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/insert_to_db.php?gradus=";     // создание GET-запроса через другой php-файл
 //String url = "fusions.php?gradus=";         
  url += temp;
 // url += "30";                                  // переменная
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
      client.print(String("GET f0167723.xsph.ru"));      
  delay(100);
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
    delay(30000);
}
