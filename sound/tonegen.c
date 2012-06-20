#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/soundcard.h>
#include <math.h>

#define RATE 8000
#define SIZE 8
#define CHANNELS 1
#define LENGTH 1
#define PI 3.141592654

unsigned char buf[LENGTH*RATE*SIZE*CHANNELS/8]; /* Buffer to hold PCM data */



int main(int argc, char **argv)
{
		
	if ( argc != 2 )
	{
		fprintf(stderr, "Usage: %s frequency\n", argv[0]);
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

	/* Build the sample */
	unsigned char *pos = buf;
	double f1 = atof(argv[1]);
	double w1 = f1 * 2 * PI;
	int t;
	for(t = 0; t < RATE; ++t)
	{
		*pos = ((1 << 8) - 1 ) * sin(w1 * t/RATE);
		++pos;
	}

	/* Play the sample */
	unsigned long len = RATE*SIZE*CHANNELS*2*PI/(8*w1);
	while(1)
		status = write(fd, buf, len );

	return 0;
}
