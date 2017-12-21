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

static	void	stdout_test	(char *str, int typemajor, int count) ;

int
main (int argc, char *argv [])
{	unsigned int	count, bDoAll, nTests = 0 ;
	
	if (argc != 3 || !(count = atoi (argv [2])))
	{	fprintf (stderr, "This program cannot be run by itself. It needs\n") ;
		fprintf (stderr, "to be run from the stdio_test.sh sheel script.\n") ;
		exit (1) ;
		} ;

	bDoAll =! strcmp (argv [1], "all");
		
	if (bDoAll || ! strcmp (argv [1], "wav"))
	{	stdout_test	("wav", SF_FORMAT_WAV, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "aiff"))
	{	stdout_test	("aiff", SF_FORMAT_AIFF, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "au"))
	{	stdout_test	("au", SF_FORMAT_AU, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "paf"))
	{	stdout_test	("paf", SF_FORMAT_PAF, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "svx"))
	{	stdout_test	("svx", SF_FORMAT_SVX, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "nist"))
	{	stdout_test	("nist", SF_FORMAT_NIST, count) ;
		nTests++ ;
		} ;

	if (bDoAll || ! strcmp (argv [1], "ircam"))
	{	stdout_test	("ircam", SF_FORMAT_IRCAM, count) ;
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
stdout_test	(char *str, int typemajor, int count)
{	static	short	data [BUFFER_LEN] ;

	SNDFILE			*file ;
	SF_INFO			sfinfo ;
	unsigned int	k, total, this_write ;
	
	fprintf (stderr, "    %-5s : writing %d samples to stdout  ... ", str, count) ;
	
	sfinfo.samplerate  = 44100 ;
	sfinfo.pcmbitwidth = 16 ;
	sfinfo.format 	   = (typemajor | SF_FORMAT_PCM) ;
	sfinfo.channels    = 1 ;
	sfinfo.samples     = 0 ;

	/* Create some random data. */
	for (k = 0 ; k < BUFFER_LEN ; k++)
		data [k] = (rand () % 2000) ;
		
	if (! (file = sf_open_write ("-", &sfinfo)))
	{	fprintf (stderr, "sf_open_write failed with error : ") ;
		sf_perror (NULL) ;
		exit (1) ;
		} ;

	total = 0 ;
	
	while (total < count)
	{	this_write = (count - total > BUFFER_LEN) ? BUFFER_LEN : count - total ;
		if ((k = sf_write_short (file, data, this_write)) != this_write)
		{	fprintf (stderr, "sf_write_short # %d failed with short write (%d ->%d)\n", count, this_write, k) ;
			exit (1) ;
			} ;
		total += k ;
		} ;
	
	sf_close (file) ;
	
	fprintf (stderr, "ok\n") ;

	return ;
} /* stdout_test */
