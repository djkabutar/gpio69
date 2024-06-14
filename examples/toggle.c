#include <unistd.h>

#include <gpio69.h>

int main(void) {
	int fd = gpio_request_handle("/dev/gpiochip2", 11, GPIO_OUT);

	gpio_set_value(fd, 1);
	sleep(0.01);
	gpio_set_value(fd, 0);
	sleep(0.01);

	gpio_release_handle(fd);
}
