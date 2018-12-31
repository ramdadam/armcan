/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "fatfs.h"
#include "gfx.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void) {
    /* Mount the file system. */
    if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
        /*##-2- Register the file system object to the FatFs module ##############*/
        if (f_mount(&SDFatFS, (TCHAR const *) SDPath, 0) != FR_OK) {
            /* FatFs Initialization Error */
            Error_Handler();
        } else {
            /*##-3- Create a FAT file system (format) on the logical drive #########*/
            /* WARNING: Formatting the uSD card will delete all content on the device */
//            if (f_mkfs((TCHAR const *) SDPath, FM_FAT32, 0, buffer, sizeof(buffer)) != FR_OK) {
//                /* FatFs Format Error */
//                Error_Handler();
//            }
            if (f_open(&SDFile, "n.pgm", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
                /* 'STM32.TXT' file Open for write Error */
                Error_Handler();
            }
//            else {
//                int16_t width = 480;//480 x 272
//                int16_t height = 272;
//
//                f_printf(&SDFile, "P6\n%d %d\n255\n", width, height);
//                for (uint16_t y = 0; y < height; y++) {
//                    for (uint16_t x = 0; x < width; x++) {
//                        uint32_t color = gdispGetPixelColor(x, y);
//                        static uint8_t colorRGB[3];
//                        colorRGB[0] = GREEN_OF(color);  /* green */
//                        colorRGB[1] = BLUE_OF(color);  /* blue */
//                        colorRGB[2] = RED_OF(color);  /* red */
//                        f_write(&SDFile, colorRGB, 3, &byteswritten);
//                    }
//                }
//
//                f_close(&SDFile);
//
//            }
        }
    }
    /* USER CODE BEGIN Init */
    /* additional user code for init */
    /* USER CODE END Init */
}
uint32_t getFreeSpace() {
    DWORD fre_clust, fre_sect;
    FATFS* fs = &SDFatFS;

    FRESULT res = f_getfree("", &fre_clust, &fs);
    if (res) return 0;

    fre_sect = fre_clust * fs->csize;

    return fre_sect/2;
}

uint32_t getTotalSpace() {
    DWORD tot_sect;
    /* Get total sectors and free sectors */
    tot_sect = (SDFatFS.n_fatent - 2) * SDFatFS.csize;
    return tot_sect/2;
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void) {
    /* USER CODE BEGIN get_fattime */
    return 0;
    /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
