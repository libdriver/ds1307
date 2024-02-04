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
 * @file      driver_ds1307_register_test.c
 * @brief     driver ds1307 register test source file
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

#include "driver_ds1307_register_test.h"
#include <stdlib.h>

static ds1307_handle_t gs_handle;        /**< ds1307 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ds1307_register_test(void)
{
    uint8_t res;
    ds1307_info_t info;
    ds1307_time_t time_in;
    ds1307_time_t time_out;
    ds1307_bool_t enable;
    ds1307_output_level_t level;
    ds1307_output_mode_t mode;
    ds1307_square_wave_frequency_t freq;
    
    /* link functions */
    DRIVER_DS1307_LINK_INIT(&gs_handle, ds1307_handle_t);
    DRIVER_DS1307_LINK_IIC_INIT(&gs_handle, ds1307_interface_iic_init);
    DRIVER_DS1307_LINK_IIC_DEINIT(&gs_handle, ds1307_interface_iic_deinit);
    DRIVER_DS1307_LINK_IIC_READ(&gs_handle, ds1307_interface_iic_read);
    DRIVER_DS1307_LINK_IIC_WRITE(&gs_handle, ds1307_interface_iic_write);
    DRIVER_DS1307_LINK_DELAY_MS(&gs_handle, ds1307_interface_delay_ms);
    DRIVER_DS1307_LINK_DEBUG_PRINT(&gs_handle, ds1307_interface_debug_print);
    
    /* get ds1307 info */
    res = ds1307_info(&info);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print ds1307 info */
        ds1307_interface_debug_print("ds1307: chip is %s.\n", info.chip_name);
        ds1307_interface_debug_print("ds1307: manufacturer is %s.\n", info.manufacturer_name);
        ds1307_interface_debug_print("ds1307: interface is %s.\n", info.interface);
        ds1307_interface_debug_print("ds1307: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds1307_interface_debug_print("ds1307: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds1307_interface_debug_print("ds1307: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds1307_interface_debug_print("ds1307: max current is %0.2fmA.\n", info.max_current_ma);
        ds1307_interface_debug_print("ds1307: max temperature is %0.1fC.\n", info.temperature_max);
        ds1307_interface_debug_print("ds1307: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    ds1307_interface_debug_print("ds1307: start register test.\n");
    
    /* init ds1307 */
    res = ds1307_init(&gs_handle);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: init failed.\n");
        
        return 1;
    }
    
    /* set oscillator */
    res = ds1307_set_oscillator(&gs_handle, DS1307_BOOL_TRUE);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set oscillator failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* ds1307_set_time/ds1307_get_time test */
    ds1307_interface_debug_print("ds1307: ds1307_set_time/ds1307_get_time test.\n");
    
    /* 12H format */
    time_in.format = DS1307_FORMAT_12H;
    time_in.am_pm = DS1307_PM;
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 11 + 1;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds1307_interface_debug_print("ds1307: set time %04d-%02d-%02d PM %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res  = ds1307_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set time failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    res = ds1307_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get time failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check time %s.\n", (memcmp((uint8_t *)&time_in, (uint8_t *)&time_out, sizeof(ds1307_time_t)) == 0) ? "ok" : "error");
    
    /* 24H format */
    time_in.format = DS1307_FORMAT_24H;
    time_in.am_pm = DS1307_AM;
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds1307_interface_debug_print("ds1307: set time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res  = ds1307_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set time failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    res = ds1307_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get time failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check time %s.\n", (memcmp((uint8_t *)&time_in, (uint8_t *)&time_out, sizeof(ds1307_time_t)) == 0) ? "ok" : "error");
    
    /* ds1307_set_oscillator/ds1307_get_oscillator test */
    ds1307_interface_debug_print("ds1307: ds1307_set_oscillator/ds1307_get_oscillator test.\n");
    
    /* enable */
    res = ds1307_set_oscillator(&gs_handle, DS1307_BOOL_TRUE);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set oscillator failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: enable oscillator.\n");
    res = ds1307_get_oscillator(&gs_handle, &enable);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get oscillator failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check oscillator %s.\n", (enable == DS1307_BOOL_TRUE) ? "ok" : "error");
    
    /* disable */
    res = ds1307_set_oscillator(&gs_handle, DS1307_BOOL_FALSE);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set oscillator failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: disable oscillator.\n");
    res = ds1307_get_oscillator(&gs_handle, &enable);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get oscillator failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check oscillator %s.\n", (enable == DS1307_BOOL_FALSE) ? "ok" : "error");
    
    /* ds1307_set_output_level/ds1307_get_output_level test */
    ds1307_interface_debug_print("ds1307: ds1307_set_output_level/ds1307_get_output_level test.\n");
    
    /* set output level high */
    res = ds1307_set_output_level(&gs_handle, DS1307_OUTPUT_LEVEL_HIGH);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output level failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set output level high */
    ds1307_interface_debug_print("ds1307: set output level high.\n");
    
    /* get output level */
    res = ds1307_get_output_level(&gs_handle, &level);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get output level failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check output level %s.\n", (level == DS1307_OUTPUT_LEVEL_HIGH) ? "ok" : "error");
    
    /* set output level low */
    res = ds1307_set_output_level(&gs_handle, DS1307_OUTPUT_LEVEL_LOW);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output level failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set output level low */
    ds1307_interface_debug_print("ds1307: set output level low.\n");
    
    /* get output level */
    res = ds1307_get_output_level(&gs_handle, &level);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get output level failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check output level %s.\n", (level == DS1307_OUTPUT_LEVEL_LOW) ? "ok" : "error");
    
    /* ds1307_set_output_mode/ds1307_get_output_mode test */
    ds1307_interface_debug_print("ds1307: ds1307_set_output_mode/ds1307_get_output_mode test.\n");
    
    /* set square wave mode */
    res = ds1307_set_output_mode(&gs_handle, DS1307_OUTPUT_MODE_SQUARE_WAVE);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output mode failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set square wave mode */
    ds1307_interface_debug_print("ds1307: set square wave mode.\n");
    
    /* get square wave mode */
    res = ds1307_get_output_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get output mode failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check output mode %s.\n", (mode == DS1307_OUTPUT_MODE_SQUARE_WAVE) ? "ok" : "error");
    
    /* set level mode */
    res = ds1307_set_output_mode(&gs_handle, DS1307_OUTPUT_MODE_LEVEL);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output mode failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set level mode */
    ds1307_interface_debug_print("ds1307: set level mode.\n");
    
    /* get level mode */
    res = ds1307_get_output_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get level mode failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check output mode %s.\n", (mode == DS1307_OUTPUT_MODE_LEVEL) ? "ok" : "error");
    
    /* ds1307_set_square_wave_frequency/ds1307_get_square_wave_frequency test */
    ds1307_interface_debug_print("ds1307: ds1307_set_square_wave_frequency/ds1307_get_square_wave_frequency test.\n");
    
    /* 1Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_1_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 1Hz */
    ds1307_interface_debug_print("ds1307: set square wave frequency 1Hz.\n");
    
    /* get square wave frequency */
    res = ds1307_get_square_wave_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check square wave frequency %s.\n", (freq == DS1307_SQUARE_WAVE_FREQUENCY_1_HZ) ? "ok" : "error");
    
    /* 4096Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_4096_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 4096Hz */
    ds1307_interface_debug_print("ds1307: set square wave frequency 4096Hz.\n");
    
    /* get square wave frequency */
    res = ds1307_get_square_wave_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check square wave frequency %s.\n", (freq == DS1307_SQUARE_WAVE_FREQUENCY_4096_HZ) ? "ok" : "error");
    
    /* 8192Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_8192_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 8192Hz */
    ds1307_interface_debug_print("ds1307: set square wave frequency 8192Hz.\n");
    
    /* get square wave frequency */
    res = ds1307_get_square_wave_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check square wave frequency %s.\n", (freq == DS1307_SQUARE_WAVE_FREQUENCY_8192_HZ) ? "ok" : "error");
    
    /* 32768Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_32768_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 32768Hz */
    ds1307_interface_debug_print("ds1307: set square wave frequency 32768Hz.\n");
    
    /* get square wave frequency */
    res = ds1307_get_square_wave_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: get square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    ds1307_interface_debug_print("ds1307: check square wave frequency %s.\n", (freq == DS1307_SQUARE_WAVE_FREQUENCY_32768_HZ) ? "ok" : "error");
    
    /* finish register test */
    ds1307_interface_debug_print("ds1307: finish register test.\n");
    (void)ds1307_deinit(&gs_handle);
    
    return 0;
}
