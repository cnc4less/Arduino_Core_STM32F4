/**
  ******************************************************************************
  * @file    twi.h
  * @author  WI6LABS
  * @version V1.0.0
  * @date    01-August-2016
  * @brief   Header for twi module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TWI_H__
#define __TWI_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "PeripheralPins.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* offsetof is a gcc built-in function, this is the manual implementation */
#define OFFSETOF(type, member) ((uint32_t) (&(((type *)(0))->member)))

#define I2C_TXRX_BUFFER_SIZE    32

typedef struct i2c_s i2c_t;

struct i2c_s {
  /*  The 1st 2 members I2CName i2c
     *  and I2C_HandleTypeDef handle should
     *  be kept as the first members of this struct
     *  to have get_i2c_obj() function work as expected
     */
  I2C_TypeDef  *i2c;
  I2C_HandleTypeDef handle;
  PinName sda;
  PinName scl;
  IRQn_Type irq;
  uint8_t slaveMode;
  void (*i2c_onSlaveReceive)(uint8_t *, int);
  void (*i2c_onSlaveTransmit)(void);
  uint8_t i2cTxRxBuffer[I2C_TXRX_BUFFER_SIZE];
  uint8_t i2cTxRxBufferSize;
};

///@brief I2C state
typedef enum {
  I2C_OK = 0,
  I2C_TIMEOUT = 1,
  I2C_ERROR = 2,
  I2C_BUSY = 3
}i2c_status_e;

typedef enum {
  I2C_10KHz =   10000,
  I2C_50KHz =   50000,
  I2C_100KHz =  100000,
  I2C_200KHz =  200000,
  I2C_400KHz =  400000,
/*  I2C_600KHz =  600000,
  I2C_800KHz =  800000,
  I2C_1000KHz = 1000000*/ //Not supported
}i2c_timing_e;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void i2c_init(i2c_t *obj);
void i2c_custom_init(i2c_t *obj, i2c_timing_e timing, uint32_t addressingMode,
                    uint32_t ownAddress, uint8_t master);
void i2c_deinit(i2c_t *obj);
void i2c_setTiming(i2c_t *obj, uint32_t frequency);
i2c_status_e i2c_master_write(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint8_t size);
void i2c_slave_write_IT(i2c_t *obj, uint8_t *data, uint8_t size);
i2c_status_e i2c_master_read(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint8_t size);

i2c_status_e i2c_IsDeviceReady(i2c_t *obj, uint8_t devAddr,uint32_t trials);

void i2c_attachSlaveRxEvent(i2c_t *obj, void (*function)(uint8_t*, int) );
void i2c_attachSlaveTxEvent(i2c_t *obj, void (*function)(void) );

#ifdef __cplusplus
}
#endif

#endif /* __TWI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
