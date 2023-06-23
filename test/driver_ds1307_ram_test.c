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
 * @file      driver_ds1307_ram_test.c
 * @brief     driver ds1307 ram test source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-03-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/05/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ds1307_ram_test.h"
#include <stdlib.h>

static ds1307_handle_t gs_handle;        /**< ds1307 handle */
static uint8_t gs_buf[56];               /**< buffer */
static uint8_t gs_check_buf[56];         /**< check buffer */

/**
 * @brief     ram test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ds1307_ram_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    uint8_t j;
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
    
    /* start ram test */
    ds1307_interface_debug_print("ds1307: start ram test.\n");
    
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
    
    for (i = 0; i < times; i++)
    {
        /* rand the data */
        for (j = 0; j < 56; j++)
        {
            gs_buf[j] = rand() & 0xFF;
        }
        
        /* write ram */
        res = ds1307_write_ram(&gs_handle, 0x00, gs_buf, 56);
        if (res != 0)
        {
            ds1307_interface_debug_print("ds1307: write ram failed.\n");
            (void)ds1307_deinit(&gs_handle);
            
            return 1;
        }
        
        /* read ram */
        res = ds1307_read_ram(&gs_handle, 0x00, gs_check_buf, 56);
        if (res != 0)
        {
            ds1307_interface_debug_print("ds1307: read ram failed.\n");
            (void)ds1307_deinit(&gs_handle);
            
            return 1;
        }
        
        /* check data */
        for (j = 0; j < 56; j++)
        {
            if (gs_buf[j] != gs_check_buf[j])
            {
                ds1307_interface_debug_print("ds1307: check buffer failed.\n");
                (void)ds1307_deinit(&gs_handle);
                
                return 1;
            }
        }
        
        /* output */
        ds1307_interface_debug_print("ds1307: %d/%d check passed.\n", i + 1, times);
    }
    
    /* finish ram test */
    ds1307_interface_debug_print("ds1307: finish ram test.\n");
    (void)ds1307_deinit(&gs_handle);
    
    return 0;
}
