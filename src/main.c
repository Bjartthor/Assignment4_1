#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

// static because its only used in this file

static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static struct gpio_callback btn_cb; // what function to call when the interupt fires
static struct k_work_delayable LED_toggle_work; // toggle the LED, function not in the interrupt

static void LED_toggle(struct k_work *work)
{
	if (gpio_pin_get_dt(&btn)) {
		gpio_pin_toggle_dt(&led);
	}
}


static void btn_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	k_work_reschedule(&LED_toggle_work, K_MSEC(2)); //wait 2ms then run the LED_toggle
    // 2ms gives good margins since for error, since 1ms works from part1
}

int main(void)
{
	if (!gpio_is_ready_dt(&btn) || !gpio_is_ready_dt(&led)) {
		return 0;
	}
	gpio_pin_configure_dt(&btn, GPIO_INPUT);
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    k_work_init_delayable(&LED_toggle_work, LED_toggle); //when this note comes up run LED_toggle

    gpio_init_callback(&btn_cb, btn_isr, BIT(btn.pin)); // call button ISR for only this pin on the port
	gpio_add_callback(btn.port, &btn_cb); // let it know which port to look at
    gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE); // Interupt when button is pressed 

	while (1) {
		k_msleep(1000);
	}
}

