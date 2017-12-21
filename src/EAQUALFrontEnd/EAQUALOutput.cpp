/////////////////////////////////////////////////////////////////////////////////////
// EAQUALOutput.cpp - output (stdio/file) information for user
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// This file is part of EAQUAL - Evaluation of Audio QUALity (objective measurement of audio quality)
// Copyright (C) 2000-2002  Alexander Lerch
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// contact information:     Alexander Lerch (lerch@zplane.de)
//                          zplane.development
//                          Holsteinische Str. 39-42
//                          Aufgang 8
//                          D-12161 Berlin
//                          Germany
/////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <time.h>

#include "sndfile.h"
#include "libEAQUAL.h"
#include "zErrorCodes.h"
#include "EAQUALMain.h"
#include "version.h"


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowProgramInfo
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program info
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void ShowProgramInfo()
{
    fprintf(stdout,_NAME_STRING);
    fprintf(stdout,_VERSION_STRING);
    fprintf(stdout,_AUTHOR_STRING);
//  fprintf(stdout,_COPYRIGHT_STRING);
    fprintf(stdout,_DIV_STRING);
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowFrameNumber
//  @parameter:     - unsigned int      uiFrameNumber       number of current frame
//                  - int               iSampleRate         sample rate of audio
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print number of calculated frames
//  @creation_date: 27.12.2000
//  @last_modified: 08.01   changed output to seconds
/////////////////////////////////////////////////////////////////////////////////////
void ShowFrameNumber(unsigned int uiFrameNumber, int iSampleRate)
{
    fprintf(stderr, "\rProcessed:\t\t%2.2f seconds of audio file", uiFrameNumber*1024.0F/iSampleRate);
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowResults
//  @parameter:     - double        *pResults           pointer on program results
//                  - int           iLengthOfResults    number of results
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program results to stdio
//  @creation_date: 27.12.2001
//  @last_modified: 07.01.2002  AL: truncated ODG/DI output to 2 decimals to prevent user misuse :)
/////////////////////////////////////////////////////////////////////////////////////
void ShowResults(double *pResults, int iLengthOfResults)
{
    if (iLengthOfResults == 13)
    {
        fprintf(stdout,_LINEBREAK_STRING);
        fprintf(stdout,"Resulting ODG:\t%5.2f\n",pResults[0]);
        fprintf(stdout,"Resulting DIX:\t%5.2f\n",pResults[1]);
        fprintf(stdout,_LINEBREAK_STRING);
        fprintf(stdout,"BandwidthRef\t%5.4f\n",pResults[2]);
        fprintf(stdout,"BandwidthTest\t%5.4f\n",pResults[3]);
        fprintf(stdout,"NMR\t\t%5.4f\n",pResults[4]);
        fprintf(stdout,"WinModDiff1\t%5.4f\n",pResults[5]);
        fprintf(stdout,"ADB\t\t%5.4f\n",pResults[6]);
        fprintf(stdout,"EHS\t\t%5.4f\n",pResults[7]);
        fprintf(stdout,"AvgModDiff1\t%5.4f\n",pResults[8]);
        fprintf(stdout,"AvgModDiff2\t%5.4f\n",pResults[9]);
        fprintf(stdout,"NoiseLoud\t%5.4f\n",pResults[10]);
        fprintf(stdout,"MFPD\t\t%5.4f\n",pResults[11]);
        fprintf(stdout,"RDF\t\t%5.4f\n",pResults[12]);
    }
    else
    {
        fprintf(stdout,_LINEBREAK_STRING);
        fprintf(stdout,"Resulting ODG:\t%5.4f\n",pResults[0]);
        fprintf(stdout,"Resulting DIX:\t%5.4f\n",pResults[1]);
        fprintf(stdout,_LINEBREAK_STRING);
        fprintf(stdout,"RMSModDiff\t%5.4f\n",pResults[2]);
        fprintf(stdout,"RMSNoiseLoud\t%5.4f\n",pResults[3]);
        fprintf(stdout,"AvgLinDist\t%5.4f\n",pResults[4]);
        fprintf(stdout,"SegNMR\t\t%5.4f\n",pResults[5]);
        fprintf(stdout,"EHS\t\t%5.4f\n",pResults[6]);
    }
    fflush (stdout);
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       PrintResults
//  @parameter:     - double        *pResults           pointer on program results
//                  - int           iLengthOfResults    number of results
//                  - char          *pcOutputPath       path to file to write
//                  - char          *pcRefPath          path to ref file 
//                  - char          *pcTestPath         path to test file 
//  @result:        - int                               error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program results to file
//  @creation_date: 27.12.2001
//  @last_modified: 25.01.2002 AL: added ref and test file path
/////////////////////////////////////////////////////////////////////////////////////
int PrintResults(double *pResults, int iLengthOfResults, char *pcOutputPath, char *pcRefPath, char *pcTestPath)
{
    FILE            *pFOutputFile;
    int iIdx;
    
    if ((pFOutputFile   = fopen(pcOutputPath, "a+")) == NULL)
        return _FILE_WRITE_ERROR;

	// print filename 
	fprintf(pFOutputFile,"Ref-filename:%s   Test-filename:%s\n", pcRefPath, pcTestPath);

    // write header
    if (iLengthOfResults == 13)
        fprintf(pFOutputFile,"ODG\tDI\tBWRef\tBWTest\tNMR\tWModD1\tADB\tEHS\tAModD1\tAModD2\tNL\tMFPD\tRDF\n");
    else
        fprintf(pFOutputFile,"ODG\tDI\tRMSModDiff\tRMSNoiseLoud\tAvgLinDist\tSegNMR\tEHS\n");

    // write results
    for ( iIdx = 0; iIdx < iLengthOfResults; iIdx++)
        fprintf(pFOutputFile, "%6.5E\t", pResults[iIdx]);   

    fprintf(pFOutputFile,"\n\n");

    fclose(pFOutputFile);
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowTime
//  @parameter:     - clock_t       clStartTime     time when processing started
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print elapsed time
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void ShowTime(clock_t clStartTime)
{
    fprintf(stdout, "\nTime elapsed:\t%2.2f\n", (float)(clock() - clStartTime) / CLOCKS_PER_SEC);
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowCancel
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print cancel message
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void ShowCancel()
{
    fprintf(stdout, "Press Escape to cancel...\n\n");
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowConfiguration
//  @parameter:     - char      *pcRefPath      path of reference input file
//                  - char      *pcTestPath     path of test input file
//                  - WaveFormat    *pFormat    pointer on struct with the input wave format
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print current configuration
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void ShowConfiguration(char *pcRefPath, char *pcTestPath, WaveFormat *pFormat)
{
    fprintf(stdout, "Reference File:\t\t%s\n", pcRefPath);
    fprintf(stdout, "Test File:\t\t%s\n", pcTestPath);
    fprintf(stdout, "Sample Rate: \t\t%d\n", pFormat->iSamplingRate);
    fprintf(stdout, "Number of Channels: \t%d\n\n", pFormat->iNumOfChannel);
    
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowError
//  @parameter:     - unsigned int      rError      error code
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print the program error
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void ShowError ( unsigned int rError)
{
    if (rError) 
        fprintf (stderr, "Aborted with error number:\t%d\n", rError);
    fflush (stdout);
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowVersion
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program version
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void ShowVersion ()
{
        fprintf (stdout, "Program version number:\t\n" _VERSION_STRING);
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowHelp
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print command line help
//  @creation_date: 27.12.2001
//  @last_modified: 07.01.2002 updated for block output
/////////////////////////////////////////////////////////////////////////////////////
void ShowHelp()
{

    fprintf(stdout,"\n");
    fprintf(stdout,"Usage:\n");
    fprintf(stdout,"EAQUAL [-options] -fref reference_file -ftest test_file\n\n");
    fprintf(stdout,"\nOptions:\n");
    fprintf(stdout,"-bits\t number of bits per sample (default: -bits 16)\n");
    fprintf(stdout,"-blockout\t get intermediate results for blocks: -blockout value file_path (valid values are: All, ODG, DI, BWRef, BWTest, NMR, ADB, EHS, ModDiff1, ModDiff2, NoiseLoud, DetProb, RDF, NMRSpec, ModDiffSpec, DetProbSpec, NLSpec)");
    fprintf(stdout,"-ch\t number of channels (default: -ch 2)\n");
    fprintf(stdout,"-forget\t use forgetting factor (only MFPD)\n");
    fprintf(stdout,"-fref\t input path for reference file\n");
    fprintf(stdout,"-ftest\t input path for test file\n");
    fprintf(stdout,"-h\t print this help\n");
    fprintf(stdout,"-offset\t shift the test buffer for offset samples (if <0: shift reference buffer for offset samples\n");
    fprintf(stdout,"-silent\t path for ASCII result file, no output\n");
    fprintf(stdout,"-srate\t sampling rate of input files (default: -srate 48000)\n");
    fprintf(stdout,"-spl\t monitoring level in dBSPL(default: -spl 92)\n");
    fprintf(stdout,"-type\t analysis type: b= BASIC, a=ADVANCED (default: -type b, a not allowed in the moment)\n");
    fprintf(stdout,"-ver\t print program version\n");

    fflush (stdout);
}

/////////////////////////////////////////////////////////////////////////////////////
//  function:       OpenOutputFiles
//  @parameter:     - stOutputPaths *pOutputPaths       pointer on structure with output file paths
//                  - stOutputFiles *pOutputFiles       pointer on structure with output files
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         open output files for block output
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int OpenOutputFiles(stOutputPaths *pOutputPaths, stOutputFiles *pOutputFiles)
{
    if (pOutputPaths->pcADB)
    {
        if ((pOutputFiles->pFADB = fopen(pOutputPaths->pcADB,"wt")))
            fprintf(pOutputFiles->pFADB,"ADB per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcAll)
    {
        if ((pOutputFiles->pFAll = fopen(pOutputPaths->pcAll,"wt")))
            fprintf(pOutputFiles->pFAll,"All model output variables per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcBWRef)
    {
        if ((pOutputFiles->pFBWRef = fopen(pOutputPaths->pcBWRef,"wt")))
            fprintf(pOutputFiles->pFBWRef,"bandwidth of reference signal per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcBWTest)
    {
        if ((pOutputFiles->pFBWTest = fopen(pOutputPaths->pcBWTest,"wt")))
            fprintf(pOutputFiles->pFBWTest,"bandwidth of test signal per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcDetProb)
    {
        if ((pOutputFiles->pFDetProb = fopen(pOutputPaths->pcDetProb,"wt")))
            fprintf(pOutputFiles->pFDetProb,"detection probability per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcDI)
    {
        if ((pOutputFiles->pFDI = fopen(pOutputPaths->pcDI,"wt")))
            fprintf(pOutputFiles->pFDI,"distortion index per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcEHS)
    {
        if ((pOutputFiles->pFEHS = fopen(pOutputPaths->pcEHS,"wt")))
            fprintf(pOutputFiles->pFEHS,"harmonic structure of error per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcModDiff1)
    {
        if ((pOutputFiles->pFModDiff1 = fopen(pOutputPaths->pcModDiff1,"wt")))
            fprintf(pOutputFiles->pFModDiff1,"modulation difference 1 per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcModDiff2)
    {
        if ((pOutputFiles->pFModDiff2 = fopen(pOutputPaths->pcModDiff2,"wt")))
            fprintf(pOutputFiles->pFModDiff2,"modulation difference 2 per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcNMR)
    {
        if ((pOutputFiles->pFNMR = fopen(pOutputPaths->pcNMR,"wt")))
            fprintf(pOutputFiles->pFNMR,"NMR per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcNoiseLoud)
    {
        if ((pOutputFiles->pFNoiseLoud = fopen(pOutputPaths->pcNoiseLoud,"wt")))
            fprintf(pOutputFiles->pFNoiseLoud,"noise loudness per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcODG)
    {
        if ((pOutputFiles->pFODG = fopen(pOutputPaths->pcODG,"wt")))
            fprintf(pOutputFiles->pFODG,"objective difference grade (ODG) per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcRDF)
    {
        if ((pOutputFiles->pFRDF = fopen(pOutputPaths->pcRDF,"wt")))
            fprintf(pOutputFiles->pFRDF,"RDF per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcNoiseLoudSpectral)
    {
        if ((pOutputFiles->pFNoiseLoudSpectral = fopen(pOutputPaths->pcNoiseLoudSpectral,"wt")))
            fprintf(pOutputFiles->pFNoiseLoudSpectral,"spectral distribution of noise loudness per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcNMRSpectral)
    {
        if ((pOutputFiles->pFNMRSpectral = fopen(pOutputPaths->pcNMRSpectral,"wt")))
            fprintf(pOutputFiles->pFNMRSpectral,"spectral distribution of NMR per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcModDiffSpectral)
    {
        if ((pOutputFiles->pFModDiffSpectral = fopen(pOutputPaths->pcModDiffSpectral,"wt")))
            fprintf(pOutputFiles->pFModDiffSpectral,"spectral distribution of modulation difference 1 per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }
    if (pOutputPaths->pcDetProbSpectral)
    {
        if ((pOutputFiles->pFDetProbSpectral = fopen(pOutputPaths->pcDetProbSpectral,"wt")))
            fprintf(pOutputFiles->pFDetProbSpectral,"spectral distribution of detection probability per block...\n\n");
        else
            return _FILE_OPEN_ERROR;
    }

    return _NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////
//  function:       CloseOutputFiles
//  @parameter:     - stOutputFiles *pOutputFiles       pointer on structure with output files
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         open output files for block output
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void CloseOutputFiles(stOutputFiles *pOutputFiles)
{
    if (pOutputFiles->pFADB)
        fclose(pOutputFiles->pFADB);
    if (pOutputFiles->pFAll)
        fclose(pOutputFiles->pFAll);
    if (pOutputFiles->pFODG)
        fclose(pOutputFiles->pFODG);
    if (pOutputFiles->pFDI)
        fclose(pOutputFiles->pFDI);
    if (pOutputFiles->pFBWRef)
        fclose(pOutputFiles->pFBWRef);
    if (pOutputFiles->pFBWTest)
        fclose(pOutputFiles->pFBWTest);
    if (pOutputFiles->pFNMR)
        fclose(pOutputFiles->pFNMR);
    if (pOutputFiles->pFNMRSpectral)
        fclose(pOutputFiles->pFNMRSpectral);
    if (pOutputFiles->pFDetProb)
        fclose(pOutputFiles->pFDetProb);
    if (pOutputFiles->pFDetProbSpectral)
        fclose(pOutputFiles->pFDetProbSpectral);
    if (pOutputFiles->pFModDiff1)
        fclose(pOutputFiles->pFModDiff1);
    if (pOutputFiles->pFModDiff2)
        fclose(pOutputFiles->pFModDiff2);
    if (pOutputFiles->pFModDiffSpectral)
        fclose(pOutputFiles->pFModDiffSpectral);
    if (pOutputFiles->pFEHS)
        fclose(pOutputFiles->pFEHS);
    if (pOutputFiles->pFRDF)
        fclose(pOutputFiles->pFRDF);
    if (pOutputFiles->pFNoiseLoud)
        fclose(pOutputFiles->pFNoiseLoud);
    if (pOutputFiles->pFNoiseLoudSpectral)
        fclose(pOutputFiles->pFNoiseLoudSpectral);
}

/////////////////////////////////////////////////////////////////////////////////////
//  function:       PrintOutputFiles
//  @parameter:     - stBlockResults    *pBlockResults      results of analyisis per block
//                  - stOutputFiles     *pOutputFiles       pointer on structure with output files
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         open output files for block output
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void PrintOutputFiles(stBlockResults *pBlockResults, stOutputFiles  *pOutputFiles)
{
    if (pOutputFiles->pFODG)
        fprintf(pOutputFiles->pFODG,"%e\n", pBlockResults->pfMOVs[0]);
    if (pOutputFiles->pFDI)
        fprintf(pOutputFiles->pFDI,"%e\n", pBlockResults->pfMOVs[1]);
    if (pOutputFiles->pFBWRef)
        fprintf(pOutputFiles->pFBWRef,"%e\n", pBlockResults->pfMOVs[2]);
    if (pOutputFiles->pFBWTest)
        fprintf(pOutputFiles->pFBWTest,"%e\n", pBlockResults->pfMOVs[3]);
    if (pOutputFiles->pFNMR)
        fprintf(pOutputFiles->pFNMR,"%e\n", pBlockResults->pfMOVs[4]);
    if (pOutputFiles->pFADB)
        fprintf(pOutputFiles->pFADB,"%e\n", pBlockResults->pfMOVs[6]);
    if (pOutputFiles->pFEHS)
        fprintf(pOutputFiles->pFEHS,"%e\n", pBlockResults->pfMOVs[7]);
    if (pOutputFiles->pFModDiff1)
        fprintf(pOutputFiles->pFModDiff1,"%e\n", pBlockResults->pfMOVs[8]);
    if (pOutputFiles->pFModDiff2)
        fprintf(pOutputFiles->pFModDiff2,"%e\n", pBlockResults->pfMOVs[9]);
    if (pOutputFiles->pFNoiseLoud)
        fprintf(pOutputFiles->pFNoiseLoud,"%e\n", pBlockResults->pfMOVs[10]);
    if (pOutputFiles->pFDetProb)
        fprintf(pOutputFiles->pFDetProb,"%e\n", pBlockResults->pfMOVs[11]);
    if (pOutputFiles->pFRDF)
        fprintf(pOutputFiles->pFRDF,"%e\n", pBlockResults->pfMOVs[12]);
    if (pOutputFiles->pFAll)
    {   
        for (int i = 0; i<13; i++)
            fprintf(pOutputFiles->pFAll,"%e ", pBlockResults->pfMOVs[i]);
        fprintf(pOutputFiles->pFAll,"\n");
    }
    if (pOutputFiles->pFDetProbSpectral)
    {   
        for (int i = 0; i<109; i++)
            fprintf(pOutputFiles->pFDetProbSpectral,"%e ", pBlockResults->pfDetProbSpectral[i]);
        fprintf(pOutputFiles->pFDetProbSpectral,"\n");
    }
    if (pOutputFiles->pFModDiffSpectral)
    {   
        for (int i = 0; i<109; i++)
            fprintf(pOutputFiles->pFModDiffSpectral,"%e ", pBlockResults->pfModDiffSpectral[i]);
        fprintf(pOutputFiles->pFModDiffSpectral,"\n");
    }
    if (pOutputFiles->pFNoiseLoudSpectral)
    {   
        for (int i = 0; i<109; i++)
            fprintf(pOutputFiles->pFNoiseLoudSpectral,"%e ", pBlockResults->pfNoiseLoudSpectral[i]);
        fprintf(pOutputFiles->pFNoiseLoudSpectral,"\n");
    }
    if (pOutputFiles->pFNMRSpectral)
    {   
        for (int i = 0; i<109; i++)
            fprintf(pOutputFiles->pFNMRSpectral,"%e ", pBlockResults->pfNMRSpectral[i]);
        fprintf(pOutputFiles->pFNMRSpectral,"\n");
    }
}