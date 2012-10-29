/*
 * Useful info:
 * http://elinux.org/RPi_Low-level_peripherals
 * http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>

#define BCM2708_PERI_BASE                 0x20000000

#define GPIO_PADS                         (BCM2708_PERI_BASE + 0x100000)
#define CLOCK_BASE                        (BCM2708_PERI_BASE + 0x101000)
#define GPIO_BASE                         (BCM2708_PERI_BASE + 0x200000)
#define GPIO_PWM                          (BCM2708_PERI_BASE + 0x20C000)

/* See page 8 of the Peripherals PDF */
#define AUX_PERIPHS                       (BCM2708_PERI_BASE + 0x215000)

// see table 6-1 of the PDF, on pages 90 and 91
struct gpio_registers {
	uint32_t gpfsel0;
	uint32_t gpfsel1;
	uint32_t gpfsel2;
	uint32_t gpfsel3;
	uint32_t gpfsel4;
	uint32_t gpfsel5;
	uint32_t reserved_1;
	uint32_t gpset0;
	uint32_t gpset1;
	uint32_t reserved_2;
	uint32_t gpclr0;
	uint32_t gpclr1;
	uint32_t reserved_3;
	uint32_t gplev0;
	uint32_t gplev1;
	uint32_t reserved_4;
	uint32_t gpeds0;
	uint32_t gpeds1;
	uint32_t reserved_5;
	uint32_t gpren0;
	uint32_t gpren1;
	uint32_t reserved_6;
	uint32_t gpfen0;
	uint32_t gpfen1;
	uint32_t reserved_7;
	uint32_t gphen0;
	uint32_t gphen1;
	uint32_t reserved_8;
	uint32_t gplen0;
	uint32_t gplen1;
	uint32_t reserved_9;
	uint32_t gparen0;
	uint32_t gparen1;
	uint32_t reserved_10;
	uint32_t gpafen0;
	uint32_t gpafen1;
	uint32_t reserved_11;
	uint32_t gppud;
	uint32_t gppudclk0;
	uint32_t gppudclk1;
};

// see register map, page 8 of PDF
struct aux_peripherals {
	uint32_t aux_irq;
	uint32_t aux_enables;
	uint32_t reserved_1[14];
	uint32_t aux_mu_io_reg;
	uint32_t aux_mu_ier_reg;
	uint32_t aux_mu_iir_reg;
	uint32_t aux_mu_lcr_reg;
	uint32_t aux_mu_mcr_reg;
	uint32_t aux_mu_lsr_reg;
	uint32_t aux_mu_msr_reg;
	uint32_t aux_mu_scratch;
	uint32_t aux_mu_cntl_reg;
	uint32_t aux_mu_stat_reg;
	uint32_t aux_mu_baud_reg;
	uint32_t reserved_2[20];
	uint32_t aux_spi0_cntl0_reg;
	uint32_t aux_spi0_cntl1_reg;
	uint32_t aux_spi0_stat_reg;
	uint32_t reserved_3;
	uint32_t aux_spi0_io_reg;
	uint32_t aux_spi0_peek_reg;
	uint32_t reserved_4[10];
	uint32_t aux_spi1_cntl0_reg;
	uint32_t aux_spi1_cntl1_reg;
	uint32_t aux_spi1_stat_reg;
	uint32_t reserved_5;
	uint32_t aux_spi1_io_reg;
	uint32_t aux_spi1_peek_reg;
};

static volatile struct gpio_registers *gpio;
static volatile struct aux_peripherals *aux;


int main(int argc, char **argv)
{
        int fd, i;

        //Obtain handle to physical memory
        if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
                printf("Unable to open /dev/mem: %s\n", strerror(errno));
                return -1;
        }

        //map a page of memory to gpio at offset 0x20200000 which is where GPIO goodness starts
        gpio = (struct gpio_registers *) mmap(0,
					      getpagesize(),
					      PROT_READ|PROT_WRITE,
					      MAP_SHARED,
					      fd,
					      GPIO_BASE);

        if ((int32_t) gpio < 0) {
                printf("GPIO mmap failed: %s\n", strerror(errno));
                return -1;
        }

        aux = (struct aux_peripherals *) mmap(0,
					      getpagesize(),
					      PROT_READ|PROT_WRITE,
					      MAP_SHARED,
					      fd,
					      AUX_PERIPHS);

        if ((int32_t) aux < 0) {
                printf("AUX mmap failed: %s\n", strerror(errno));
                return -1;
        }

        //set gpio17 as an output
	//Each of the GPFSELn registers controls the functions of ten GPIO pins. GPFSEL0 controls
	//GPIO0 thru GPIO9. We want GPIO17, which is controlled by bits 21-23 of GPFSEL1.
        //increment the pointer to 0x20200004, which is GPFSEL1, see table 6-2 on page 92 of the PDF
        //set the value through a little bit twiddling where we only modify the bits 21-23 in the register
	gpio->gpfsel1 &= ~(7 << 21);
	gpio->gpfsel1 |=  (1 << 21);

        //toggle gpio17 every second, just long enough to run over to the scope and make sure it works
	for (i = 0; i < 10; i++) {
		gpio->gpset0 = 1 << 17;    //set the pin high
		sleep(1);
		gpio->gpclr0 = 1 << 17;    //set the pin to low
		sleep(1);
	}

	aux->aux_mu_baud_reg = 999;    // 31.25 kbaud for MIDI, see PDF section 2.2.1 page 11
}
