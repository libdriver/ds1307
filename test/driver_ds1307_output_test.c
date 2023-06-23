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
 * @file      driver_ds1307_output_test.c
 * @brief     driver ds1307 output test source file
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
 
#include "driver_ds1307_output_test.h"
#include <stdlib.h>

static ds1307_handle_t gs_handle;        /**< ds1307 handle */

/**
 * @brief     output test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ds1307_output_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    ds1307_info_t info;
    
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
    
    /* start output test */
    ds1307_interface_debug_print("ds1307: start output test.\n");
    
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
    
    /* set square wave mode */
    res = ds1307_set_output_mode(&gs_handle, DS1307_OUTPUT_MODE_SQUARE_WAVE);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output mode failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    ds1307_interface_debug_print("ds1307: set square wave mode 1Hz.\n");
    
    /* 1Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_1_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        ds1307_interface_delay_ms(1000);
        
        /* output */
        ds1307_interface_debug_print("ds1307: run %ds.\n", i + 1);
    }
    
    /* output */
    ds1307_interface_debug_print("ds1307: set square wave mode 4096Hz.\n");
    
    /* 4096Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_4096_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        ds1307_interface_delay_ms(1000);
        
        /* output */
        ds1307_interface_debug_print("ds1307: run %ds.\n", i + 1);
    }
    
    /* output */
    ds1307_interface_debug_print("ds1307: set square wave mode 8192Hz.\n");
    
    /* 8192Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_8192_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        ds1307_interface_delay_ms(1000);
        
        /* output */
        ds1307_interface_debug_print("ds1307: run %ds.\n", i + 1);
    }
    
    /* output */
    ds1307_interface_debug_print("ds1307: set square wave mode 32768Hz.\n");
    
    /* 32768Hz */
    res = ds1307_set_square_wave_frequency(&gs_handle, DS1307_SQUARE_WAVE_FREQUENCY_32768_HZ);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set square wave frequency failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        ds1307_interface_delay_ms(1000);
        
        /* output */
        ds1307_interface_debug_print("ds1307: run %ds.\n", i + 1);
    }
    
    /* set level mode */
    res = ds1307_set_output_mode(&gs_handle, DS1307_OUTPUT_MODE_LEVEL);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output mode failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    ds1307_interface_debug_print("ds1307: set output mode level high.\n");
    
    /* set high */
    res = ds1307_set_output_level(&gs_handle, DS1307_OUTPUT_LEVEL_HIGH);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output level failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        ds1307_interface_delay_ms(1000);
        
        /* output */
        ds1307_interface_debug_print("ds1307: run %ds.\n", i + 1);
    }
    
    /* output */
    ds1307_interface_debug_print("ds1307: set output mode level low.\n");
    
    /* set low */
    res = ds1307_set_output_level(&gs_handle, DS1307_OUTPUT_LEVEL_LOW);
    if (res != 0)
    {
        ds1307_interface_debug_print("ds1307: set output level failed.\n");
        (void)ds1307_deinit(&gs_handle);
        
        return 1;
    }
    
    /* loop */
    for (i = 0; i < times; i++)
    {
        /* delay 1000ms */
        ds1307_interface_delay_ms(1000);
        
        /* output */
        ds1307_interface_debug_print("ds1307: run %ds.\n", i + 1);
    }
    
    /* finish output test */
    ds1307_interface_debug_print("ds1307: finish output test.\n");
    (void)ds1307_deinit(&gs_handle);
    
    return 0;
}
