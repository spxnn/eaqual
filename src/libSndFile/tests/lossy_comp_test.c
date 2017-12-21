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

#ifndef M_PI
	#define M_PI 3.14159265359
#endif

#define	BUFFER_SIZE		(1<<14)
#define	SAMPLE_RATE		11025

static	void	lcomp_test_short	(char *str, char *filename, int filetype, int chan, double margin) ;
static	void	lcomp_test_int		(char *str, char *filename, int filetype, int chan, double margin) ;
static	void	lcomp_test_float	(char *str, char *filename, int filetype, int chan, double margin) ;
static	void	lcomp_test_double	(char *str, char *filename, int filetype, int chan, double margin) ;

static	void	sdlcomp_test_short	(char *str, char *filename, int filetype, int chan, double margin) ;
static	void	sdlcomp_test_int	(char *str, char *filename, int filetype, int chan, double margin) ;
static	void	sdlcomp_test_float	(char *str, char *filename, int filetype, int chan, double margin) ;
static	void	sdlcomp_test_double	(char *str, char *filename, int filetype, int chan, double margin) ;

static	int		error_function (double data, double orig, double margin) ;
static	int		decay_response (int k) ;

static	void	gen_signal_double (double *data, int channels, unsigned int datalen) ;

static	void	smoothed_diff_short (short *data, unsigned int datalen) ;
static	void	smoothed_diff_int (int *data, unsigned int datalen) ;
static	void	smoothed_diff_float (float *data, unsigned int datalen) ;
static	void	smoothed_diff_double (double *data, unsigned int datalen) ;

/* Force the start of these buffers to be double aligned. Sparc-solaris will
** choke if they are not.
*/
static	double	data_buffer [BUFFER_SIZE + 1] ;
static	double	orig_buffer [BUFFER_SIZE + 1] ;
static	double	smooth_buffer [BUFFER_SIZE + 1] ;

