// device.c

#include "device.h"
#include "log.h" 
#include <stdio.h>
#include "pico/unique_id.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sysinfo.h"
#include "pico/version.h"
#include <stddef.h> //offsetof

void device_info(void)
{
    // читаем серийный номер платы функцией SDK

    char board_id[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];
    pico_get_unique_board_id_string(board_id, sizeof(board_id));

    // читаем регистр CHIP_ID по адресу и разбираем три поля
    //??? Дописать.И 3 поля
    volatile uint32_t *chip_id = (uint32_t *)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET);
    uint32_t id = *chip_id;

    uint32_t manufacturer = (id & SYSINFO_CHIP_ID_MANUFACTURER_BITS) >> SYSINFO_CHIP_ID_MANUFACTURER_LSB;
    uint32_t part = (id & SYSINFO_CHIP_ID_PART_BITS) >> SYSINFO_CHIP_ID_PART_LSB;
    uint32_t revision = (id & SYSINFO_CHIP_ID_REVISION_BITS) >> SYSINFO_CHIP_ID_REVISION_LSB;

    // печатаем пять строк паспорта
    printf("project: %s\n", DEVICE_PROJECT);
    printf("repo: %s\n", DEVICE_REPO);
    printf("board: %s\n", DEVICE_BOARD);
    printf("serial: %s\n", board_id);
    printf("chip: manufacturer 0x%03x, part 0x%04x, revision %u\n", manufacturer, part, revision);
    printf("pico-sdk: %s\n", PICO_SDK_VERSION_STRING);
}

  //     const struct info_t device_card[] = {
  //            derevision//    uint8_t revision;
  //            FIRMWARE_VERSION,//uint32_t version;
   //           DEVICE_NAME//char name[13];
   //    };

// struct info_t device_card[] = {
  //            device_card.revision= revision//    uint8_t revision;
  //            device_card.version = 2 //FIRMWARE_VERSION,//uint32_t version;
   //           device_card.name = DEVICE_NAME//char name[13];
 //      };

 struct info_t device_card[] = {
            0x0010000, //    uint8_t revision;
            2, //FIRMWARE_VERSION,//uint32_t version;
            DEVICE_NAME, //char name[13];
 }
void dev_info(void)  
{  
       //вычисляем поля - скопировано из device_info(void)
       volatile uint32_t *chip_id = (uint32_t *)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET);
    uint32_t id = *chip_id;
    uint32_t revision = (id & SYSINFO_CHIP_ID_REVISION_BITS) >> SYSINFO_CHIP_ID_REVISION_LSB;
    device_card.revision= revision;


         // шапка: struct   address     size offset value
    printf("%-10s %-10s %-10s\n",
            "struct","address","size","offset","value");
    printf("- %-13s 0x%08x %5u %6u 0x%13s\n",
           "device_card",
           &device_card
           sizeof(device_card);
    )
    
    printf("- %-13s 0x%08x %5u %6u 0x%5u\n",
           "revision",
           &device_card.revision,
           sizeof(device_card.revision),//offsetof(тип, поле) -смещение:расстояние в байтах от начала структуры до поля.
           offsetof(struct info_t, revision),
           device_card.revision);
    printf("- %-13s 0x%08x %5u %6u 0x%08x\n",
           "version",
           &device_card.version,
           sizeof(device_card.version),//offsetof(тип, поле) -смещение:расстояние в байтах от начала структуры до поля.
           offsetof(struct info_t, version),
           device_card.version);
    printf("- %-13s 0x%08x %5u %6u 0x%13s\n",
           "name",
           &device_card.name,
           sizeof(device_card.name),//offsetof(тип, поле) -смещение:расстояние в байтах от начала структуры до поля.
           offsetof(struct info_t, name),
           device_card.name);
}

