/////////////////////////////////////////////////////////////////////////////////////
// EAQUALMain.cpp - command line application of EAQUAL
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

#ifdef WIN32
    #include <conio.h>
#endif
#include <time.h>

#include "sndfile.h"
#include "libEAQUAL.h"
#include "zErrorCodes.h"
#include "EAQUALMain.h"

/////////////////////////////////////////////////////////////////////////////////////
// constants
#define _BLOCK_LENGTH       2048
#define _HALF_BLOCK_LENGTH  (_BLOCK_LENGTH>>1)
#define _IN_BUFFER_LENGTH   (_BLOCK_LENGTH<<3)

/////////////////////////////////////////////////////////////////////////////////////
// macros
#define HandleError(rError) (ShowError(rError))


/////////////////////////////////////////////////////////////////////////////////////
// local functions
/////////////////////////////////////////////////////////////////////////////////////
//  @function:      CheckInputFormats
//  @parameter:     - SF_INFO   sfRefInfo   info structure with audio format of reference file
//                  - SF_INFO   sfTestInfo  info structure with audio format of test file
//                  - SNDFILE   *pFRef      pointer on reference file
//                  - SNDFILE   *pFTest     pointer on test file
//  @result:        - int       iError      error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         checks if the input formats are equal and valid
//  @creation_date: 23.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
static inline int   CheckInputFormats(  SF_INFO     sfRefInfo, 
                                        SF_INFO     sfTestInfo, 
                                        SNDFILE     *pFRef, 
                                        SNDFILE     *pFTest);
/////////////////////////////////////////////////////////////////////////////////////
//  @function:      DivChannels
//  @parameter:     - short *pSampleBuffer  buffer with interleaved stereo data
//                  - short *pAudioBuffer1  resulting audio buffer left
//                  - short *pAudioBuffer2  resulting audio buffer right
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         divide the interleaved audio data in two single buffers
//  @creation_date: 23.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
static inline void  DivChannels (       short       *pSampleBuffer, 
                                        short       *pAudioBuffer1, 
                                        short       *pAudioBuffer2);


