#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/soundcard.h>

#define RATE 44100
#define SIZE 16
#define CHANNELS 2
#define LENGTH 2

unsigned char buf[LENGTH*RATE*SIZE*CHANNELS/8]; /* Buffer to hold PCM data */

int main(int argc, char **argv)
{
	if ( argc != 2 )
	{
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	int fd; /* Sound device */
	int arg; /* ioctl argument */
	int status; /* Return value of syscalls */

	/* open dsp */
	fd = open("/dev/dsp", O_RDWR);
	if (fd < 0)
	{
		perror("Failed to open /dev/dsp");
		exit(1);
	}

	/* Set sample parameters */
	arg = SIZE; /* Sample size */
	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
	if ( status == -1 )
	{
		perror("SOUND_PCM_WRITE_BITS failed");
		exit(1);
	}
	if ( arg != SIZE)
	{
		perror("Unable to set sample size");
		exit(1);
	}

	arg = CHANNELS; /* Mono or stereo */
	status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
	if ( status == -1 )
	{
		perror("SOUND_PCM_WRITE_CHANNELS failed");
		exit(1);
	}
	if (arg != CHANNELS)
	{
		perror("unable to set number of channels");
		exit(1);
	}

	arg = RATE;	   /* sampling rate */
	status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1)
	{
		perror("SOUND_PCM_WRITE_WRITE ioctl failed");
		exit(1);
	}

	FILE *f = fopen(argv[1], "r");
	fseek(f, 44, SEEK_SET);
	size_t bytes = 0;
	while ( ! feof(f) )
	{
		memset(buf, 0, LENGTH*RATE*SIZE*CHANNELS/8); /* Zero the memory */
		bytes = fread(buf, 1, LENGTH*RATE*SIZE*CHANNELS/8, f);
		status = write(fd, buf, bytes);
	}
	fclose(f);
	close(fd);

	return 0;
}
