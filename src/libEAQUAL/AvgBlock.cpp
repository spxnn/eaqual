/////////////////////////////////////////////////////////////////////////////////////
// AvgBlock.cpp: implementation of the CAvgBlock class
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

#include "AvgBlock.h"

#define _INIT_THRESH_EN_LENGTH  100
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAvgBlock::CAvgBlock(eaqstEAQUALInstance *pInstance)
{
    zINT    iCh,
            iNumOfBands     = (pInstance->bModelType == _BASIC) ? _NUM_BANDS_BASIC : _NUM_BANDS_ADVANCED;
    m_bBeginOfFile          = _TRUE;

    pInstance->uiThreshLoud     = 0;
    pInstance->uiThreshBoundBeg = 0;
    pInstance->uiThreshBoundEnd = 0;
    pInstance->fSumStepsAbThr   = 0;
    pInstance->uiBlocksDistorted= 0;
    
    pInstance->uiThreshDel  = static_cast<zINT>(pInstance->WaveFormat.iSamplingRate / _FFT_LENGTH + 0.5F);
    pInstance->puiThreshEn  = m_puiThreshEn = new zUINT32[_INIT_THRESH_EN_LENGTH];

    // alloc memory and initialize
    for (iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
        pInstance->ChannelData[iCh].pfChannelMOVs   = m_pfChannelMOVs[iCh]  = new zFLOAT[iNumOfBands];
    for (iCh = pInstance->WaveFormat.iNumOfChannel; iCh < _NUM_CHANNELS_MAX; iCh++)
        m_pfChannelMOVs[iCh]                        = 0;
}

CAvgBlock::~CAvgBlock()
{
    if ( m_puiThreshEn      != 0) delete [] m_puiThreshEn;
    for ( zINT iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++)
        if (m_pfChannelMOVs[iCh] != 0) delete [] m_pfChannelMOVs[iCh];
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avProbAvg
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//                  - zINT  iNumOfBands     length of vector    
//  @result:        - zFLOAT    fProb           resulting average of probability
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates binaural spectral average according BS.1387, eqs. (79),(81) 
//  @creation_date: 14.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgBlock::avProbAvg(eaqstEAQUALInstance *pInstance, zINT iNumOfBands)
{
    zINT        iCh,
                iFreq;

    zFLOAT      fTmp,
                fProb       = 1.0F;
    
    for ( iFreq = 0; iFreq < iNumOfBands; iFreq++)
    {
        fTmp                = 0.0F;
        for ( iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
        {
            stChannelData *pChannel = &(pInstance->ChannelData[iCh]);
            
            fTmp            = (pChannel->pfProb[iFreq] > fTmp) ? pChannel->pfProb[iFreq] : fTmp;
        }
        fProb              *= (1.0F - fTmp); 
    }

    fProb                   = 1.0F - fProb;

    return fProb;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avDataBound
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//                  - zINT  iLength         length of time buffer   
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         set flags for Data boundary according BS.1387, p.238, 5.2.4.4 
//  @creation_date: 12.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CAvgBlock::avDataBound(eaqstEAQUALInstance *pInstance, zINT iLength)
{
    zINT        iIdx,
                iCh,
                iNumOfChannels  = pInstance->WaveFormat.iNumOfChannel;

    zFLOAT      fSum[_NUM_CHANNELS_MAX],
                *pfTimeSignal;


    for ( iCh = 0; iCh < iNumOfChannels; iCh++)
    {
        pfTimeSignal        = pInstance->ChannelData[iCh].pfTimeSignalRef;
        fSum[iCh]           = 0;
        for ( iIdx = 0; iIdx < _NUM_ADD_SAMPLES; iIdx++)
            fSum[iCh]      += zplFABS(pfTimeSignal[iIdx]);
    }
        
    for (iIdx = 0; iIdx < iLength - _NUM_ADD_SAMPLES; iIdx++)
    {
        for ( iCh = 0; iCh < iNumOfChannels; iCh++)
        {
            pfTimeSignal        = pInstance->ChannelData[iCh].pfTimeSignalRef;
            if (fSum[iCh] > 200)
            {
                pInstance->uiThreshBoundEnd     = pInstance->uiFramesProcessed-1;

                if ( m_bBeginOfFile )
                {
                    pInstance->uiThreshBoundBeg = pInstance->uiFramesProcessed-1;
                    m_bBeginOfFile              = _FALSE;
                }

                return _NO_ERROR;
            }
            fSum[iCh]      -= zplFABS(pfTimeSignal[iIdx]);
            fSum[iCh]      += zplFABS(pfTimeSignal[iIdx + _NUM_ADD_SAMPLES]);
        }
    }
    
    return _NO_ERROR;

}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avEnThresh
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//                  - zUINT32               uiLength    length of time buffer   
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         set Flags for Energy threshold according BS.1387, p.238, 5.2.4.3
//  @creation_date: 18.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CAvgBlock::avEnThresh(eaqstEAQUALInstance *pInstance, zUINT32   uiLength)
{
    zUINT32     iCh,
                iIdx,
                iNumOfChannels  = pInstance->WaveFormat.iNumOfChannel;
    zUINT32     *puiNumEnFrames = &(pInstance->uiNumEnFrames);
    
    zFLOAT      fEnergy = 0.0F;


    for (iCh = 0; iCh < iNumOfChannels; iCh++)
    {
        stChannelData   *pChannel   = &(pInstance->ChannelData[iCh]);

        for ( iIdx = (uiLength>>1); iIdx < uiLength; iIdx++)
        {
            fEnergy    += pChannel->pfTimeSignalRef[iIdx] * pChannel->pfTimeSignalRef[iIdx];
            fEnergy    += pChannel->pfTimeSignalTest[iIdx] * pChannel->pfTimeSignalTest[iIdx];
        }
    }
    
    if (fEnergy < _ENERGY_THRESHOLD * iNumOfChannels)
    {
        // is the allocated array too short?
        if ((*puiNumEnFrames % _INIT_THRESH_EN_LENGTH) == 0)
        {
            zUINT32 *puiTmp = new zUINT32[*puiNumEnFrames + _INIT_THRESH_EN_LENGTH];
            if (puiTmp == 0) return _MEM_ALLOC_FAILED;

            zplCopyVec( puiTmp, pInstance->puiThreshEn, *puiNumEnFrames);

            delete [] pInstance->puiThreshEn;

            pInstance->puiThreshEn      = m_puiThreshEn = puiTmp;
        }

        pInstance->puiThreshEn[*puiNumEnFrames] = pInstance->uiFramesProcessed - 1;
        *puiNumEnFrames += 1;
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avLoudThresh
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         set Flags for Loudness threshold according BS.1387, p.238, 5.2.4.2
//  @creation_date: 13.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CAvgBlock::avLoudThresh(eaqstEAQUALInstance *pInstance)
{

    if (!pInstance->uiThreshLoud)
    {
        zINT    iCh,
                iLoudDel,
                iStepSize       = (pInstance->bModelType == _BASIC) ? _STEPSIZE_FFT : _STEPSIZE_FB;

        bool    bRefAbThresh    = _FALSE,
                bTestAbThresh   = _FALSE;


        for (iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
        {
            stChannelData *pChannel = &(pInstance->ChannelData[iCh]);
            
            if (pChannel->fLoudRef >= .1)
                bRefAbThresh    = _TRUE;

            if (pChannel->fLoudTest >= .1)
                bTestAbThresh   = _TRUE;
        }
    
        if (bRefAbThresh & bTestAbThresh)
        {
            iLoudDel                    = static_cast<zINT>(pInstance->WaveFormat.iSamplingRate/iStepSize * 0.05F);
            pInstance->uiThreshLoud     = pInstance->uiFramesProcessed - 1 + iLoudDel;
        }
    }


    return _NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avStepAvg
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//                  - zINT  iNumOfBands     length of vector    
//  @result:        - zFLOAT    fSteps          resulting average of steps above threshold
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates binaural spectral average according BS.1387, eqs. (80),(82) 
//  @creation_date: 14.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgBlock::avStepAvg(eaqstEAQUALInstance *pInstance, zINT iNumOfBands)
{
    zINT        iCh,
                iIdx;

    zFLOAT      fTmp,
                fSteps      = 0.0F,
                *pfSteps;
    
    for ( iIdx = 0; iIdx < iNumOfBands; iIdx++)
    {
        fTmp                = 0.0F;
        for ( iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
        {
            pfSteps         = pInstance->ChannelData[iCh].pfStepsAbThr;
            if (pfSteps[iIdx] > fTmp)
                fTmp        = pfSteps[iIdx];
        }
        fSteps             += fTmp; 
    }

    if ((m_bBeginOfFile == _FALSE) && (pInstance->uiThreshBoundEnd == pInstance->uiFramesProcessed - 1))
        pInstance->fSumStepsAbThr   += fSteps;
    
    return fSteps;
}



/////////////////////////////////////////////////////////////////////////////////////
//  @method:        Process
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates the MOVs per block and other needed variables 
//  @creation_date: 15.02.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CAvgBlock::Process( eaqstEAQUALInstance *pInstance)
{
    zINT    iCh,
            iNumBandsFFT;
    zFLOAT  fDetProb;


    
    ////////////////////////////////////////////////////////////
    // find data boundary and frames with low energy
    avDataBound(    pInstance, _FFT_LENGTH);
    avEnThresh(     pInstance, _FFT_LENGTH);

    ////////////////////////////////////////////////////////////
    // set loudness flag
    avLoudThresh(   pInstance);


    
    if (pInstance->bModelType == _BASIC)
        iNumBandsFFT    = _NUM_BANDS_BASIC;
    else
        iNumBandsFFT    = _NUM_BANDS_BASIC;

    // detection probability (same for both channels)
    fDetProb        = avProbAvg(pInstance, iNumBandsFFT);

    // steps above threshold
    avStepAvg(pInstance, iNumBandsFFT);
    if (fDetProb > .5F)
        pInstance->uiBlocksDistorted++;
    
    for ( iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
    {
        stChannelData   *pChannel   = &(pInstance->ChannelData[iCh]);
        zFLOAT          *pfMOVs     = pChannel->pfChannelMOVs;

        // bandwidth already calculated
        pfMOVs[0]   = pChannel->fBWRef;
        pfMOVs[1]   = pChannel->fBWTest;
        
        // nmr
        pfMOVs[2]   = avLinAvg( pChannel->pfNMR, iNumBandsFFT);
        
        // modulation difference 1
        pfMOVs[3]   = 100.0F * avLinAvg( pChannel->pfModDiff1, iNumBandsFFT);

        // harmonic structure of error already calculated
        pfMOVs[5]   = pChannel->fEHS;
        
        // modulation difference 2
        pfMOVs[7]   = 100.0F * avLinAvg( pChannel->pfModDiff2, iNumBandsFFT);

        // noise loudness
        pfMOVs[8]   = 24.0F * avLinAvg( pChannel->pfNoiseLoud, iNumBandsFFT);
        
        // detection probability (same for both channels)
        pfMOVs[9]   = fDetProb;
        
        // relative disturbed frames already calculated
        pfMOVs[10]  = pChannel->fRDF;
        
        // steps above threshold
        pfMOVs[4]   = iNumBandsFFT * avLinAvg( pChannel->pfStepsAbThr, iNumBandsFFT);
        
        // TempWt already calculated
        pfMOVs[6]   = pChannel->fTempWt;
    }

    return _NO_ERROR;
}

