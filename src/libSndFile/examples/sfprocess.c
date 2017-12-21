#include    <stdio.h>
#include    <sndfile.h>

#define BUFFER_LEN  1024

void
process_data (double *data, int count, int channels)
{
    /* Process the data here. 
    ** If the soundfile contains more then 1 channel
    ** you need to take care of the interleaving.
    */
    
} /* process_data */
        

int     
main (void)
{   static double data [BUFFER_LEN] ;

    char         *infilename, *outfilename ;
    SNDFILE      *infile, *outfile ;
    SF_INFO      sfinfo ;
    int          readcount ;

    infilename = "input.wav" ;
    outfilename = "output.wav" ;
        
    if (! (infile = sf_open_read (infilename, &sfinfo)))
    {   printf ("Not able to open input file %s.\n", infilename) ;
        sf_perror (NULL) ;
        return  1 ;
        } ;
    
    if (! (outfile = sf_open_write (outfilename, &sfinfo)))
    {   printf ("Not able to open output file %s.\n", outfilename) ;
        sf_perror (NULL) ;
        return  1 ;
        } ;
        
    while ((readcount = sf_read_double (infile, data, BUFFER_LEN, 0)))
    {   process_data (data, readcount, sfinfo.channels) ;
        sf_write_double (outfile, data, readcount, 0) ;
        } ;

    sf_close (infile) ;
    sf_close (outfile) ;
    
    return 0 ;
} /* main */

