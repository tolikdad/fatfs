/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "SerialFlashMemory.h"
#include "MemoryManager.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM        0    /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC        1    /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB        2    /* Example: Map USB MSD to physical drive 2 */

#define sectorSize 512
#define spiFlashBlockSize 256
#define amountOfBlocks 3072

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (BYTE pdrv) {
    switch (pdrv) {
        case DEV_RAM :
            return RES_OK;
        case DEV_MMC :
            return RES_OK;
        case DEV_USB :
            return RES_OK;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize ( BYTE pdrv) {
    switch (pdrv) {
        case DEV_RAM :
            return RES_OK;
        case DEV_MMC :
            return RES_OK;
        case DEV_USB :
            return RES_OK;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
                   BYTE pdrv,        /* Physical drive nmuber to identify the drive */
                   BYTE *buff,        /* Data buffer to store read data */
                   LBA_t sector,    /* Start sector in LBA */
                   UINT count        /* Number of sectors to read */
) {
    DRESULT result = RES_OK;
    UINT i;
    SerialFlashMemory *serialFlashMemory = SerialFlashMemoryShared();
    for(i = 0; i < count; i++) {
        uint32_t offset = i * sectorSize;
        uint32_t address = (sector + offset) * spiFlashBlockSize;
        SerialFlashMemoryAddress firstAddress = SerialFlashMemoryAddressNew(address);
        size_t count = serialFlashMemory->read(buff + offset, &firstAddress, spiFlashBlockSize);
        if (count != spiFlashBlockSize) {
            result = RES_ERROR;
            break;
        }
        SerialFlashMemoryAddress secondAddress = SerialFlashMemoryAddressNew(address + spiFlashBlockSize);
        count = serialFlashMemory->read(buff + offset + spiFlashBlockSize, &secondAddress, spiFlashBlockSize);
        if (count != spiFlashBlockSize) {
            result = RES_ERROR;
            break;
        }
    }
    return result;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
                    BYTE pdrv,            /* Physical drive nmuber to identify the drive */
                    const BYTE *buffeer,    /* Data to be written */
                    LBA_t sector,        /* Start sector in LBA */
                    UINT count            /* Number of sectors to write */
) {
    DRESULT result = RES_OK;
    UINT i;
    unsigned char buff[256];
    SerialFlashMemory *serialFlashMemory = SerialFlashMemoryShared();
    for(i = 0; i < count; i++) {
        uint32_t offset = i * sectorSize;
        uint32_t address = (sector + offset) * spiFlashBlockSize;
        SerialFlashMemoryAddress firstAddress = SerialFlashMemoryAddressNew(address);
        memcpy(buff, buffeer + offset, 256);
        DRESULT res = serialFlashMemory->write(buff, &firstAddress, spiFlashBlockSize);
        if (res != RES_OK) {
            result = res;
            break;
        }
        memcpy(buff, buffeer + offset + spiFlashBlockSize, 256);
        SerialFlashMemoryAddress secondAddress = SerialFlashMemoryAddressNew(address + spiFlashBlockSize);
        res = serialFlashMemory->write( buff, &secondAddress, spiFlashBlockSize);
        if (res != RES_OK) {
            result = res;
            break;
        }
    }
    return result;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
                    BYTE pdrv,        /* Physical drive nmuber (0..) */
                    BYTE cmd,        /* Control code */
                    void *buff        /* Buffer to send/receive control data */
) {
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT: {
            LBA_t *sectorCount = (LBA_t *) buff;
            *sectorCount = (amountOfBlocks * spiFlashBlockSize) / sectorSize;
            return RES_OK;
        }
        case GET_BLOCK_SIZE: {
            LBA_t *blockSize = (LBA_t *) buff;
            *blockSize = sectorSize;
            return RES_OK;
        }
        default:
            return RES_ERROR;
    }
    return RES_PARERR;
}
