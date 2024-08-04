[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1307

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1307/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS1307串列即時時鐘（RTC）是一種集成56位元組NV SRAM的低功耗、全二進位編碼十進位（BCD）時鐘/行事曆時鐘晶片。 地址和數據通過I2C雙向匯流排串列傳輸。 時鐘/行事曆提供秒、分鐘、小時、天、日期、月份和年份資訊。 對於少於31天的月份，月末日期會自動調整，包括閏年的更正。 時鐘以24小時或12小時的形式運行，帶有AM/PM指示器。 DS1307有一個內寘的電源感應電路，可以檢測電源故障並自動切換到備用電源。 當晶片通過備用電源運行時，計時操作仍在繼續。

LibDriver DS1307是LibDriver推出的DS1307全功能驅動，該驅動提供時間讀取和方波輸出等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example output](#example-output)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver DS1307的源文件。

/interface目錄包含了LibDriver DS1307與平台無關的IIC總線模板。

/test目錄包含了LibDriver DS1307驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver DS1307編程範例。

/doc目錄包含了LibDriver DS1307離線文檔。

/datasheet目錄包含了DS1307數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/ds1307/index.html](https://www.libdriver.com/docs/ds1307/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
