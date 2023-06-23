### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. DS1307

#### 3.1 Command Instruction

1. Show ds1307 chip and driver information.

   ```shell
   ds1307 (-i | --information)
   ```

2. Show ds1307 help.

   ```shell
   ds1307 (-h | --help)
   ```

3. Show ds1307 pin connections of the current board.

   ```shell
   ds1307 (-p | --port)
   ```

4. Run ds1307 register test.

   ```shell
   ds1307 (-t reg | --test=reg)
   ```

5. Run ds1307 ram test, num means test times.

   ```shell
   ds1307 (-t ram | --test=ram) [--times=<num>]
   ```

6. Run ds1307 output test, num means test times.

   ```shell
   ds1307 (-t output | --test=output) [--times=<num>]
   ```

7. Run ds1307 basic set time function, time is the unix timestamp.

   ```shell
   ds1307 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
   ```

8. Run ds1307 basic get time function.

   ```shell
   ds1307 (-e basic-get-time | --example=basic-get-time)
   ```

9. Run ds1307 basic set ram function, address is the ram start address, hex is the set data and it is hexadecimal.

    ```shell
    ds1307 (-e basic-set-ram | --example=basic-set-ram) --addr=<address> --data=<hex>
    ```

10. Run ds1307 basic get ram function, address is the ram start address.

    ```shell
    ds1307 (-e basic-get-ram | --example=basic-get-ram) --addr=<address> 
    ```

11. Run ds1307 output set time function, time is the unix timestamp.

      ```shell
      ds1307 (-e output-set-time | --example=output-set-time) --timestamp=<time>
      ```

12. Run ds1307 output get time function.

      ```shell
      ds1307 (-e output-get-time | --example=output-get-time)
      ```

13. Run ds1307 output set mode function.

     ```shell
     ds1307 (-e output-set-mode | --example=output-set-mode) --mode=<LEVEL | SQUARE_WAVE>
     ```

14. Run ds1307 output get mode function.

     ```shell
     ds1307 (-e output-get-mode | --example=output-get-mode)
     ```

15. Run ds1307 output set level function.

     ```shell
     ds1307 (-e output-set-level | --example=output-set-level) --level=<LOW | HIGH>
     ```

16. Run ds1307 output get level function. 

     ```shell
     ds1307 (-e output-get-level | --example=output-get-level)
     ```

17. Run ds1307 output set frequency function.

     ```shell
     ds1307 (-e output-set-freq | --example=output-set-freq) --freq=(1HZ | 4096HZ | 8192HZ | 32768HZ)
     ```

18. Run ds1307 output get frequency function.

     ```shell
     ds1307 (-e output-get-freq | --example=output-get-freq)
     ```

#### 3.2 Command Example

```shell
ds1307 -i

ds1307: chip is Maxim Integrated DS1307.
ds1307: manufacturer is Maxim Integrated.
ds1307: interface is IIC.
ds1307: driver version is 1.0.
ds1307: min supply voltage is 4.5V.
ds1307: max supply voltage is 5.5V.
ds1307: max current is 1.50mA.
ds1307: max temperature is 85.0C.
ds1307: min temperature is -40.0C.
```

```shell
ds1307 -p

ds1307: SCL connected to GPIOB PIN8.
ds1307: SDA connected to GPIOB PIN9.
```

```shell
ds1307 -t reg

ds1307: chip is Maxim Integrated DS1307.
ds1307: manufacturer is Maxim Integrated.
ds1307: interface is IIC.
ds1307: driver version is 1.0.
ds1307: min supply voltage is 4.5V.
ds1307: max supply voltage is 5.5V.
ds1307: max current is 1.50mA.
ds1307: max temperature is 85.0C.
ds1307: min temperature is -40.0C.
ds1307: start register test.
ds1307: ds1307_set_time/ds1307_get_time test.
ds1307: set time 2025-01-17 PM 09:31:09 5.
ds1307: check time ok.
ds1307: set time 2018-04-02 19:29:28 3.
ds1307: check time ok.
ds1307: ds1307_set_oscillator/ds1307_get_oscillator test.
ds1307: enable oscillator.
ds1307: check oscillator ok.
ds1307: disable oscillator.
ds1307: check oscillator ok.
ds1307: ds1307_set_output_level/ds1307_get_output_level test.
ds1307: set output level high.
ds1307: check output level ok.
ds1307: set output level low.
ds1307: check output level ok.
ds1307: ds1307_set_output_mode/ds1307_get_output_mode test.
ds1307: set square wave mode.
ds1307: check output mode ok.
ds1307: set level mode.
ds1307: check output mode ok.
ds1307: ds1307_set_square_wave_frequency/ds1307_get_square_wave_frequency test.
ds1307: set square wave frequency 1Hz.
ds1307: check square wave frequency ok.
ds1307: set square wave frequency 4096Hz.
ds1307: check square wave frequency ok.
ds1307: set square wave frequency 8192Hz.
ds1307: check square wave frequency ok.
ds1307: set square wave frequency 32768Hz.
ds1307: check square wave frequency ok.
ds1307: finish register test.
```

