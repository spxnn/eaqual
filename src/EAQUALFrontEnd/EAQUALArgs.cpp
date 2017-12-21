/////////////////////////////////////////////////////////////////////////////////////
// EAQUALArgs.cpp - functions for parsing command line args
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


#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sndfile.h"
#include "libEAQUAL.h"
#include "EAQUALMain.h"
#include "zErrorCodes.h"


/////////////////////////////////////////////////////////////////////////////////////
//  function:       InitArgs
//  @parameter:     - stCLOptions       *pOptions       pointer on struct for command line options, to be written
//                  - WaveFormat        *pWFormat       pointer on struct for wave format, to be written
//                  - SF_INFO           *sfRefInfo      pointer on struct for reference wave format, to be written
//                  - SF_INFO           *sfTestInfo     pointer on struct for test wave format, to be written
//                  - int               *piOffset       pointer on offset/delay between reference and test file, to be written
//                  - stOutputPaths     *pOutputPaths   pointer on structure with output paths, to be written
//                  - stOutputFiles     *pOutputFiles   pointer on structure with output files, to be written
//  @result:        int                                 Error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         initialize all command line args
//  @creation_date: 27.12.2001
//  @last_modified: 07.01.2002 AL: added outputpath structure
/////////////////////////////////////////////////////////////////////////////////////
int InitArgs(stCLOptions *pOptions, WaveFormat *pWFormat, SF_INFO *sfRefInfo, SF_INFO *sfTestInfo, int *piOffset, stOutputPaths *pOutputPaths, stOutputFiles *pOutputFiles)
{
    *piOffset                       = 0;
    pOptions->bForgetFactorEnabled  = 0;
    pOptions->bModelType            = 0;
    pOptions->fSPL                  = 92;

    pWFormat->iNumOfBits            = 16;
    pWFormat->iNumOfChannel         = 2;
    pWFormat->iSamplingRate         = 48000;

    sfRefInfo->pcmbitwidth  = sfTestInfo->pcmbitwidth   = pWFormat->iNumOfBits;
    sfRefInfo->channels     = sfTestInfo->channels      = pWFormat->iNumOfChannel;
    sfRefInfo->samplerate   = sfTestInfo->samplerate    = pWFormat->iSamplingRate;
    sfRefInfo->format       = sfTestInfo->format        = 0x0;
	
    pOutputPaths->pcADB                 = 0;
    pOutputPaths->pcAll                 = 0;
    pOutputPaths->pcODG                 = 0;
    pOutputPaths->pcDI                  = 0;
    pOutputPaths->pcBWRef               = 0;
    pOutputPaths->pcBWTest              = 0;
    pOutputPaths->pcDetProb             = 0;
    pOutputPaths->pcDetProbSpectral     = 0;
    pOutputPaths->pcEHS                 = 0;
    pOutputPaths->pcModDiff1            = 0;
    pOutputPaths->pcModDiff2            = 0;
    pOutputPaths->pcModDiffSpectral     = 0;
    pOutputPaths->pcNMR                 = 0;
    pOutputPaths->pcNMRSpectral         = 0;
    pOutputPaths->pcNoiseLoud           = 0;
    pOutputPaths->pcNoiseLoudSpectral   = 0;
    pOutputPaths->pcRDF                 = 0;
    pOutputPaths->pcResult              = 0;

    pOutputFiles->pFADB                 = 0;
    pOutputFiles->pFAll                 = 0;
    pOutputFiles->pFODG                 = 0;
    pOutputFiles->pFDI                  = 0;
    pOutputFiles->pFBWRef               = 0;
    pOutputFiles->pFBWTest              = 0;
    pOutputFiles->pFDetProb             = 0;
    pOutputFiles->pFDetProbSpectral     = 0;
    pOutputFiles->pFEHS                 = 0;
    pOutputFiles->pFModDiff1            = 0;
    pOutputFiles->pFModDiff2            = 0;
    pOutputFiles->pFModDiffSpectral     = 0;
    pOutputFiles->pFNMR                 = 0;
    pOutputFiles->pFNMRSpectral         = 0;
    pOutputFiles->pFNoiseLoud           = 0;
    pOutputFiles->pFNoiseLoudSpectral   = 0;
    pOutputFiles->pFRDF                 = 0;

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ParseArgs
//  @parameter:     - stCLOptions       *pOptions       pointer on struct for command line options, to be written
//                  - WaveFormat            *pWFormat       pointer on struct for wave format, to be written
//                  - char              **pcRefPath     pointer on reference file, to be written
//                  - char              **pcTestPath    pointer on test file, to be written
//                  - stOutputPaths     *pOutputPaths   pointer on structure with output paths
//                  - SF_INFO           *sfRefInfo      pointer on struct for reference wave format, to be written
//                  - SF_INFO           *sfTestInfo     pointer on struct for test wave format, to be written
//                  - int               *piOffset       pointer on offset/delay between reference and test file, to be written
//                  - bool              *pbOutputMode   pointer on bool for decision stdio<->file, to be written
//                  - bool              *pbBlockOutput  pointer on bool for decision if block output or not, to be written
//                  - int               argc            number of command line args
//                  - char              *argv           command line
//  @result:        int                                 Error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         parse command line args
//  @creation_date: 27.12.2000
//  @last_modified: 07.01.2002 AL: added support for block output
/////////////////////////////////////////////////////////////////////////////////////
int ParseArgs(stCLOptions *pOptions, WaveFormat *pWFormat, char **pcRefPath, char **pcTestPath, stOutputPaths *pOutputPaths, SF_INFO *sfRefInfo, SF_INFO *sfTestInfo, int *piOffset, bool *pbOutputMode, bool *pbBlockOutput, int argc, char* argv[])
{
    int i = 0;
    
    // read args
    while (++i < argc)
    {
        char    *cToken,
                *cNextArg;

        cToken = argv[i];
        if(*cToken++ == '-')
        {
            if(i+1 < argc) 
                cNextArg = argv[i+1];
            else
                cNextArg = "";

            // path reference file
            if (strcmp(cToken, "fref")==0)
                *pcRefPath      = cNextArg;

            // path test file
            if (strcmp(cToken, "ftest")==0)
                *pcTestPath     = cNextArg;
            
            // ouput mode
            if (strcmp(cToken, "silent")==0)
            {
                *pbOutputMode   = 1;
                pOutputPaths->pcResult  = cNextArg;
            }
            
            // block ouput mode
            if (strcmp(cToken, "blockout")==0)
            {
                *pbBlockOutput  = 1;
                if (strcmp(cNextArg, "All")==0)
                    pOutputPaths->pcAll                 = argv[i+2];
                else if (strcmp(cNextArg, "ODG")==0)
                    pOutputPaths->pcODG                 = argv[i+2];
                else if (strcmp(cNextArg, "DI")==0)
                    pOutputPaths->pcDI                  = argv[i+2];
                else if (strcmp(cNextArg, "BWRef")==0)
                    pOutputPaths->pcBWRef               = argv[i+2];
                else if (strcmp(cNextArg, "BWTest")==0)
                    pOutputPaths->pcBWTest              = argv[i+2];
                else if (strcmp(cNextArg, "NMR")==0)
                    pOutputPaths->pcNMR                 = argv[i+2];
                else if (strcmp(cNextArg, "ADB")==0)
                    pOutputPaths->pcADB                 = argv[i+2];
                else if (strcmp(cNextArg, "EHS")==0)
                    pOutputPaths->pcEHS                 = argv[i+2];
                else if (strcmp(cNextArg, "ModDiff1")==0)
                    pOutputPaths->pcModDiff1            = argv[i+2];
                else if (strcmp(cNextArg, "ModDiff2")==0)
                    pOutputPaths->pcModDiff2            = argv[i+2];
                else if (strcmp(cNextArg, "ModDiff1")==0)
                    pOutputPaths->pcNoiseLoud           = argv[i+2];
                else if (strcmp(cNextArg, "DetProb")==0)
                    pOutputPaths->pcDetProb             = argv[i+2];
                else if (strcmp(cNextArg, "RDF")==0)
                    pOutputPaths->pcRDF                 = argv[i+2];
                else if (strcmp(cNextArg, "NMRSpec")==0)
                    pOutputPaths->pcNMRSpectral         = argv[i+2];
                else if (strcmp(cNextArg, "DetProbSpec")==0)
                    pOutputPaths->pcDetProbSpectral     = argv[i+2];
                else if (strcmp(cNextArg, "ModDiffSpec")==0)
                    pOutputPaths->pcModDiffSpectral     = argv[i+2];
                else if (strcmp(cNextArg, "NLSpec")==0)
                    pOutputPaths->pcNoiseLoudSpectral   = argv[i+2];
                else
                    return _WRONG_CL_ARG;
            }
            
            // forgetting factor
            if (strcmp(cToken, "forget")==0)
                pOptions->bForgetFactorEnabled = 1;
            
            // sampling rate (default 48000)
            if (strcmp(cToken, "srate")==0)
            {
                sfRefInfo->samplerate   = 
                sfTestInfo->samplerate  = 
                pWFormat->iSamplingRate = atoi(cNextArg);
            }
            
            // bits per sample (default 16)
            if (strcmp(cToken, "bits")==0)
            {
                sfRefInfo->pcmbitwidth  = 
                sfTestInfo->pcmbitwidth =
                pWFormat->iNumOfBits    = atoi(cNextArg);
            }

            // number of channels (default 2)
            if (strcmp(cToken, "ch")==0)
            {
                sfRefInfo->channels     = 
                sfTestInfo->channels    = 
                pWFormat->iNumOfChannel = atoi(cNextArg);
            }
            
            // sample offset
            if (strcmp(cToken, "offset")==0)
                *piOffset               = atoi(cNextArg);
            
            // analysis type (default _BASIC)
            if (strcmp(cToken, "type")==0)
            {
                if (strcmp(cNextArg, "b")==0) 
                    pOptions->bModelType    = 0; // basic
                else
                    pOptions->bModelType    = 1; // advanced
            }

            // monitoring level (default 92)
            if (strcmp(cToken, "spl")==0)
                pOptions->fSPL              = (float)atof(cNextArg);

            // help
            if ((strcmp(cToken, "h")==0) || (strcmp(cToken, "help")==0) || (strcmp(cToken, "?")==0))
                return _ONLY_HELP_ARG;

            // program version
            if (strcmp(cToken, "ver")==0)
                return _ONLY_VERSION_ARG;
        }
    }

    if  ((*pcRefPath == 0) || (*pcTestPath == 0))
        return  _WRONG_CL_ARG;

    return _NO_ERROR;
}
