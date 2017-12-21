/*
** Copyright (C) 2001 Erik de Castro Lopo <erikd@zip.com.au>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/



#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<math.h>

#include	<sndfile.h>

#define	BUFFER_LEN		(1<<15)
#define LOG_BUFFER_SIZE	1024


static	void	test_float_peak	(char *str, char *filename, int typemajor) ;

static void		check_logged_peaks (char *buffer) ;

/* Force the start of this buffer to be double aligned. Sparc-solaris will
** choke if its not.
*/
static	double	data [BUFFER_LEN] ;
static	char	log_buffer [LOG_BUFFER_SIZE] ;
int		
main (int argc, char *argv[])
{	char	*filename ;
	int		bDoAll = 0 ;
	int		nTests = 0 ;

	if (argc != 2)
	{	printf ("Usage : %s <test>\n", argv [0]) ;
		printf ("    Where <test> is one of the following:\n") ;
		printf ("           wav  - test WAV file peak chunk\n") ;
		printf ("           aiff - test AIFF file PEAK chunk\n") ;
		printf ("           all  - perform all tests\n") ;
		exit (1) ;
		} ;

	bDoAll=!strcmp (argv [1], "all");
		
	if (bDoAll || ! strcmp (argv [1], "wav"))
	{	filename = "test.wav" ;
		test_float_peak ("wav", filename, SF_FORMAT_WAV) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "aiff"))
	{	filename = "test.aiff" ;
		test_float_peak	("aiff", filename, SF_FORMAT_AIFF) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (nTests == 0)
	{	printf ("Mono : ************************************\n") ;
		printf ("Mono : *  No '%s' test defined.\n", argv [1]) ;
		printf ("Mono : ************************************\n") ;
		return 1 ;
		} ;

	return 0;
} /* main */

/*============================================================================================
**	Here are the test functions.
*/ 

static void	
test_float_peak (char *str, char *filename, int typemajor)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	unsigned int	k, frames, count ;

	printf ("    test_float_peak : %s ... ", str) ;
	
	sfinfo.samplerate  = 44100 ;
	sfinfo.pcmbitwidth = 32 ;
	sfinfo.format 	   = (typemajor | SF_FORMAT_FLOAT) ;
	sfinfo.channels    = 4 ;
	sfinfo.samples     = 0 ;

	frames = BUFFER_LEN / sfinfo.channels ;
	
	/* Create some random data with a peak value of 0.66. */
	for (k = 0 ; k < BUFFER_LEN ; k++)
		data [k] = (rand () % 2000) / 3000.0 ;
		
	/* Insert some larger peaks a know locations. */
	data [4 * (frames / 8) + 0] = (frames / 8) * 0.01 ;	/* First channel */
	data [4 * (frames / 6) + 1] = (frames / 6) * 0.01 ;	/* Second channel */
	data [4 * (frames / 4) + 2] = (frames / 4) * 0.01 ;	/* Third channel */
	data [4 * (frames / 2) + 3] = (frames / 2) * 0.01 ;	/* Fourth channel */
		
	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;

	/*	Write the data in four passed. The data is designed so that peaks will 
	**	be written in the different calls to sf_write_double ().
	*/
	for (count = 0 ; count < 4 ; count ++)
	{	if ((k = sf_write_double (file, data + count * BUFFER_LEN / 4, BUFFER_LEN / 4, 0)) != BUFFER_LEN / 4)
		{	printf ("sf_write_double # %d failed with short write (%d ->%d)\n", count, BUFFER_LEN / 4, k) ;
			exit (1) ;
			} ;
		} ;
	
	sf_close (file) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != (typemajor | SF_FORMAT_FLOAT))
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", (typemajor | SF_FORMAT_FLOAT), sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples != frames)
	{	printf ("Incorrect number of samples in file. (%u => %u)\n", frames, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != 4)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 32)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;
		
	/* Get the log buffer data. */
	sf_get_header_info	(file, log_buffer, LOG_BUFFER_SIZE, 0) ;
	
	check_logged_peaks (log_buffer) ;

	sf_close (file) ;

	printf ("ok\n") ;
} /* test_float_peak */

static void
check_logged_peaks (char *buffer)
{	char 	*cptr ;
	int		k, chan, channel_count, position ;
	float	value ;
	
	if (strstr (buffer, "should") || strstr (buffer, "*"))
	{	printf ("Something wrong in buffer. Dumping.\n") ; 
		puts (buffer) ;
		exit (1) ;
		} ;

	if (! (cptr = strstr (buffer, "Channels")) || sscanf (cptr, "Channels      : %d", &channel_count) != 1)
	{	printf ("Couldn't find channel count.\n") ;
		exit (1) ;
		} ;
	
	if (channel_count != 4)
	{	printf ("Wrong channel count (4 ->%d).\n", channel_count) ;
		exit (1) ;
		} ;
		
	if (! (cptr = strstr (buffer, "Ch   Position       Value")))
	{	printf ("Can't find PEAK data.\n") ;
		exit (1) ;
		} ;
		
	for (k = 0 ; k < channel_count ; k++)
	{	if (! (cptr = strchr (cptr, '\n')))
		{	printf ("Got lost.\n") ;
			exit (1) ;
			} ;
		if (sscanf (cptr, "%d %d %f", &chan, &position, &value) != 3)
		{	printf ("sscanf failed.\n") ;
			exit (1) ;
			} ;
		if (position == 0)
		{	printf ("Error : peak position for channel %d should not be at offset 0.\n", chan) ;
			printf (buffer) ;
			exit (1) ;
			} ;
		if (chan != k || fabs (position * 0.01 - value) > 1e-6)
		{	printf (buffer) ;
			printf ("%d %f %f\n", chan, position * 0.01, value) ;
			exit (1) ;
			} ;
		cptr ++ ; /* Move past current newline. */
		} ;

} /* check_logged_peaks */
