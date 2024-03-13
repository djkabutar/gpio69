struct spi_trans {
	int fd;
	int speed;
	int error;
};

int gpio_export(const char *pin);

int gpio_unexport(const char *pin);

void gpio_set_direction(const char *pin, const char *direction);

void gpio_set_value(const char *pin, const char *value);

int gpio_get_value(const char *pin);

struct spi_trans setup_spi(const char *spidev, int speed);

int spi_write_transaction(struct spi_trans trans, unsigned char *data, int len);
