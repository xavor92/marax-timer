#include <Arduino_GFX_Library.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <time.h>

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
StateMachine *state_machine = NULL;

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
    gfx->fillScreen(BLACK);
    gfx->setTextColor(WHITE, BLACK);
    gfx->setTextWrap(false);

    state_machine = new StateMachine(gfx);
}

void loop()
{
    // we do not known if have serial data yet, so iterate over example data
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
        state_machine->update_state_machine(&decoder.data);
        state_machine->draw_state(40, 40, false);

        if (!real_data)
        {
            // delay a bit to simulate real data
            delay(100);
        }
    }
}