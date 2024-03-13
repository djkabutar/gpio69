#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/spi/spidev.h>

#include <gpio69.h>

#define BPW 8

struct spi_trans setup_spi(const char *spidev, int speed)
{
	int spi_m = SPI_MODE_0;
	const unsigned char bpw = BPW;
	struct spi_trans trans = {0};

	trans.speed = speed;

	trans.fd = open(spidev, O_RDWR);
	if (trans.fd < 0) {
		trans.error = 1;
		goto error;
	}

	if (ioctl(trans.fd, SPI_IOC_WR_MODE, &spi_m) < 0) {
		trans.error = 1;
		goto error;
	}

	if (ioctl(trans.fd, SPI_IOC_WR_BITS_PER_WORD, &bpw) < 0) {
		trans.error = 1;
		goto error;
	}

	if (ioctl(trans.fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
		trans.error = 1;
		goto error;
	}

error:
	return trans;
}

int spi_write_transaction(struct spi_trans trans, unsigned char *data, int len)
{
	struct spi_ioc_transfer spi = {0};

	if (trans.error)
		return -1;

	spi.tx_buf = (unsigned long) data;
	spi.rx_buf = (unsigned long) data;
	spi.len = len;
	spi.delay_usecs = 0;
	spi.speed_hz = trans.speed;
	spi.bits_per_word = BPW;

	return ioctl(trans.fd, SPI_IOC_MESSAGE(1), &spi);
}
