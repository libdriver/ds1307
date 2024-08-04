[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1307

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1307/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS1307串行实时时钟（RTC）是一种集成56字节NV SRAM的低功耗、全二进制编码十进制（BCD）时钟/日历时钟芯片。地址和数据通过I2C双向总线串行传输。时钟/日历提供秒、分钟、小时、天、日期、月份和年份信息。对于少于31天的月份，月末日期会自动调整，包括闰年的更正。时钟以24小时或12小时的形式运行，带有AM/PM指示器。DS1307有一个内置的电源感应电路，可以检测电源故障并自动切换到备用电源。当芯片通过备用电源运行时，计时操作仍在继续。

LibDriver DS1307是LibDriver推出的DS1307全功能驱动，该驱动提供时间读取和方波输出等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example output](#example-output)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver DS1307的源文件。

/interface目录包含了LibDriver DS1307与平台无关的IIC总线模板。

/test目录包含了LibDriver DS1307驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver DS1307编程范例。

/doc目录包含了LibDriver DS1307离线文档。

/datasheet目录包含了DS1307数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_ds1307_basic.h"

uint8_t res;
uint8_t addr;
uint8_t data;
uint32_t t;
ds1307_time_t time;

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

...
    
t = 1612686500;

/* set time */
res = ds1307_basic_set_timestamp((time_t)t);
if (res != 0)
{
    (void)ds1307_basic_deinit();

    return 1;
}

/* output */
ds1307_interface_debug_print("ds1307: set timestamp %d.\n", (time_t)t);

...
    
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

...

addr = 0;
data = 0x08;

/* write ram */
res = ds1307_basic_write_ram(addr, &data, 1);
if (res != 0)
{
    (void)ds1307_basic_deinit();

    return 1;
}

/* output */
ds1307_interface_debug_print("ds1307: ram write addr %d 0x%02X.\n", addr, data);

/* read ram */
res = ds1307_basic_read_ram(addr, &data, 1);
if (res != 0)
{
    (void)ds1307_basic_deinit();

    return 1;
}

/* output */
ds1307_interface_debug_print("ds1307: ram read addr %d 0x%02X.\n", addr, data);

...
    
/* basic deinit */
(void)ds1307_basic_deinit();

return 0;
```

#### example output

```C
#include "driver_ds1307_output.h"

uint8_t res;
uint32_t t;
ds1307_time_t time;

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

...
    
t = 1612686500;

/* set time */
res = ds1307_output_set_timestamp((time_t)t);
if (res != 0)
{
    (void)ds1307_output_deinit();

    return 1;
}

/* output */
ds1307_interface_debug_print("ds1307: set timestamp %d.\n", (time_t)t);

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

...

/* set mode */
res = ds1307_output_set_mode(DS1307_OUTPUT_MODE_LEVEL);
if (res != 0)
{
    (void)ds1307_output_deinit();

    return 1;
}

/* set level */
res = ds1307_output_set_level(DS1307_OUTPUT_LEVEL_LOW);
if (res != 0)
{
    (void)ds1307_output_deinit();

    return 1;
}

...

/* set mode */
res = ds1307_output_set_mode(DS1307_OUTPUT_MODE_SQUARE_WAVE);
if (res != 0)
{
    (void)ds1307_output_deinit();

    return 1;
}

/* set freq */
res = ds1307_output_set_square_wave_frequency(DS1307_SQUARE_WAVE_FREQUENCY_1_HZ);
if (res != 0)
{
    (void)ds1307_output_deinit();

    return 1;
}

...
    
/* output deinit */
(void)ds1307_output_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/ds1307/index.html](https://www.libdriver.com/docs/ds1307/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。