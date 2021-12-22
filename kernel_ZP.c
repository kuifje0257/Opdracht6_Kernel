//sudo insmod kernel_ZP.ko

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>

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
	int i;

	printk(KERN_INFO "%s\n", __func__);
	


	// register LED GPIOs, turn LEDs on
	ret = gpio_request_array(leds, ARRAY_SIZE(leds));

	for(i = 0; i < ARRAY_SIZE(leds); i++) {
		gpio_set_value(leds[i].gpio, 1); 
		printk(KERN_INFO "BMC: %d is aan.",leds[i].gpio);
	}
	printk(KERN_INFO "Leds on.");

	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
	}
	msleep(1);
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
	printk(KERN_INFO "Leds out.");
	// unregister all GPIOs
	gpio_free_array(leds, ARRAY_SIZE(leds));
	msleep(1);
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Branko De Caluwé");
MODULE_DESCRIPTION("Opdracht6-Kernel");

module_init(gpiomod_init);

module_exit(gpiomod_exit);
