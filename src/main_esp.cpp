#include <Arduino_GFX_Library.h>
#include <Arduino_GFX_Library.h>
#include <SoftwareSerial.h>
#include <SPI.h>

#include "decoder.hpp"
#include "example_data.hpp"
#include "hardware.hpp"
#include "state_machine.hpp"

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO, VSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// Software serial with inverted signals, no TX pin
SoftwareSerial mySerial(RX_LELIT, -1, true);

char *example_data_ptr = (char *)example_data;
Decoder decoder;
int i;

void setup()
{
    Serial.begin(9600);
    Serial.setRxInvert(true);
    Serial.println("Hello World!");

    pinMode(RX_LELIT, INPUT);

    mySerial.begin(9600);
    if (!mySerial)
    { // If the object did not initialize, then its configuration is invalid
        Serial.println("Invalid EspSoftwareSerial pin configuration, check config");
    }

    gfx->begin();
    gfx->fillScreen(WHITE);
    gfx->setCursor(100, 100);
    gfx->setTextColor(BLACK, WHITE);
    gfx->setTextSize(2);
    gfx->setTextWrap(false);
}

void general_data_display(lelit_data_t *data)
{
    gfx->setCursor(0, 40);
    gfx->printf("   Mode: %6s\n", data->mode == COFFEE ? "Coffee" : "Steam");
    gfx->printf("   Steam:   %3d", data->steam_temp);
    // Use special functionality to write degree symbol
    gfx->write(0xF8);
    gfx->printf("C\n");
    gfx->printf("   Target:  %3d", data->target_steam_temp);
    gfx->write(0xF8);
    gfx->printf("C\n");
    gfx->printf("   Coffee:  %3d", data->coffee_temp);
    gfx->write(0xF8);
    gfx->printf("C\n");
    gfx->printf("   Boost:   %3ds\n", data->boost_countdown);
    gfx->printf("   Heating: %3d\n", data->heating_element_on);
    gfx->printf("   Pump:    %3d\n", data->pump_on);
}

void loop()
{
    // we do not have serial data yet, so iterate over example data
    bool real_data = false;
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
    }
    else
    {
        rc = mySerial.read();
    }

    if (rc != 0xFF)
    {
        // forward all bytes to UART for debugging & capturing
        Serial.print(rc);
        updated = decoder.handle_bytes(rc);
    }

    if (updated)
    {
        i++;
        general_data_display(&decoder.data);
        if (!real_data)
        {
            // delay a bit to simulate real data
            delay(200);
        }
    }

    gfx->setCursor(100, 200);
    gfx->println(i);
}