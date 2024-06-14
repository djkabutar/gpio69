#include <unistd.h>

#include <gpio69.h>

int main(void) {
	gpio_export("75");
	gpio_set_direction("75", "out");

	gpio_set_value_legacy("75", "1");
	sleep(0.01);
	gpio_set_value_legacy("75", "0");
	sleep(0.01);

	gpio_unexport("75");
}
