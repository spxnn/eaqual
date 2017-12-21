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

/*
**	Check the log buffer for any hint of an incorrectly write/read header.
*/ 

#include	<string.h>

#include	<sndfile.h>
#include	"check_log_buffer.h"

#define		BUFFER_SIZE		2048

void
check_log_buffer (SNDFILE *file)
{	static char	buffer [BUFFER_SIZE] ;
	int			count ;
	
	memset (buffer, 0, BUFFER_SIZE) ;
	
	/* Get the log buffer data. */
	count = sf_get_header_info	(file, buffer, BUFFER_SIZE, 0) ;

	if (BUFFER_SIZE - count < 2)
	{	printf ("Possible long log buffer.\n") ;
		exit (1) ;
		}
	
	/* Look for "Should" */
	if (strstr (buffer, "ould"))
	{	puts ("\n\nLog buffer contains `ould'. Dumping.\n") ;
		puts (buffer) ;
		exit (1) ;
		} ;
	
	/* Look for "**" */
	if (strstr (buffer, "*"))
	{	puts ("\n\nLog buffer contains `*'. Dumping.\n") ;
		puts (buffer) ;
		exit (1) ;
		} ;
	
	return ;
} /* check_log_buffer */

