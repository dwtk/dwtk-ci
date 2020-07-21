/*
 * dwtk-ice: An USB-connected In-Circuit Emulator for dwtk.
 * Copyright (C) 2019 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the GPL-2 License.
 * See the file COPYING.
 */

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "bits.h"
#include "usbdrv/usbdrv.h"

typedef enum {
    CMD_PORT1_STATUS = 1,
    CMD_PORT1_POWERON,
    CMD_PORT1_POWEROFF,
    CMD_PORT2_STATUS,
    CMD_PORT2_POWERON,
    CMD_PORT2_POWEROFF,
} usb_command_t;

static uint8_t val = 0;


usbMsgLen_t
usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (void*) data;

    switch ((usb_command_t) rq->bRequest) {
        case CMD_PORT1_STATUS: {
            val = PIN_TEST(P_PORT1) ? 0x01 : 0x00;
            usbMsgPtr = &val;
            return 1;
        }
        case CMD_PORT1_POWERON: {
            PORT_SET(P_PORT1);
            break;
        }
        case CMD_PORT1_POWEROFF: {
            PORT_CLEAR(P_PORT1);
            break;
        }
        case CMD_PORT2_STATUS: {
            val = PIN_TEST(P_PORT2) ? 0x01 : 0x00;
            usbMsgPtr = &val;
            return 1;
        }
        case CMD_PORT2_POWERON: {
            PORT_SET(P_PORT2);
            break;
        }
        case CMD_PORT2_POWEROFF: {
            PORT_CLEAR(P_PORT2);
            break;
        }
    }

    return 0;
}


int
main(void)
{
    wdt_enable(WDTO_2S);

    DDR_SET(P_LEDR);
    DDR_SET(P_LEDG);
    DDR_SET(P_PORT1);
    DDR_SET(P_PORT2);
    PORT_CLEAR(P_PORT1);
    PORT_CLEAR(P_PORT2);

    usbInit();
    usbDeviceDisconnect();

    wdt_reset();
    uint8_t i = 0xff;
    while (i--) {
        _delay_ms(1);
    }
    usbDeviceConnect();

    sei();

    for (;;) {
        wdt_reset();
        usbPoll();
        PORT_CLEAR(P_LEDG);
    }

    return 0;
}
