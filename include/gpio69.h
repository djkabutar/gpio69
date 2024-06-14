#include <linux/gpio.h>

#define GPIO_IN GPIOHANDLE_REQUEST_INPUT
#define GPIO_OUT GPIOHANDLE_REQUEST_OUTPUT

struct spi_trans {
	int fd;
	int speed;
	int error;
};

int gpio_export(const char *pin);

int gpio_unexport(const char *pin);

void gpio_set_direction(const char *pin, const char *direction);

void gpio_set_value_legacy(const char *pin, const char *value);

int gpio_get_value_legacy(const char *pin);

int gpio_request_handle(const char* gpiochip, unsigned int offset, int flags);

int gpio_release_handle(int fd);

int gpio_set_value(int fd, unsigned char value);

int gpio_get_value(int fd);

struct spi_trans setup_spi(const char *spidev, int speed);

int spi_write_transaction(struct spi_trans trans, unsigned char *data, int len);
