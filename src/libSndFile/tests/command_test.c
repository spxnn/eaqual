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

#define	BUFFER_LEN		(1<<10)
#define LOG_BUFFER_SIZE	1024


static	void	float_norm_test	(void) ;


/* Force the start of this buffer to be double aligned. Sparc-solaris will
** choke if its not.
*/

static	float	float_data [BUFFER_LEN] ;


int		
main (int argc, char *argv[])
{	/*-char	*filename ;-*/
	int		bDoAll = 0 ;
	int		nTests = 0 ;

	if (argc != 2)
	{	printf ("Usage : %s <test>\n", argv [0]) ;
		printf ("    Where <test> is one of the following:\n") ;
		printf ("           ver   - test sf_command (\"get lib ver\")\n") ;
		printf ("           text  - test adding of text strings\n") ;
		printf ("           fnorm - test floating point normalisation\n") ;
		printf ("           dnorm - test double precision floating point normalisation\n") ;
		printf ("           peak  - test PEAK chunk for integer files\n") ;
		printf ("           all   - perform all tests\n") ;
		exit (1) ;
		} ;

	bDoAll =! strcmp (argv [1], "all");
		
	if (bDoAll || ! strcmp (argv [1], "ver"))
	{	char buffer [128] ;
		sf_command (NULL, "lib version", buffer, sizeof (buffer)) ;
		printf ("version : %s\n", buffer) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "fnorm"))
	{	float_norm_test () ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "text"))
	{	sf_command (NULL, "read text", NULL, 0) ;
		sf_command (NULL, "write text", NULL, 0) ;
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
float_norm_test (void)
{	static char filename [] = "fnorm.wav" ;

	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	unsigned int	k ;

	printf ("    float_norm_test : ") ;
	fflush (stdout) ;
	
	sfinfo.samplerate  = 44100 ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = (SF_FORMAT_WAV | SF_FORMAT_PCM) ;
	sfinfo.channels    = 1 ;
	sfinfo.samples     = BUFFER_LEN ;

	/* Create float_data with all values being less than 1.0. */
	for (k = 0 ; k < BUFFER_LEN / 2 ; k++)
		float_data [k] = (k + 5) / (2.0 * BUFFER_LEN) ;
	for (k = BUFFER_LEN / 2 ; k < BUFFER_LEN ; k++)
		float_data [k] = (k + 5) ;
		
	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;

	/* Data is normalised so trun normalisation on before writing. */
	sf_command (file, "norm float", "on", 0) ;

	if ((k = sf_write_float (file, float_data, BUFFER_LEN / 2)) != BUFFER_LEN / 2)
	{	printf ("sf_write_float failed with short write (%d ->%d)\n", BUFFER_LEN, k) ;
		exit (1) ;
		} ;
	
	sf_command (file, "norm float", "off", 0) ;

	if ((k = sf_write_float (file, float_data + BUFFER_LEN / 2, BUFFER_LEN / 2)) != BUFFER_LEN / 2)
	{	printf ("sf_write_float failed with short write (%d ->%d)\n", BUFFER_LEN, k) ;
		exit (1) ;
		} ;
	
	sf_close (file) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM))
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", (SF_FORMAT_WAV | SF_FORMAT_PCM), sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples != BUFFER_LEN)
	{	printf ("\n\nError (%s:%d) : Incorrect number of samples in file. (%u => %u)\n", __FILE__, __LINE__, BUFFER_LEN, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != 1)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	/* Read float_data and check that it is NOT normalised. */
	if ((k = sf_read_float (file, float_data, BUFFER_LEN)) != BUFFER_LEN)
	{	printf ("\n\nError (%s:%d) : sf_read_float failed with short read (%d ->%d)\n", __FILE__, __LINE__, BUFFER_LEN, k) ;
		exit (1) ;
		} ;

	for (k = 0 ; k < BUFFER_LEN ; k++)
		if (float_data [k] < 1.0)
		{	printf ("\n\nError (%s:%d) : float_data [%d] == %f which is less than 1.0\n", __FILE__, __LINE__, k, float_data [k]) ;
			exit (1) ;
			} ;
			
	/* Seek to start of file, turn normalisation on, read float_data and check again. */
	sf_seek (file, 0, SEEK_SET) ;
	sf_command (file, "norm float", "on", 0) ;
	
	if ((k = sf_read_float (file, float_data, BUFFER_LEN)) != BUFFER_LEN)
	{	printf ("\n\nError (%s:%d) : sf_read_float failed with short read (%d ->%d)\n", __FILE__, __LINE__, BUFFER_LEN, k) ;
		exit (1) ;
		} ;

	for (k = 0 ; k < BUFFER_LEN ; k++)
		if (float_data [k] > 1.0)
		{	printf ("\n\nError (%s:%d) : float_data [%d] == %f which is greater than 1.0\n", __FILE__, __LINE__, k, float_data [k]) ;
			exit (1) ;
			} ;

	/* Seek to start of file, turn normalisation off, read float_data and do final check. */
	sf_seek (file, 0, SEEK_SET) ;
	sf_command (file, "norm float", "off", 0) ;
	
	if ((k = sf_read_float (file, float_data, BUFFER_LEN)) != BUFFER_LEN)
	{	printf ("\n\nError (%s:%d) : sf_read_float failed with short read (%d ->%d)\n", __FILE__, __LINE__, BUFFER_LEN, k) ;
		exit (1) ;
		} ;

	for (k = 0 ; k < BUFFER_LEN ; k++)
		if (float_data [k] < 1.0)
		{	printf ("\n\nError (%s:%d) : float_data [%d] == %f which is less than 1.0\n", __FILE__, __LINE__, k, float_data [k]) ;
			exit (1) ;
			} ;
			

	sf_close (file) ;

	unlink (filename) ;

	printf ("ok\n") ;
} /* float_norm_test */


