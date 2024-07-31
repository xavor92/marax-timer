#include "SPI.h"
#include <Arduino_GFX_Library.h>
#include "hardware.hpp"
#include <SoftwareSerial.h>
#include "example_data.hpp"

#include <Arduino_GFX_Library.h>
#include "decoder.hpp"

Arduino_DataBus* bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO, VSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// Software serial with inverted signals, no TX pin
SoftwareSerial mySerial(RX_LELIT, -1, true);


char *example_data_ptr = (char *)example_data;
Decoder decoder;
int i;

void setup() {
  Serial.begin(9600);
  Serial.setRxInvert(true);
  Serial.println("Hello World!");

  pinMode(RX_LELIT, INPUT);

  mySerial.begin(9600);
  if (!mySerial) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid EspSoftwareSerial pin configuration, check config");
  }

  gfx->begin();
  gfx->fillScreen(WHITE);
  gfx->setCursor(100, 100);
  gfx->setTextColor(BLACK, WHITE);
  gfx->println("Start!");
  
}

void print_data_to_display(lelit_data_t *data)
{
    gfx->setCursor(100, 0);
    gfx->printf("      Mode: %s\n", data->mode == COFFEE ? "Coffee" : "Steam");
    gfx->printf("      Steam Temp: %d\n", data->steam_temp);
    gfx->printf("      Target Steam Temp: %d\n", data->target_steam_temp);
    gfx->printf("      Coffee Temp: %d\n", data->coffee_temp);
    gfx->printf("      Boost Countdown: %d\n", data->boost_countdown);
    gfx->printf("      Heating Element: %d\n", data->heating_element_on);
    gfx->printf("      Pump: %d\n", data->pump_on);
}

void loop() {
  // we do not have serial data yet, so iterate over example data
  bool real_data = true;
  bool updated = false;
  char rc = '\0';

  if (!real_data)
  {
    rc = *example_data_ptr;
    example_data_ptr++;
    if (*example_data_ptr == 0)
    {
      example_data_ptr = (char *)example_data;
    }
  } else {
    rc = mySerial.read();
  }

  if (rc != 0xFF) {
    // Serial.printf("rc: 0x%02X / %c\n", rc, rc);
    updated = decoder.handle_bytes(rc);
  }


  if (updated)
  {
    i++;
    print_data_to_display(&decoder.data);
  }

  gfx->setCursor(30, 100);
  gfx->println(i);
  gfx->printf("Updated: %s\n", updated ? "true" : "false");
  gfx->printf("Ptr: %p\n", example_data_ptr);
}