AVR_MCU   ?= attiny24a
AVR_F_CPU ?= 12000000UL
AVR_LFUSE ?= 0xDF
AVR_HFUSE ?= 0xDF

FIRMWARE_NAME ?= controller

FIRMWARE_CFLAGS = \
	-I. \
	$(NULL)

FIRMWARE_SOURCES = \
	main.c \
	usbdrv/usbdrv.c \
	usbdrv/usbdrvasm.S \
	$(NULL)

FIRMWARE_HEADERS = \
	usbconfig.h \
	usbdrv/usbdrv.h \
	$(NULL)
