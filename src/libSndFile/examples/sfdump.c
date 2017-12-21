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
#include	<ctype.h>

#include	<sndfile.h>

#define	 BUFFER_LEN      4096

static void	
print_usage (char *progname)
{	printf ("\nUsage : %s -f <file>\n", progname) ;
	printf ("\n") ;
} /* print_usage */

int     main (int argc, char *argv[])
{	static	 int	buffer [BUFFER_LEN] ;
	char 		*progname, *filename = NULL ;
	SNDFILE	 	*infile ;
	SF_INFO	 	sfinfo ;
	int			k, count = 1024, start = 0, readcount ;

	progname = strrchr (argv [0], '/') ;
	progname = progname ? progname + 1 : argv [0] ;
	
	if (argc < 2)
	{	print_usage (progname) ;
		exit (0) ;
		} ;
		
	for (k = 1 ; k < argc; k++)
	{	if (! strcmp (argv [k], "-f") && k + 1 < argc)
			filename = argv [++k] ;
		else if (strstr (argv [k], "-c") && k + 1 < argc)
			count = atoi (argv [++k]) ;
		else if (strstr (argv [k], "-s") && k + 1 < argc)
			start = atoi (argv [++k]) ;
		else
		{	printf ("Error: unrecognised option : %s\n", argv [k]) ;
			print_usage (progname) ;
			exit (0) ;
			} ;
		} ;
		
	if (! filename)
	{	printf ("Error : no filename.\n") ;
		print_usage (progname) ;
		exit (0) ;
		} ;
		
	if (! (infile = sf_open_read (filename, &sfinfo)))
	{	printf ("Error : Not able to open input file %s.\n", filename) ;
		sf_perror (NULL) ;
		return  1 ;
		} ;
		
	while ((readcount = sf_read_int (infile, buffer, BUFFER_LEN)))
	{	if (start > readcount)
		{	start -= readcount ;
			continue ;
			}
	
		for (k = start ; k < readcount && count > 0 ; k++, count --)
			printf ("%d\n", buffer [k]) ;
		
		if (k == count)
			break ;
		} ;

	sf_close (infile) ;
	
	return 0 ;
} /* main */

