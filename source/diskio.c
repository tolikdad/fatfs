/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "FatIOLayer.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM        0    /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC        1    /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB        2    /* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (BYTE pdrv) {
    switch (pdrv) {
        case DEV_RAM :
            return RAM_disk_status();
        case DEV_MMC :
            return RAM_disk_status();
        case DEV_USB :
            return RAM_disk_status();
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize ( BYTE pdrv) {
    switch (pdrv) {
        case DEV_RAM :
            return RAM_disk_initialize();
        case DEV_MMC :
            return RAM_disk_initialize();
        case DEV_USB :
            return RAM_disk_initialize();
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
    switch (pdrv) {
        case DEV_RAM :
            return RAM_disk_read(buff, sector, count);
        case DEV_MMC :
            return RAM_disk_read(buff, sector, count);
        case DEV_USB :
            return RAM_disk_read(buff, sector, count);
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
                    BYTE pdrv,            /* Physical drive nmuber to identify the drive */
                    const BYTE *buff,    /* Data to be written */
                    LBA_t sector,        /* Start sector in LBA */
                    UINT count            /* Number of sectors to write */
) {
    switch (pdrv) {
        case DEV_RAM :
            return RAM_disk_write(buff, sector, count);
        case DEV_MMC :
            return RAM_disk_write(buff, sector, count);
        case DEV_USB :
            return RAM_disk_write(buff, sector, count);
    }
    return RES_PARERR;
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
    switch (pdrv) {
        case DEV_RAM :
            return RAM_disk_ioctl (cmd, buff);
        case DEV_MMC :
            return RAM_disk_ioctl (cmd, buff);
        case DEV_USB :
            return RAM_disk_ioctl (cmd, buff);
    }
    return RES_PARERR;
}
