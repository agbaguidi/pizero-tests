#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>

#define STRIP_NODE DT_ALIAS(led_strip)

#if !DT_NODE_HAS_STATUS(STRIP_NODE, okay)
#error "LED strip device is not enabled"
#endif

#define NUM_PIXELS DT_PROP(STRIP_NODE, chain_length)

static const struct device *strip = DEVICE_DT_GET(STRIP_NODE);

static struct led_rgb pixels[NUM_PIXELS];

static int set_color(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i].r = r;
        pixels[i].g = g;
        pixels[i].b = b;
    }

    return led_strip_update_rgb(strip, pixels, NUM_PIXELS);
}

int main(void)
{
	int res = 0;

    printk("Hello from Waveshare PiZero RP2350!\r\n");

    printk("LED strip: %s\r\n", strip->name);
    printk("Number of pixels: %d\r\n", NUM_PIXELS);

    if (!device_is_ready(strip)) {
        printk("ERROR: LED strip device is NOT ready\r\n");
        return 0;
    }

    printk("LED strip device is ready\r\n");

    while (1) {

        printk("RED\r\n");
        res = set_color(0x05, 0x00, 0x00);
        if (res < 0) {
            printk("ERROR: Failed to set color\r\n");
        }
        k_sleep(K_SECONDS(2));

        printk("GREEN\r\n");
        res = set_color(0x00, 0x05, 0x00);
        if (res < 0) {
            printk("ERROR: Failed to set color\r\n");
        }
        k_sleep(K_SECONDS(2));

        printk("BLUE\r\n");
        res = set_color(0x00, 0x00, 0x05);
        if (res < 0) {
            printk("ERROR: Failed to set color\r\n");
        }
        k_sleep(K_SECONDS(2));

        printk("WHITE\r\n");
        res = set_color(0x05, 0x05, 0x05);
        if (res < 0) {
            printk("ERROR: Failed to set color\r\n");
        }
        k_sleep(K_SECONDS(2));

        printk("OFF\r\n");
        res = set_color(0x00, 0x00, 0x00);
        if (res < 0) {
            printk("ERROR: Failed to set color\r\n");
        }
        k_sleep(K_SECONDS(2));
    }

    return 0;
}
