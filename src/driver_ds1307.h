/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_ds1307.h
 * @brief     driver ds1307 header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-05-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/05/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_DS1307_H
#define DRIVER_DS1307_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ds1307_driver ds1307 driver function
 * @brief    ds1307 driver modules
 * @{
 */

/**
 * @addtogroup ds1307_base_driver
 * @{
 */

/**
 * @brief ds1307 bool enumeration definition
 */
typedef enum
{
    DS1307_BOOL_FALSE = 0x00,        /**< disable function */
    DS1307_BOOL_TRUE  = 0x01,        /**< enable function */
} ds1307_bool_t;

/**
 * @brief ds1307 am pm enumeration definition
 */
typedef enum
{
    DS1307_AM = 0x00,        /**< am */
    DS1307_PM = 0x01,        /**< pm */
} ds1307_am_pm_t;

/**
 * @brief ds1307 format enumeration definition
 */
typedef enum
{
    DS1307_FORMAT_12H = 0x01,        /**< 12h format */
    DS1307_FORMAT_24H = 0x00,        /**< 24h format */
} ds1307_format_t;

/**
 * @brief ds1307 output level enumeration definition
 */
typedef enum
{
    DS1307_OUTPUT_LEVEL_LOW  = 0x00,        /**< low level */
    DS1307_OUTPUT_LEVEL_HIGH = 0x01,        /**< high level */
} ds1307_output_level_t;

/**
 * @brief ds1307 output mode enumeration definition
 */
typedef enum
{
    DS1307_OUTPUT_MODE_LEVEL       = 0x00,        /**< level mode */
    DS1307_OUTPUT_MODE_SQUARE_WAVE = 0x01,        /**< square wave mode */
} ds1307_output_mode_t;

/**
 * @brief ds1307 square wave frequency enumeration definition
 */
typedef enum
{
    DS1307_SQUARE_WAVE_FREQUENCY_1_HZ     = 0x00,        /**< 1Hz */
    DS1307_SQUARE_WAVE_FREQUENCY_4096_HZ  = 0x01,        /**< 4.096kHz */
    DS1307_SQUARE_WAVE_FREQUENCY_8192_HZ  = 0x02,        /**< 8.192kHz */
    DS1307_SQUARE_WAVE_FREQUENCY_32768_HZ = 0x03,        /**< 32.768kHz */
} ds1307_square_wave_frequency_t;

/**
 * @brief ds1307 time structure definition
 */
typedef struct ds1307_time_s
{
    uint16_t year;                 /**< year */
    uint8_t month;                 /**< month */
    uint8_t week;                  /**< week */
    uint8_t date;                  /**< date */
    uint8_t hour;                  /**< hour */
    uint8_t minute;                /**< minute */
    uint8_t second;                /**< second */
    ds1307_format_t format;        /**< data format */
    ds1307_am_pm_t am_pm;          /**< am pm */
} ds1307_time_t;

/**
 * @brief ds1307 handle structure definition
 */
typedef struct ds1307_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    uint8_t inited;                                                                     /**< inited flag */
} ds1307_handle_t;

/**
 * @brief ds1307 information structure definition
 */
typedef struct ds1307_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ds1307_info_t;

/**
 * @}
 */

/**
 * @defgroup ds1307_link_driver ds1307 link driver function
 * @brief    ds1307 link driver modules
 * @ingroup  ds1307_driver
 * @{
 */

/**
 * @brief     initialize ds1307_handle_t structure
 * @param[in] HANDLE points to a ds1307 handle structure
 * @param[in] STRUCTURE is ds1307_handle_t
 * @note      none
 */
#define DRIVER_DS1307_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE points to a ds1307 handle structure
 * @param[in] FUC points to an iic_init function address
 * @note      none
 */
#define DRIVER_DS1307_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE points to a ds1307 handle structure
 * @param[in] FUC points to an iic_deinit function address
 * @note      none
 */
#define DRIVER_DS1307_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE points to a ds1307 handle structure
 * @param[in] FUC points to an iic_read function address
 * @note      none
 */
#define DRIVER_DS1307_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE points to a ds1307 handle structure
 * @param[in] FUC points to an iic_write function address
 * @note      none
 */
