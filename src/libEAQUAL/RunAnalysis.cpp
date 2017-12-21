/////////////////////////////////////////////////////////////////////////////////////
// RunAnalysis.cpp: implementation of the CRunAnalysis class.
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


#include "EAQUALCore.h"
#include "libEAQUAL.h"
#include "RunAnalysis.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunAnalysis::CRunAnalysis()
{
    m_bBeginOfFile  = _TRUE;

    m_pCAvgBlock    = 0;
    m_pCAvgTime     = 0;
    m_pCCalcTables  = 0;
    m_pCFFTModel    = 0;
    m_pCKogModel    = 0;
    m_pCMOVBlock    = 0;
    m_pCPreProc     = 0;
    m_pCFileIO      = 0;
}

CRunAnalysis::~CRunAnalysis()
{
    if ( m_pCAvgBlock   != 0) delete m_pCAvgBlock;
    if ( m_pCAvgTime    != 0) delete m_pCAvgTime;
    if ( m_pCCalcTables != 0) delete m_pCCalcTables;
//  if ( m_pCFBModel    != 0) delete m_pCFBModel;
    if ( m_pCFFTModel   != 0) delete m_pCFFTModel;
    if ( m_pCKogModel   != 0) delete m_pCKogModel;
    if ( m_pCMOVBlock   != 0) delete m_pCMOVBlock;
    if ( m_pCPreProc    != 0) delete m_pCPreProc;
    if ( m_pCFileIO     != 0) delete m_pCFileIO;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        raProcess
//  @parameter:     - stAudioData *pAudioData   input audio data ref and test
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         process of one block of data    
//  @creation_date: 09.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CRunAnalysis::raProcess(stAudioData *pAudioData)
{
    zINT    iCh,
            iIdx,
            iNumOfMOVs  = (EAQUALInstance.bModelType == _BASIC) ? _NUM_MOV_BASIC : _NUM_MOV_ADVANCED;

    zERROR  rErr;
    
    
    // you can't read results while processing
    m_ProcessingBlockFinished   = _FALSE;   
    
    // copy the data vectors into libs memory
    for (iCh = 0; iCh < EAQUALInstance.WaveFormat.iNumOfChannel; iCh++)
    {
        zFLOAT  *pfAudioRef     = EAQUALInstance.ChannelData[iCh].pfTimeSignalRef,
                *pfAudioTest    = EAQUALInstance.ChannelData[iCh].pfTimeSignalTest;

        // copy new audio data in buffer and move old data
        zplCopyVec (pfAudioRef, pfAudioRef + _HALF_FFT_LENGTH, _HALF_FFT_LENGTH);
        zplCopyVec (pfAudioTest, pfAudioTest + _HALF_FFT_LENGTH, _HALF_FFT_LENGTH);
        for (iIdx = 0; iIdx < _HALF_FFT_LENGTH; iIdx++)
        {
            pfAudioRef[iIdx + _HALF_FFT_LENGTH]     = static_cast<zFLOAT>(pAudioData->pRefData[iCh][iIdx]);
            pfAudioTest[iIdx + _HALF_FFT_LENGTH]    = static_cast<zFLOAT>(pAudioData->pTestData[iCh][iIdx]);
        }
    }
    if (m_bBeginOfFile)
    {
        m_bBeginOfFile = _FALSE;
        #if ( ADD_SILENCE_BEGIN == OFF)
            return _NO_ERROR;
        #endif
    }
    
    // begin processing
    
    // psychoacoustic model FFT based
    m_pCFFTModel->Process (&EAQUALInstance);

    // pattern pre processing
    m_pCPreProc->Process (&EAQUALInstance);

    // calculation of MOVs
    m_pCMOVBlock->Process (&EAQUALInstance);

    // averaging of some MOVs (mainly in the spectral domain)
    m_pCAvgBlock->Process (&EAQUALInstance);

    // write temporary data to file
    for (iCh = 0; iCh < EAQUALInstance.WaveFormat.iNumOfChannel; iCh++)
        if ((rErr = m_pCFileIO->fioWriteTmpFile (EAQUALInstance.ChannelData[iCh].pfChannelMOVs, iNumOfMOVs)))
            return rErr;

    // update FrameCount
    EAQUALInstance.uiFramesProcessed++;

    // may the block data being read?
    m_ProcessingBlockFinished   = _TRUE;
    
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        raFinalProcess
//  @parameter:     - zFLOAT    **pResults      pointer on pointer on the results, to be written
//                  - zINT      *pResultLength  pointer on the length of pResults, to be written
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         final processing of data and output of data 
//  @creation_date: 16.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CRunAnalysis::raFinalProcess(zFLOAT **pResults, zINT *pResultLength)
{
    zERROR  rErr    = _NO_ERROR;

    // you can't read block results after final processing 
    m_ProcessingBlockFinished   = _FALSE;   
    
    // frame count is one to high...
    EAQUALInstance.uiFramesProcessed--;
    
    // finish of temp file writing
    if (( rErr = m_pCFileIO->fioEndWrite ())) return rErr;

    // read the time data from temp file
    if (( rErr = m_pCFileIO->fioReadTmpFile (&EAQUALInstance))) return rErr;

    // final averaging
    m_pCAvgTime->Process (&EAQUALInstance);

    // neural network
    m_pCKogModel->Process (EAQUALInstance.pfMOVs);

    // scaling of bandwidth for output
    if (EAQUALInstance.bModelType == _BASIC)
    {
        zplMultVecConst (&EAQUALInstance.pfMOVs[2], 48000.0F / _FFT_LENGTH, 2);
        *pResultLength  = _NUM_MOV_BASIC + 2;
    }
    else
        *pResultLength  = _NUM_MOV_ADVANCED + 2;

    // set output pointer
    *pResults           = EAQUALInstance.pfMOVs;

    return _NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        raInit
//  @parameter:     - stCLOptions       *pOptions       pointer struct with CL Options
//                  - WaveFormat        *pWaveFormat    pointer Waveformat
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         initialize RunAnalysis class    
//  @creation_date: 16.12.2001
//  @last_modified: 07.01.2002 AL: added checking for valid input formats
/////////////////////////////////////////////////////////////////////////////////////
zERROR CRunAnalysis::raInit(stCLOptions *pOptions, WaveFormat *pWaveFormat)
{
    zINT    iCh;
    zERROR  rRes    = _NO_ERROR;

    bStopProcessing             = _FALSE;
    m_ProcessingBlockFinished   = _FALSE;

    memset(&EAQUALInstance,0,sizeof(EAQUALInstance));

    EAQUALInstance.uiFramesProcessed                    = 1;
    EAQUALInstance.uiNumEnFrames                        = 0;

    // set globals
    EAQUALInstance.bModelType                           = pOptions->bModelType;
    EAQUALInstance.GlobalOptions.fSPL                   = pOptions->fSPL;
    EAQUALInstance.GlobalOptions.bForgetFactorEnabled   = pOptions->bForgetFactorEnabled;

    EAQUALInstance.WaveFormat.iNumOfBits                = pWaveFormat->iNumOfBits;
    EAQUALInstance.WaveFormat.iNumOfChannel             = pWaveFormat->iNumOfChannel;
    EAQUALInstance.WaveFormat.iSamplingRate             = pWaveFormat->iSamplingRate;

    // check for input formats
    // samplerate must be > 44100
    if (EAQUALInstance.WaveFormat.iSamplingRate < 44100)
        return _INVALID_SAMPLE_FREQ;
    // number of channels has to be 2 or less
    if (EAQUALInstance.WaveFormat.iNumOfChannel > 2)
        return _INVALID_NUM_OF_CHANNEL;
    // advanced model is not implemented, so only _BASIC is valid
    if (EAQUALInstance.bModelType != _BASIC)
        return _INVALID_FUNCTION_ARGS;

    // initialize classes here
    if (! (m_pCAvgBlock     = new CAvgBlock(&EAQUALInstance)))                      return _MEM_ALLOC_FAILED;
    if (! (m_pCAvgTime      = new CAvgTime(&EAQUALInstance)))                       return _MEM_ALLOC_FAILED;
    if (! (m_pCCalcTables   = new CCalcTables(EAQUALInstance.bModelType)))          return _MEM_ALLOC_FAILED;
    if (  (rRes = m_pCCalcTables->Process(&EAQUALInstance)))                        return rRes;
//  if (! (m_pCFBModel      = new CFBModel(&EAQUALInstance)))                       return _MEM_ALLOC_FAILED;
    if (! (m_pCFFTModel     = new CFFTModel(&EAQUALInstance)))                      return _MEM_ALLOC_FAILED;
    if (! (m_pCKogModel     = new CKogModel(&EAQUALInstance.Tables.stTablesNN)))    return _MEM_ALLOC_FAILED;
    if (! (m_pCMOVBlock     = new CMOVBlock(&EAQUALInstance)))                      return _MEM_ALLOC_FAILED;
    if (! (m_pCPreProc      = new CPreProc(&EAQUALInstance)))                       return _MEM_ALLOC_FAILED;
    if (! (m_pCFileIO       = new CFileIO()))                                       return _MEM_ALLOC_FAILED;

    // set pointers
    for (iCh = 0; iCh < EAQUALInstance.WaveFormat.iNumOfChannel; iCh++)
    {
        EAQUALInstance.ChannelData[iCh].pfTimeSignalRef     = m_afTimeSignalRef[iCh];
        EAQUALInstance.ChannelData[iCh].pfTimeSignalTest    = m_afTimeSignalTest[iCh];

        // initialize pointers
        zplSetZero(m_afTimeSignalRef[iCh], _FFT_LENGTH);
        zplSetZero(m_afTimeSignalTest[iCh], _FFT_LENGTH);
    }
    return rRes;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        raGetBlockResults
//  @parameter:     - stBlockResults    *pstResults     pointer on structure of results, to be written
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         get results of one audio block  
//  @creation_date: 06.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CRunAnalysis::raGetBlockResults(stBlockResults *pstResults)
{
    if (m_ProcessingBlockFinished != _TRUE)
        return _FUNCTION_NOT_READY;
    
    // calculate block MOVs per channel if not already calculated
    raCalcChannelMOVs();
    
    // average over channels
    m_pCAvgTime->avChannelAvg (&EAQUALInstance);
    
    // neural network
    m_pCKogModel->Process (EAQUALInstance.pfMOVs);

    // scaling of bandwidth for output
    if (EAQUALInstance.bModelType == _BASIC)
        zplMultVecConst (&EAQUALInstance.pfMOVs[2], 48000.0F / _FFT_LENGTH, 2);

    // set pointers in structure
    raSetOutputPointers (pstResults);

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        raCalcChannelMOVs
//  @parameter:     - 
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate block MOVs per channels   
//  @creation_date: 06.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CRunAnalysis::raCalcChannelMOVs()
{
    zINT    iCh;

    // channel loop
    for (iCh = 0; iCh < EAQUALInstance.WaveFormat.iNumOfChannel; iCh++)
    {
        zFLOAT *pfChannelMOVs   = EAQUALInstance.ChannelData[iCh].pfChannelMOVs;

        // pfChannelMOVs[0], pfChannelMOVs[1]: leave bandwidth unchanged

        // NMR
        pfChannelMOVs[2]        = _10INVLN10 * zplLOG(pfChannelMOVs[2]);
        
        // pfChannelMOVs[3] WinModDiff1 unchanged

        // ADB
        if (pfChannelMOVs[9] > 0.5F)
        {
            if (pfChannelMOVs[4] > 0)
                pfChannelMOVs[4]    = _INVLN10 * zplLOG (pfChannelMOVs[4]);
            else
                pfChannelMOVs[4]    = -.5F;
        }
        else
            pfChannelMOVs[4]    = 0.0F;

        // EHS
        if (EAQUALInstance.puiThreshEn[EAQUALInstance.uiNumEnFrames-1] == EAQUALInstance.uiFramesProcessed-1)
            pfChannelMOVs[5]    = 0.0F;

        // ModDiff2 unchanged
        // pfChannelMOVs[7]    *= pfChannelMOVs[6];

        // ModDiff1
        pfChannelMOVs[6]       = pfChannelMOVs[3];

        // pfChannelMOVs[8] NoiseLoud unchanged

        // pfChannelMOVs[9] detection probability unchanged

        // pfChannelMOVs[10] relative disturbed frames unchanged
    }
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        raSetOutputPointers
//  @parameter:     - stBlockResults    *pstResults     pointer on structure of results, to be written
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         set the pointers for block output   
//  @creation_date: 06.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CRunAnalysis::raSetOutputPointers(stBlockResults *pstResults)
{
    // set modeltype so that it's sure how the output is looking for the right number of MOVs
    pstResults->bModelType          = (EAQUALInstance.bModelType == _BASIC) ? _BASIC : _ADVANCED;

    // set pointer on the MOVs per Block
    pstResults->pfMOVs              = EAQUALInstance.pfMOVs;

    // set pointer on spectral MOVs
    pstResults->pfDetProbSpectral   = EAQUALInstance.ChannelData[pstResults->iThisChannelPlease].pfProb;
    pstResults->pfModDiffSpectral   = EAQUALInstance.ChannelData[pstResults->iThisChannelPlease].pfModDiff1;
    pstResults->pfNMRSpectral       = EAQUALInstance.ChannelData[pstResults->iThisChannelPlease].pfNMR;
    pstResults->pfNoiseLoudSpectral = EAQUALInstance.ChannelData[pstResults->iThisChannelPlease].pfNoiseLoud;

    return _NO_ERROR;
}