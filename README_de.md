[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS1307

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds1307/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Die serielle Echtzeituhr (RTC) DS1307 ist eine stromsparende, vollständig binär codierte Dezimaluhr (BCD) mit 56 Byte NV-SRAM. Adressen und Daten werden seriell über einen bidirektionalen I2C-Bus übertragen. Die Uhr/der Kalender bietet Informationen zu Sekunden, Minuten, Stunden, Tag, Datum, Monat und Jahr. Das Monatsendedatum wird für Monate mit weniger als 31 Tagen automatisch angepasst, einschließlich Korrekturen für Schaltjahre. Die Uhr arbeitet entweder im 24-Stunden- oder 12-Stunden-Format mit AM/PM-Anzeige. Der DS1307 verfügt über eine integrierte Stromerkennungsschaltung, die Stromausfälle erkennt und automatisch auf die Notstromversorgung umschaltet. Der Zeitnehmungsbetrieb wird fortgesetzt, während das Teil mit der Notstromversorgung betrieben wird.

LibDriver DS1307 ist ein voll funktionsfähiger Treiber von ds1307, der von LibDriver gestartet wurde. Er bietet Zeitablesung, Rechteckwellenausgabe und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example output](#example-output)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver DS1307-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver DS1307 IIC.

/test enthält den Testcode des LibDriver DS1307-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver DS1307-Beispielcode.

/doc enthält das LibDriver DS1307-Offlinedokument.

/Datenblatt enthält DS1307-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/ds1307/index.html](https://www.libdriver.com/docs/ds1307/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.