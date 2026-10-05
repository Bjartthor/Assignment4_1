#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);

int main(void)
{
    if (!gpio_is_ready_dt(&btn)) { return 0; }
    gpio_pin_configure_dt(&btn, GPIO_INPUT);
    while (1){
        int val = gpio_pin_get_dt(&btn);
        printk("button = %d\n", val);
        k_msleep(500);

    }
}
