#include <unistd.h>

#include <gpio69.h>

int main(void) {
	gpio_export("15");
	gpio_set_direction("15", "out");

	gpio_set_value("15", "1");
	sleep(1);
	gpio_set_value("15", "0");

	gpio_unexport("15");
}
