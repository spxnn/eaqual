/////////////////////////////////////////////////////////////////////////////////////
// MOVBlock.cpp: implementation of the CMOVBlock class.
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

#include "MOVBlock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _EHS_MAX_LAG    256

CMOVBlock::CMOVBlock(eaqstEAQUALInstance *pInstance)
{
    stChannelData   *pChannel;
    zINT            iCh,
                    iNumOfBands = ( pInstance->bModelType == _BASIC ) ? _NUM_BANDS_BASIC : _NUM_BANDS_FB;

    for ( iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++) 
    {
        pChannel    = &(pInstance->ChannelData[iCh]);
        
        pChannel->pfModDiff1    = m_pfModDiff1[iCh]     = new zFLOAT[iNumOfBands];
        pChannel->pfNoiseLoud   = m_pfNoiseLoud[iCh]    = new zFLOAT[iNumOfBands];
        pChannel->pfNMR         = m_pfNMR[iCh]          = new zFLOAT[iNumOfBands];
        pChannel->pfChannelMOVs = m_afMOVs[iCh];
    }
    for ( iCh = pInstance->WaveFormat.iNumOfChannel; iCh < _NUM_CHANNELS_MAX; iCh++) 
    {
        m_pfModDiff1[iCh]   = 0;
        m_pfNoiseLoud[iCh]  = 0;
        m_pfNMR[iCh]        = 0;
    }
    
    if (pInstance->bModelType == _BASIC)
    {
        for ( iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++) 
        {
            pChannel    = &(pInstance->ChannelData[iCh]);
            
            pChannel->pfModDiff2    = m_pfModDiff2[iCh]     = new zFLOAT[iNumOfBands];
            pChannel->pfProb        = m_pfProb[iCh]         = new zFLOAT[iNumOfBands];
            pChannel->pfStepsAbThr  = m_pfStepsAbThr[iCh]   = new zFLOAT[iNumOfBands];
        }
        for ( iCh = pInstance->WaveFormat.iNumOfChannel; iCh < _NUM_CHANNELS_MAX; iCh++) 
        {
            m_pfModDiff2[iCh]       = 0;
            m_pfProb[iCh]           = 0;
            m_pfStepsAbThr[iCh]     = 0;
        }
    }
    else
    {
        for ( iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++) 
        {
            m_pfModDiff2[iCh]   = 0;
            m_pfProb[iCh]       = 0;
            m_pfStepsAbThr[iCh] = 0;
        }
    }
    
}

