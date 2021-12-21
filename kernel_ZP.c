//sudo insmod kernel_ZP.ko mystring="hello" GPIO=17,22,27

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

/*
 * Struct defining pins, direction and inital state 
 */
static struct gpio leds[] = {
		{ 17, GPIOF_OUT_INIT_HIGH, "LED 1" },  //Pin 11
		{ 27, GPIOF_OUT_INIT_HIGH, "LED 2" },  //Pin 13
		{ 22, GPIOF_OUT_INIT_HIGH, "LED 3" },  //Pin 15
};

/*
 * Module init function
 */
static int __init gpiomod_init(void)
{
	int ret = 0;

	printk(KERN_INFO "%s\n", __func__);


	// register LED GPIOs, turn LEDs on
	ret = gpio_request_array(leds, ARRAY_SIZE(leds));

	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
	}

	return ret;
}

/*
 * Module exit function
 */
static void __exit gpiomod_exit(void)
{
	int i;

	printk(KERN_INFO "%s\n", __func__);

	// turn all LEDs off
	for(i = 0; i < ARRAY_SIZE(leds); i++) {
		gpio_set_value(leds[i].gpio, 0); 
		printk(KERN_INFO "BMC: %d is out.",leds[i].gpio);
	}
	
	// unregister all GPIOs
	gpio_free_array(leds, ARRAY_SIZE(leds));
}

MODULE_AUTHOR("Branko De Caluwé");
MODULE_DESCRIPTION("Opdracht6-Kernel");

module_init(gpiomod_init);
module_exit(gpiomod_exit);