/////////////////////////////////////////////////////////////////////////////////////
// main application
int main(int argc, char* argv[])
{
    unsigned int    rError,
                    uiFramesProcessed   = 0;
    int             i,
                    iNumSamplesRef,
                    iNumSamplesTest,
                    iLengthOfResults,
                    iOffset,
                    iSamplesLeft    = 0;
    bool            bReadNewData    = 1,
                    bOutputMode     = 0,
                    bBlockOutput    = 0;
    void            *pInstanceHandle;
    double          *pfResults,
                    afResults[13];
    char            *pcRefPath  = 0,
                    *pcTestPath = 0;
    short           asRefBuffer[2][_IN_BUFFER_LENGTH],
                    asTestBuffer[2][_IN_BUFFER_LENGTH],
                    asSampleBuffer[_IN_BUFFER_LENGTH<<1];
    clock_t         clStartTime;
    stCLOptions     CLOptions;
    WaveFormat      WFormat;
    stAudioData     AudioData;
    SNDFILE         *pFRefFile,
                    *pFTestFile;
    SF_INFO         sfRefInfo,
                    sfTestInfo;
    stOutputPaths   OutputPaths;
    stOutputFiles   OutputFiles;

    stBlockResults  BlockResults;
    

    // init basic configuration
    InitArgs(       &CLOptions, 
                    &WFormat, 
                    &sfRefInfo, 
                    &sfTestInfo,
                    &iOffset,
                    &OutputPaths,
                    &OutputFiles);

    // read basic configuration
    if ( argc > 1)
        rError      = ParseArgs(    &CLOptions, 
                                    &WFormat, 
                                    &pcRefPath, 
                                    &pcTestPath,
                                    &OutputPaths,
                                    &sfRefInfo,
                                    &sfTestInfo,
                                    &iOffset,
                                    &bOutputMode,
                                    &bBlockOutput,
                                    argc, 
                                    argv);
    else
    {
        // print help and close application
        rError      = _ONLY_HELP_ARG;
    }

    
    // show program info
    if (!bOutputMode)
        ShowProgramInfo();

    switch (rError)
    {
    case _NO_ERROR:
        break;
    case _ONLY_HELP_ARG:
    {
        // print help and close application
        ShowHelp();
        return _NO_ERROR;
    }
    case _ONLY_VERSION_ARG:
    {
        // print help and close application
        ShowVersion();
        return _NO_ERROR;
    }
    default:
    {
        HandleError (rError);
        return rError;
    }
    }

    // open soundfiles
    pFRefFile       = sf_open_read (pcRefPath, 
                                    &sfRefInfo);
    pFTestFile      = sf_open_read (pcTestPath, 
                                    &sfTestInfo);

    // copy sndfile data
    WFormat.iNumOfBits      = sfRefInfo.pcmbitwidth;
    WFormat.iNumOfChannel   = sfRefInfo.channels;
    WFormat.iSamplingRate   = sfRefInfo.samplerate;

    // check if the input is valid
    if ((rError     = CheckInputFormats (   sfRefInfo, 
                                            sfTestInfo, 
                                            pFRefFile, 
                                            pFTestFile)))   
    {
        HandleError (rError);
        return rError;
    }

    // open files for block output
    if (bBlockOutput)
    {
        if ((rError     = OpenOutputFiles ( &OutputPaths, 
                                            &OutputFiles))) 
        {
            HandleError (rError);
            return rError;
        }
    }

    if (!bOutputMode)
        ShowConfiguration(pcRefPath, pcTestPath, &WFormat);

    #ifdef WIN32
        if (!bOutputMode)
            ShowCancel();
    #endif

    // iOffset !=0 if reference and test not synchronous --> read those samples and discard it
    if (iOffset > 0)
        iNumSamplesTest = sf_read_short (pFTestFile, asTestBuffer[0], WFormat.iNumOfChannel*iOffset);
    else if (iOffset < 0)
        iNumSamplesRef  = sf_read_short (pFRefFile, asRefBuffer[0],WFormat.iNumOfChannel*iOffset);
    
    // create new instance of EAQUAL
    pInstanceHandle = eaqualCreateInstance ();

    // initialize the new instance
    if ((rError     = eaqualInitInstance (  pInstanceHandle,
		&CLOptions, 
		&WFormat))) 
    {
        HandleError (rError);
        eaqualKillInstance (pInstanceHandle);
        return rError;
    }
	
    // set starting time
    clStartTime     = clock();

    // processing loop
    while (bReadNewData)
    {
        // read audio data
        if (!iSamplesLeft)
        {   
            // read new data in input buffer
            if (WFormat.iNumOfChannel == 1)
            {
                iNumSamplesRef  = sf_read_short (pFRefFile, asRefBuffer[0], _IN_BUFFER_LENGTH);
                iNumSamplesTest = sf_read_short (pFTestFile, asTestBuffer[0], _IN_BUFFER_LENGTH);
                AudioData.pRefData[0]   = &asRefBuffer[0][0];
                AudioData.pTestData[0]  = &asTestBuffer[0][0];
            }
            else // two channels
            {
                iNumSamplesRef  = sf_read_short (pFRefFile, asSampleBuffer, _IN_BUFFER_LENGTH<<1)/2;
                DivChannels (asSampleBuffer, &asRefBuffer[0][0], &asRefBuffer[1][0]);
                iNumSamplesTest = sf_read_short (pFTestFile, asSampleBuffer, _IN_BUFFER_LENGTH<<1)/2;
                DivChannels (asSampleBuffer, &asTestBuffer[0][0], &asTestBuffer[1][0]);
                // set structure pointer
                AudioData.pRefData[0]   = &asRefBuffer[0][0];
                AudioData.pTestData[0]  = &asTestBuffer[0][0];
                AudioData.pRefData[1]   = &asRefBuffer[1][0];
                AudioData.pTestData[1]  = &asTestBuffer[1][0];
            }
            if (!(iNumSamplesRef & iNumSamplesTest))
                bReadNewData    = 0;

            // check the number of read samples and set number of samples in buffer
            else if ((iNumSamplesRef & iNumSamplesTest) == _IN_BUFFER_LENGTH)
                iSamplesLeft    = _IN_BUFFER_LENGTH;
            else if ((iNumSamplesRef == iNumSamplesTest) || (iNumSamplesRef < iNumSamplesTest))
                iSamplesLeft    = iNumSamplesRef + (_HALF_BLOCK_LENGTH)- (iNumSamplesRef%(_HALF_BLOCK_LENGTH));
            else
                iSamplesLeft    = iNumSamplesTest + (_HALF_BLOCK_LENGTH)- (iNumSamplesTest%(_HALF_BLOCK_LENGTH));
        }
        // process and analyze audio data block
        if ((rError = eaqualProcessInstance (pInstanceHandle, 
                                            &AudioData)))
        {
            HandleError (rError);
            eaqualKillInstance (pInstanceHandle);
            return rError;
        }

        #ifdef WIN32
            // user interruption?
            else if(kbhit())
            {
                short c;
                c = _getch(); 
                if(!c) c = _getch();
                if(c == 27) 
                    bReadNewData        = 0;
            }
        #endif

        // block results output
        if (bBlockOutput)
        {
            BlockResults.iThisChannelPlease = 0;
            if (!(eaqualGetBlockResults (   pInstanceHandle, 
                                            &BlockResults)))
                PrintOutputFiles (          &BlockResults, 
                                            &OutputFiles);
        }

        // changes values for next loop
        iSamplesLeft           -= _HALF_BLOCK_LENGTH;
        AudioData.pRefData[0]  += _HALF_BLOCK_LENGTH;
        AudioData.pRefData[1]  += _HALF_BLOCK_LENGTH;
        AudioData.pTestData[0] += _HALF_BLOCK_LENGTH;
        AudioData.pTestData[1] += _HALF_BLOCK_LENGTH;

        // print the number of frames
        if (!bOutputMode)
            ShowFrameNumber (uiFramesProcessed++, WFormat.iSamplingRate);
    }

    // we have finished audio processing, now get the results
    if ((rError = eaqualEndInstance (   pInstanceHandle,
                                        &pfResults,
                                        &iLengthOfResults))) 
    {
        HandleError (rError);
        eaqualKillInstance (pInstanceHandle);
        return rError;
    }

    
    // print elapsed time
    if (!bOutputMode)
        ShowTime(clStartTime);

    // copy results
    for ( i = 0; i< iLengthOfResults; i++)
        afResults[i]    = pfResults[i];

    // printf results
    if (!bOutputMode)
        ShowResults(afResults, iLengthOfResults);
    else
        if ((rError = PrintResults(afResults, iLengthOfResults, OutputPaths.pcResult, pcRefPath, pcTestPath))) 
        {
            HandleError (rError);
            eaqualKillInstance (pInstanceHandle);
            return rError;
        }

    // close output files
    if (bBlockOutput)
        CloseOutputFiles (&OutputFiles);    

    // all is done, kill instance now
    eaqualKillInstance (pInstanceHandle);
    
    return rError;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      CheckInputFormats
//  @parameter:     - SF_INFO   sfRefInfo   info structure with audio format of reference file
//                  - SF_INFO   sfTestInfo  info structure with audio format of test file
//                  - SNDFILE   *pFRef      pointer on reference file
//                  - SNDFILE   *pFTest     pointer on test file
//  @result:        - int       iError      error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         checks if the input formats are equal and valid
//  @creation_date: 23.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
static inline int   CheckInputFormats(SF_INFO sfRefInfo, SF_INFO sfTestInfo, SNDFILE *pFRef, SNDFILE *pFTest)
{
    if (( sfRefInfo.channels    != sfTestInfo.channels)     ||
        ( sfRefInfo.pcmbitwidth != sfTestInfo.pcmbitwidth)  ||
        ( sfRefInfo.samplerate  != sfTestInfo.samplerate))
        return _ILLEGAL_FILE_FORMAT;
    else 
        if (( pFRef     == 0) || (pFRef == NULL) ||
            ( pFTest    == 0) || (pFTest == NULL))
            return _FILE_OPEN_ERROR;
        else
            return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      DivChannels
//  @parameter:     - short *pSampleBuffer  buffer with interleaved stereo data
//                  - short *pAudioBuffer1  resulting audio buffer left
//                  - short *pAudioBuffer2  resulting audio buffer right
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         divide the interleaved audio data in two single buffers
//  @creation_date: 23.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
static inline void  DivChannels ( short *pSampleBuffer, short *pAudioBuffer1, short *pAudioBuffer2)
{
    int k;
    for ( k = 0; k < _IN_BUFFER_LENGTH; k++)
    {
        pAudioBuffer1[k]    = pSampleBuffer[2*k];
        pAudioBuffer2[k]    = pSampleBuffer[2*k+1];
    }
}


