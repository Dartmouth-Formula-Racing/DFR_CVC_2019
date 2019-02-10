/*
 * function_test.c
 *
 *  Created on: Dec 11, 2018
 *      Author: f002bc7
 */


/*----------------------------------------------------------------------/
/ Low level disk I/O module function checker                            /
/-----------------------------------------------------------------------/
/ WARNING: The data on the target drive will be lost!
*/

#include <stdio.h>
#include <string.h>
#include "ff.h"         /* Declarations of sector size */
#include "diskio.h"     /* Declarations of disk functions */
#include "sd_diskio_dma.h"
#include "ffconf.h"
#include "stm32f7xx_nucleo_144.h"
#include "cvc_serial.h"
#include "FreeRTOS.h"
#include "task.h"

static char message_buff[MAX_STRING_LENGTH];
static
DWORD pn (		/* Pseudo random number generator */
    DWORD pns	/* 0:Initialize, !0:Read */
)
{
    static DWORD lfsr;
    UINT n;


    if (pns) {
        lfsr = pns;
        for (n = 0; n < 32; n++) pn(0);
    }
    if (lfsr & 1) {
        lfsr >>= 1;
        lfsr ^= 0x80200003;
    } else {
        lfsr >>= 1;
    }
    return lfsr;
}


int test_diskio (
    BYTE pdrv,      /* Physical drive number to be checked (all data on the drive will be lost) */
    UINT ncyc,      /* Number of test cycles */
    DWORD* buff,    /* Pointer to the working buffer */
    UINT sz_buff    /* Size of the working buffer in unit of byte */
)
{
    UINT n, cc, ns;
    DWORD sz_drv, lba, lba2, sz_eblk, pns = 1;
    WORD sz_sect;
    BYTE *pbuff = (BYTE*)buff;
    DSTATUS ds;
    DRESULT dr;


    sprintf(message_buff, message_buff, "test_diskio(%u, %u, 0x%08X, 0x%08X)\n", pdrv, ncyc, (UINT)buff, sz_buff);
//    printf("%s", message_buff);
    ////console_write(message_buff);

    if (sz_buff < _MAX_SS + 4) {
        sprintf(message_buff, message_buff, "Insufficient work area to run program.\n");
//        printf("%s", message_buff);
        //console_write(message_buff);
        return 1;
    }

    for (cc = 1; cc <= ncyc; cc++) {
        sprintf(message_buff, "**** Test cycle %u of %u start ****\n", cc, ncyc);
//        printf("%s", message_buff);
        //console_write(message_buff);

        sprintf(message_buff, " disk_initalize(%u)", pdrv);
//        printf("%s", message_buff);
        //console_write(message_buff);

        ds = disk_initialize(pdrv);
        if (ds & STA_NOINIT) {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 2;
        } else {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        }

        sprintf(message_buff, "**** Get drive size ****\n");
//        printf("%s", message_buff);
        //console_write(message_buff);

        sprintf(message_buff, " disk_ioctl(%u, GET_SECTOR_COUNT, 0x%08X)", pdrv, (UINT)&sz_drv);
//        printf("%s", message_buff);
        //console_write(message_buff);

        sz_drv = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_COUNT, &sz_drv);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 3;
        }
        if (sz_drv < 128) {
            sprintf(message_buff, "Failed: Insufficient drive size to test.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 4;
        }
        sprintf(message_buff, " Number of sectors on the drive %u is %lu.\n", pdrv, sz_drv);
//        printf("%s", message_buff);
        //console_write(message_buff);


#if _MAX_SS != _MIN_SS
        sprintf(message_buff, "**** Get sector size ****\n");
        sprintf(message_buff, " disk_ioctl(%u, GET_SECTOR_SIZE, 0x%X)", pdrv, (UINT)&sz_sect);
        sz_sect = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_SIZE, &sz_sect);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
        } else {
            sprintf(message_buff, " - failed.\n");
            return 5;
        }
        sprintf(message_buff, " Size of sector is %u bytes.\n", sz_sect);
#else
        sz_sect = _MAX_SS;
#endif

        sprintf(message_buff, "**** Get block size ****\n");
//        printf("%s", message_buff);
        //console_write(message_buff);

        sprintf(message_buff, " disk_ioctl(%u, GET_BLOCK_SIZE, 0x%X)", pdrv, (UINT)&sz_eblk);
