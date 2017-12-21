/*
** Copyright (C) 1999-2001 Erik de Castro Lopo <erikd@zip.com.au>
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
#include	"check_log_buffer.h"

#define	SAMPLE_RATE			11025
#define	BUFFER_SIZE			(1<<14)

static	void	float_scaled_test		(char *str, int filetype, int bitwidth, double tolerance) ;
static	void	double_scaled_test		(char *str, int filetype, int bitwidth, double tolerance) ;

static	int		error_function (double data, double orig, double scale, double margin) ;
static	void	gen_float_signal (float *data, unsigned int datalen) ;
static	void	gen_double_signal (double *data, unsigned int datalen) ;

static	double	orig_data [BUFFER_SIZE] ;
static	double	test_data [BUFFER_SIZE] ;

int		main (int argc, char *argv[])
{	
	/* Float tests. */

	float_scaled_test	("float", SF_FORMAT_WAV  | SF_FORMAT_FLOAT, 32, 0.005) ;

	/* Test both signed (AIFF) and unsigned (WAV) 8 bit files. */
	float_scaled_test	("pcm_s8", SF_FORMAT_AIFF | SF_FORMAT_PCM, 8, 0.01) ;
	float_scaled_test	("pcm_u8", SF_FORMAT_WAV  | SF_FORMAT_PCM, 8, 0.01) ;

	float_scaled_test	("pcm_16", SF_FORMAT_WAV  | SF_FORMAT_PCM, 16, 0.01) ;
	float_scaled_test	("pcm_24", SF_FORMAT_WAV  | SF_FORMAT_PCM, 24, 0.01) ;
	float_scaled_test	("pcm_32", SF_FORMAT_WAV  | SF_FORMAT_PCM, 32, 0.01) ;
	
	float_scaled_test	("ulaw", SF_FORMAT_AU | SF_FORMAT_ULAW, 16, 0.01) ;
	float_scaled_test	("alaw", SF_FORMAT_AU | SF_FORMAT_ALAW, 16, 0.01) ;
	
	float_scaled_test	("imaadpcm", SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 16, 0.05) ;
	float_scaled_test	("msadpcm" , SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 16, 0.05) ;
	float_scaled_test	("gsm610"  , SF_FORMAT_WAV | SF_FORMAT_GSM610, 16, 0.08) ;

	float_scaled_test	("g721_32", SF_FORMAT_AU | SF_FORMAT_G721_32, 16, 0.02) ;
	float_scaled_test	("g723_24", SF_FORMAT_AU | SF_FORMAT_G723_24, 16, 0.02) ;
	
	/* Double tests. */

	/* Test both signed (AIFF) and unsigned (WAV) 8 bit files. */
	double_scaled_test	("pcm_s8", SF_FORMAT_AIFF | SF_FORMAT_PCM, 8, 0.01) ;
	double_scaled_test	("pcm_u8", SF_FORMAT_WAV  | SF_FORMAT_PCM, 8, 0.01) ;

	double_scaled_test	("pcm_16", SF_FORMAT_WAV  | SF_FORMAT_PCM, 16, 0.01) ;
	double_scaled_test	("pcm_24", SF_FORMAT_WAV  | SF_FORMAT_PCM, 24, 0.01) ;
	double_scaled_test	("pcm_32", SF_FORMAT_WAV  | SF_FORMAT_PCM, 32, 0.01) ;
	
	double_scaled_test	("ulaw", SF_FORMAT_AU | SF_FORMAT_ULAW, 16, 0.01) ;
	double_scaled_test	("alaw", SF_FORMAT_AU | SF_FORMAT_ALAW, 16, 0.01) ;
	
	double_scaled_test	("imaadpcm", SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 16, 0.05) ;
	double_scaled_test	("msadpcm" , SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 16, 0.05) ;
	double_scaled_test	("gsm610"  , SF_FORMAT_WAV | SF_FORMAT_GSM610, 16, 0.08) ;

	double_scaled_test	("g721_32", SF_FORMAT_AU | SF_FORMAT_G721_32, 16, 0.02) ;
	double_scaled_test	("g723_24", SF_FORMAT_AU | SF_FORMAT_G723_24, 16, 0.02) ;

	return 0;
} /* main */