int		
main (int argc, char *argv[])
{	char	*filename ;
	int		bDoAll = 0 ;
	int		nTests = 0 ;

	if (argc != 2)
	{	printf ("Usage : %s <test>\n", argv [0]) ;
		printf ("    Where <test> is one of the following:\n") ;
		printf ("           wav_ima     - test IMA ADPCM WAV file functions\n") ;
		printf ("           wav_msadpcm - test MS ADPCM WAV file functions\n") ;
		printf ("           wav_gsm610  - test GSM 6.10 WAV file functions\n") ;
		printf ("           wav_ulaw    - test u-law WAV file functions\n") ;
		printf ("           wav_alaw    - test A-law WAV file functions\n") ;
		printf ("           all         - perform all tests\n") ;
		exit (1) ;
		} ;
		
	bDoAll = !strcmp (argv [1], "all") ;

	if (bDoAll || ! strcmp (argv [1], "wav_pcm"))
	{	/* This is just a sanity test for PCM encoding. */
		filename = "test.wav" ;
		lcomp_test_short	("wav_pcm", filename, SF_FORMAT_WAV | SF_FORMAT_PCM, 2, 0.00001) ;
		lcomp_test_int		("wav_pcm", filename, SF_FORMAT_WAV | SF_FORMAT_PCM, 2, 0.00001) ;
		lcomp_test_float	("wav_pcm", filename, SF_FORMAT_WAV | SF_FORMAT_PCM, 2, 0.005) ;
		lcomp_test_double	("wav_pcm", filename, SF_FORMAT_WAV | SF_FORMAT_PCM, 2, 0.005) ;
		unlink (filename) ;
		nTests++ ;
		} ;
		
	/* For all the rest, if the file format supports more than 1 channel, use stereo. */

	if (bDoAll || ! strcmp (argv [1], "wav_ima"))
	{	filename = "test.wav" ;
		lcomp_test_short	("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;
		lcomp_test_int		("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;
		lcomp_test_float	("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;
		lcomp_test_double	("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;

		sdlcomp_test_short	("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;
		sdlcomp_test_int	("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;
		sdlcomp_test_float	("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;
		sdlcomp_test_double	("wav_ima", filename, SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM, 2, 0.17) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "wav_msadpcm"))
	{	filename = "test.wav" ;
		lcomp_test_short	("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;
		lcomp_test_int		("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;
		lcomp_test_float	("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;
		lcomp_test_double	("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;

		sdlcomp_test_short	("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;
		sdlcomp_test_int	("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;
		sdlcomp_test_float	("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;
		sdlcomp_test_double	("wav_msadpcm", filename, SF_FORMAT_WAV | SF_FORMAT_MS_ADPCM, 2, 0.36) ;

		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "wav_ulaw"))
	{	filename = "test.wav" ;
		lcomp_test_short	("wav_ulaw", filename, SF_FORMAT_WAV | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_int		("wav_ulaw", filename, SF_FORMAT_WAV | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_float	("wav_ulaw", filename, SF_FORMAT_WAV | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_double	("wav_ulaw", filename, SF_FORMAT_WAV | SF_FORMAT_ULAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "wav_alaw"))
	{	filename = "test.wav" ;
		lcomp_test_short	("wav_alaw", filename, SF_FORMAT_WAV | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_int		("wav_alaw", filename, SF_FORMAT_WAV | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_float	("wav_alaw", filename, SF_FORMAT_WAV | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_double	("wav_alaw", filename, SF_FORMAT_WAV | SF_FORMAT_ALAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "wav_gsm610"))
	{	filename = "test.wav" ;
		/* Don't do lcomp_test_XXX as the errors are too big. */
		sdlcomp_test_short	("wav_gsm610", filename, SF_FORMAT_WAV | SF_FORMAT_GSM610, 1, 0.2) ;
		sdlcomp_test_int	("wav_gsm610", filename, SF_FORMAT_WAV | SF_FORMAT_GSM610, 1, 0.2) ;
		sdlcomp_test_float	("wav_gsm610", filename, SF_FORMAT_WAV | SF_FORMAT_GSM610, 1, 0.2) ;
		sdlcomp_test_double	("wav_gsm610", filename, SF_FORMAT_WAV | SF_FORMAT_GSM610, 1, 0.2) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "au_ulaw"))
	{	filename = "test.au" ;
		lcomp_test_short	("au_ulaw", filename, SF_FORMAT_AU | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_int		("au_ulaw", filename, SF_FORMAT_AU | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_float	("au_ulaw", filename, SF_FORMAT_AU | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_double	("au_ulaw", filename, SF_FORMAT_AU | SF_FORMAT_ULAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "au_alaw"))
	{	filename = "test.au" ;
		lcomp_test_short	("au_alaw", filename, SF_FORMAT_AU | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_int		("au_alaw", filename, SF_FORMAT_AU | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_float	("au_alaw", filename, SF_FORMAT_AU | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_double	("au_alaw", filename, SF_FORMAT_AU | SF_FORMAT_ALAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "aule_ulaw"))
	{	filename = "test.au" ;
		lcomp_test_short	("aule_ulaw", filename, SF_FORMAT_AULE | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_int		("aule_ulaw", filename, SF_FORMAT_AULE | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_float	("aule_ulaw", filename, SF_FORMAT_AULE | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_double	("aule_ulaw", filename, SF_FORMAT_AULE | SF_FORMAT_ULAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "aule_alaw"))
	{	filename = "test.au" ;
		lcomp_test_short	("aule_alaw", filename, SF_FORMAT_AULE | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_int		("aule_alaw", filename, SF_FORMAT_AULE | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_float	("aule_alaw", filename, SF_FORMAT_AULE | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_double	("aule_alaw", filename, SF_FORMAT_AULE | SF_FORMAT_ALAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "au_g721"))
	{	filename = "test.au" ;
		lcomp_test_short	("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;
		lcomp_test_int		("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;
		lcomp_test_float	("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;
		lcomp_test_double	("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;

		sdlcomp_test_short	("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;
		sdlcomp_test_int	("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;
		sdlcomp_test_float  ("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;
		sdlcomp_test_double	("au_g721", filename, SF_FORMAT_AU | SF_FORMAT_G721_32, 1, 0.05) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "aule_g721"))
	{	filename = "test.au" ;
		lcomp_test_short	("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		lcomp_test_int		("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		lcomp_test_float	("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		lcomp_test_double	("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		
		sdlcomp_test_short	("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		sdlcomp_test_int	("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		sdlcomp_test_float	("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		sdlcomp_test_double	("aule_g721", filename, SF_FORMAT_AULE | SF_FORMAT_G721_32, 1, 0.05) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "au_g723"))
	{	filename = "test.au" ;
		lcomp_test_short	("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		lcomp_test_int		("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		lcomp_test_float	("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		lcomp_test_double	("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		
		sdlcomp_test_short	("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		sdlcomp_test_int	("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		sdlcomp_test_float	("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		sdlcomp_test_double	("au_g723", filename, SF_FORMAT_AU | SF_FORMAT_G723_24, 1, 0.15) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "aule_g723"))
	{	filename = "test.au" ;
		lcomp_test_short	("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		lcomp_test_int		("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		lcomp_test_float	("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		lcomp_test_double	("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		
		sdlcomp_test_short	("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		sdlcomp_test_int	("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		sdlcomp_test_float	("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		sdlcomp_test_double	("aule_g723", filename, SF_FORMAT_AULE | SF_FORMAT_G723_24, 1, 0.15) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "ircam_ulaw"))
	{	filename = "test.ircam" ;
		lcomp_test_short	("ircam_ulaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_int		("ircam_ulaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_float	("ircam_ulaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ULAW, 2, 0.04) ;
		lcomp_test_double	("ircam_ulaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ULAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "ircam_alaw"))
	{	filename = "test.ircam" ;
		lcomp_test_short	("ircam_alaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_int		("ircam_alaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_float	("ircam_alaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ALAW, 2, 0.04) ;
		lcomp_test_double	("ircam_alaw", filename, SF_FORMAT_IRCAM | SF_FORMAT_ALAW, 2, 0.04) ;
		unlink (filename) ;
		nTests++ ;
		} ;

	if (nTests == 0)
	{	printf ("************************************\n") ;
		printf ("*  No '%s' test defined.\n", argv [1]) ;
		printf ("************************************\n") ;
		return 1 ;
		} ;

	return 0;
} /* main */

/*============================================================================================
**	Here are the test functions.
*/ 
 
static void	
lcomp_test_short (char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	short			*orig, *data ;

	printf ("    lcomp_test_short    : %s ... ", str) ;
	
	datalen = BUFFER_SIZE / channels ;

	data = (short*) data_buffer ;
	orig = (short*) orig_buffer ;
	gen_signal_double (orig_buffer, channels, datalen) ;
	for (k = 0 ; k < channels * datalen ; k++)
		orig [k] = (short) (orig_buffer [k]) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_writef_short (file, orig, datalen)) != datalen)
	{	printf ("sf_writef_short failed with short write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (short)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + datalen / 2))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_readf_short (file, data, datalen)) != datalen)
	{	printf ("short read (%d).\n", k) ;
		exit (1) ;
		} ;

	for (k = 0 ; k < datalen ; k++)
	{	if (error_function ((double) data [k], (double) orig [k], margin))
		{	printf ("Line %d: Incorrect sample A (#%d : %d should be %d).\n", __LINE__, k, data [k], orig [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_readf_short (file, data, datalen)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, channels * sfinfo.samples - datalen, k) ;
		exit (1) ;
		} ;

	/*	This check is only for block based encoders which must append silence
	**	to the end of a file so as to fill out a block.
	*/
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [channels * k]) > decay_response (channels * k))
			{	printf ("Line %d : Incorrect sample B (#%d : abs (%d) should be < %d).\n", __LINE__, channels * (datalen + k), data [channels * k], decay_response (channels * k)) ;
				exit (1) ;
				} ;

	if (! sfinfo.seekable)
	{	printf ("ok\n") ;
		return ;
		} ;

	/* Now test sf_seek function. */
	
	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
	{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
		exit (1) ;
		} ;

	for (m = 0 ; m < 3 ; m++)
	{	if ((k = sf_readf_short (file, data, 11)) != 11)
		{	printf ("Line %d: Incorrect read length B (11 => %d).\n", __LINE__, k) ;
			exit (1) ;
			} ;

		for (k = 0 ; k < channels * 11 ; k++)
			if (error_function ((double) data [k], (double) orig [k + channels * m * 11], margin))
			{	printf ("Line %d: Incorrect sample C (m = %d) (#%d : %d => %d).\n", __LINE__, m, k + channels * m * 11, orig [k + channels * m * 11], data [k]) ;
				for (m = 0 ; m < channels ; m++)
					printf ("%d ", data [m]) ;
				printf ("\n") ;
				exit (1) ;
				} ;
		} ;

	seekpos = BUFFER_SIZE / 10 ;
	
	/* Check seek from start of file. */
	if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
	{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
		exit (1) ;
		} ;

	if ((k = sf_readf_short (file, data, 1)) != 1)
	{	printf ("Line %d: sf_readf_short (file, data, 1) returned %d.\n", __LINE__, k) ;
		exit (1) ;
		} ;
	
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin))
	{	printf ("Line %d: sf_seek (SEEK_SET) followed by sf_readf_short failed (%d, %d).\n", __LINE__, orig [1], data [0]) ;
		exit (1) ;
		} ;
	
	if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
	{	printf ("Line %d: sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", __LINE__, k, seekpos + 1) ;
		exit (1) ;
		} ;

	seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
	k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
	sf_readf_short (file, data, 1) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("Line %d: sf_seek (forwards, SEEK_CUR) followed by sf_readf_short failed (%d, %d) (%d, %d).\n", __LINE__, data [0], orig [seekpos * channels], k, seekpos + 1) ;
		exit (1) ;
		} ;
	
	seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
	/* Check seek backward from current position. */
	k = sf_seek (file, -20, SEEK_CUR) ;
	sf_readf_short (file, data, 1) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_readf_short failed (%d, %d) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos) ;
		exit (1) ;
		} ;
	
	/* Check that read past end of file returns number of items. */
	sf_seek (file, (short) sfinfo.samples, SEEK_SET) ;

 	if ((k = sf_readf_short (file, data, datalen)) != 0)
 	{	printf ("Line %d: Return value from sf_readf_short past end of file incorrect (%d).\n", __LINE__, k) ;
 		exit (1) ;
 		} ;
	
	/* Check seek backward from end. */
	if ((k = sf_seek (file, 5 - (short) sfinfo.samples, SEEK_END)) != 5)
	{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
		exit (1) ;
		} ;

	sf_readf_short (file, data, channels) ;
	if (error_function ((double) data [0], (double) orig [5], margin))
	{	printf ("Line %d: sf_seek (SEEK_END) followed by sf_readf_short failed (%d should be %d).\n", __LINE__, data [0], orig [5]) ;
		exit (1) ;
		} ;

	sf_close (file) ;

	printf ("ok\n") ;
} /* lcomp_test_short */

/*--------------------------------------------------------------------------------------------
*/ 
 
static void	
lcomp_test_int (char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	int				*orig, *data ;

	printf ("    lcomp_test_int      : %s ... ", str) ;
	
	datalen = BUFFER_SIZE / channels ;

	data = (int*) data_buffer ;
	orig = (int*) orig_buffer ;
	gen_signal_double (orig_buffer, channels, datalen) ;
	for (k = 0 ; k < channels * datalen ; k++)
		orig [k] = (int) (orig_buffer [k]) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_writef_int (file, orig, datalen)) != datalen)
	{	printf ("sf_writef_int failed with short write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (int)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + datalen / 2))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_readf_int (file, data, datalen)) != datalen)
	{	printf ("short read (%d).\n", k) ;
		exit (1) ;
		} ;

	for (k = 0 ; k < datalen ; k++)
	{	if (error_function ((double) data [k], (double) orig [k], margin))
		{	printf ("Line %d: Incorrect sample A (#%d : %d should be %d).\n", __LINE__, k, data [k], orig [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_readf_int (file, data, datalen)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, channels * sfinfo.samples - datalen, k) ;
		exit (1) ;
		} ;

	/*	This check is only for block based encoders which must append silence
	**	to the end of a file so as to fill out a block.
	*/
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [channels * k]) > decay_response (channels * k))
			{	printf ("Line %d : Incorrect sample B (#%d : abs (%d) should be < %d).\n", __LINE__, channels * (datalen + k), data [channels * k], decay_response (channels * k)) ;
				exit (1) ;
				} ;

	if (! sfinfo.seekable)
	{	printf ("ok\n") ;
		return ;
		} ;

	/* Now test sf_seek function. */
	
	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
	{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
		exit (1) ;
		} ;

	for (m = 0 ; m < 3 ; m++)
	{	if ((k = sf_readf_int (file, data, 11)) != 11)
		{	printf ("Line %d: Incorrect read length B (11 => %d).\n", __LINE__, k) ;
			exit (1) ;
			} ;

		for (k = 0 ; k < channels * 11 ; k++)
			if (error_function ((double) data [k], (double) orig [k + channels * m * 11], margin))
			{	printf ("Line %d: Incorrect sample C (m = %d) (#%d : %d => %d).\n", __LINE__, m, k + channels * m * 11, orig [k + channels * m * 11], data [k]) ;
				for (m = 0 ; m < channels ; m++)
					printf ("%d ", data [m]) ;
				printf ("\n") ;
				exit (1) ;
				} ;
		} ;

	seekpos = BUFFER_SIZE / 10 ;
	
	/* Check seek from start of file. */
	if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
	{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
		exit (1) ;
		} ;

	if ((k = sf_readf_int (file, data, 1)) != 1)
	{	printf ("Line %d: sf_readf_int (file, data, 1) returned %d.\n", __LINE__, k) ;
		exit (1) ;
		} ;
	
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin))
	{	printf ("Line %d: sf_seek (SEEK_SET) followed by sf_readf_int failed (%d, %d).\n", __LINE__, orig [1], data [0]) ;
		exit (1) ;
		} ;
	
	if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
	{	printf ("Line %d: sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", __LINE__, k, seekpos + 1) ;
		exit (1) ;
		} ;

	seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
	k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
	sf_readf_int (file, data, 1) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("Line %d: sf_seek (forwards, SEEK_CUR) followed by sf_readf_int failed (%d, %d) (%d, %d).\n", __LINE__, data [0], orig [seekpos * channels], k, seekpos + 1) ;
		exit (1) ;
		} ;
	
	seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
	/* Check seek backward from current position. */
	k = sf_seek (file, -20, SEEK_CUR) ;
	sf_readf_int (file, data, 1) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_readf_int failed (%d, %d) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos) ;
		exit (1) ;
		} ;
	
	/* Check that read past end of file returns number of items. */
	sf_seek (file, (int) sfinfo.samples, SEEK_SET) ;

 	if ((k = sf_readf_int (file, data, datalen)) != 0)
 	{	printf ("Line %d: Return value from sf_readf_int past end of file incorrect (%d).\n", __LINE__, k) ;
 		exit (1) ;
 		} ;
	
	/* Check seek backward from end. */
	if ((k = sf_seek (file, 5 - (int) sfinfo.samples, SEEK_END)) != 5)
	{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
		exit (1) ;
		} ;

	sf_readf_int (file, data, channels) ;
	if (error_function ((double) data [0], (double) orig [5], margin))
	{	printf ("Line %d: sf_seek (SEEK_END) followed by sf_readf_short failed (%d should be %d).\n", __LINE__, data [0], orig [5]) ;
		exit (1) ;
		} ;

	sf_close (file) ;

	printf ("ok\n") ;
} /* lcomp_test_int */

/*--------------------------------------------------------------------------------------------
*/ 

static void	
lcomp_test_float (char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	float				*orig, *data ;

	printf ("    lcomp_test_float    : %s ... ", str) ;
	
	datalen = BUFFER_SIZE / channels ;

	data = (float*) data_buffer ;
	orig = (float*) orig_buffer ;
	gen_signal_double (orig_buffer, channels, datalen) ;
	for (k = 0 ; k < channels * datalen ; k++)
		orig [k] = (float) (orig_buffer [k]) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_writef_float (file, orig, datalen)) != datalen)
	{	printf ("sf_writef_float failed with short write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (float)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + datalen / 2))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_readf_float (file, data, datalen)) != datalen)
	{	printf ("short read (%d).\n", k) ;
		exit (1) ;
		} ;

	for (k = 0 ; k < datalen ; k++)
	{	if (error_function ((double) data [k], (double) orig [k], margin))
		{	printf ("Line %d: Incorrect sample A (#%d : %f should be %f).\n", __LINE__, k, data [k], orig [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_readf_float (file, data, datalen)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, channels * sfinfo.samples - datalen, k) ;
		exit (1) ;
		} ;

	/*	This check is only for block based encoders which must append silence
	**	to the end of a file so as to fill out a block.
	*/
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [channels * k]) > decay_response (channels * k))
			{	printf ("Line %d : Incorrect sample B (#%d : abs (%f) should be < %d).\n", __LINE__, channels * (datalen + k), data [channels * k], decay_response (channels * k)) ;
				exit (1) ;
				} ;

	if (! sfinfo.seekable)
	{	printf ("ok\n") ;
		return ;
		} ;

	/* Now test sf_seek function. */
	
	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
	{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
		exit (1) ;
		} ;

	for (m = 0 ; m < 3 ; m++)
	{	if ((k = sf_readf_float (file, data, 11)) != 11)
		{	printf ("Line %d: Incorrect read length B (11 => %d).\n", __LINE__, k) ;
			exit (1) ;
			} ;

		for (k = 0 ; k < channels * 11 ; k++)
			if (error_function ((double) data [k], (double) orig [k + channels * m * 11], margin))
			{	printf ("Line %d: Incorrect sample C (m = %d) (#%d : %f => %f).\n", __LINE__, m, k + channels * m * 11, orig [k + channels * m * 11], data [k]) ;
				for (m = 0 ; m < channels ; m++)
					printf ("%f ", data [m]) ;
				printf ("\n") ;
				exit (1) ;
				} ;
		} ;

	seekpos = BUFFER_SIZE / 10 ;
	
	/* Check seek from start of file. */
	if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
	{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
		exit (1) ;
		} ;

	if ((k = sf_readf_float (file, data, 1)) != 1)
	{	printf ("Line %d: sf_readf_float (file, data, 1) returned %d.\n", __LINE__, k) ;
		exit (1) ;
		} ;
	
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin))
	{	printf ("Line %d: sf_seek (SEEK_SET) followed by sf_readf_float failed (%f, %f).\n", __LINE__, orig [1], data [0]) ;
		exit (1) ;
		} ;
	
	if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
	{	printf ("Line %d: sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", __LINE__, k, seekpos + 1) ;
		exit (1) ;
		} ;

	seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
	k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
	sf_readf_float (file, data, 1) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("Line %d: sf_seek (forwards, SEEK_CUR) followed by sf_readf_float failed (%f, %f) (%d, %d).\n", __LINE__, data [0], orig [seekpos * channels], k, seekpos + 1) ;
		exit (1) ;
		} ;
	
	seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
	/* Check seek backward from current position. */
	k = sf_seek (file, -20, SEEK_CUR) ;
	sf_readf_float (file, data, 1) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_readf_float failed (%f, %f) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos) ;
		exit (1) ;
		} ;
	
	/* Check that read past end of file returns number of items. */
	sf_seek (file, (float) sfinfo.samples, SEEK_SET) ;

 	if ((k = sf_readf_float (file, data, datalen)) != 0)
 	{	printf ("Line %d: Return value from sf_readf_float past end of file incorrect (%d).\n", __LINE__, k) ;
 		exit (1) ;
 		} ;
	
	/* Check seek backward from end. */
	if ((k = sf_seek (file, 5 - (float) sfinfo.samples, SEEK_END)) != 5)
	{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
		exit (1) ;
		} ;

	sf_readf_float (file, data, channels) ;
	if (error_function ((double) data [0], (double) orig [5], margin))
	{	printf ("Line %d: sf_seek (SEEK_END) followed by sf_readf_short failed (%f should be %f).\n", __LINE__, data [0], orig [5]) ;
		exit (1) ;
		} ;

	sf_close (file) ;

	printf ("ok\n") ;
} /* lcomp_test_float */

/*--------------------------------------------------------------------------------------------
*/ 

static void	
lcomp_test_double (char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	double				*orig, *data ;

	printf ("    lcomp_test_double   : %s ... ", str) ;
	
	datalen = BUFFER_SIZE / channels ;

	data = (double*) data_buffer ;
	orig = (double*) orig_buffer ;
	gen_signal_double (orig_buffer, channels, datalen) ;
	for (k = 0 ; k < channels * datalen ; k++)
		orig [k] = (double) (orig_buffer [k]) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_writef_double (file, orig, datalen, 0)) != datalen)
	{	printf ("sf_writef_double failed with short write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (double)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + datalen / 2))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_readf_double (file, data, datalen, 0)) != datalen)
	{	printf ("short read (%d).\n", k) ;
		exit (1) ;
		} ;

	for (k = 0 ; k < datalen ; k++)
	{	if (error_function ((double) data [k], (double) orig [k], margin))
		{	printf ("Line %d: Incorrect sample A (#%d : %f should be %f).\n", __LINE__, k, data [k], orig [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_readf_double (file, data, datalen, 0)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, channels * sfinfo.samples - datalen, k) ;
		exit (1) ;
		} ;

	/*	This check is only for block based encoders which must append silence
	**	to the end of a file so as to fill out a block.
	*/
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [channels * k]) > decay_response (channels * k))
			{	printf ("Line %d : Incorrect sample B (#%d : abs (%f) should be < %d).\n", __LINE__, channels * (datalen + k), data [channels * k], decay_response (channels * k)) ;
				exit (1) ;
				} ;

	if (! sfinfo.seekable)
	{	printf ("ok\n") ;
		return ;
		} ;

	/* Now test sf_seek function. */
	
	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
	{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
		exit (1) ;
		} ;

	for (m = 0 ; m < 3 ; m++)
	{	if ((k = sf_readf_double (file, data, 11, 0)) != 11)
		{	printf ("Line %d: Incorrect read length B (11 => %d).\n", __LINE__, k) ;
			exit (1) ;
			} ;

		for (k = 0 ; k < channels * 11 ; k++)
			if (error_function ((double) data [k], (double) orig [k + channels * m * 11], margin))
			{	printf ("Line %d: Incorrect sample C (m = %d) (#%d : %f => %f).\n", __LINE__, m, k + channels * m * 11, orig [k + channels * m * 11], data [k]) ;
				for (m = 0 ; m < channels ; m++)
					printf ("%f ", data [m]) ;
				printf ("\n") ;
				exit (1) ;
				} ;
		} ;

	seekpos = BUFFER_SIZE / 10 ;
	
	/* Check seek from start of file. */
	if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
	{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
		exit (1) ;
		} ;

	if ((k = sf_readf_double (file, data, 1, 0)) != 1)
	{	printf ("Line %d: sf_readf_double (file, data, 1, 0) returned %d.\n", __LINE__, k) ;
		exit (1) ;
		} ;
	
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin))
	{	printf ("Line %d: sf_seek (SEEK_SET) followed by sf_readf_double failed (%f, %f).\n", __LINE__, orig [1], data [0]) ;
		exit (1) ;
		} ;
	
	if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
	{	printf ("Line %d: sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", __LINE__, k, seekpos + 1) ;
		exit (1) ;
		} ;

	seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
	k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
	sf_readf_double (file, data, 1, 0) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("Line %d: sf_seek (forwards, SEEK_CUR) followed by sf_readf_double failed (%f, %f) (%d, %d).\n", __LINE__, data [0], orig [seekpos * channels], k, seekpos + 1) ;
		exit (1) ;
		} ;
	
	seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
	/* Check seek backward from current position. */
	k = sf_seek (file, -20, SEEK_CUR) ;
	sf_readf_double (file, data, 1, 0) ;
	if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
	{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_readf_double failed (%f, %f) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos) ;
		exit (1) ;
		} ;
	
	/* Check that read past end of file returns number of items. */
	sf_seek (file, (double) sfinfo.samples, SEEK_SET) ;

 	if ((k = sf_readf_double (file, data, datalen, 0)) != 0)
 	{	printf ("Line %d: Return value from sf_readf_double past end of file incorrect (%d).\n", __LINE__, k) ;
 		exit (1) ;
 		} ;
	
	/* Check seek backward from end. */
	if ((k = sf_seek (file, 5 - (double) sfinfo.samples, SEEK_END)) != 5)
	{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
		exit (1) ;
		} ;

	sf_readf_double (file, data, channels, 0) ;
	if (error_function ((double) data [0], (double) orig [5], margin))
	{	printf ("Line %d: sf_seek (SEEK_END) followed by sf_readf_short failed (%f should be %f).\n", __LINE__, data [0], orig [5]) ;
		exit (1) ;
		} ;

	sf_close (file) ;

	printf ("ok\n") ;
} /* lcomp_test_double */

/*========================================================================================
**	Smoothed differential loss compression tests.
*/

static void	
sdlcomp_test_short	(char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	short			*orig, *data, *smooth ;

channels = 1 ;
	printf ("    sdlcomp_test_short  : %s ... ", str) ;
	
	datalen = BUFFER_SIZE ;

	orig = (short*) orig_buffer ;
	data = (short*) data_buffer ;
	smooth = (short*) smooth_buffer ;

	gen_signal_double (orig_buffer, channels, datalen) ;
	for (k = 0 ; k < datalen ; k++)
		orig [k] = (short) (orig_buffer [k]) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_write_short (file, orig, datalen)) != datalen)
	{	printf ("sf_write_short failed with short write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (short)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + 400))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", sfinfo.samples, datalen) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_read_short (file, data, datalen)) != datalen)
	{	printf ("short read (%d).\n", k) ;
		exit (1) ;
		} ;

	memcpy (smooth, orig, datalen * sizeof (short)) ;
	smoothed_diff_short (data, datalen) ;
	smoothed_diff_short (smooth, datalen) ;

	for (k = 1 ; k < datalen ; k++)
	{	if (error_function ((double) data [k], (double) smooth [k], margin))
		{	printf ("Incorrect sample A (#%d : %d should be %d).\n", k, data [k], smooth [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_read_short (file, data, datalen)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, k, sfinfo.samples - datalen) ;
		exit (1) ;
		} ;
		
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM &&
		(sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_GSM610)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [k]) > decay_response (k))
			{	printf ("Incorrect sample B (#%d : abs (%d) should be < %d).\n", datalen + k, data [k], decay_response (k)) ;
				exit (1) ;
				} ;

	/* Now test sf_seek function. */
	if (sfinfo.seekable)
	{	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
		{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
			exit (1) ;
			} ;
	
		for (m = 0 ; m < 3 ; m++)
		{	if ((k = sf_read_short (file, data, datalen / 7)) != datalen / 7)
			{	printf ("Line %d: Incorrect read length B (%d => %d).\n", __LINE__, datalen / 7, k) ;
				exit (1) ;
				} ;
	
			smoothed_diff_short (data, datalen / 7) ;
			memcpy (smooth, orig + m * datalen / 7, datalen / 7 * sizeof (short)) ;
			smoothed_diff_short (smooth, datalen / 7) ;
			
			for (k = 0 ; k < datalen / 7 ; k++)
				if (error_function ((double) data [k], (double) smooth [k], margin))
				{	printf ("Incorrect sample C (#%d (%d) : %d => %d).\n", k, k + m * (datalen / 7), smooth [k], data [k]) ;
					for (m = 0 ; m < 10 ; m++)
						printf ("%d ", data [k]) ;
					printf ("\n") ;
					exit (1) ;
					} ;
			} ; /* for (m = 0 ; m < 3 ; m++) */
	
		seekpos = BUFFER_SIZE / 10 ;
		
		/* Check seek from start of file. */
		if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
		{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
			exit (1) ;
			} ;
		if ((k = sf_read_short (file, data, 1)) != 1)
		{	printf ("sf_read_short (file, data, 1) returned %d.\n", k) ;
			exit (1) ;
			} ;
		
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin))
		{	printf ("sf_seek (SEEK_SET) followed by sf_read_short failed (%d, %d).\n", orig [1], data [0]) ;
			exit (1) ;
			} ;
		
		if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
		{	printf ("sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", k, seekpos + 1) ;
			exit (1) ;
			} ;
	
		seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
		k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
		sf_read_short (file, data, channels) ;
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (forwards, SEEK_CUR) followed by sf_read_short failed (%d, %d) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos + 1) ;
			exit (1) ;
			} ;
		
		seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
		/* Check seek backward from current position. */
		k = sf_seek (file, -20, SEEK_CUR) ;
		sf_read_short (file, data, channels) ;
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_read_short failed (%d, %d) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos) ;
			exit (1) ;
			} ;
		
		/* Check that read past end of file returns number of items. */
		sf_seek (file, (int) sfinfo.samples, SEEK_SET) ;
	
	 	if ((k = sf_read_short (file, data, datalen)) != 0)
	 	{	printf ("Line %d: Return value from sf_read_short past end of file incorrect (%d).\n", __LINE__, k) ;
	 		exit (1) ;
	 		} ;
		
		/* Check seek backward from end. */
		
		if ((k = sf_seek (file, 5 - (int) sfinfo.samples, SEEK_END)) != 5)
		{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
			exit (1) ;
			} ;
	
		sf_read_short (file, data, channels) ;
		if (error_function ((double) data [0], (double) orig [5], margin))
		{	printf ("sf_seek (SEEK_END) followed by sf_read_short failed (%d should be %d).\n", data [0], orig [5]) ;
			exit (1) ;
			} ;
		} /* if (sfinfo.seekable) */

	sf_close (file) ;

	printf ("ok\n") ;
} /* sdlcomp_test_short */

static	void	
sdlcomp_test_int	(char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	int				*orig, *data, *smooth ;

channels = 1 ;

	printf ("    sdlcomp_test_int    : %s ... ", str) ;
	
	datalen = BUFFER_SIZE ;

	orig = (int*) orig_buffer ;
	data = (int*) data_buffer ;
	smooth = (int*) smooth_buffer ;

	gen_signal_double (orig_buffer, channels, datalen) ;
	for (k = 0 ; k < datalen ; k++)
		orig [k] = (int) (orig_buffer [k]) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_writef_int (file, orig, datalen)) != datalen)
	{	printf ("sf_writef_int failed with int write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (int)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + 400))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", sfinfo.samples, datalen) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_readf_int (file, data, datalen)) != datalen)
	{	printf ("int read (%d).\n", k) ;
		exit (1) ;
		} ;

	memcpy (smooth, orig, datalen * sizeof (int)) ;
	smoothed_diff_int (data, datalen) ;
	smoothed_diff_int (smooth, datalen) ;

	for (k = 1 ; k < datalen ; k++)
	{	if (error_function ((double) data [k], (double) smooth [k], margin))
		{	printf ("Incorrect sample A (#%d : %d should be %d).\n", k, data [k], smooth [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_readf_int (file, data, datalen)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, k, sfinfo.samples - datalen) ;
		exit (1) ;
		} ;
		
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_IMA_ADPCM &&
		(sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM &&
		(sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_GSM610)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [k]) > decay_response (k))
			{	printf ("Incorrect sample B (#%d : abs (%d) should be < %d).\n", datalen + k, data [k], decay_response (k)) ;
				exit (1) ;
				} ;

	/* Now test sf_seek function. */
	if (sfinfo.seekable)
	{	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
		{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
			exit (1) ;
			} ;
	
		for (m = 0 ; m < 3 ; m++)
		{	if ((k = sf_readf_int (file, data, datalen / 7)) != datalen / 7)
			{	printf ("Line %d: Incorrect read length B (%d => %d).\n", __LINE__, datalen / 7, k) ;
				exit (1) ;
				} ;
	
			smoothed_diff_int (data, datalen / 7) ;
			memcpy (smooth, orig + m * datalen / 7, datalen / 7 * sizeof (int)) ;
			smoothed_diff_int (smooth, datalen / 7) ;
			
			for (k = 0 ; k < datalen / 7 ; k++)
				if (error_function ((double) data [k], (double) smooth [k], margin))
				{	printf ("Incorrect sample C (#%d (%d) : %d => %d).\n", k, k + m * (datalen / 7), smooth [k], data [k]) ;
					for (m = 0 ; m < 10 ; m++)
						printf ("%d ", data [k]) ;
					printf ("\n") ;
					exit (1) ;
					} ;
			} ; /* for (m = 0 ; m < 3 ; m++) */
	
		seekpos = BUFFER_SIZE / 10 ;
		
		/* Check seek from start of file. */
		if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
		{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
			exit (1) ;
			} ;
		if ((k = sf_readf_int (file, data, 1)) != 1)
		{	printf ("sf_readf_int (file, data, 1) returned %d.\n", k) ;
			exit (1) ;
			} ;
		
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin))
		{	printf ("sf_seek (SEEK_SET) followed by sf_readf_int failed (%d, %d).\n", orig [1], data [0]) ;
			exit (1) ;
			} ;
		
		if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
		{	printf ("sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", k, seekpos + 1) ;
			exit (1) ;
			} ;
	
		seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
		k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
		sf_readf_int (file, data, 1) ;
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (forwards, SEEK_CUR) followed by sf_readf_int failed (%d, %d) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos + 1) ;
			exit (1) ;
			} ;
		
		seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
		/* Check seek backward from current position. */
		k = sf_seek (file, -20, SEEK_CUR) ;
		sf_readf_int (file, data, 1) ;
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_readf_int failed (%d, %d) (%d, %d).\n", data [0], orig [seekpos * channels], k, seekpos) ;
			exit (1) ;
			} ;
		
		/* Check that read past end of file returns number of items. */
		sf_seek (file, (int) sfinfo.samples, SEEK_SET) ;
	
	 	if ((k = sf_readf_int (file, data, datalen)) != 0)
	 	{	printf ("Line %d: Return value from sf_readf_int past end of file incorrect (%d).\n", __LINE__, k) ;
	 		exit (1) ;
	 		} ;
		
		/* Check seek backward from end. */
		
		if ((k = sf_seek (file, 5 - (int) sfinfo.samples, SEEK_END)) != 5)
		{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
			exit (1) ;
			} ;
	
		sf_readf_int (file, data, 1) ;
		if (error_function ((double) data [0], (double) orig [5], margin))
		{	printf ("sf_seek (SEEK_END) followed by sf_readf_int failed (%d should be %d).\n", data [0], orig [5]) ;
			exit (1) ;
			} ;
		} /* if (sfinfo.seekable) */

	sf_close (file) ;

	printf ("ok\n") ;
} /* sdlcomp_test_int */

static void	
sdlcomp_test_float	(char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	float			*orig, *data, *smooth ;

channels = 1 ;
	printf ("    sdlcomp_test_float  : %s ... ", str) ;
	
	datalen = BUFFER_SIZE ;

	orig = (float*) orig_buffer ;
	data = (float*) data_buffer ;
	smooth = (float*) smooth_buffer ;

	gen_signal_double (orig_buffer, channels, datalen) ;
	for (k = 0 ; k < datalen ; k++)
		orig [k] = (int) (orig_buffer [k]) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_write_float (file, orig, datalen)) != datalen)
	{	printf ("sf_write_float failed with int write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (float)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + 400))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", sfinfo.samples, datalen) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_read_float (file, data, datalen)) != datalen)
	{	printf ("int read (%d).\n", k) ;
		exit (1) ;
		} ;

	memcpy (smooth, orig, datalen * sizeof (float)) ;
	smoothed_diff_float (data, datalen) ;
	smoothed_diff_float (smooth, datalen) ;

	for (k = 1 ; k < datalen ; k++)
	{	if (error_function (data [k], smooth [k], margin))
		{	printf ("Incorrect sample A (#%d : %d should be %d).\n", k, (int) data [k], (int) smooth [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_read_float (file, data, datalen)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, k, sfinfo.samples - datalen) ;
		exit (1) ;
		} ;
		
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM &&
		(sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_GSM610)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [k]) > decay_response (k))
			{	printf ("Incorrect sample B (#%d : abs (%d) should be < %d).\n", datalen + k, (int) data [k], (int) decay_response (k)) ;
				exit (1) ;
				} ;

	/* Now test sf_seek function. */
	if (sfinfo.seekable)
	{	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
		{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
			exit (1) ;
			} ;
	
		for (m = 0 ; m < 3 ; m++)
		{	if ((k = sf_read_float (file, data, datalen / 7)) != datalen / 7)
			{	printf ("Line %d: Incorrect read length B (%d => %d).\n", __LINE__, datalen / 7, k) ;
				exit (1) ;
				} ;
	
			smoothed_diff_float (data, datalen / 7) ;
			memcpy (smooth, orig + m * datalen / 7, datalen / 7 * sizeof (float)) ;
			smoothed_diff_float (smooth, datalen / 7) ;
			
			for (k = 0 ; k < datalen / 7 ; k++)
				if (error_function ((float) data [k], (float) smooth [k], margin))
				{	printf ("Incorrect sample C (#%d (%d) : %d => %d).\n", k, k + m * (datalen / 7), (int) smooth [k], (int) data [k]) ;
					for (m = 0 ; m < 10 ; m++)
						printf ("%d ", (int) data [k]) ;
					printf ("\n") ;
					exit (1) ;
					} ;
			} ; /* for (m = 0 ; m < 3 ; m++) */
	
		seekpos = BUFFER_SIZE / 10 ;
		
		/* Check seek from start of file. */
		if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
		{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
			exit (1) ;
			} ;
		if ((k = sf_read_float (file, data, 1)) != 1)
		{	printf ("sf_read_float (file, data, 1) returned %d.\n", k) ;
			exit (1) ;
			} ;
		
		if (error_function ((float) data [0], (float) orig [seekpos * channels], margin))
		{	printf ("sf_seek (SEEK_SET) followed by sf_read_float failed (%d, %d).\n", (int) orig [1], (int) data [0]) ;
			exit (1) ;
			} ;
		
		if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
		{	printf ("sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", k, seekpos + 1) ;
			exit (1) ;
			} ;
	
		seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
		k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
		sf_read_float (file, data, channels) ;
		if (error_function ((float) data [0], (float) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (forwards, SEEK_CUR) followed by sf_read_float failed (%d, %d) (%d, %d).\n", (int) data [0], (int) orig [seekpos * channels], k, seekpos + 1) ;
			exit (1) ;
			} ;
		
		seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
		/* Check seek backward from current position. */
		k = sf_seek (file, -20, SEEK_CUR) ;
		sf_read_float (file, data, channels) ;
		if (error_function ((float) data [0], (float) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_read_float failed (%d, %d) (%d, %d).\n", (int) data [0], (int) orig [seekpos * channels], k, seekpos) ;
			exit (1) ;
			} ;
		
		/* Check that read past end of file returns number of items. */
		sf_seek (file, (int) sfinfo.samples, SEEK_SET) ;
	
	 	if ((k = sf_read_float (file, data, datalen)) != 0)
	 	{	printf ("Line %d: Return value from sf_read_float past end of file incorrect (%d).\n", __LINE__, k) ;
	 		exit (1) ;
	 		} ;
		
		/* Check seek backward from end. */
		
		if ((k = sf_seek (file, 5 - (int) sfinfo.samples, SEEK_END)) != 5)
		{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
			exit (1) ;
			} ;
	
		sf_read_float (file, data, channels) ;
		if (error_function ((float) data [0], (float) orig [5], margin))
		{	printf ("sf_seek (SEEK_END) followed by sf_read_float failed (%d should be %d).\n", (int) data [0], (int) orig [5]) ;
			exit (1) ;
			} ;
		} /* if (sfinfo.seekable) */

	sf_close (file) ;

	printf ("ok\n") ;
} /* sdlcomp_test_float */

static void	
sdlcomp_test_double	(char *str, char *filename, int filetype, int channels, double margin)
{	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	int				k, m, seekpos ;
	unsigned int	datalen ;
	double			*orig, *data, *smooth ;

channels = 1 ;
	printf ("    sdlcomp_test_double : %s ... ", str) ;
	
	datalen = BUFFER_SIZE ;

	orig = orig_buffer ;
	data = data_buffer ;
	smooth = smooth_buffer ;

	gen_signal_double (orig_buffer, channels, datalen) ;
		
	sfinfo.samplerate  = SAMPLE_RATE ;
	sfinfo.samples     = 123456789 ;	/* Ridiculous value. */
	sfinfo.channels    = channels ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = filetype ;

	if (! (file = sf_open_write (filename, &sfinfo)))
	{	printf ("sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if ((k = sf_write_double (file, orig, datalen, 0)) != datalen)
	{	printf ("sf_write_double failed with int write (%d => %d).\n", datalen, k) ;
		exit (1) ;
		} ;
	sf_close (file) ;
	
	memset (data, 0, datalen * sizeof (double)) ;
	memset (&sfinfo, 0, sizeof (sfinfo)) ;
	
	if (! (file = sf_open_read (filename, &sfinfo)))
	{	printf ("sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;
	
	if (sfinfo.format != filetype)
	{	printf ("Returned format incorrect (0x%08X => 0x%08X).\n", filetype, sfinfo.format) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < datalen / channels)
	{	printf ("Too few samples in file. (%d should be a little more than %d)\n", datalen, sfinfo.samples) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples > (datalen + 400))
	{	printf ("Too many samples in file. (%d should be a little more than %d)\n", sfinfo.samples, datalen) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels != channels)
	{	printf ("Incorrect number of channels in file.\n") ;
		exit (1) ;
		} ;

	if (sfinfo.pcmbitwidth != 16)
	{	printf ("Incorrect bit width (%d).\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;

	check_log_buffer (file) ;
		
	if ((k = sf_read_double (file, data, datalen, 0)) != datalen)
	{	printf ("int read (%d).\n", k) ;
		exit (1) ;
		} ;

	memcpy (smooth, orig, datalen * sizeof (double)) ;
	smoothed_diff_double (data, datalen) ;
	smoothed_diff_double (smooth, datalen) ;

	for (k = 1 ; k < datalen ; k++)
	{	if (error_function (data [k], smooth [k], margin))
		{	printf ("Incorrect sample A (#%d : %d should be %d).\n", k, (int) data [k], (int) smooth [k]) ;
			exit (1) ;
			} ;
		} ;

	if ((k = sf_read_double (file, data, datalen, 0)) != sfinfo.samples - datalen)
	{	printf ("Line %d: Incorrect read length A (%d should be %d).\n", __LINE__, k, sfinfo.samples - datalen) ;
		exit (1) ;
		} ;
		
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_MS_ADPCM &&
		(sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_GSM610)
		for (k = 0 ; k < sfinfo.samples - datalen ; k++)
			if (abs (data [k]) > decay_response (k))
			{	printf ("Incorrect sample B (#%d : abs (%d) should be < %d).\n", datalen + k, (int) data [k], (int) decay_response (k)) ;
				exit (1) ;
				} ;

	/* Now test sf_seek function. */
	if (sfinfo.seekable)
	{	if ((k = sf_seek (file, 0, SEEK_SET)) != 0)
		{	printf ("Line %d: Seek to start of file failed (%d).\n", __LINE__, k) ;
			exit (1) ;
			} ;
	
		for (m = 0 ; m < 3 ; m++)
		{	if ((k = sf_read_double (file, data, datalen / 7, 0)) != datalen / 7)
			{	printf ("Line %d: Incorrect read length B (%d => %d).\n", __LINE__, datalen / 7, k) ;
				exit (1) ;
				} ;
	
			smoothed_diff_double (data, datalen / 7) ;
			memcpy (smooth, orig + m * datalen / 7, datalen / 7 * sizeof (double)) ;
			smoothed_diff_double (smooth, datalen / 7) ;
			
			for (k = 0 ; k < datalen / 7 ; k++)
				if (error_function ((double) data [k], (double) smooth [k], margin))
				{	printf ("Incorrect sample C (#%d (%d) : %d => %d).\n", k, k + m * (datalen / 7), (int) smooth [k], (int) data [k]) ;
					for (m = 0 ; m < 10 ; m++)
						printf ("%d ", (int) data [k]) ;
					printf ("\n") ;
					exit (1) ;
					} ;
			} ; /* for (m = 0 ; m < 3 ; m++) */
	
		seekpos = BUFFER_SIZE / 10 ;
		
		/* Check seek from start of file. */
		if ((k = sf_seek (file, seekpos, SEEK_SET)) != seekpos)
		{	printf ("Seek to start of file + %d failed (%d).\n", seekpos, k) ;
			exit (1) ;
			} ;
		if ((k = sf_read_double (file, data, 1, 0)) != 1)
		{	printf ("sf_read_double (file, data, 1) returned %d.\n", k) ;
			exit (1) ;
			} ;
		
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin))
		{	printf ("sf_seek (SEEK_SET) followed by sf_read_double failed (%d, %d).\n", (int) orig [1], (int) data [0]) ;
			exit (1) ;
			} ;
		
		if ((k = sf_seek (file, 0, SEEK_CUR)) != seekpos + 1)
		{	printf ("sf_seek (SEEK_CUR) with 0 offset failed (%d should be %d)\n", k, seekpos + 1) ;
			exit (1) ;
			} ;
	
		seekpos = sf_seek (file, 0, SEEK_CUR) + BUFFER_SIZE / 5 ;
		k = sf_seek (file, BUFFER_SIZE / 5, SEEK_CUR) ;
		sf_read_double (file, data, 1, 0) ;
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (forwards, SEEK_CUR) followed by sf_read_double failed (%d, %d) (%d, %d).\n", (int) data [0], (int) orig [seekpos * channels], k, seekpos + 1) ;
			exit (1) ;
			} ;
		
		seekpos = sf_seek (file, 0, SEEK_CUR) - 20 ;
		/* Check seek backward from current position. */
		k = sf_seek (file, -20, SEEK_CUR) ;
		sf_read_double (file, data, 1, 0) ;
		if (error_function ((double) data [0], (double) orig [seekpos * channels], margin) || k != seekpos)
		{	printf ("sf_seek (backwards, SEEK_CUR) followed by sf_read_double failed (%d, %d) (%d, %d).\n", (int) data [0], (int) orig [seekpos * channels], k, seekpos) ;
			exit (1) ;
			} ;
		
		/* Check that read past end of file returns number of items. */
		sf_seek (file, (int) sfinfo.samples, SEEK_SET) ;
	
	 	if ((k = sf_read_double (file, data, datalen, 0)) != 0)
	 	{	printf ("Line %d: Return value from sf_read_double past end of file incorrect (%d).\n", __LINE__, k) ;
	 		exit (1) ;
	 		} ;
		
		/* Check seek backward from end. */
		
		if ((k = sf_seek (file, 5 - (int) sfinfo.samples, SEEK_END)) != 5)
		{	printf ("sf_seek (SEEK_END) returned %d instead of %d.\n", k, 5) ;
			exit (1) ;
			} ;
	
		sf_read_double (file, data, 1, 0) ;
		if (error_function ((double) data [0], (double) orig [5], margin))
		{	printf ("sf_seek (SEEK_END) followed by sf_read_double failed (%d should be %d).\n", (int) data [0], (int) orig [5]) ;
			exit (1) ;
			} ;
		} /* if (sfinfo.seekable) */

	sf_close (file) ;

	printf ("ok\n") ;
} /* sdlcomp_test_double */

/*========================================================================================
**	Auxiliary functions
*/

static int 
decay_response (int k)
{	if (k < 1)
		return ((int) 30000.0) ;
	return (int) (30000.0 / (0.5 * k * k)) ;
} /* decay_response */

static void	
gen_signal_double (double *data, int channels, unsigned int datalen)
{	unsigned int k, ramplen ;
	double	amp = 0.0 ;
	
	ramplen = datalen / 18 ;

	if (channels == 1)
	{	for (k = 0 ; k < datalen ; k++)
		{	if (k <= ramplen)
				amp = 30000.0 * k / ((double) ramplen) ;
			else if (k > datalen - ramplen)
				amp = 30000.0 * (datalen - k) / ((double) ramplen) ;
			data [k] = amp * (0.4 * sin (33.3 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))
							+ 0.3 * cos (201.1 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))) ;
			} ;
		}
	else
	{	for (k = 0 ; k < datalen ; k ++)
		{	if (k <= ramplen)
				amp = 30000.0 * k / ((double) ramplen) ;
			else if (k > datalen - ramplen)
				amp = 30000.0 * (datalen - k) / ((double) ramplen) ;

			data [2 * k] = amp * (0.4 * sin (33.3 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))
							+ 0.3 * cos (201.1 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))) ;
			data [2 * k + 1] = amp * (0.4 * sin (55.5 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))
							+ 0.3 * cos (201.1 * 2.0 * M_PI * ((double) (k+1)) / ((double) SAMPLE_RATE))) ;
			} ;
		} ;
	
	return ;
} /* gen_signal_double */

static int 
error_function (double data, double orig, double margin)
{	double error ;

	if (fabs (orig) <= 500.0)
		error = fabs (fabs (data) - fabs(orig)) / 2000.0 ;
	else if (fabs (orig) <= 1000.0)
		error = fabs (data - orig) / 3000.0 ;
	else
		error = fabs (data - orig) / fabs (orig) ;
		
	if (error > margin)
	{	printf ("\n\n*******************\nError : %f\n", error) ;
		return 1 ;
		} ;
	return 0 ;
} /* error_function */

static void	
smoothed_diff_short (short *data, unsigned int datalen)
{	unsigned int k ;
	double memory = 0.0 ;

	/* Calculate the smoothed sample-to-sample difference. */
	for (k = 0 ; k < datalen - 1 ; k++)
	{	memory = 0.7 * memory + (1 - 0.7) * (double) (data [k+1] - data [k]) ;
		data [k] = (short) memory ;
		} ;
	data [datalen-1] = data [datalen-2] ;
	
} /* smoothed_diff_short */

static void	
smoothed_diff_int (int *data, unsigned int datalen)
{	unsigned int k ;
	double memory = 0.0 ;

	/* Calculate the smoothed sample-to-sample difference. */
	for (k = 0 ; k < datalen - 1 ; k++)
	{	memory = 0.7 * memory + (1 - 0.7) * (double) (data [k+1] - data [k]) ;
		data [k] = (int) memory ;
		} ;
	data [datalen-1] = data [datalen-2] ;
	
} /* smoothed_diff_int */

static	void	
smoothed_diff_float (float *data, unsigned int datalen)
{	unsigned int k ;
	float memory = 0.0 ;

	/* Calculate the smoothed sample-to-sample difference. */
	for (k = 0 ; k < datalen - 1 ; k++)
	{	memory = 0.7 * memory + (1 - 0.7) * (data [k+1] - data [k]) ;
		data [k] = memory ;
		} ;
	data [datalen-1] = data [datalen-2] ;
	
} /* smoothed_diff_float */

static	void	
smoothed_diff_double (double *data, unsigned int datalen)
{	unsigned int k ;
	double memory = 0.0 ;

	/* Calculate the smoothed sample-to-sample difference. */
	for (k = 0 ; k < datalen - 1 ; k++)
	{	memory = 0.7 * memory + (1 - 0.7) * (data [k+1] - data [k]) ;
		data [k] = memory ;
		} ;
	data [datalen-1] = data [datalen-2] ;
	
} /* smoothed_diff_double */
