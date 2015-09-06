# ESP8266_Bitcoin

Have your ESP8266 monitor the balance in a Bitcoin address using a 3rd party blockchain API, and make something happen (e.g. light a WS2811 LED strip) when a payment is received. Applications: beverage vending, parking meters, bus stop heaters etc.

Requires:
- ESP8266 Arduino core: https://github.com/esp8266/Arduino
- TextFinder: http://playground.arduino.cc/Code/TextFinder
- Adafruit NeoPixel Library: https://github.com/adafruit/Adafruit_NeoPixel

To be implemented:
- Websocket or webhooks instead of continuous polling.
- Webserver in AP mode for hosting configuration page.
  - SSID and WPA-PSK input
  - Bitcoin public address input
  - Blockchain API provider
  - Payment threshold, GPIO action and time. (e.g. 1000 bits sets GPIO2 for 10 seconds)
