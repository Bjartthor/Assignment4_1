#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main(void)
{
	if (!gpio_is_ready_dt(&btn) || !gpio_is_ready_dt(&led)) {
		return 0;
	}
	gpio_pin_configure_dt(&btn, GPIO_INPUT);
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	int last = 0;
	while (1) {
		int now = gpio_pin_get_dt(&btn);
		if (now && !last) {  
            k_msleep(1);   
            now = gpio_pin_get_dt(&btn);
            if (now && !last){
                gpio_pin_toggle_dt(&led);
            }
		} 
		last = now;
	}
}
