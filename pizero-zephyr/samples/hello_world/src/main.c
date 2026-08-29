#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

    printk("Hello World\r\n");

    if (!device_is_ready(uart)) {
        printk("ERROR: UART0 is not ready\r\n");
        return 0;
    }

    printk("UART0 is ready\r\n");

    while (1) {
        const char msg[] = "Hello World\r\n";

        for (size_t i = 0; i < sizeof(msg) - 1; i++) {
            uart_poll_out(uart, msg[i]);
        }

        k_sleep(K_SECONDS(1));
    }

    return 0;
}
