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

static	void	stdin_test	(char *str, int typemajor, int count) ;

int
main (int argc, char *argv [])
{	unsigned int	count, bDoAll = 0, nTests = 0 ;
	
	if (argc != 3 || !(count = atoi (argv [2])))
	{	fprintf (stderr, "This program cannot be run by itself. It needs\n") ;
		fprintf (stderr, "to be run from the stdio_test.sh sheel script.\n") ;
		exit (1) ;
		} ;
		
	bDoAll = ! strcmp (argv [1], "all") ;

	if (bDoAll || ! strcmp (argv [1], "wav"))
	{	stdin_test	("wav", SF_FORMAT_WAV, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "aiff"))
	{	stdin_test	("aiff", SF_FORMAT_AIFF, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "au"))
	{	stdin_test	("au", SF_FORMAT_AU, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "paf"))
	{	stdin_test	("paf", SF_FORMAT_PAF, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "svx"))
	{	stdin_test	("svx", SF_FORMAT_SVX, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "nist"))
	{	stdin_test	("nist", SF_FORMAT_NIST, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "ircam"))
	{	stdin_test	("ircam", SF_FORMAT_IRCAM, count) ;
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

static	void	
stdin_test	(char *str, int typemajor, int count)
{	static	short	data [BUFFER_LEN] ;

	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	unsigned int	k, total ;
	
	fprintf (stderr, "    %-5s : reading %d samples from stdin ... ", str, count) ;
	
	if (! (file = sf_open_read ("-", &sfinfo)))
	{	fprintf (stderr, "sf_open_read failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;

	if (sfinfo.samplerate  != 44100)
	{	fprintf (stderr, "\n\nError : sample rate (%u) should be 44100\n", sfinfo.samplerate) ;
		exit (1) ;
		} ;
	
	if (sfinfo.pcmbitwidth  != 16)
	{	fprintf (stderr, "\n\nError : pcm bit width (%u) should be 16\n", sfinfo.pcmbitwidth) ;
		exit (1) ;
		} ;
	
	if (sfinfo.channels  != 1)
	{	fprintf (stderr, "\n\nError : channels (%u) should be 1\n", sfinfo.channels) ;
		exit (1) ;
		} ;
	
	if (sfinfo.samples < count)
	{	fprintf (stderr, "\n\nError : sample count (%u, 0x%08x) should be %d\n", sfinfo.samples, sfinfo.samples, count) ;
		exit (1) ;
		} ;
	
	total = 0 ;
	while ((k = sf_read_short (file, data, BUFFER_LEN)))
		total += k ;


	if (total != count)
	{	fprintf (stderr, "\n\nError : expected (%u) samples, read %u\n", count, total) ;
		exit (1) ;
		} ;
		
	sf_close (file) ;
	
	fprintf (stderr, "ok\n") ;

	return ;
} /* stdin_test */
