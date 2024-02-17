[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1307

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1307/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS1307 직렬 실시간 클록(RTC)은 저전력, 전체 BCD(Binary-coded Decimal) 클록/캘린더에 56바이트의 NV SRAM을 더한 것입니다. 주소와 데이터는 I2C 양방향 버스를 통해 직렬로 전송됩니다. 시계/달력은 초, 분, 시, 요일, 날짜, 월 및 연도 정보를 제공합니다. 윤년 보정을 포함하여 31일 미만의 달은 월말 날짜가 자동으로 조정됩니다. 시계는 AM/PM 표시기가 있는 24시간 또는 12시간 형식으로 작동합니다. DS1307에는 정전을 감지하고 자동으로 백업 전원으로 전환하는 전력 감지 회로가 내장되어 있습니다. 부품이 백업 공급 장치에서 작동하는 동안 시간 기록 작업이 계속됩니다.

LibDriver DS1307은 LibDriver에서 출시한 ds1307의 전체 기능 드라이버입니다. 시간 읽기, 구형파 출력 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example output](#example-output)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver DS1307의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver DS1307용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver DS1307드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver DS1307프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver DS1307오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 DS1307데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/ds1307/index.html](https://www.libdriver.com/docs/ds1307/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.