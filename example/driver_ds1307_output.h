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
 * @file      driver_ds1307_output.h
 * @brief     driver ds1307 output header file
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
  
#ifndef DRIVER_DS1307_OUTPUT_H
#define DRIVER_DS1307_OUTPUT_H

#include "driver_ds1307_interface.h"
#include <time.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ds1307_example_driver
 * @{
 */

/**
 * @brief  output example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ds1307_output_init(void);

/**
 * @brief  output example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds1307_output_deinit(void);

/**
 * @brief     output example set the time
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t ds1307_output_set_time(ds1307_time_t *t);

/**
 * @brief     output example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t ds1307_output_set_timestamp(time_t timestamp);

/**
 * @brief     output example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t ds1307_output_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      output example get the time
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t ds1307_output_get_time(ds1307_time_t *t);

/**
 * @brief      output example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t ds1307_output_get_timestamp(time_t *timestamp);

/**
 * @brief      output example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t ds1307_output_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      output example get the ascii time
 * @param[out] *buf points to an ascii buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ds1307_output_get_ascii_time(char *buf, uint8_t len);

/**
 * @brief     output example set output mode
 * @param[in] mode is the output mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 * @note      none
 */
uint8_t ds1307_output_set_mode(ds1307_output_mode_t mode);

/**
 * @brief      output example get output mode
 * @param[out] *mode points to an output mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 * @note       none
 */
uint8_t ds1307_output_get_mode(ds1307_output_mode_t *mode);

/**
 * @brief     output example set output level
 * @param[in] level is the output level
 * @return    status code
 *            - 0 success
 *            - 1 set level failed
 * @note      none
 */
uint8_t ds1307_output_set_level(ds1307_output_level_t level);

/**
 * @brief      output example get output level
 * @param[out] *level points to an output level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get level failed
 * @note       none
 */
uint8_t ds1307_output_get_level(ds1307_output_level_t *level);

/**
 * @brief     output example set square wave frequency
 * @param[in] freq is the square wave frequency
 * @return    status code
 *            - 0 success
 *            - 1 set square wave frequency failed
 * @note      none
 */
uint8_t ds1307_output_set_square_wave_frequency(ds1307_square_wave_frequency_t freq);

/**
 * @brief      output example get square wave frequency
 * @param[out] *freq points to a square wave frequency buffer
 * @return     status code
 *             - 0 success
 *             - 1 get square wave frequency failed
 * @note       none
 */
uint8_t ds1307_output_get_square_wave_frequency(ds1307_square_wave_frequency_t *freq);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
