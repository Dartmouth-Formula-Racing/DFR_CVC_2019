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

static DWORD buff[_MAX_SS];  /* Working buffer (4 sector in size) */

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
    UINT ncyc      /* Number of test cycles */
)
{

	UINT sz_buff = sizeof(buff);
    UINT n, cc, ns;
    DWORD sz_drv, lba, lba2, sz_eblk, pns = 1;
    WORD sz_sect;
    BYTE *pbuff = (BYTE*)buff;
    DSTATUS ds;
    DRESULT dr;


    if (sz_buff < _MAX_SS + 4) {
        /* Insufficient work area to run program */
        return 1;
    }

    for (cc = 1; cc <= ncyc; cc++) {

        /**** Test cycle 'cc' of 'ncyc' start ****/

        ds = disk_initialize(pdrv);
        if (ds & STA_NOINIT) {
            /* - failed */
            return 2;
        } else {
            /* - ok */
        }

        /**** Get drive size ****/

        sz_drv = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_COUNT, &sz_drv);
        if (dr == RES_OK) {
            /* - ok */

        } else {
            /* - failed */
            return 3;
        }
        if (sz_drv < 128) {
            /* Failed: Insufficient drive size to test. */
            return 4;
        }
        /* Number of sectors on the drive 'pdrv' is 'sz_drv' */


#if _MAX_SS != _MIN_SS
        /**** Get sector size ****/
        sz_sect = 0;
        dr = disk_ioctl(pdrv, GET_SECTOR_SIZE, &sz_sect);
        if (dr == RES_OK) {
            /* - ok. */
        } else {
            /* - failed. */
            return 5;
        }
        /* Size of sector is 'sz_sect' bytes. */
#else
        sz_sect = _MAX_SS;
#endif

        /**** Get block size ****/

        sz_eblk = 0;
        dr = disk_ioctl(pdrv, GET_BLOCK_SIZE, &sz_eblk);
        if (dr == RES_OK) {
            /* - ok. */

        } else {
            /* - failed. */

        }
        if (dr == RES_OK || sz_eblk >= 2) {
            /* Size of the erase block is 'sz_eblk' sectors. */

        } else {
            /* Size of the erase block is unknown. */

        }

        /**** Single sector write test 1 ****/

        lba = 0;
        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n] = (BYTE)pn(0);

        dr = disk_write(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            /* - ok. */

        } else {
            /* - failed. */

            return 6;
        }

        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            /* - ok. */

        } else {
            /* - failed. */

            return 7;
        }
        memset(pbuff, 0, sz_sect);

        dr = disk_read(pdrv, pbuff, lba, 1);
        if (dr == RES_OK) {
            /* - ok. */

        } else {
            /* - failed. */

            return 8;
        }
        for (n = 0, pn(pns); n < sz_sect && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == sz_sect) {
            /* Data matched. */

        } else {
            /* Failed: Read data differs from the data written. */

            return 10;
        }
        pns++;


        /**** Multiple sector write test ****/

        lba = 1; ns = sz_buff / sz_sect;
        if (ns > 4) ns = 4;
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns); n++) pbuff[n] = (BYTE)pn(0);

        dr = disk_write(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            /* - ok. */

        } else {
            /* - failed. */

            return 11;
        }

        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
        if (dr == RES_OK) {
            /* - ok. */

        } else {
            /* - failed. */

            return 12;
        }
        memset(pbuff, 0, sz_sect * ns);

        dr = disk_read(pdrv, pbuff, lba, ns);
        if (dr == RES_OK) {
            /* - ok. */

        } else {
            /* - failed. */

            return 13;
        }
        for (n = 0, pn(pns); n < (UINT)(sz_sect * ns) && pbuff[n] == (BYTE)pn(0); n++) ;
        if (n == (UINT)(sz_sect * ns)) {
            /* Data matched. */

        } else {
            /* Failed: Read data differs from the data written. */

            return 14;
        }
        pns++;


        /**** Single sector write test (misaligned address) ****/
//
//        lba = 5;
//        for (n = 0, pn(pns); n < sz_sect; n++) pbuff[n+3] = (BYTE)pn(0);
//
//        dr = disk_write(pdrv, pbuff+3, lba, 1);
//        if (dr == RES_OK) {
//            /* - ok. */
//
//        } else {
//            /* - failed. */
//
//            return 15;
//        }
//
//        dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
//        if (dr == RES_OK) {
//            /* - ok. */
//
//        } else {
//            /* - failed. */
//
//            return 16;
//        }
//        memset(pbuff+5, 0, sz_sect);
//
//        dr = disk_read(pdrv, pbuff+5, lba, 1);
//        if (dr == RES_OK) {
//            /* - ok. */
//
//        } else {
//            /* - failed. */
//
//            return 17;
//        }
//        for (n = 0, pn(pns); n < sz_sect && pbuff[n+5] == (BYTE)pn(0); n++) ;
//        if (n == sz_sect) {
//            /* Data matched. */
//
//        } else {
//            /* Failed: Read data differs from the data written. */
//
//            return 18;
//        }
//        pns++;
//
//
//        /**** 4GB barrier test ****/
//
//        if (sz_drv >= 128 + 0x80000000 / (sz_sect / 2)) {
//            lba = 6; lba2 = lba + 0x80000000 / (sz_sect / 2);
//            for (n = 0, pn(pns); n < (UINT)(sz_sect * 2); n++) pbuff[n] = (BYTE)pn(0);
//
//            dr = disk_write(pdrv, pbuff, lba, 1);
//            if (dr == RES_OK) {
//                /* - ok. */
//
//            } else {
//                /* - failed. */
//
//                return 19;
//            }
//
//            dr = disk_write(pdrv, pbuff+sz_sect, lba2, 1);
//            if (dr == RES_OK) {
//                /* - ok. */
//
//            } else {
//                /* - failed. */
//
//                return 20;
//            }
//
//            dr = disk_ioctl(pdrv, CTRL_SYNC, 0);
//            if (dr == RES_OK) {
//            	  /* - ok. */
//
//            } else {
//                /* - failed. */
//
//                return 21;
//            }
//            memset(pbuff, 0, sz_sect * 2);
//
//            dr = disk_read(pdrv, pbuff, lba, 1);
//            if (dr == RES_OK) {
//                /* - ok. */
//
//            } else {
//                /* - failed. */
//
//                return 22;
//            }
//
//            dr = disk_read(pdrv, pbuff+sz_sect, lba2, 1);
//            if (dr == RES_OK) {
//                /* - ok. */
//
//            } else {
//                /* - failed. */
//
//                return 23;
//            }
//            for (n = 0, pn(pns); pbuff[n] == (BYTE)pn(0) && n < (UINT)(sz_sect * 2); n++) ;
//            if (n == (UINT)(sz_sect * 2)) {
//                /* Data matched. */
//
//            } else {
//                /* Failed: Read data differs from the data written. */
//
//                return 24;
//            }
//        } else {
//            /* Test skipped. */
//
//        }
//        pns++;

        /**** Test cycle 'cc' of 'ncyc' completed ****/

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

    char SDPath[4]; /* SD disk logical drive path */
    FATFS_LinkDriver(&SD_Driver, SDPath);

    /* Check function/compatibility of the physical drive #0 */
    rc = test_diskio(0, 3);//, buff, sizeof buff);

    if (rc) {
        /* Sorry the function/compatibility test failed. (rc=%d)\nFatFs will not work with this disk driver. */
        BSP_LED_On(LED_RED);

    } else {
        /* Congratulations! The disk driver works well. */
        BSP_LED_On(LED_GREEN);
        vTaskDelay(1);
    }

    return rc;
}