CMOVBlock::~CMOVBlock()
{
    for (zINT iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++)
    {
        if ( m_pfModDiff1[iCh]      != 0) delete [] m_pfModDiff1[iCh];
        if ( m_pfModDiff2[iCh]      != 0) delete [] m_pfModDiff2[iCh];
        if ( m_pfNoiseLoud[iCh]     != 0) delete [] m_pfNoiseLoud[iCh];
        if ( m_pfNMR[iCh]           != 0) delete [] m_pfNMR[iCh];
        if ( m_pfProb[iCh]          != 0) delete [] m_pfProb[iCh];
        if ( m_pfStepsAbThr[iCh]    != 0) delete [] m_pfStepsAbThr[iCh];
    }       
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        mobModDiff
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zFLOAT            *pfIntNoise     pointer on table with intern noise
//                  - zBOOL         bModelType      model type (Basic or Advanced)
//                  - zINT          iType           1 or 2 (ModDiff1 or ModDiff2), only needed for basic version
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable ModDiff1/2 according to BS.1387, eq. (64)
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CMOVBlock::mobModDiff(stChannelData *pChannelData, zFLOAT *pfIntNoise, zBOOL bModelType, zINT iType)
{
    zFLOAT      fNegWt  = 1.0F,
                fOffset = 1.0F,
                fLevWt  = 100.0F,
                fw;

    zFLOAT      *pfModDiff,
                *pfRefMod       = pChannelData->pfModRef,
                *pfTestMod      = pChannelData->pfModTest,
                *pfEnLoudRef    = pChannelData->pfPPEnLoudRef,
                *pfTempWt       = &(pChannelData->fTempWt);
    
    zINT        iIdx,
                iNumOfBands;

    if (bModelType == _BASIC)
    {
        switch (iType)
        {
        case 1:
            {
                pfModDiff   = pChannelData->pfModDiff1;
                iNumOfBands = _NUM_BANDS_BASIC;
                break;
            }
        case 2:
            {
                pfModDiff   = pChannelData->pfModDiff2;
                fNegWt      = 0.1F;
                fOffset     = 0.01F;
                iNumOfBands = _NUM_BANDS_BASIC;
                break;
            }
        default:
            {
                return _INVALID_FUNCTION_ARGS;
            }
        }
    }
    else // bModelType _ADVANCED
    {
        pfModDiff       = pChannelData->pfModDiff1;
        fLevWt          = 1.0F;
        iNumOfBands     = _NUM_BANDS_ADVANCED;
    }

    *pfTempWt           = 0.0F;

    for ( iIdx = 0; iIdx < iNumOfBands; iIdx++ )
    {
        fw              = ( pfRefMod[iIdx] > pfTestMod[iIdx] ) ? fNegWt : 1.0F;

        pfModDiff[iIdx] = fw * zplFABS(pfTestMod[iIdx] - pfRefMod[iIdx]) / ( fOffset + pfRefMod[iIdx]);

        //eq . (65)
        *pfTempWt      += pfEnLoudRef[iIdx] / (pfEnLoudRef[iIdx] + fLevWt * zplPOW(pfIntNoise[iIdx], .3F) );

    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        mobNoiseLoud
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zFLOAT            *pfIntNoise     pointer on table with intern noise
//                  - char          *cMOV           type of variable to calculate
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable NoiseLoudness according to BS.1387, eq. (66)
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CMOVBlock::mobNoiseLoud(stChannelData *pChannelData, zFLOAT *pfIntNoise, char *cMOV)
{   
    zINT        iIdx,
                iNumOfBands = _NUM_BANDS_BASIC;

    zFLOAT      fTemp,
                fsRef,
                fsTest,
                fBeta,
                fAlpha      = 1.5F,
                fThreshFac  = 0.15F,
                fS0         = 1.0F,
                *pfNL       = pChannelData->pfNoiseLoud,
                *pfRefMod   = pChannelData->pfModRef,
                *pfTestMod  = pChannelData->pfModTest,
                *pfRefSpec  = pChannelData->pfPattSpreadRef,
                *pfTestSpec = pChannelData->pfPattSpreadTest;

    switch (*cMOV)
    {
    //noise loudness B
    case 'B':
        {
            fS0             = 0.5F;
            break;
        }

    //noise loudness A
    case 'A':
        {
            iNumOfBands     = _NUM_BANDS_ADVANCED;
            fAlpha          = 2.5F;
            fThreshFac      = 0.3F;
            break;
        }

    //missing components
    case 'M':
        {
            iNumOfBands     = _NUM_BANDS_ADVANCED;
            break;
        }

    //lin dist
    case 'L':
        {
            iNumOfBands     = _NUM_BANDS_ADVANCED;
            break;
        }
    default:
        {
            return _INVALID_FUNCTION_ARGS;
        }
    }

    for ( iIdx = 0; iIdx < iNumOfBands; iIdx++)
    {
        fBeta           = zplEXP(fAlpha * (pfRefSpec[iIdx] - pfTestSpec[iIdx])/pfRefSpec[iIdx]);
        fsRef           = fThreshFac *  pfRefMod[iIdx] + fS0;
        fsTest          = fThreshFac *  pfTestMod[iIdx] + fS0;

        fTemp           = (fsTest * pfTestSpec[iIdx]) - (fsRef * pfRefSpec[iIdx]);
        fTemp           = (fTemp > 0) ? ( fTemp / (pfIntNoise[iIdx] + ( fsRef * pfRefSpec[iIdx] * fBeta))) : 0.0F;
        fTemp           = zplPOW(1.0F + fTemp, 0.23F) - 1.0F;

        pfNL[iIdx]      = zplPOW( pfIntNoise[iIdx] / fsTest, 0.23F);
        pfNL[iIdx]     *= fTemp;
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        mobNMR
//  @parameter:     - zFLOAT            *pfNMR          resulting NMR- spectrum
//                  - zFLOAT            *pfNoiseSpec    pointer on noise spectrum
//                  - zFLOAT            *pfMaskSpec     pointer on masking spectrum
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable NMR according to BS.1387, eq.(70)
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
inline void CMOVBlock::mobNMR(zFLOAT    *pfNMR, zFLOAT  *pfNoiseSpec, zFLOAT *pfMaskSpec, zINT iNumOfBands)
{   
    zplDivVec (pfNoiseSpec, pfMaskSpec, pfNMR, iNumOfBands);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        mobDetProb
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable Detection Probalbility according to BS.1387, pp.233 + 234
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CMOVBlock::mobDetProb(stChannelData *pChannelData)
{   
    zFLOAT      fAsymExcitation,
                fAsymPowTmp,
                fs,
                fa,
                fb,
                fError,
                *pfRefSpec      = pChannelData->pfdBSpreadRef,
                *pfTestSpec     = pChannelData->pfdBSpreadTest,
                *pfDetProb      = pChannelData->pfProb,
                *pfStepsAbThr   = pChannelData->pfStepsAbThr;

    zINT        iIdx;

//  fLog2                   = _INVLN10 / _INVLN2;


    for ( iIdx = 0; iIdx < _NUM_BANDS_BASIC; iIdx++)
    {
        // eq. (75)
        fError                  = pfRefSpec[iIdx] - pfTestSpec[iIdx];

        if (fError > 0)
        {
            fAsymExcitation     = 0.3F * pfRefSpec[iIdx]; 
            fAsymExcitation    += 0.7F * pfTestSpec[iIdx];
            fb                  = 4.0F;
            fa                  = 7.40717223331057E-1F;                 // = 10^(log10(log10(2))/b)
        }
        else
        {
            fAsymExcitation     = pfTestSpec[iIdx]; 
            fb                  = 6.0F;
            fa                  = 8.18656338497233E-1F;                 // = 10^(log10(log10(2))/b)
        }

        // eq. (74)
        if ( fAsymExcitation > 0)
        {
            fs                  = 5.95072F * zplPOW(6.39468F/fAsymExcitation, 1.71332F);
            fs                 -= .198719F;

            fAsymPowTmp         = fAsymExcitation;
            fs                 += 5.50197E-02F * fAsymPowTmp;       // ^1

            fAsymPowTmp        *= fAsymExcitation;
            fs                 -= 1.02438E-03F * fAsymPowTmp;       // ^2

            fAsymPowTmp        *= fAsymExcitation;
            fs                 += 5.05622E-06F * fAsymPowTmp;       // ^3

            fAsymPowTmp        *= fAsymExcitation;
            fs                 += 9.01033E-11F * fAsymPowTmp;       // ^4
        }
        else
            fs                  = 1E+30F;
        fa                     /= fs;
        
        // eq. (77)
        fa                      = zplPOW(fa * fError, fb);

        pfDetProb[iIdx]         = 1.0F - zplPOW(0.1F, fa); 

        // eq. (78)
        pfStepsAbThr[iIdx]      = zplIABS( static_cast<zINT>(fError)) / fs; // verified by Thilo Thiede, mail 15.10.00

    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        mobBandwidth
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zINT          iSampleRate     SampleRate of Input audio signals
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable Bandwidth according to BS.1387, p.232
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CMOVBlock::mobBandwidth(stChannelData *pChannelData, zINT iSampleRate)
{   
    zINT        iFreqIdx,
        iCutIdx;
    zFLOAT      fZeroThresh,
                *pfBWRef    = &pChannelData->fBWRef,
                *pfBWTest   = &pChannelData->fBWTest,
                *pfRefSpec  = pChannelData->pfdBSpecRef,
                *pfTestSpec = pChannelData->pfdBSpecTest;
    
    // initialize
    *pfBWRef    = 0.0F;
    *pfBWTest   = 0.0F;
    
    // Index is sample rate dependent; see p.232: k=921
    if (iSampleRate == 48000)
        iCutIdx     = 920;
    else
    {
        if (iSampleRate >= 44100)
            iCutIdx = static_cast<zINT> (21562.5F * (_FFT_LENGTH) / iSampleRate);   // only valid for sample rates above 44100!
        else
            return _INVALID_SAMPLE_FREQ;
    }
    fZeroThresh     = pfTestSpec[iCutIdx + 1];
    
    for ( iFreqIdx = iCutIdx + 1; iFreqIdx < _HALF_FFT_LENGTH; iFreqIdx++)
    {
        if (fZeroThresh < pfTestSpec[iFreqIdx])
            fZeroThresh = pfTestSpec[iFreqIdx];
    }
    
    // find bandwidth ref
    for ( iFreqIdx = iCutIdx; iFreqIdx >= 0; iFreqIdx-- )
    {
        if (pfRefSpec[iFreqIdx] >= 10.0F + fZeroThresh)
        {
            *pfBWRef        = iFreqIdx + 1.0F;
            break;
        }
    }
    
    // find bandwidth test (< bandwidth ref)
    for (iFreqIdx = iFreqIdx; iFreqIdx >= 0; iFreqIdx--)
    {
        if ( pfTestSpec[iFreqIdx] >= 5.0F + fZeroThresh)
        {
            *pfBWTest   = iFreqIdx + 1.0F;
            break;
        }
    }
    
    // modification for other samplerates than 48000
    if (iSampleRate != 48000)
    {
        *pfBWRef       *= iSampleRate / 48000.0F;
        *pfBWTest      *= iSampleRate / 48000.0F;
    }
    
    return _NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        mobRDF
//  @parameter:     - zFLOAT            *pfNMRSpec      pointer on NMR spectrum
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        - zFLOAT            frame disturbed (yes = 1 / no = 0)
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable RDF according to BS.1387, p.233
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
inline zFLOAT   CMOVBlock::mobRDF(zFLOAT *pfNMRSpec, zINT iNumOfBands)
{   
    for (; iNumOfBands > 0; iNumOfBands--)
    {
        if ( *pfNMRSpec++ >= 1.412537544622754F )           // >=zplPOW(10, 0.15F)
            return  1;                                      // frame is disturbed
    }
    return 0;                                               // frame is not disturbed
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        mobEHS
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zFLOAT            *pfHannWin      pointer on half hanning window of length _FFT_LENGTH
//                  - zINT          iSampleRate     SampleRate of Input audio signals
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Model Output Variable EHS according to BS.1387, p.236
//  @creation_date: 03.07.2000
//  @last_modified: 28.07.2000  AL: some changes in calculation acc. William Treuniet (CRC)
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CMOVBlock::mobEHS(stChannelData *pChannelData, zFLOAT *pfHannWin, zINT iSampleRate)
{
    zUINT32     k,
                n,
                uiMaxLag;

    zINT        iStepSize;
    zFLOAT      fNorm1,
                fNorm2,
                fEnergy,
                fMean   = 0.0F,
                afError[_EHS_MAX_LAG<<1],
                afACF[_EHS_MAX_LAG],
                *pfEHS  = &pChannelData->fEHS;

    // initialize
    *pfEHS      = 0.0F;
    
    if ((iSampleRate == 44100) || (iSampleRate == 48000))
        uiMaxLag    = _EHS_MAX_LAG;
    else
    {
        // not tested until now... (AL)
        uiMaxLag    = static_cast<zUINT32>(9000 * _FFT_LENGTH / iSampleRate);
        k           = 0xFFFFFFFE;
        while (uiMaxLag & k)
        {
            uiMaxLag   &= k;
            k         <<= 1;
        }
    }

    // calculate error spectrum
    zplDiffVec (pChannelData->pfdBSpecWRef, pChannelData->pfdBSpecWTest, afError, uiMaxLag<<1);

    // calculate energy for ACF
    fNorm1          = 0.0F;
    for (k = 0; k < uiMaxLag; k++)
        fNorm1     += afError[k] * afError[k];
    fNorm2          = fNorm1;
    
    // calculate autocorrelation
    for (k = 1; k <= uiMaxLag; k++)
    {
        afACF[k-1]      = 0.0F;
        
        for (n = 0; n < uiMaxLag; n++)
            afACF[k-1] += afError[n] * afError[n + k];
        
        fNorm2         -= afError[k-1] * afError[k-1];
        fNorm2         += afError[uiMaxLag + k-1] * afError[uiMaxLag + k-1];
        
        fEnergy         = fNorm1 * fNorm2;
        if ( fEnergy != 0.0F)
        {
            fEnergy         = zplSQRT(fEnergy);
            afACF[k-1]     /= fEnergy;
        }
        else
            afACF[k-1]      = 0.0F;
        
        fMean          += afACF[k-1];
    }
    fMean              /= uiMaxLag;
    
    iStepSize           = _FFT_LENGTH / uiMaxLag;
    // subtract average value and window for FFT
    for( k = 0; k < uiMaxLag>>1; k++, pfHannWin += iStepSize )
    {
        afACF[k]       -= fMean;
        afACF[k]       *= *pfHannWin * iStepSize;
    }
    pfHannWin -= 1;
    for( k = uiMaxLag>>1; k < uiMaxLag; k++, pfHannWin -= iStepSize )
    {
        afACF[k]       -= fMean;
        afACF[k]       *= *pfHannWin * iStepSize;
    }
    
    // this code should replace the line of code below
    // find log dualis of usMaxLag (note: usMaxLag must be power of two
    // hack; check for errors!!
    for (k = 0; k < 16; k++)
    {
        if ( (uiMaxLag >> k) & 0x0001)  
            break;
    }
    // calculate FFT
    zplRealFFT ( afACF, k);
    
    // calculate power spectrum and find max
    //  pfACFw[0]          *= pfACFw[0];    //index zero not needed
    
    for (k = 1; k < (uiMaxLag>>1); k++)
    {
        afACF[k]               *= afACF[k];                 //real part
        afACF[uiMaxLag - k]    *= afACF[uiMaxLag - k];      //imag part
        afACF[k]               += afACF[uiMaxLag - k];      //power spectrum
        
        if (afACF[k - 1] < afACF[k])
            if (*pfEHS < afACF[k])
                *pfEHS = afACF[k];
    }

    // scale EHS
    *pfEHS     *= 1000;

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        Process
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance of EAQUAL
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:             
//  @creation_date: 09.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CMOVBlock::Process(eaqstEAQUALInstance *pInstance)
{
    zINT            iCh,
                    iNumOfBands;
    zINT8           *cNLType;
    zFLOAT          *pfIntNoise;
    stChannelData   *pChannelData;

    if (pInstance->bModelType == _BASIC)
    {
        pfIntNoise  = pInstance->Tables.stTablesFFT.pfIntNoise;
        iNumOfBands = _NUM_BANDS_BASIC;
        cNLType     = "B";
    }
    else
    {
        pfIntNoise  = pInstance->Tables.stTablesFB.pfIntNoise;
        iNumOfBands = _NUM_BANDS_ADVANCED;
        cNLType     = "A";
    }

    for (iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
    {
        pChannelData        = &pInstance->ChannelData[iCh];

        // modulation difference
        mobModDiff (    pChannelData, 
                        pfIntNoise, 
                        pInstance->bModelType, 
                        1);

        // noise loudness 
        mobNoiseLoud (  pChannelData, 
                        pfIntNoise, 
                        cNLType);

        // noise to mask ratio
        mobNMR (        pChannelData->pfNMR, 
                        pChannelData->pfBarkNoise, 
                        pChannelData->pfMaskRef, 
                        iNumOfBands);
    }

    if (pInstance->bModelType == _BASIC)
    {
        for (iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
        {
            pChannelData    = &pInstance->ChannelData[iCh];

            // modulation difference
            mobModDiff (    pChannelData, 
                            pfIntNoise, 
                            pInstance->bModelType, 
                            2);

            // Detection Probability and steps_above_threshold
            mobDetProb (    pChannelData);

            // Bandwidth
            mobBandwidth (  pChannelData, 
                            pInstance->WaveFormat.iSamplingRate);

            // RDF
            pChannelData->fRDF = mobRDF (       pChannelData->pfNMR, 
                                                iNumOfBands);

            // EHS
            mobEHS (        pChannelData, 
                            pInstance->Tables.stTablesFFT.pfHannWin, 
                            pInstance->WaveFormat.iSamplingRate);
        }
    }

    return _NO_ERROR;
}