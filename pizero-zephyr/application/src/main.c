#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    printk("Hello from Waveshare PiZero RP2350!\r\n");

    while (1) {
        k_sleep(K_SECONDS(1));
        printk("Zephyr is running\r\n");
    }

    return 0;
}