```shell
ds1307 -t ram --times=3

ds1307: chip is Maxim Integrated DS1307.
ds1307: manufacturer is Maxim Integrated.
ds1307: interface is IIC.
ds1307: driver version is 1.0.
ds1307: min supply voltage is 4.5V.
ds1307: max supply voltage is 5.5V.
ds1307: max current is 1.50mA.
ds1307: max temperature is 85.0C.
ds1307: min temperature is -40.0C.
ds1307: start ram test.
ds1307: 1/3 check passed.
ds1307: 2/3 check passed.
ds1307: 3/3 check passed.
ds1307: finish ram test.
```

```shell
ds1307 -t output --times=3

ds1307: chip is Maxim Integrated DS1307.
ds1307: manufacturer is Maxim Integrated.
ds1307: interface is IIC.
ds1307: driver version is 1.0.
ds1307: min supply voltage is 4.5V.
ds1307: max supply voltage is 5.5V.
ds1307: max current is 1.50mA.
ds1307: max temperature is 85.0C.
ds1307: min temperature is -40.0C.
ds1307: start output test.
ds1307: set square wave mode 1Hz.
ds1307: run 1s.
ds1307: run 2s.
ds1307: run 3s.
ds1307: set square wave mode 4096Hz.
ds1307: run 1s.
ds1307: run 2s.
ds1307: run 3s.
ds1307: set square wave mode 8192Hz.
ds1307: run 1s.
ds1307: run 2s.
ds1307: run 3s.
ds1307: set square wave mode 32768Hz.
ds1307: run 1s.
ds1307: run 2s.
ds1307: run 3s.
ds1307: set output mode level high.
ds1307: run 1s.
ds1307: run 2s.
ds1307: run 3s.
ds1307: set output mode level low.
ds1307: run 1s.
ds1307: run 2s.
ds1307: run 3s.
ds1307: finish output test.
```

```shell
ds1307 -e basic-set-time --timestamp=1612686500

ds1307: set timestamp 1612686500.
```

```shell
ds1307 -e basic-get-time

ds1307: 2021-02-07 16:28:35 7.
```

```shell
ds1307 -e basic-set-ram --addr=0 --data=0x08

ds1307: ram write addr 0 0x08.
```

```shell
ds1307 -e basic-get-ram --addr=0

ds1307: ram read addr 0 0x08.
```

```shell
ds1307 -e output-set-time --timestamp=1612686500

ds1307: set timestamp 1612686500.
```

```shell
ds1307 -e output-get-time

ds1307: 2021-02-07 16:28:37 7.
```

```shell
ds1307 -e output-set-mode --mode=LEVEL

ds1307: set mode LEVEL.
```

```shell
ds1307 -e output-get-mode

ds1307: get mode LEVEL.
```

```shell
ds1307 -e output-set-level --level=LOW

ds1307: set level LOW.
```

```shell
ds1307 -e output-get-level

ds1307: get level LOW.
```

```shell
ds1307 -e output-set-freq --freq=1HZ

ds1307: set freq 1HZ.
```

```shell
ds1307 -e output-get-freq

ds1307: get freq 1HZ.
```

```shell
ds1307 -h

Usage:
  ds1307 (-i | --information)
  ds1307 (-h | --help)
  ds1307 (-p | --port)
  ds1307 (-t reg | --test=reg)
  ds1307 (-t ram | --test=ram) [--times=<num>]
  ds1307 (-t output | --test=output) [--times=<num>]
  ds1307 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
  ds1307 (-e basic-get-time | --example=basic-get-time)
  ds1307 (-e basic-set-ram | --example=basic-set-ram) --addr=<address> --data=<hex>
  ds1307 (-e basic-get-ram | --example=basic-get-ram) --addr=<address>
  ds1307 (-e output-set-time | --example=output-set-time) --timestamp=<time>
  ds1307 (-e output-get-time | --example=output-get-time)
  ds1307 (-e output-set-mode | --example=output-set-mode) --mode=<LEVEL | SQUARE_WAVE>
  ds1307 (-e output-get-mode | --example=output-get-mode)
  ds1307 (-e output-set-level | --example=output-set-level) --level=<LOW | HIGH>
  ds1307 (-e output-get-level | --example=output-get-level)
  ds1307 (-e output-set-freq | --example=output-set-freq) --freq=<1HZ | 4096HZ | 8192HZ | 32768HZ>
  ds1307 (-e output-get-freq | --example=output-get-freq)

Options:
      --addr=<address>            Set ram address.([default: 0])
      --data=<hex>                Set ram data and it is hexadecimal.([default: 0x00])
  -e <basic-set-time | basic-get-time | basic-set-ram | basic-get-ram | output-set-time | output-get-time |
      output-set-mode | output-get-mode | output-set-level | output-get-level |
      output-set-freq | output-get-freq>
                                  Run the driver example.
      --freq=<1HZ | 4096HZ | 8192HZ | 32768HZ>
                                  Set output frequency.([default: 1HZ])
  -h, --help                      Show the help.
  -i, --information               Show the chip information.
      --mode=<LEVEL | SQUARE_WAVE>
                                  Set the output mode.([default: LEVEL])
      --level=<LOW | HIGH>        Set output level.([default: LOW])
  -p, --port                      Display the pin connections of the current board.
  -t <reg | ram | output>, --test=<reg | ram | output>
                                  Run the driver test.
      --times=<num>               Set the running times.([default: 3])
      --timestamp=<time>          Set the the unix timestamp.
```

#### 3.3 Command Problem

There is some unknown problem in the stm32f407 iic interface of ds1307, ram test command may try many times to run successfully.
