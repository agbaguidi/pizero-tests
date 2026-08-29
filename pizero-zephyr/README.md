# pizero-zephyr

Zephyr module for the [Waveshare RP2350-PiZero](https://www.waveshare.com/wiki/RP2350-PiZero):
out-of-tree board, HAT shields, and bring-up samples.

This repository is the west **manifest**. Pin Zephyr here; other products (for
example BACprobe-Micro) should pull this repo in as a west project.

## Workspace

From the west workspace root (the directory that contains `.west/` and
`pizero-zephyr/`):

```text
west update
```

Board id:

```text
waveshare_pizero_rp2350/rp2350b/m33
```

Console is **UART0** on **GPIO0 (TX)** / **GPIO1 (RX)** at **115200** 8N1.

## Build samples

Bare board (no HAT):

```text
west build -b waveshare_pizero_rp2350/rp2350b/m33 pizero-zephyr/samples/hello_world
```

HAT samples set `SHIELD` in their own `CMakeLists.txt`. You can also pass
shields on the command line for any app:

```text
west build -b waveshare_pizero_rp2350/rp2350b/m33 pizero-zephyr/samples/rgb_led_hat
west build -b waveshare_pizero_rp2350/rp2350b/m33 pizero-zephyr/samples/rs485_can_hat
```

```text
west build -b waveshare_pizero_rp2350/rp2350b/m33 <app> -- -DSHIELD="waveshare_rgb_led_hat"
```

Multiple shields: `-DSHIELD="waveshare_rgb_led_hat waveshare_rs485_can_hat"`.

## Header buses (disabled by default)

Overlays and shields should set `status = "okay"` on the node they need.

| Bus   | Pins                                      | Node   | Default  |
| ----- | ----------------------------------------- | ------ | -------- |
| UART0 | GP0 TX, GP1 RX                            | uart0  | enabled  |
| UART1 | GP4 TX, GP5 RX                            | uart1  | disabled |
| I2C1  | GP2 SDA, GP3 SCL                          | i2c1   | disabled |
| I2C0  | GP8 SDA, GP9 SCL                          | i2c0   | disabled |
| SPI1  | GP10 SCK, GP11 TX, GP12 RX, GP13 CSN      | spi1   | disabled |
| SPI0  | GP16 RX, GP17 CSN, GP18 SCK, GP19 TX      | spi0   | disabled |
| ADC   | SoC GP40–47 (not the 40-pin GP26/27)      | adc    | disabled |
| PWM   | assign pins in an overlay                 | pwm    | disabled |

`SPI0` SCK is GPIO18, also used by the RGB LED HAT. Do not enable both on that pin.
