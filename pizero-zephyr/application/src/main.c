#include <zephyr/kernel.h>

int main(void)
{
    while (1) {
        printk("Waveshare PiZero RP2350\n");
        k_sleep(K_SECONDS(1));
    }

    return 0;
}