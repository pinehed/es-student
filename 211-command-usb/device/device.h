// device.h
#include <stdint.h> //ради uint8_t и uint32_

#pragma once

// константы: имя устройства, версия прошивки, проект, ссылка на репозиторий
#define DEVICE_NAME "es-led-module"
#define FIRMWARE_VERSION "2.0.0"

#define DEVICE_PROJECT "211-command-usb"
#define DEVICE_REPO "https://github.com/pinehed/es-student"

// запасное значение DEVICE_BOARD на случай сборки без определения компилятора
#ifndef DEVICE_BOARD
#define DEVICE_BOARD "unknown"
#endif


void device_info(void);
void dev_info(void);


struct info_t
{
    uint8_t revision;
    uint32_t version;
    char name[13];
};

extern struct info_t device_card; 