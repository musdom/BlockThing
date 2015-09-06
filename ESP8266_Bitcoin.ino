#include <ESP8266WiFi.h>
#include <TextFinder.h>
#include <Adafruit_NeoPixel.h>
#include <Ticker.h>

#define PIN            2
#define NUMPIXELS      5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Ticker flipper;

const char* ssid     = "<SSID>";
const char* password = "<WIFI_PASSWORD>";
const char* host = "api.blockcypher.com";
String address;
uint32_t rateLimit = 0, balance = 0, balanceLocal = 0;
uint8_t i = 0, count = 0, state = 0, balInit = 0;

void setup() {
  strip.begin();
  strip.setBrightness(40);
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

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
  Serial.println("");
  Serial.println("");
}

void loop() {
  flipper.attach(0.3, flip, 0xFFFFFF);
  delay(500);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  TextFinder finder(client);

  String url = "/v1/btc/main/addrs/<BITCOIN_ADDRESS>/balance?token=<OPTIONAL_API_TOKEN>";

  Serial.print("Requesting data from ");
  Serial.println(host);
  Serial.println("");
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
              
  delay(1000);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {

    //String json = client.readString();
    flipper.attach_ms(50, flip, 0x0000FF);

    if (finder.find("X-Ratelimit-Remaining")) {
      rateLimit = finder.getValue();
      Serial.print("Remaining queries: ");
      Serial.println(rateLimit);
    }
    
    if (finder.find("address")) {
      address = client.readStringUntil('\n');
      Serial.print("Address: ");
      Serial.println(address.substring(4, 38));
    }
    
    if (finder.find("final_balance")) {
      balance = finder.getValue()/100;
      Serial.print("Balance: ");
      Serial.print(balance);
      Serial.println(" bits");
      Serial.println("");
      if (balInit && balance > balanceLocal) {
        strip.setBrightness(255);
        flipper.attach_ms(50, flip, 0x00FF00);
        uint32_t payment = balance - balanceLocal;
        Serial.print("Payment: ");
        Serial.print(payment);
        Serial.println(" bits");
        delay(10000);
        balanceLocal = balance;
      }
      balanceLocal = balance;
      balInit = 1;
      Serial.println("");
    }
    client.flush();
    delay(1000);
    flipper.detach();
    strip.setBrightness(40);
  }
}

void flip(int color)
{
  state = !state;
  for (uint8_t i = 0; i < NUMPIXELS; i++){
    strip.setPixelColor(i, state*color);
  }
  strip.show();
}
