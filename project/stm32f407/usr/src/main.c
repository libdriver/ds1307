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
 * @file      main.c
 * @brief     main source file
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
#include "driver_ds1307_ram_test.h"
#include "driver_ds1307_output_test.h"
#include "driver_ds1307_basic.h"
#include "driver_ds1307_output.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */

/**
 * @brief     ds1307 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ds1307(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"data", required_argument, NULL, 2},
        {"freq", required_argument, NULL, 3},
        {"mode", required_argument, NULL, 4},
        {"level", required_argument, NULL, 5},
        {"times", required_argument, NULL, 6},
        {"timestamp", required_argument, NULL, 7},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char mode_name[32] = {0};
    uint32_t times = 3;
    uint32_t t = 0;
    uint8_t time_flag = 0;
    uint8_t addr = 0x00;
    uint8_t addr_flag = 0;
    uint8_t data = 0x00;
    uint8_t data_flag = 0x00;
    ds1307_square_wave_frequency_t freq = DS1307_SQUARE_WAVE_FREQUENCY_1_HZ;
    uint8_t freq_flag = 0x00;
    ds1307_output_mode_t mode = DS1307_OUTPUT_MODE_LEVEL;
    uint8_t mode_flag = 0x00;
    ds1307_output_level_t level = DS1307_OUTPUT_LEVEL_LOW;
    uint8_t level_flag = 0x00;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* address */
            case 1 :
            {
                addr = atol(optarg) & 0xFF;
                addr_flag = 1;

                break;
            }

            /* data */
            case 2 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* set the data */
                data = hex_data % 0xFF;
                data_flag = 1;

                break;
            }

            /* freq */
            case 3 :
            {
                /* set the freq */
                if (strcmp("1HZ", optarg) == 0)
                {
                    freq = DS1307_SQUARE_WAVE_FREQUENCY_1_HZ;
                    freq_flag = 1;
                }
                else if (strcmp("4096HZ", optarg) == 0)
                {
                    freq = DS1307_SQUARE_WAVE_FREQUENCY_4096_HZ;
                    freq_flag = 1;
                }
                else if (strcmp("8192HZ", optarg) == 0)
                {
                    freq = DS1307_SQUARE_WAVE_FREQUENCY_8192_HZ;
                    freq_flag = 1;
                }
                else if (strcmp("32768HZ", optarg) == 0)
                {
                    freq = DS1307_SQUARE_WAVE_FREQUENCY_32768_HZ;
                    freq_flag = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* mode */
            case 4 :
            {
                /* set the mode */
                if (strcmp("LEVEL", optarg) == 0)
                {
                    mode = DS1307_OUTPUT_MODE_LEVEL;
                    strcpy(mode_name, "LEVEL");
                    mode_flag = 1;
                }
                else if (strcmp("SQUARE_WAVE", optarg) == 0)
                {
                    mode = DS1307_OUTPUT_MODE_SQUARE_WAVE;
                    strcpy(mode_name, "SQUARE_WAVE");
                    mode_flag = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* level */
            case 5 :
            {
                /* set the level */
                if (strcmp("LOW", optarg) == 0)
                {
                    level = DS1307_OUTPUT_LEVEL_LOW;
                    level_flag = 1;
                }
                else if (strcmp("HIGH", optarg) == 0)
                {
                    level = DS1307_OUTPUT_LEVEL_HIGH;
                    level_flag = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* running times */
            case 6 :
            {
                /* set the times */
                times = atol(optarg);

                break;
            }

            /* timestamp */
            case 7 :
            {
                /* set the timestamp */
                t = atoi(optarg);
                time_flag = 1;

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (ds1307_register_test() != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_ram", type) == 0)
    {
        /* run ram test */
        if (ds1307_ram_test(times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_output", type) == 0)
    {
        /* run output test */
        if (ds1307_output_test(times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_basic-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

        /* basic init */
        res = ds1307_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the zone */
        res = ds1307_basic_set_timestamp_time_zone(8);
        if (res != 0)
        {
            (void)ds1307_basic_deinit();

            return 1;
        }

        /* set time */
        res = ds1307_basic_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)ds1307_basic_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: set timestamp %d.\n", (time_t)t);

        /* basic deinit */
        (void)ds1307_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-get-time", type) == 0)
    {
        uint8_t res;
        ds1307_time_t time;

        /* basic init */
        res = ds1307_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* basic get time */
        res = ds1307_basic_get_time(&time);
        if (res != 0)
        {
            (void)ds1307_basic_deinit();

            return 1;
        }

        /* output */
        if (time.format == DS1307_FORMAT_24H)
        {
            ds1307_interface_debug_print("ds1307: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                         time.year, time.month, time.date,
                                         time.hour, time.minute, time.second, time.week
                                        );
        }
        else
        {
            ds1307_interface_debug_print("ds1307: %04d-%02d-%02d %s %02d:%02d:%02d %d.\n",
                                         time.year, time.month, time.date, (time.am_pm == DS1307_AM) ? "AM" : "PM",
                                         time.hour, time.minute, time.second, time.week
                                        );
        }

        /* basic deinit */
        (void)ds1307_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-set-ram", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (addr_flag != 1)
        {
            return 5;
        }

        /* check the flag */
        if (data_flag != 1)
        {
            return 5;
        }

        /* basic init */
        res = ds1307_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* write ram */
        res = ds1307_basic_write_ram(addr, &data, 1);
        if (res != 0)
        {
            (void)ds1307_basic_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: ram write addr %d 0x%02X.\n", addr, data);

        /* basic deinit */
        (void)ds1307_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-get-ram", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (addr_flag != 1)
        {
            return 5;
        }

        /* basic init */
        res = ds1307_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* read ram */
        res = ds1307_basic_read_ram(addr, &data, 1);
        if (res != 0)
        {
            (void)ds1307_basic_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: ram read addr %d 0x%02X.\n", addr, data);

        /* basic deinit */
        (void)ds1307_basic_deinit();

        return 0;
    }
    else if (strcmp("e_output-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the zone */
        res = ds1307_output_set_timestamp_time_zone(8);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* set time */
        res = ds1307_output_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: set timestamp %d.\n", (time_t)t);

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-time", type) == 0)
    {
        uint8_t res;
        ds1307_time_t time;

        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* output get time */
        res = ds1307_output_get_time(&time);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        if (time.format == DS1307_FORMAT_24H)
        {
            ds1307_interface_debug_print("ds1307: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                         time.year, time.month, time.date,
                                         time.hour, time.minute, time.second, time.week
                                        );
        }
        else
        {
            ds1307_interface_debug_print("ds1307: %04d-%02d-%02d %s %02d:%02d:%02d %d.\n",
                                         time.year, time.month, time.date, (time.am_pm == DS1307_AM) ? "AM" : "PM",
                                         time.hour, time.minute, time.second, time.week
                                        );
        }

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-set-mode", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (mode_flag != 1)
        {
            return 5;
        }

        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* set mode */
        res = ds1307_output_set_mode(mode);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: set mode %s.\n", mode_name);

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-mode", type) == 0)
    {
        uint8_t res;
        
        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* get mode */
        res = ds1307_output_get_mode(&mode);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: get mode %s.\n", mode == DS1307_OUTPUT_MODE_LEVEL ? "LEVEL" : "SQUARE_WAVE");

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-set-level", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (level_flag != 1)
        {
            return 5;
        }

        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* set level */
        res = ds1307_output_set_level(level);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: set level %s.\n", level == DS1307_OUTPUT_LEVEL_LOW ? "LOW" : "HIGH");

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-level", type) == 0)
    {
        uint8_t res;

        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* get level */
        res = ds1307_output_get_level(&level);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: get level %s.\n", level == DS1307_OUTPUT_LEVEL_LOW ? "LOW" : "HIGH");

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-set-freq", type) == 0)
    {
        const char str[][10] = {"1HZ", "4096HZ", "8192HZ", "32768HZ"};
        uint8_t res;

        /* check the flag */
        if (freq_flag != 1)
        {
            return 5;
        }

        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* set freq */
        res = ds1307_output_set_square_wave_frequency(freq);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: set freq %s.\n", str[freq]);

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-freq", type) == 0)
    {
        const char str[][10] = {"1HZ", "4096HZ", "8192HZ", "32768HZ"};
        uint8_t res;

        /* output init */
        res = ds1307_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* get freq */
        res = ds1307_output_get_square_wave_frequency(&freq);
        if (res != 0)
        {
            (void)ds1307_output_deinit();

            return 1;
        }

        /* output */
        ds1307_interface_debug_print("ds1307: get freq %s.\n", str[freq]);

        /* output deinit */
        (void)ds1307_output_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ds1307_interface_debug_print("Usage:\n");
        ds1307_interface_debug_print("  ds1307 (-i | --information)\n");
        ds1307_interface_debug_print("  ds1307 (-h | --help)\n");
        ds1307_interface_debug_print("  ds1307 (-p | --port)\n");
        ds1307_interface_debug_print("  ds1307 (-t reg | --test=reg)\n");
        ds1307_interface_debug_print("  ds1307 (-t ram | --test=ram) [--times=<num>]\n");
        ds1307_interface_debug_print("  ds1307 (-t output | --test=output) [--times=<num>]\n");
        ds1307_interface_debug_print("  ds1307 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>\n");
        ds1307_interface_debug_print("  ds1307 (-e basic-get-time | --example=basic-get-time)\n");
        ds1307_interface_debug_print("  ds1307 (-e basic-set-ram | --example=basic-set-ram) --addr=<address> --data=<hex>\n");
        ds1307_interface_debug_print("  ds1307 (-e basic-get-ram | --example=basic-get-ram) --addr=<address>\n");
        ds1307_interface_debug_print("  ds1307 (-e output-set-time | --example=output-set-time) --timestamp=<time>\n");
        ds1307_interface_debug_print("  ds1307 (-e output-get-time | --example=output-get-time)\n");
        ds1307_interface_debug_print("  ds1307 (-e output-set-mode | --example=output-set-mode) --mode=<LEVEL | SQUARE_WAVE>\n");
        ds1307_interface_debug_print("  ds1307 (-e output-get-mode | --example=output-get-mode)\n");
        ds1307_interface_debug_print("  ds1307 (-e output-set-level | --example=output-set-level) --level=<LOW | HIGH>\n");
        ds1307_interface_debug_print("  ds1307 (-e output-get-level | --example=output-get-level)\n");
        ds1307_interface_debug_print("  ds1307 (-e output-set-freq | --example=output-set-freq) --freq=<1HZ | 4096HZ | 8192HZ | 32768HZ>\n");
        ds1307_interface_debug_print("  ds1307 (-e output-get-freq | --example=output-get-freq)\n");
        ds1307_interface_debug_print("\n");
        ds1307_interface_debug_print("Options:\n");
        ds1307_interface_debug_print("      --addr=<address>            Set ram address.([default: 0])\n");
        ds1307_interface_debug_print("      --data=<hex>                Set ram data and it is hexadecimal.([default: 0x00])\n");
        ds1307_interface_debug_print("  -e <basic-set-time | basic-get-time | basic-set-ram | basic-get-ram | output-set-time | output-get-time |\n");
        ds1307_interface_debug_print("      output-set-mode | output-get-mode | output-set-level | output-get-level |\n");
        ds1307_interface_debug_print("      output-set-freq | output-get-freq>\n");
        ds1307_interface_debug_print("                                  Run the driver example.\n");
        ds1307_interface_debug_print("      --freq=<1HZ | 4096HZ | 8192HZ | 32768HZ>\n");
        ds1307_interface_debug_print("                                  Set output frequency.([default: 1HZ])\n");
        ds1307_interface_debug_print("  -h, --help                      Show the help.\n");
        ds1307_interface_debug_print("  -i, --information               Show the chip information.\n");
        ds1307_interface_debug_print("      --mode=<LEVEL | SQUARE_WAVE>\n");
        ds1307_interface_debug_print("                                  Set the output mode.([default: LEVEL])\n");
        ds1307_interface_debug_print("      --level=<LOW | HIGH>        Set output level.([default: LOW])\n");
        ds1307_interface_debug_print("  -p, --port                      Display the pin connections of the current board.\n");
        ds1307_interface_debug_print("  -t <reg | ram | output>, --test=<reg | ram | output>\n");
        ds1307_interface_debug_print("                                  Run the driver test.\n");
        ds1307_interface_debug_print("      --times=<num>               Set the running times.([default: 3])\n");
        ds1307_interface_debug_print("      --timestamp=<time>          Set the the unix timestamp.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ds1307_info_t info;

        /* print ds1307 info */
        ds1307_info(&info);
        ds1307_interface_debug_print("ds1307: chip is %s.\n", info.chip_name);
        ds1307_interface_debug_print("ds1307: manufacturer is %s.\n", info.manufacturer_name);
        ds1307_interface_debug_print("ds1307: interface is %s.\n", info.interface);
        ds1307_interface_debug_print("ds1307: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds1307_interface_debug_print("ds1307: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds1307_interface_debug_print("ds1307: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds1307_interface_debug_print("ds1307: max current is %0.2fmA.\n", info.max_current_ma);
        ds1307_interface_debug_print("ds1307: max temperature is %0.1fC.\n", info.temperature_max);
        ds1307_interface_debug_print("ds1307: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ds1307_interface_debug_print("ds1307: SCL connected to GPIOB PIN8.\n");
        ds1307_interface_debug_print("ds1307: SDA connected to GPIOB PIN9.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register ds1307 function */
    shell_init();
    shell_register("ds1307", ds1307);
    uart_print("ds1307: welcome to libdriver ds1307.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ds1307: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ds1307: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ds1307: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ds1307: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ds1307: param is invalid.\n");
            }
            else
            {
                uart_print("ds1307: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
