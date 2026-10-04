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

Schematics, datasheets, and HAT PDFs live under [`documents/`](../documents/).

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
west build -b waveshare_pizero_rp2350/rp2350b/m33 pizero-zephyr/samples/microsd
```

```text
west build -b waveshare_pizero_rp2350/rp2350b/m33 <app> -- -DSHIELD="waveshare_rgb_led_hat"
```

Multiple shields: `-DSHIELD="waveshare_rgb_led_hat waveshare_rs485_can_hat"`.

## Onboard microSD

The TF slot is wired in the board DTS as **SPI SDHC** on `spi1` (Zephyr has no RP2350 SDIO host driver yet):

| Signal | GPIO | Role |
| ------ | ---- | ---- |
| SD_SCK / SDIO_SCK | GP30 | SPI1 SCK |
| SD_MOSI / SDIO_CMD | GP31 | SPI1 TX |
| SD_MISO / SDIO_D0 | GP40 | SPI1 RX |
| SD_CS / SDIO_D3 | GP43 | `cs-gpios` (`gpio0_hi` pin 11) |
| SDIO_D1 / D2 | GP41 / GP42 | unused in SPI mode |

Card detect (`/CD`) is tied to GND on the schematic, so presence is detected by disk init / FatFS mount. Use a **FAT32** card with `samples/microsd`.

## Header buses (disabled by default)

Overlays and shields should set `status = "okay"` on the node they need.

| Bus   | Pins                                      | Node   | Default  |
| ----- | ----------------------------------------- | ------ | -------- |
| UART0 | GP0 TX, GP1 RX                            | uart0  | enabled  |
| UART1 | GP4 TX, GP5 RX                            | uart1  | disabled |
| I2C1  | GP2 SDA, GP3 SCL                          | i2c1   | disabled |
| I2C0  | GP8 SDA, GP9 SCL                          | i2c0   | disabled |
| SPI1  | GP30 SCK, GP31 TX, GP40 RX, GP43 CS (TF)  | spi1   | enabled (microSD) |
| SPI1 header alt | GP10 SCK, GP11 TX, GP12 RX, GP13 CSN | `spi1_default` | unused (remux only if TF unused) |
| SPI0  | GP16 RX, GP17 CSN, GP18 SCK, GP19 TX      | spi0   | disabled |
| ADC   | SoC GP40–47 (conflicts with TF on GP40+)  | adc    | disabled |
| PWM   | assign pins in an overlay                 | pwm    | disabled |

`SPI0` SCK is GPIO18, also used by the RGB LED HAT. Do not enable both on that pin.
