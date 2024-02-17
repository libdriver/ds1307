[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1307

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1307/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS1307 シリアル リアルタイム クロック (RTC) は、低電力、フル 2 進化 10 進数 (BCD) クロック/カレンダーと 56 バイトの NV SRAM です。 アドレスとデータは、I2C 双方向バスを介してシリアル転送されます。 時計/カレンダーは、秒、分、時、日、日付、月、年の情報を提供します。 31 日未満の月については、閏年の修正も含めて月末日が自動的に調整されます。 時計は、AM/PM インジケーター付きの 24 時間形式または 12 時間形式で動作します。 DS1307 には、電源障害を検出し、自動的にバックアップ電源に切り替える電源検出回路が内蔵されています。 部品がバックアップ電源で動作している間、計時動作は継続します。

LibDriver DS1307 は、LibDriver によって起動された ds1307 のフル機能ドライバーです。時間読み取り、方形波出力、その他の機能を提供します。 LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example output](#example-output)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver DS1307のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver DS1307用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver DS1307ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver DS1307プログラミング例が含まれています。

/ docディレクトリには、LibDriver DS1307オフラインドキュメントが含まれています。

/ datasheetディレクトリには、DS1307データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/ds1307/index.html](https://www.libdriver.com/docs/ds1307/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。