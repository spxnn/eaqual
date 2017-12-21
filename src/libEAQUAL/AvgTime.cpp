/////////////////////////////////////////////////////////////////////////////////////
// AvgTime.cpp: implementation of the CAvgTime class.
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
#include "AvgTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAvgTime::CAvgTime( eaqstEAQUALInstance *pInstance )
{
    zINT    iNumOfMOVs  = (pInstance->bModelType == _BASIC) ? _NUM_MOV_BASIC +2 : _NUM_MOV_ADVANCED +2;
    pInstance->pfMOVs   = m_pfMOVs  = new zFLOAT[iNumOfMOVs];

    // initialize this array
    zplSetZero(m_pfMOVs, iNumOfMOVs);
}

CAvgTime::~CAvgTime()
{
    if (m_pfMOVs) delete [] m_pfMOVs;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avLinAvg
//  @parameter:     - zFLOAT *pfMOV pointer on vector on which the average is calulated
//                  - zINT  iBegIdx     begin index for averaging   
//                  - zINT  iEndIdx     end index for averaging
//  @result:        - zFLOAT    fMOVAvg     result of averaging
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates linear average according BS.1387, eq. (89), overloaded
//  @creation_date: 13.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
inline zFLOAT   CAvgTime::avLinAvg(zFLOAT *pfMOV, zINT iBegIdx, zINT iEndIdx)
{
    zINT    iIdx;
    zFLOAT  fMOVAvg             = 0;

    for (iIdx = iBegIdx; iIdx <= iEndIdx; iIdx++)
        fMOVAvg        += pfMOV[iIdx];

    return (fMOVAvg/(iEndIdx - iBegIdx + 1));
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avLinAvg
//  @parameter:     - zFLOAT *pfMOV     pointer on vector on which the average is calulated
//                  - zFLOAT    *pfWeight   vector with Weights for temporal averaging of length this->uiFramesProcessed
//                  - zINT  iBegIdx     begin index for averaging   
//                  - zINT  iEndIdx     end index for averaging
//  @result:        - zFLOAT    fMOVAvg     result of averaging
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates linear average according BS.1387, eq. (90), overloaded
//  @creation_date: 13.07.2000
//  @last_modified: 01.11.2001 AL: possible division by zero removed
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgTime::avLinAvg(zFLOAT *pfMOV, zFLOAT *pfWeight, zINT iBegIdx, zINT iEndIdx)
{
    zINT    iIdx;

    zFLOAT  fMOVAvg         = 0,
            fSumWeight      = 0;

    for (iIdx = iBegIdx; iIdx <= iEndIdx; iIdx++)
    {
        fMOVAvg            += pfWeight[iIdx] * pfMOV[iIdx];
        fSumWeight         += pfWeight[iIdx];
    }
    
    if (fSumWeight != 0)
        fMOVAvg            /= fSumWeight;

    return fMOVAvg;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avEHSAvg
//  @parameter:     - zFLOAT        *pfEHS          pointer on vector on which the average is calulated
//                  - zUINT32   *puiThreshEn    which values are not valid for averaging?   
//                  - zINT      iBegIdx         begin index for averaging   
//                  - zINT      iEndIdx         end index for averaging
//                  - zUINT32   uiNumEnFrames   number of Frames to be averaged
//  @result:        - zFLOAT        fEHS            result of averaging
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates linear average of EHS according BS.1387, eq. (89) and chapter 5.2.4.3
//  @creation_date: 18.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgTime::avEHSAvg(zFLOAT *pfEHS, zUINT32 *puiThreshEn, zUINT32 uiBegIdx, zUINT32 uiEndIdx, zUINT32 uiNumEnFrames)
{
    zUINT32     iIdx,
                iNumOfFrames    = 0,
                iEnIdx          = 0;

    zFLOAT      fEHS            = 0.0F;

    while ((puiThreshEn[iEnIdx] < uiBegIdx) && (iEnIdx < uiNumEnFrames))
        iEnIdx++;

    for (iIdx = uiBegIdx; iIdx <= uiEndIdx; iIdx++)
    {
        if (iIdx != puiThreshEn[iEnIdx])
        {
            fEHS       += pfEHS[iIdx];
            iNumOfFrames++;
        }
        else
            if(iEnIdx != uiNumEnFrames - 1)
                iEnIdx++;
    }

    if (iNumOfFrames)
        return (fEHS / iNumOfFrames);
    else
        return 0.0F;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avWinAvg
//  @parameter:     - zFLOAT *pfMOV     pointer on vector on which the average is calulated
//                  - zINT  iBegIdx     begin index for averaging   
//                  - zINT  iEndIdx     end index for averaging
//  @result:        - zFLOAT    fMOVAvg     resulting window- averaged MOV
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates windowed average according BS.1387, eq. (93)
//  @creation_date: 13.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgTime::avWinAvg(zFLOAT *pfMOV, zUINT32 uiBegIdx, zUINT32 uiEndIdx)
{
    zUINT32     uiFrameIdx,
                uiWindowIdx,
                uiNumOfFrames;

    zFLOAT      fMOVAvg     = 0,
                fTmp;

    uiNumOfFrames           = uiEndIdx - uiBegIdx + 1 - _AVG_WIN_LENGTH + 1;
 
    for ( uiFrameIdx = uiBegIdx + _AVG_WIN_LENGTH - 1 ; uiFrameIdx <= uiEndIdx; uiFrameIdx++)
    {
        fTmp                = 0;

        for ( uiWindowIdx = 0; uiWindowIdx < _AVG_WIN_LENGTH; uiWindowIdx++)
            fTmp           += zplSQRT(pfMOV[uiFrameIdx - uiWindowIdx]);

        fTmp               /= _AVG_WIN_LENGTH;
        fTmp                = zplPOW(fTmp, _AVG_WIN_LENGTH);
        
        fMOVAvg            += fTmp;
    }
    fMOVAvg                /= uiNumOfFrames;

    return (zplSQRT(fMOVAvg));
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avNLAvg
//  @parameter:     - zFLOAT *pfMOV     pointer on vector on which the average is calulated
//                  - zINT  iBegIdx     begin index for averaging   
//                  - zINT  iEndIdx     end index for averaging
//                  - zBOOL bModelType  basic or advanced model
//  @result:        - zFLOAT    fMOVAvg     resulting rms- averaged MOV
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates rms of NL according BS.1387, eq. (91) and table (12)
//  @creation_date: 07.08.2000
//  @last_modified: 2002-01-31 AL: removed possible division by zero
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgTime::avNLAvg(zFLOAT *pfMOV, zINT iBegIdx, zINT iEndIdx, zBOOL bModelType)
{
    zINT        iFrameIdx;

    zFLOAT      fMOVAvg     = 0.0F,
                fNL_Min     = 0.0F;

    if (bModelType == _ADVANCED)
        fNL_Min             = 0.1F;     

    for (iFrameIdx = iBegIdx; iFrameIdx <= iEndIdx; iFrameIdx++)
    {
        if (pfMOV[iFrameIdx] >= fNL_Min)
            fMOVAvg        += (pfMOV[iFrameIdx] * pfMOV[iFrameIdx]);
    }

	if (iFrameIdx = iEndIdx - iBegIdx + 1)
	    return zplSQRT(fMOVAvg / (iEndIdx - iBegIdx + 1));
	else
		return 0.0F;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avBWAvg
//  @parameter:     - zFLOAT *pfBWRef       pointer on reference Bandwidth over time vector
//                  - zFLOAT *pfBWTest      pointer on test bandwidth over time vector
//                  - zFLOAT *pBW           pointer on resulting bandwidths
//                  - zINT  iSampleRate     sampling rate of input audio files
//                  - zINT  iBegIdx         begin index for averaging   
//                  - zINT  iEndIdx         end index for averaging
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates linear average of Bandwidth according BS.1387, eq. (89) and chapter 4.4.2
//  @creation_date: 18.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CAvgTime::avBWAvg(zFLOAT *pfBWRef, zFLOAT *pfBWTest, zFLOAT *pBW, zINT iSampleRate, zINT iBegIdx, zINT iEndIdx)
{
    zINT        iIdx,
                iNumOfFrames        = 0,
                iLowBandwidthBound  = 346;

    if (iSampleRate != 48000)
        iLowBandwidthBound      = static_cast<zINT>(iLowBandwidthBound * 48000.0F / iSampleRate + .5);

    pBW[0]          = 0.0F; // Ref
    pBW[1]          = 0.0F; // Test

    for (iIdx = iBegIdx; iIdx <= iEndIdx; iIdx++)
    {
        if (pfBWRef[iIdx] > iLowBandwidthBound)
        {
            pBW[0]    += pfBWRef[iIdx];
            pBW[1]    += pfBWTest[iIdx];
            
            iNumOfFrames++;
        }
    }

    if ( iNumOfFrames )
    {
        pBW[0]     /= iNumOfFrames;
        pBW[1]     /= iNumOfFrames;
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avBWAvg
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates linear average of Bandwidth according BS.1387, eq. (89) and chapter 4.4.2
//  @creation_date: 09.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CAvgTime::avChannelAvg(eaqstEAQUALInstance *pInstance)
{
    zINT    iCh,
            iIdx,
            iNumOfMOVs = (pInstance->bModelType == _BASIC) ? _NUM_MOV_BASIC : _NUM_MOV_ADVANCED,
            iNumOfChannels  = pInstance->WaveFormat.iNumOfChannel;

    zFLOAT  *pfMOV  = &pInstance->pfMOVs[2];

    // initialize MOV result array
    zplSetZero (pfMOV, iNumOfMOVs);

    for (iIdx = 0; iIdx < iNumOfMOVs; iIdx++)
    {
        for (iCh = 0; iCh < iNumOfChannels; iCh++)
            pfMOV[iIdx]    += pInstance->ChannelData[iCh].pfChannelMOVs[iIdx];

        pfMOV[iIdx]        /= iNumOfChannels;
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avADB
//  @parameter:     - zINT      iNumDistorted   number of distorted blocks
//                  - zFLOAT        fStepsAbThr     number of steps above threshold 
//  @result:        - zFLOAT                        resulting ADB
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable ADB according to BS.1387, p.235
//  @creation_date: 18.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgTime::avADB(zINT iNumDistorted, zFLOAT fStepsAbThr)
{
    if ( iNumDistorted > 0)
        return ((fStepsAbThr > 0) ? (zplLOG(fStepsAbThr / iNumDistorted) * _INVLN10) : -0.5F);
    
    else
        return 0.0F;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        avMFPD
//  @parameter:     - zFLOAT *pfMOV                 pointer on detection probability
//                  - zINT  iBegIdx                 begin index for averaging   
//                  - zINT  iEndIdx                 end index for averaging
//                  - zBOOL bForgetFactorEnabled    is the forgetting factor (c_1, eq. (85)) enabled?
//  @result:        - zFLOAT    fPDet                   MFPD
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable MFPD according to BS.1387, pp.234+235
//  @creation_date: 20.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CAvgTime::avMFPD(zFLOAT *pfMOV, zINT iBegIdx, zINT iEndIdx, zBOOL bForgetFactorEnabled)
{
    zINT    iIdx;
    
    zFLOAT  fMOV,
            fPDet   = 0;

    if (bForgetFactorEnabled == _FALSE)
    {
        for (iIdx = iBegIdx; iIdx < iEndIdx; iIdx++)
        {
            fMOV    = pfMOV[iIdx];
            fMOV    = .1F * fMOV + .9F * fPDet;
            if (fMOV > fPDet)
                fPDet   = fMOV;
        }
    }
    else
    {
        for (iIdx = iBegIdx; iIdx <= iEndIdx; iIdx++)
        {
            fMOV    = pfMOV[iIdx];
            fMOV    = .1F * fMOV + .9F * fPDet;
            if (.99F * fMOV > fPDet)
                fPDet   = .99F * fMOV;
        }   
    }

    return fPDet;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        Process
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates the MOVs averaged over time and channels 
//  @creation_date: 15.02.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CAvgTime::Process(  eaqstEAQUALInstance *pInstance)
{
    zINT    iCh;
    zUINT32 uiBeginIdx,
            uiEndIdx;
    zFLOAT  fADB,
            fMFPD;

    fADB    = avADB(    pInstance->uiBlocksDistorted, 
                        pInstance->fSumStepsAbThr);
    fMFPD   = avMFPD(   pInstance->ChannelData[0].pfTimeMOVs[9], 
                        pInstance->uiThreshBoundBeg, 
                        pInstance->uiThreshBoundEnd, 
                        pInstance->GlobalOptions.bForgetFactorEnabled);
    
    for (iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
    {
        stChannelData   *pChannelData   = &pInstance->ChannelData[iCh];
        zFLOAT          *pfMOVs     = pChannelData->pfChannelMOVs;
        
        // calculate MOVs per channel
        uiBeginIdx                  = (pInstance->uiThreshBoundBeg > pInstance->uiThreshDel) ? pInstance->uiThreshBoundBeg : pInstance->uiThreshDel;
        uiEndIdx                    = pInstance->uiThreshBoundEnd;

        // modulation difference 1
        pfMOVs[6]                   = avLinAvg( pChannelData->pfTimeMOVs[3], 
                                                pChannelData->pfTimeMOVs[6], 
                                                uiBeginIdx, 
                                                uiEndIdx);
        // modulation difference 2
        pfMOVs[7]                   = avLinAvg( pChannelData->pfTimeMOVs[7], 
                                                pChannelData->pfTimeMOVs[6], 
                                                uiBeginIdx, 
                                                uiEndIdx);
        // windowed modulation difference 1
        pfMOVs[3]                   = avWinAvg( pChannelData->pfTimeMOVs[3], 
                                                uiBeginIdx, 
                                                uiEndIdx);

        if (uiBeginIdx < pInstance->uiThreshLoud)
            uiBeginIdx              = pInstance->uiThreshLoud;

        // noise loudness
        pfMOVs[8]                   = avNLAvg(  pChannelData->pfTimeMOVs[8], 
                                                uiBeginIdx, 
                                                uiEndIdx, 
                                                pInstance->bModelType);

        uiBeginIdx                  = pInstance->uiThreshBoundBeg;
        uiEndIdx                    = pInstance->uiThreshBoundEnd;

        // noise-to- mask- ratio
        pfMOVs[2]                   = avLinAvg( pChannelData->pfTimeMOVs[2], 
                                                uiBeginIdx, 
                                                uiEndIdx);
        pfMOVs[2]                   = _10INVLN10 * zplLOG(pfMOVs[2]);
        
        // relative distorted frames
        pfMOVs[10]                  = avLinAvg( pChannelData->pfTimeMOVs[10], 
                                                uiBeginIdx, 
                                                uiEndIdx);
        // bandwidth
                                      avBWAvg(  pChannelData->pfTimeMOVs[0] , 
                                                pChannelData->pfTimeMOVs[1], 
                                                pfMOVs,
                                                pInstance->WaveFormat.iSamplingRate, 
                                                uiBeginIdx, 
                                                uiEndIdx);

        // average distorted block
        pfMOVs[4]                   = fADB;                                   

        // maximum filtered probability of detection
        pfMOVs[9]                   = fMFPD;                                      

        // harmonic structure of error
        pfMOVs[5]                   = avEHSAvg( pChannelData->pfTimeMOVs[5], 
                                                pInstance->puiThreshEn,
                                                uiBeginIdx, 
                                                uiEndIdx,
                                                pInstance->uiNumEnFrames);
    }

    // averaging over channels
    avChannelAvg(pInstance);
    return _NO_ERROR;
}
