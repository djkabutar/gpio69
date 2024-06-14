#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include <gpio69.h>

int gpio_export(const char *pin)
{
	const char *exp = "/sys/class/gpio/export";
	char *gpio_pin = NULL;
	size_t gpio_pin_len;
	int fd = -1;

	/* Calculate the required length for gpio_pin */
	gpio_pin_len = strlen("/sys/class/gpio/gpio") + strlen(pin) + 1;

	gpio_pin = (char *) malloc(gpio_pin_len);

	snprintf(gpio_pin, gpio_pin_len,
			"/sys/class/gpio/gpio%s", pin);

	/* If pin is already exported, return */
	if (!access(gpio_pin, F_OK))
		goto exit;

	fd = open(exp, O_WRONLY);
	if (fd < 0) {
		fprintf(stdout, "Failed to open %s\n", exp);
		return fd;
	}

	if (write(fd, pin, strlen(pin)) == -1)
		fprintf(stdout, "Failed to export pin %s to %s\n", pin, exp);

	close(fd);

exit:
	return 0;
}

int gpio_unexport(const char *pin)
{
	const char *unexport = "/sys/class/gpio/unexport";
	char *gpio_pin = NULL;
	size_t gpio_pin_len;
	int fd = -1;

	/* Calculate the required length for gpio_pin */
	gpio_pin_len = strlen("/sys/class/gpio/gpio") + strlen(pin) + 1;

	gpio_pin = (char *) malloc(gpio_pin_len);

	snprintf(gpio_pin, gpio_pin_len,
			"/sys/class/gpio/gpio%s", pin);

	/* If pin is already unexported, return */
	if (access(gpio_pin, F_OK) == -1)
		goto exit;

	fd = open(unexport, O_WRONLY);
	if (fd < 0) {
		fprintf(stdout, "Failed to open %s\n", unexport);
		return fd;
	}

	if (write(fd, pin, strlen(pin)) == -1)
		perror("Failed to unexport pin");

	close(fd);

exit:
	return 0;
}

void gpio_set_direction(const char *pin, const char *direction)
{
	int fd = -1;
	char *gpio_direction = NULL;
	size_t gpio_direction_len;

	/* Calculate the required length for gpio_direction */
	gpio_direction_len = strlen("/sys/class/gpio/gpio") + strlen(pin)
			+ strlen("/direction") + 1;

	gpio_direction = (char *) malloc(gpio_direction_len);
	if (!gpio_direction)
		perror("Failed to allocate memory for gpio_direction");

	snprintf(gpio_direction, gpio_direction_len,
			"/sys/class/gpio/gpio%s/direction", pin);

	if (access(gpio_direction, F_OK) == -1)
		goto free_gpio;

	fd = open(gpio_direction, O_WRONLY);
	if (fd < 0) {
		free(gpio_direction);
		perror("Failed to open");
	}

	if (write(fd, direction, strlen(direction)) == -1)
		fprintf(stdout, "Failed to write direction to %s\n",
				gpio_direction);

	close(fd);
free_gpio:
	free(gpio_direction);
}

void gpio_set_value_legacy(const char *pin, const char *value)
{
	const char *default_gpio = "/sys/class/gpio/gpio";
	char *gpio_value = NULL;
	size_t gpio_value_len;
	int fd = -1;

	/* Calculate the required length for gpio_value */
	gpio_value_len = strlen(default_gpio) + strlen(pin)
			+ strlen("/value") + 1;

	gpio_value = (char *) malloc(gpio_value_len);
	if (!gpio_value)
		perror("Failed to allocate memory for gpio_value");

	snprintf(gpio_value, gpio_value_len, "%s%s/value", default_gpio, pin);

	fd = open(gpio_value, O_WRONLY);
	if (fd < 0)
		goto free_gpio;

	if (write(fd, value, 1) == -1)
		fprintf(stdout, "Failed to write value to %s\n", gpio_value);

	close(fd);

free_gpio:
	free(gpio_value);
}

int gpio_get_value_legacy(const char *pin)
{
	const char *default_gpio = "/sys/class/gpio/gpio";
	char *gpio_value = NULL;
	size_t gpio_value_len;
	char value[1];
	int fd = -1;

	/* Calculate the required length for gpio_value */
	gpio_value_len = strlen(default_gpio) + strlen(pin)
			+ strlen("/value") + 1;

	gpio_value = (char *) malloc(gpio_value_len);
	if (!gpio_value)
		perror("Failed to allocate memory for gpio_value");

	snprintf(gpio_value, gpio_value_len, "%s%s/value", default_gpio, pin);

	fd = open(gpio_value, O_RDONLY);
	if (fd < 0)
		goto free_gpio;

	if (read(fd, value, 1) == -1)
		fprintf(stdout, "Failed to read value to %s\n", gpio_value);

	close(fd);

free_gpio:
	free(gpio_value);

	return (value[0] == 0x31);
}

int gpio_request_handle(const char* gpiochip, unsigned int offset, int flags)
{
	struct gpiohandle_request req;
	int fd = -1;
	int ret = 0;

	fd = open(gpiochip, O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open gpiochip");
		return fd;
	}

	req.lineoffsets[0] = offset;
	req.lines = 1;
	req.flags = flags;

	ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
	if (ret == -1)
	{
		perror("Unable to get line handle");
		close(fd);
		return ret;
	}

	return req.fd;
}

int gpio_release_handle(int fd)
{
	close(fd);
	return 0;
}

int gpio_set_value(int fd, unsigned char value)
{
	struct gpiohandle_data data;
	int ret = 0;
	data.values[0] = value;

	ret = ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
	if (ret == -1)
		perror("Unable to set line value");

	return ret;
}

int gpio_get_value(int fd)
{
	struct gpiohandle_data data;
	int ret = 0;

	ret = ioctl(fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
	if (ret == -1)
	{
		perror("Unable to get line value");
		return ret;
	}

	return data.values[0];
}