#define DRIVER_DS1307_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a ds1307 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_DS1307_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a ds1307 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_DS1307_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ds1307_base_driver ds1307 base driver function
 * @brief    ds1307 base driver modules
 * @ingroup  ds1307_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a ds1307 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ds1307_info(ds1307_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a ds1307 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t ds1307_init(ds1307_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a ds1307 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1307_deinit(ds1307_handle_t *handle);

/**
 * @brief     set the current time
 * @param[in] *handle points to a ds1307 handle structure
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 *            - 4 time is invalid
 * @note      none
 */
uint8_t ds1307_set_time(ds1307_handle_t *handle, ds1307_time_t *t);

/**
 * @brief      get the current time
 * @param[in]  *handle points to a ds1307 handle structure
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1307_get_time(ds1307_handle_t *handle, ds1307_time_t *t);

/**
 * @brief      read ram
 * @param[in]  *handle points to a ds1307 handle structure
 * @param[in]  addr is the ram address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read ram failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 addr > 55
 *             - 5 len is invalid
 * @note       none
 */
uint8_t ds1307_read_ram(ds1307_handle_t *handle, uint8_t addr, uint8_t *buf, uint8_t len);

/**
 * @brief     write ram
 * @param[in] *handle points to a ds1307 handle structure
 * @param[in] addr is the ram address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write ram failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr > 55
 *            - 5 len is invalid
 * @note      none
 */
uint8_t ds1307_write_ram(ds1307_handle_t *handle, uint8_t addr, uint8_t *buf, uint8_t len);

/**
 * @brief     enable or disable the oscillator
 * @param[in] *handle points to a ds1307 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set oscillator failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1307_set_oscillator(ds1307_handle_t *handle, ds1307_bool_t enable);

/**
 * @brief      get the chip oscillator status
 * @param[in]  *handle points to a ds1307 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get oscillator failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1307_get_oscillator(ds1307_handle_t *handle, ds1307_bool_t *enable);

/**
 * @brief     set the output level
 * @param[in] *handle points to a ds1307 handle structure
 * @param[in] level is the output level
 * @return    status code
 *            - 0 success
 *            - 1 set output level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1307_set_output_level(ds1307_handle_t *handle, ds1307_output_level_t level);

/**
 * @brief      get the output level
 * @param[in]  *handle points to a ds1307 handle structure
 * @param[out] *level points to an output level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get output level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1307_get_output_level(ds1307_handle_t *handle, ds1307_output_level_t *level);

/**
 * @brief     set the output mode
 * @param[in] *handle points to a ds1307 handle structure
 * @param[in] mode is the output mode
 * @return    status code
 *            - 0 success
 *            - 1 set output mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1307_set_output_mode(ds1307_handle_t *handle, ds1307_output_mode_t mode);

/**
 * @brief      get the output mode
 * @param[in]  *handle points to a ds1307 handle structure
 * @param[out] *mode points to an output mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get output mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1307_get_output_mode(ds1307_handle_t *handle, ds1307_output_mode_t *mode);

/**
 * @brief     set the square wave frequency
 * @param[in] *handle points to a ds1307 handle structure
 * @param[in] freq is the square wave frequency
 * @return    status code
 *            - 0 success
 *            - 1 set square wave frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1307_set_square_wave_frequency(ds1307_handle_t *handle, ds1307_square_wave_frequency_t freq);

/**
 * @brief      get the square wave frequency
 * @param[in]  *handle points to a ds1307 handle structure
 * @param[out] *freq points to a square wave frequency buffer
 * @return     status code
 *             - 0 success
 *             - 1 get square wave frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1307_get_square_wave_frequency(ds1307_handle_t *handle, ds1307_square_wave_frequency_t *freq);

/**
 * @}
 */

/**
 * @defgroup ds1307_extern_driver ds1307 extern driver function
 * @brief    ds1307 extern driver modules
 * @ingroup  ds1307_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to a ds1307 handle structure
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds1307_set_reg(ds1307_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a ds1307 handle structure
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds1307_get_reg(ds1307_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
