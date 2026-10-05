#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
// static because its only used in this file

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

static int set_pwm_value(int percent)
{
	uint32_t pulse_length = (uint64_t)pwm_led.period * percent / 100; // uint64_t to avoid overflow 20e6*100 at 100% duty cycle
	return pwm_set_dt(&pwm_led, pwm_led.period, pulse_length); 
}


int main(void)
{

	set_pwm_value(25);
	k_msleep(1000000);   /* hold 25 % so the scope can measure it */
	
	while (1) {
		for (int duty = 0; duty <= 100; duty++) {  // increase up for 1s
			set_pwm_value(duty);
			k_msleep(10);
		}
		for (int duty = 100; duty >= 0; duty--) {   // decrease for 1s
			set_pwm_value(duty);
			k_msleep(10);
	}
}
}