//        printf("%s", message_buff);
        //console_write(message_buff);

        sz_eblk = 0;
        dr = disk_ioctl(pdrv, GET_BLOCK_SIZE, &sz_eblk);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        }
        if (dr == RES_OK || sz_eblk >= 2) {
            sprintf(message_buff, " Size of the erase block is %lu sectors.\n", sz_eblk);
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " Size of the erase block is unknown.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        }

        /* Single sector write test */
        sprintf(message_buff, "**** Single sector write test 1 ****\n");
//        printf("%s", message_buff);
        //console_write(message_buff);

        lba = 0;
        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n] = (BYTE)pn(0);
        sprintf(message_buff, " disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
//        printf("%s", message_buff);
        //console_write(message_buff);

        dr = disk_write(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 6;
        }
        sprintf(message_buff, " disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
//        printf("%s", message_buff);
        //console_write(message_buff);

        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 7;
        }
        memset(pbuff, 0, sz_sect);
        sprintf(message_buff, " disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
//        printf("%s", message_buff);
        //console_write(message_buff);

        dr = disk_read(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 8;
        }
        for (n = 0, pn(pns); n < sz_sect && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == sz_sect) {
            sprintf(message_buff, " Data matched.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, "Failed: Read data differs from the data written.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 10;
        }
        pns++;


        sprintf(message_buff, "**** Multiple sector write test ****\n");
//        printf("%s", message_buff);
        //console_write(message_buff);

        lba = 1; ns = sz_buff / sz_sect;
        if (ns > 4) ns = 4;
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns); n++) pbuff[n] = (BYTE)pn(0);
//        sprintf(message_buff, " disk_write(%u, 0x%X, %lu, %u)", pdrv, (UINT)pbuff, lba, ns);
//        printf("%s", message_buff);
        //console_write(message_buff);

        dr = disk_write(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 11;
        }
        sprintf(message_buff, " disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
//        printf("%s", message_buff);
        //console_write(message_buff);

        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 12;
        }
        memset(pbuff, 0, sz_sect * ns);
        sprintf(message_buff, " disk_read(%u, 0x%X, %lu, %u)", pdrv, (UINT)pbuff, lba, ns);
//        printf("%s", message_buff);
        //console_write(message_buff);

        dr = disk_read(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            sprintf(message_buff, " - ok.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, " - failed.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 13;
        }
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns) && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == (UINT)(sz_sect * ns)) {
            sprintf(message_buff, " Data matched.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

        } else {
            sprintf(message_buff, "Failed: Read data differs from the data written.\n");
//            printf("%s", message_buff);
            //console_write(message_buff);

            return 14;
        }
        pns++;


//        sprintf(message_buff, "**** Single sector write test (misaligned address) ****\n");
////        printf("%s", message_buff);
//        //console_write(message_buff);
//
//        lba = 5;
//        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n+3] = (BYTE)pn(0);
//        sprintf(message_buff, " disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+3), lba);
////        printf("%s", message_buff);
//        //console_write(message_buff);
//
//        dr = disk_write(pdrv, pbuff+3, lba, 1);
//        if (dr == RES_OK) {
//            sprintf(message_buff, " - ok.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//        } else {
//            sprintf(message_buff, " - failed.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            return 15;
//        }
//        sprintf(message_buff, " disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
////        printf("%s", message_buff);
//        //console_write(message_buff);
//
//        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
//        if (dr == RES_OK) {
//            sprintf(message_buff, " - ok.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//        } else {
//            sprintf(message_buff, " - failed.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            return 16;
//        }
//        memset(pbuff+5, 0, sz_sect);
//        sprintf(message_buff, " disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+5), lba);
////        printf("%s", message_buff);
//        //console_write(message_buff);
//
//        dr = disk_read(pdrv, pbuff+5, lba, 1);
//        if (dr == RES_OK) {
//            sprintf(message_buff, " - ok.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//        } else {
//            sprintf(message_buff, " - failed.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            return 17;
//        }
//        for (n = 0, pn(pns); n < sz_sect && pbuff[n+5] == (BYTE)pn(0); n++) ;
//        if (n == sz_sect) {
//            sprintf(message_buff, " Data matched.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//        } else {
//            sprintf(message_buff, "Failed: Read data differs from the data written.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            return 18;
//        }
//        pns++;
//
//
//        sprintf(message_buff, "**** 4GB barrier test ****\n");
////        printf("%s", message_buff);
//        //console_write(message_buff);
//
//        if (sz_drv >= 128 + 0x80000000 / (sz_sect / 2)) {
//            lba = 6; lba2 = lba + 0x80000000 / (sz_sect / 2);
//            for (n = 0, pn(pns); n < (UINT)(sz_sect * 2); n++) pbuff[n] = (BYTE)pn(0);
//            sprintf(message_buff, " disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            dr = disk_write(pdrv, pbuff, lba, 1);
//            if (dr == RES_OK) {
//                sprintf(message_buff, " - ok.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//            } else {
//                sprintf(message_buff, " - failed.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//                return 19;
//            }
//            sprintf(message_buff, " disk_write(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+sz_sect), lba2);
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            dr = disk_write(pdrv, pbuff+sz_sect, lba2, 1);
//            if (dr == RES_OK) {
//                sprintf(message_buff, " - ok.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//            } else {
//                sprintf(message_buff, " - failed.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//                return 20;
//            }
//            sprintf(message_buff, " disk_ioctl(%u, CTRL_SYNC, NULL)", pdrv);
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
//            if (dr == RES_OK) {
//            sprintf(message_buff, " - ok.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            } else {
//                sprintf(message_buff, " - failed.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//                return 21;
//            }
//            memset(pbuff, 0, sz_sect * 2);
//            sprintf(message_buff, " disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)pbuff, lba);
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            dr = disk_read(pdrv, pbuff, lba, 1);
//            if (dr == RES_OK) {
//                sprintf(message_buff, " - ok.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//            } else {
//                sprintf(message_buff, " - failed.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//                return 22;
//            }
//            sprintf(message_buff, " disk_read(%u, 0x%X, %lu, 1)", pdrv, (UINT)(pbuff+sz_sect), lba2);
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//            dr = disk_read(pdrv, pbuff+sz_sect, lba2, 1);
//            if (dr == RES_OK) {
//                sprintf(message_buff, " - ok.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//            } else {
//                sprintf(message_buff, " - failed.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//                return 23;
//            }
//            for (n = 0, pn(pns); pbuff[n] == (BYTE)pn(0) && n < (UINT)(sz_sect * 2); n++) ;
//            if (n == (UINT)(sz_sect * 2)) {
//                sprintf(message_buff, " Data matched.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//            } else {
//                sprintf(message_buff, "Failed: Read data differs from the data written.\n");
////                printf("%s", message_buff);
//                //console_write(message_buff);
//
//                return 24;
//            }
//        } else {
//            sprintf(message_buff, " Test skipped.\n");
////            printf("%s", message_buff);
//            //console_write(message_buff);
//
//        }
//        pns++;

        sprintf(message_buff, "**** Test cycle %u of %u completed ****\n\n", cc, ncyc);
//        printf("%s", message_buff);
        //console_write(message_buff);

    }

    return 0;
}



int function_test_main (void)
{
	BSP_LED_Init(LED_RED);
	BSP_LED_Init(LED_BLUE);
	BSP_LED_Init(LED_GREEN);

	BSP_LED_Off(LED_RED);
	BSP_LED_Off(LED_GREEN);
	BSP_LED_Off(LED_BLUE);


    int rc;
    DWORD buff[_MAX_SS];  /* Working buffer (4 sector in size) */

    char SDPath[4]; /* SD disk logical drive path */
    FATFS_LinkDriver(&SD_Driver, SDPath);

    /* Check function/compatibility of the physical drive #0 */
    rc = test_diskio(0, 3, buff, sizeof buff);

    if (rc) {
        sprintf(message_buff, "Sorry the function/compatibility test failed. (rc=%d)\nFatFs will not work with this disk driver.\n", rc);
//        printf("%s", message_buff);
//        //console_write(message_buff);
        BSP_LED_On(LED_RED);

    } else {
        sprintf(message_buff, message_buff, "Congratulations! The disk driver works well.\n");
//        printf("%s", message_buff);
//        //console_write(message_buff);
        BSP_LED_On(LED_GREEN);
        vTaskDelay(1);
    }

    return rc;
}