/*============================================================================================
 *	Here are the test functions.
 */ 

static	
void	float_scaled_test (char *str, int filetype, int bitwidth, double tolerance)
{	char 			filename [256] ;
	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	unsigned int	k ;
	float			*float_data, *float_test ;
	double			scale ;
	
	strncpy (filename, str, sizeof (filename)) ;
	filename [sizeof (filename) - 1] = 0 ;
	strncat (filename, ".dat", sizeof (filename)) ;
	filename [sizeof (filename) - 1] = 0 ;

	printf ("    float_scaled_test  : %s ", str) ;
	for (k = strlen (str) ; k < 10 ; k++)
		putchar ('.') ;
	putchar (' ') ;
	
	float_data = (float*) orig_data ;
	float_test = (float*) test_data ;

	gen_float_signal (float_data, BUFFER_SIZE) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = BUFFER_SIZE ;
	sfinfo.channels    = 1 ;
	sfinfo.pcmbitwidth = bitwidth ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	sf_command (file, "norm float", "on", 0) ;
	
	if (sf_write_float (file, float_data, BUFFER_SIZE) != BUFFER_SIZE)
	{	printf ("sf_write_float failed with error : ") ;
		sf_perror (file) ;
		exit (1) ;
		} ;
		
	sf_close (file) ;
	
	memset (float_test, 0, BUFFER_SIZE * sizeof (float)) ;

	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < BUFFER_SIZE)
	{	printf ("%s (line %d) : Incorrect number of samples in file (too short). (%d should be %d)\n", __FILE__, __LINE__, sfinfo.samples, BUFFER_SIZE) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != 1)
	{	printf ("%s (line %d) : Incorrect number of channels in file.\n", __FILE__, __LINE__) ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != bitwidth)
	{	printf ("%s (line %d) : Incorrect bit width (%d => %d).\n", __FILE__, __LINE__, bitwidth, sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
	
	sf_command (file, "norm float", "on", 0) ;
		
	if ((k = sf_read_float (file, float_test, BUFFER_SIZE)) < 0.99 * BUFFER_SIZE)
	{	printf ("%s (line %d) : short read (%d).\n", __FILE__, __LINE__, k) ;
		exit (1) ;
		} ;

	sf_close (file) ;

	scale = bitwidth > 8 ? 32600.0 : 126.0 ;
	for (k = 0 ; k < BUFFER_SIZE ; k++)
		if (error_function (float_test [k], float_data [k], scale, tolerance))
		{	printf ("%s (line %d) : Incorrect sample (#%d : %f should be %f).\n", __FILE__, __LINE__, k, float_test [k], float_data [k]) ;
			exit (1) ;
			} ;

	unlink (filename) ;
			
	printf ("ok\n") ;
} /* float_scaled_test */

static	
void	double_scaled_test (char *str, int filetype, int bitwidth, double tolerance)
{	char 			filename [256] ;
	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	unsigned int	k ;
	double			scale ;
	
	strncpy (filename, str, sizeof (filename)) ;
	filename [sizeof (filename) - 1] = 0 ;
	strncat (filename, ".dat", sizeof (filename)) ;
	filename [sizeof (filename) - 1] = 0 ;
	

	printf ("    double_scaled_test : %s ", str) ;
	for (k = strlen (str) ; k < 10 ; k++)
		putchar ('.') ;
	putchar (' ') ;

	gen_double_signal (orig_data, BUFFER_SIZE) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = BUFFER_SIZE ;
	sfinfo.channels    = 1 ;
	sfinfo.pcmbitwidth = bitwidth ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("%s (line %d) : sf_open_write failed with error : ", __FILE__, __LINE__) ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;

	/* This does nothing at the moment. */	
	sf_command (file, "norm double", "on", 0) ;
	
	if (sf_write_double (file, orig_data, BUFFER_SIZE, 1) != BUFFER_SIZE)
	{	printf ("%s (line %d) : sf_write_double failed with error : ", __FILE__, __LINE__) ;
		sf_perror (file) ;
		exit (1) ;
		} ;
		
	sf_close (file) ;
	
	memset (test_data, 0, sizeof (test_data)) ;

	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("%s (line %d) : sf_open_read failed with error : ", __FILE__, __LINE__) ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("%s (line %d) : Returned format incorrect (0x%08X => 0x%08X).\n", __FILE__, __LINE__, filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < BUFFER_SIZE)
	{	printf ("%s (line %d) : Incorrect number of samples in file (too short). (%d should be %d)\n", __FILE__, __LINE__, sfinfo.samples, BUFFER_SIZE) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != 1)
	{	printf ("%s (line %d) : Incorrect number of channels in file.\n", __FILE__, __LINE__) ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != bitwidth)
	{	printf ("%s (line %d) : Incorrect bit width (%d => %d).\n", __FILE__, __LINE__, bitwidth, sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	/* This does nothing at the moment. */	
	sf_command (file, "norm double", "on", 0) ;
	
	if ((k = sf_read_double (file, test_data, BUFFER_SIZE, 1)) < 0.99 * BUFFER_SIZE)
	{	printf ("%s (line %d) : short read (%d).\n", __FILE__, __LINE__, k) ;
		exit (1) ;
		} ;

	sf_close (file) ;

	scale = bitwidth > 8 ? 32600.0 : 126.0 ;
	for (k = 0 ; k < BUFFER_SIZE ; k++)
		if (error_function (test_data [k], orig_data [k], scale, tolerance))
		{	printf ("%s (line %d) : Incorrect sample (#%d : %f should be %f).\n", __FILE__, __LINE__, k, test_data [k], orig_data [k]) ;
			exit (1) ;
			} ;

	unlink (filename) ;
			
	printf ("ok\n") ;
} /* double_scaled_test */

/*========================================================================================
**	Auxiliary functions
*/

static
void	gen_float_signal (float *data, unsigned int datalen)
{	unsigned int k, ramplen ;
	float	amp = 0.0 ;
	
	ramplen = datalen / 20 ;

	for (k = 0 ; k < datalen ; k++)
	{	if (k <= ramplen)
			amp = k / ((float) ramplen) ;
		else if (k > datalen - ramplen)
			amp = (datalen - k) / ((float) ramplen) ;
		data [k] = amp * (0.5 * sin (202.2 * 2.0 * M_PI * ((float) (k+1)) / ((float) SAMPLE_RATE))
							+ 0.4 * cos (441.1 * 2.0 * M_PI * ((float) (k+1)) / ((float) SAMPLE_RATE))) ;
		} ;
	return ;
} /* gen_float_signal */

static
void	gen_double_signal (double *data, unsigned int datalen)
{	unsigned int k, ramplen ;
	double	amp = 0.0 ;
	
	ramplen = datalen / 20 ;

	for (k = 0 ; k < datalen ; k++)
	{	if (k <= ramplen)
			amp = k / ((double) ramplen) ;
		else if (k > datalen - ramplen)
			amp = (datalen - k) / ((double) ramplen) ;
		data [k] = amp * (0.5 * sin (202.2 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))
							+ 0.4 * cos (441.1 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))) ;
		} ;
	return ;
} /* gen_double_signal */

static
int error_function (double data, double orig, double scale, double margin)
{	double error ;

	data = scale * fabs (data) ;
	orig = scale * fabs (orig) ;

	if (data < 1.0 && orig < 1.0)
		return 0 ;

	if (data < 2.0 && orig < 1.0)
		return 0 ;

	if (data < 1.0 && orig < 2.0)
		return 0 ;

	error = fabs (orig - data) * orig / scale ;
	if (error / (data + orig) > margin)
	{	return 1 ;
		} ;
	return 0 ;
} /* error_function */
