//sudo insmod kernel_MPT.ko mystring="hello" GPIO=17,22,27 levels=1,0,1 toggle=2

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>


static char *mystring = "blah";
static int GPIO[3] = { 16, 20, 21 };
static int levels[3] = { 0, 1, 1 };
static int toggle = 0;
static int arr_argc = 0;

module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");
module_param_array(GPIO, int, &arr_argc, 0000);
MODULE_PARM_DESC(GPIO, "GPIO PINS");
module_param_array(levels, int, &arr_argc, 0000);
MODULE_PARM_DESC(levels, "LOW(0) or HIGH(1)");
module_param(toggle, int, 0000);
MODULE_PARM_DESC(toggle, "An int");
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
	int j;
	int LED1_State;
	int LED2_State;
	int LED3_State;

	printk(KERN_INFO "%s\n", __func__);
	printk(KERN_INFO "mystring is a string: %s\n", mystring);
	for (i = 0; i < (sizeof GPIO / sizeof (int)); i++)
	{
		//gpio_set_value(GPIO[i], levels[i]); 

		printk(KERN_INFO "GPIO[%d] = %d and level is %d and toggle is %d\n", i, GPIO[i],levels[i],toggle);

		gpio_set_value(GPIO[i], levels[i]); 
	}

	LED1_State=levels[0];
	LED2_State=levels[1];
	LED3_State=levels[2];

	for(j=0; j <10;j++){
		LED1_State=!LED1_State;
		LED2_State=!LED2_State;
		LED3_State=!LED3_State;

		gpio_set_value(GPIO[0], LED1_State); 
		gpio_set_value(GPIO[1], LED2_State); 
		gpio_set_value(GPIO[2], LED3_State); 
		msleep(toggle*1000);
	}
	
	// register LED GPIOs, turn LEDs on
	ret = gpio_request_array(leds, ARRAY_SIZE(leds));
	printk(KERN_INFO "Leds toggle.");
	msleep(toggle*1000);
	gpio_set_value(GPIO[0], 0); 
	gpio_set_value(GPIO[1], 0); 
	gpio_set_value(GPIO[2], 0); 

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
		printk(KERN_INFO "BMC: %d is uit.",leds[i].gpio);
	}
	printk(KERN_INFO "Leds off.");
	
	// unregister all GPIOs
	gpio_free_array(leds, ARRAY_SIZE(leds));
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Branko De Caluwé");
MODULE_DESCRIPTION("Opdracht6-Kernel");

module_init(gpiomod_init);
module_exit(gpiomod_exit);
