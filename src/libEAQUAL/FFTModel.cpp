/////////////////////////////////////////////////////////////////////////////////////
// FFTModel.cpp: implementation of the CFFTModel class.
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

#include "FFTModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFFTModel::CFFTModel(eaqstEAQUALInstance *pInst):CPsychoModel(pInst)
{

    zINT    iCh,
            iNumOfBands = ( pInst->bModelType == _BASIC ) ? _NUM_BANDS_BASIC : _NUM_BANDS_ADVANCED;

    for (iCh = 0; iCh < pInst->WaveFormat.iNumOfChannel; iCh++)
    {
        stChannelData   *pChannel   = &(pInst->ChannelData[iCh]);
        
        pChannel->pfFFTRef          = m_pfRefFFT[iCh]       = new zFLOAT[_FFT_LENGTH];
        pChannel->pfFFTTest         = m_pfTestFFT[iCh]      = new zFLOAT[_FFT_LENGTH];
        pChannel->pfdBSpecRef       = m_pfRefdBSpec[iCh]    = new zFLOAT[_FFT_LENGTH];
        pChannel->pfdBSpecTest      = m_pfTestdBSpec[iCh]   = new zFLOAT[_FFT_LENGTH];
        pChannel->pfdBSpecWRef      = m_pfRefdBSpecW[iCh]   = new zFLOAT[_FFT_LENGTH];
        pChannel->pfdBSpecWTest     = m_pfTestdBSpecW[iCh]  = new zFLOAT[_FFT_LENGTH];
        pChannel->pfSpecNoise       = m_pfNoiseSpec[iCh]    = new zFLOAT[_FFT_LENGTH];
        pChannel->pfBarkRef         = m_pfRefBark[iCh]      = new zFLOAT[iNumOfBands];
        pChannel->pfBarkTest        = m_pfTestBark[iCh]     = new zFLOAT[iNumOfBands];
        pChannel->pfBarkNoise       = m_pfNoiseBark[iCh]    = new zFLOAT[iNumOfBands];
        pChannel->pfFreqSpreadRef   = m_pfRefFreqSpread[iCh]= new zFLOAT[iNumOfBands];
        pChannel->pfFreqSpreadTest  = m_pfTestFreqSpread[iCh]= new zFLOAT[iNumOfBands];
        pChannel->pfSpreadRef       = m_pfRefSpread[iCh]    = new zFLOAT[iNumOfBands];
        pChannel->pfSpreadTest      = m_pfTestSpread[iCh]   = new zFLOAT[iNumOfBands];
        pChannel->pfdBSpreadRef     = m_pfRefdBSpread[iCh]  = new zFLOAT[iNumOfBands];
        pChannel->pfdBSpreadTest    = m_pfTestdBSpread[iCh] = new zFLOAT[iNumOfBands];
        pChannel->pfMaskRef         = m_pfRefMask[iCh]      = new zFLOAT[iNumOfBands];
    }
    for (iCh = pInst->WaveFormat.iNumOfChannel; iCh < _NUM_CHANNELS_MAX; iCh++)
    {
        m_pfRefFFT[iCh]         = 0;
        m_pfTestFFT[iCh]        = 0;
        m_pfRefdBSpec[iCh]      = 0;
        m_pfTestdBSpec[iCh]     = 0;
        m_pfRefdBSpecW[iCh]     = 0;
        m_pfTestdBSpecW[iCh]    = 0;
        m_pfNoiseSpec[iCh]      = 0;
        m_pfRefBark[iCh]        = 0;
        m_pfTestBark[iCh]       = 0;
        m_pfNoiseBark[iCh]      = 0;
        m_pfRefFreqSpread[iCh]  = 0;
        m_pfTestFreqSpread[iCh] = 0;
        m_pfRefSpread[iCh]      = 0;
        m_pfTestSpread[iCh]     = 0;
        m_pfRefdBSpread[iCh]    = 0;
        m_pfTestdBSpread[iCh]   = 0;
        m_pfRefMask[iCh]        = 0;
    }
}

CFFTModel::~CFFTModel()
{
    for (zINT iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++)
    {
        if ( m_pfRefFFT[iCh]        != 0) delete [] m_pfRefFFT[iCh];
        if ( m_pfTestFFT[iCh]       != 0) delete [] m_pfTestFFT[iCh];
        if ( m_pfRefdBSpec[iCh]     != 0) delete [] m_pfRefdBSpec[iCh];
        if ( m_pfTestdBSpec[iCh]    != 0) delete [] m_pfTestdBSpec[iCh];
        if ( m_pfRefdBSpecW[iCh]    != 0) delete [] m_pfRefdBSpecW[iCh];
        if ( m_pfTestdBSpecW[iCh]   != 0) delete [] m_pfTestdBSpecW[iCh];
        if ( m_pfNoiseSpec[iCh]     != 0) delete [] m_pfNoiseSpec[iCh];
        if ( m_pfRefBark[iCh]       != 0) delete [] m_pfRefBark[iCh];
        if ( m_pfTestBark[iCh]      != 0) delete [] m_pfTestBark[iCh];
        if ( m_pfNoiseBark[iCh]     != 0) delete [] m_pfNoiseBark[iCh];
        if ( m_pfRefFreqSpread[iCh] != 0) delete [] m_pfRefFreqSpread[iCh];
        if ( m_pfTestFreqSpread[iCh]!= 0) delete [] m_pfTestFreqSpread[iCh];
        if ( m_pfRefdBSpread[iCh]   != 0) delete [] m_pfRefdBSpread[iCh];
        if ( m_pfTestdBSpread[iCh]  != 0) delete [] m_pfTestdBSpread[iCh];
        if ( m_pfRefSpread[iCh]     != 0) delete [] m_pfRefSpread[iCh];
        if ( m_pfTestSpread[iCh]    != 0) delete [] m_pfTestSpread[iCh];
        if ( m_pfRefMask[iCh]       != 0) delete [] m_pfRefMask[iCh];
    }
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmFFT
//  @parameter:     - zFLOAT     *pfFFTData time data to be transformed
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         computes inline FFT of pfFFTData
//  @creation_date: 22.03.2000
//  @last_modified: 09.07.2000
//                  13.11.2001  AL: changed mult with hann window
/////////////////////////////////////////////////////////////////////////////////////
inline void CFFTModel::fmFFT(zFLOAT *pfFFTData)
{
    zUINT32 uiFreq,
            uiBitDepth  = 0;
    zFLOAT  *pfHannWin  = m_pTables->stTablesFFT.pfHannWin;

    // window the input data with hann window
    for (uiFreq = 0; uiFreq < _HALF_FFT_LENGTH; uiFreq++)
        pfFFTData[uiFreq]   *= *pfHannWin++;
    for (uiFreq = _HALF_FFT_LENGTH; uiFreq < _FFT_LENGTH; uiFreq++)
        pfFFTData[uiFreq]   *= *(-- pfHannWin);

    // calc bit depth of FFT
    // find log dualis of _FFT_LENGTH (note: _FFT_LENGTH must be power of two
    // hack; check for errors!!
    uiFreq          = _FFT_LENGTH;
    while (uiFreq)
    {
        uiFreq    >>= 1;
        uiBitDepth++;
        if ( uiFreq & 0x00000001 )  
            break;
    }

    // calc the FFT
    zplRealFFT(pfFFTData, uiBitDepth);

    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmScale
//  @parameter:     - zFLOAT    *pfAudioData    audio data with length of FFT
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         scales data with factor calculated according BS1387, p.203 (eq. (5))
//  @creation_date: 09.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
inline void CFFTModel::fmScale(zFLOAT   *pfAudioData)
{
    zplMultVecConst(    pfAudioData, 
                        m_pTables->stTablesFFT.fScaleFac, 
                        _FFT_LENGTH);
    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmCalcPower
//  @parameter:     - zFLOAT    *pfSpecData     pointer on spectral data with length of FFT
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates power spectrum of FFT values (based on Sorensen FFT, file fft.cpp)
//  @creation_date: 30.11.2000
//  @last_modified: 13.11.2001  AL: made the thing more readable
/////////////////////////////////////////////////////////////////////////////////////
inline void CFFTModel::fmCalcPower(zFLOAT   *pfSpecData)
{
    zINT        iUp,
                iDown;

    // first we have 1024 real values, the rest is imag; there is no imag for freq=0
    pfSpecData[0]   = pfSpecData[0] * pfSpecData[0];

    for (iUp = 1, iDown = _FFT_LENGTH-1; iUp < _HALF_FFT_LENGTH; iUp++, iDown--)
    {
        pfSpecData[iUp]   *= pfSpecData[iUp]; 
        pfSpecData[iUp]   += (pfSpecData[iDown] * pfSpecData[iDown]);
    }

    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmLogSpec
//  @parameter:     - zFLOAT    *pfSpecData     pointer on spectral data with length iLength
//                  - zFLOAT    *pfdBData       pointer on resulting buffer for log spectrum
//                  - zINT      iLength         length of vectors
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates dB spectrum of FFT values
//  @creation_date: 09.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
inline void CFFTModel::fmLogSpec(zFLOAT *pfEnData, zFLOAT *pfdBData, zINT iLength)
{
    for (iLength -=1; iLength >=0; iLength--)
    {
        if (pfEnData[iLength])
            pfdBData[iLength]           = _10INVLN10 * zplLOG(pfEnData[iLength]);
        else
            pfdBData[iLength]           = -379.29779453596F;            // = 10 * zplLog(_FLT_MIN) * _INVLN10;
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmCalcNoise
//  @parameter:     - zFLOAT    *pfRefSpec      energy spectrum of reference
//                  - zFLOAT    *pfTestSpec     energy spectrum of test
//                  - zFLOAT    *pfNoiseSpec    resulting noise energy spectrum
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates noise from energy spectra according BS.1387, p.227 (eq. (62))
//  @creation_date: 09.07.2000
//  @last_modified: 13.11.2001  AL: use of binomial formula
/////////////////////////////////////////////////////////////////////////////////////
inline void CFFTModel::fmCalcNoise(zFLOAT *pfRefSpec, zFLOAT *pfTestSpec, zFLOAT *pfNoiseSpec)
{
    zINT    iFreqIdx;

    for (iFreqIdx = 0; iFreqIdx < _HALF_FFT_LENGTH; iFreqIdx++)
    {
//      pfNoiseSpec[iFreqIdx]       = zplSQRT(pfRefSpec[iFreqIdx]);
//      pfNoiseSpec[iFreqIdx]      -= zplSQRT(pfTestSpec[iFreqIdx]);
//      pfNoiseSpec[iFreqIdx]      *= pfNoiseSpec[iFreqIdx];

        pfNoiseSpec[iFreqIdx]   = pfRefSpec[iFreqIdx] + pfTestSpec[iFreqIdx];
        pfNoiseSpec[iFreqIdx]  -= 2.0F * zplSQRT(pfRefSpec[iFreqIdx] * pfTestSpec[iFreqIdx]);
    }
    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmGroupInBands
//  @parameter:     - zFLOAT    *pfSpecData     pointer on buffer with power spectrum (length: half FFT length)
//                  - zFLOAT    *pfBandData     output: pointer on buffer with grouped freq values
//                  - zINT      iSampleRate     Sample rate of audio signal
//                  - zINT      iNumOfBands     number of resulting freq bands
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         grouping power spectrum in bands (BS.1387, pp.204-211)
//  @creation_date: 26.06.2000
//  @last_modified: 21.07.2000  AL: optimized (compiler switch)
/////////////////////////////////////////////////////////////////////////////////////
void    CFFTModel::fmGroupInBands(zFLOAT *pfSpecData, zFLOAT *pfBandData, zINT iSampleRate, zINT iNumOfBands)
{
    
    zINT        iBandIdx,
                iFreqIdx;
    zFLOAT      fFreqRes    = static_cast<zFLOAT>(iSampleRate)/_FFT_LENGTH;

    zFLOAT64    fInvFreqRes = 1.0F / fFreqRes;

    for (iBandIdx = 0; iBandIdx < iNumOfBands; iBandIdx++)
    {
        pfBandData[iBandIdx]    = 0;

        for (iFreqIdx = 0; iFreqIdx < _HALF_FFT_LENGTH; iFreqIdx++)
        {
            #if (OPTIMIZE_FREQ_GROUPING == OFF)
                // standard code
                // line inside frequency group
                if ( ((iFreqIdx - 0.5) * fFreqRes >= m_pTables->stTablesFFT.pfLowerBound[iBandIdx]) && ((iFreqIdx + 0.5) * fFreqRes <= m_pTables->stTablesFFT.pfUpperBound[iBandIdx])) 
                    pfBandData[iBandIdx]   += pfSpecData[iFreqIdx];

                // frequency group inside
                else if ( ((iFreqIdx - 0.5) * fFreqRes < m_pTables->stTablesFFT.pfLowerBound[iBandIdx]) && ((iFreqIdx + 0.5) * fFreqRes > m_pTables->stTablesFFT.pfUpperBound[iBandIdx])) 
                    pfBandData[iBandIdx]   += pfSpecData[iFreqIdx] * (m_pTables->stTablesFFT.pfUpperBound[iBandIdx] - m_pTables->stTablesFFT.pfLowerBound[iBandIdx])/fFreqRes; // non standard compliant, because standard results in negative value!!

                // left border outside group, right border in group
                else if ( ((iFreqIdx - 0.5) * fFreqRes < m_pTables->stTablesFFT.pfLowerBound[iBandIdx]) && ((iFreqIdx + 0.5) * fFreqRes > m_pTables->stTablesFFT.pfLowerBound[iBandIdx])) 
                    pfBandData[iBandIdx]   += pfSpecData[iFreqIdx] * ((iFreqIdx + 0.5F) * fFreqRes - m_pTables->stTablesFFT.pfLowerBound[iBandIdx])/fFreqRes;

                // right border outside group, left border in group
                else if ( ((iFreqIdx - 0.5) * fFreqRes < m_pTables->stTablesFFT.pfUpperBound[iBandIdx]) && ((iFreqIdx + 0.5) * fFreqRes > m_pTables->stTablesFFT.pfUpperBound[iBandIdx])) 
                    pfBandData[iBandIdx]   += pfSpecData[iFreqIdx] * (m_pTables->stTablesFFT.pfUpperBound[iFreqIdx] - (iFreqIdx - 0.5F) * fFreqRes) / fFreqRes;

                // line outside frequency group
                else
                    pfBandData[iBandIdx]   += 0.0F;


            #else   // #if (OPTIMIZE_FREQ_GROUPING==OFF)
                    // optimized code (40%->5%)

                zFLOAT64    fLow,
                            fUp;
                zFLOAT      *pfLowBound = m_pTables->stTablesFFT.pfLowerBound,
                            *pfUpBound  = m_pTables->stTablesFFT.pfUpperBound;

                fLow                            = (iFreqIdx - .5) * fFreqRes;       //lower limit of frequency line

                // line outside frequency group
                if ( fLow <= pfUpBound[iBandIdx])
                {
                    fUp                         = fLow + fFreqRes;                  //upper limit of frequency line

                    if ( fUp >= pfLowBound[iBandIdx] )
                    {
                        // left border outside group
                        if ( fLow <= pfLowBound[iBandIdx] )
                            pfBandData[iBandIdx]   += (pfUpBound[iBandIdx] > fUp) ? 
                                                        static_cast<zFLOAT>(pfSpecData[iFreqIdx] * (fUp - pfLowBound[iBandIdx]) * fInvFreqRes) : 
                                                        static_cast<zFLOAT>(pfSpecData[iFreqIdx] * (pfUpBound[iBandIdx] - pfLowBound[iBandIdx]) * fInvFreqRes); 
                        
                        // left border inside group
                        else
                            pfBandData[iBandIdx]   += (pfUpBound[iBandIdx] < fUp) ? 
                                                        static_cast<zFLOAT>(pfSpecData[iFreqIdx] * (pfUpBound[iBandIdx] - fLow) * fInvFreqRes) : 
                                                        static_cast<zFLOAT>(pfSpecData[iFreqIdx] * (fUp - fLow) * fInvFreqRes); 
                    }
                    else
                    {
                        fUp                         = - fUp;
                        fUp                        += pfLowBound[iBandIdx];
                        fUp                        *= fInvFreqRes;
//                      fUp                         = zplFLOOR( fUp );
                        iFreqIdx                   += static_cast<zINT>( fUp );
                    }
                }
                else
                    break;
        #endif// #if (OPTIMIZE_FREQ_GROUPING==OFF)
        }

        if (pfBandData[iBandIdx] < 1.0E-12F)
            pfBandData[iBandIdx]    = 1.0E-12F;
    }
    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmFreqSpread
//  @parameter:     - zFLOAT        *pfEn           energy spectrum to be spreaded
//                  - zFLOAT        *pfEnSpread     resulting spreaded spectrum
//                  - zBOOL         bModelType      basic or advanced model
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         computes SpreadSpectrum (frequency spreading) of energy
//  @creation_date: 02.04.2000
//  @last_modified: 12.10.2001: cosmetic changes in table usage (pCTables->...)
//                  18.01.2002: AL: no more pow() in here
/////////////////////////////////////////////////////////////////////////////////////
void CFFTModel::fmFreqSpread(zFLOAT *pfEn, zFLOAT *pfEnSpread, zBOOL bModelType)
{
    
    zINT                            iBarkj,                 // Masker
                                    iBarkk,                 // Maskee
                                    iNumOfBands;


    zFLOAT                          fAtt,
                                    fNorm,
                                    fSlope,
                                    fSlopeLow,
                                    fEnj,
                                    fBRes,
                                    afEnSpreadTmp[_NUM_BANDS_BASIC];
    zFLOAT                          *pfSlope    = m_pTables->stTablesFFT.pfSlopeSpread,
                                    *pfNorm     = m_pTables->stTablesFFT.pfNormSpread;

    if (bModelType == _BASIC)
    {
        iNumOfBands = _NUM_BANDS_BASIC;
        fBRes       = _BARKRES_BASIC;
    }
    else
    {
        iNumOfBands = _NUM_BANDS_ADVANCED;
        fBRes       = _BARKRES_ADVANCED;
    }

    // initialize pfEnSpread
    zplSetZero (pfEnSpread, iNumOfBands);

    fSlopeLow                       =  zplPOW(10, - fBRes * 2.7F);
    fBRes                          *= .2F;

    for (iBarkj = 0; iBarkj < iNumOfBands; iBarkj++)
    {
        fEnj                        = pfEn[iBarkj];
//      if(!fEnj)
//          fEnj                    = _FLT_MIN;

        afEnSpreadTmp[iBarkj]       = fEnj;
        
        // initialize for lower slope
        fSlope                      = fSlopeLow;                                                // lower slope is const (27dB)
        fNorm                       = 1.0F;
        fAtt                        = 1.0F;
        
        // lower slope
        for (iBarkk = iBarkj; iBarkk > 0; iBarkk--)                                     
        {
            fAtt                   *= fSlope;                                                   // attenuation for band cBarkk
            afEnSpreadTmp[iBarkk-1] = fEnj * fAtt;                                              // nonnormalized masking threshold
            fNorm                  += fAtt;                                                     // normalization factor
        }
        
        // initialize new for upper slope
        fAtt                        = 1.0F;
        fSlope                      = zplPOW(fEnj,  fBRes);
        fSlope                     *= pfSlope[iBarkj];

        // upper slope
        for (iBarkk = iBarkj; iBarkk < iNumOfBands-1; iBarkk++)                                 
        {
            fAtt                   *= fSlope;                                                   // attenuation for next band cBarkk
            afEnSpreadTmp[iBarkk+1] = fEnj *  fAtt;                                             // nonnormalized masking threshold
            fNorm                  += fAtt;                                                     // normalization factor
        }
        fNorm                       = 1.0F/fNorm;

        // nonlinear superposition (part of eq. 17)
        for (iBarkk = 0; iBarkk < iNumOfBands; iBarkk++)
        {
            afEnSpreadTmp[iBarkk]  *= fNorm;
            /////////////////////////////////////////////////////////////////////////////////////////
            // ToDo: put an approximation for pow(x,0.4) here:
            /////////////////////////////////////////////////////////////////////////////////////////
//          pfEnSpread[iBarkk]     += zplPOW(afEnSpreadTmp[iBarkk], 0.4F);              
            pfEnSpread[iBarkk]     += zplEXP(.4F * zplLOG(afEnSpreadTmp[iBarkk]));              
        }
    }

    // normalization (part of eq 17)
    for (iBarkk = 0; iBarkk < iNumOfBands; iBarkk++)
    {
//      pfEnSpread[iBarkk]          = zplPOW(pfEnSpread[iBarkk], 2.5F);
//      pfEnSpread[iBarkk]         *= pfNorm[iBarkk];                                           // resulting excitation pattern
        pfEnSpread[iBarkk]         *= pfEnSpread[iBarkk] * pfEnSpread[iBarkk] * pfEnSpread[iBarkk] * pfEnSpread[iBarkk];
        pfEnSpread[iBarkk]          = zplSQRT (pfEnSpread[iBarkk]) * pfNorm[iBarkk];
    }       
    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmTimeSpread
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zFLOAT            *pfFiltFac      filtering factors
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         computes postmasking with first order low pass (time spreading) (BS.1387, p.213)
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void CFFTModel::fmTimeSpread(stChannelData *pChannelData, zFLOAT *pfFiltFac, zINT iNumOfBands)
{
    zINT        iIdx;

    zFLOAT  *pfEnRef            = pChannelData->pfFreqSpreadRef,
            *pfEnTest           = pChannelData->pfFreqSpreadTest,
            *pfEnLastBlockRef   = pChannelData->pfEnLastBlockRef,
            *pfEnLastBlockTest  = pChannelData->pfEnLastBlockTest,
            *pfEnSmearedRef     = pChannelData->pfSpreadRef,
            *pfEnSmearedTest    = pChannelData->pfSpreadTest;

    for (iIdx = 0; iIdx < iNumOfBands; iIdx ++)
    {
        pfEnLastBlockRef[iIdx]  *= pfFiltFac[iIdx];
        pfEnLastBlockRef[iIdx]  += (1 - pfFiltFac[iIdx]) * pfEnRef[iIdx];
        pfEnSmearedRef[iIdx]     = ( pfEnLastBlockRef[iIdx] < pfEnRef[iIdx]) ? pfEnRef[iIdx] : pfEnLastBlockRef[iIdx];

        pfEnLastBlockTest[iIdx] *= pfFiltFac[iIdx];
        pfEnLastBlockTest[iIdx] += (1 - pfFiltFac[iIdx]) * pfEnTest[iIdx];
        pfEnSmearedTest[iIdx]    = ( pfEnLastBlockTest[iIdx] < pfEnTest[iIdx]) ? pfEnTest[iIdx] : pfEnLastBlockTest[iIdx];
    }
    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fmAddMask
//  @parameter:     - zFLOAT        *pfSpecData     pointer on threshold
//                  - zFLOAT        *pfMask         pointer on masking threshold
//                  - zINT          iNumOfBands     number of freq bands
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         adding weighting function for masking to threshold (BS.1387, p.214)
//  @creation_date: 06.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
inline void CFFTModel::fmAddMask(zFLOAT *pfSpecData, zFLOAT *pfMask, zINT iNumOfBands)
{
    zplMultVec( pfSpecData, 
                m_pTables->stTablesFFT.pfMaskWeight, 
                pfMask,
                iNumOfBands);
    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        Process
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on EAQUALInstance Data
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         process psychoacoustic model
//  @creation_date: 16.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CFFTModel::Process(eaqstEAQUALInstance *pInstance)
{
    zINT    iCh,
            iNumOfBands,
            iSampleRate,
            iNumOfChannels  = pInstance->WaveFormat.iNumOfChannel;

    zERROR  rErr    = _NO_ERROR;

    eaqsTablesFFT   *pTables    = &(m_pTables->stTablesFFT);

    zFLOAT  *pfRefFFT,
            *pfTestFFT,
            *pfRefBark,
            *pfTestBark;

    iNumOfBands     = ( pInstance->bModelType == _BASIC ) ? _NUM_BANDS_BASIC : _NUM_BANDS_ADVANCED;
    iSampleRate     = pInstance->WaveFormat.iSamplingRate;

    for (iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++)
    {
        if (iCh >= iNumOfChannels) continue;

        stChannelData   *pChannelData   = &(pInstance->ChannelData[iCh]);

        pfRefFFT    = pChannelData->pfFFTRef;
        pfTestFFT   = pChannelData->pfFFTTest;
        pfRefBark   = pChannelData->pfBarkRef;
        pfTestBark  = pChannelData->pfBarkTest;
        

        // copy time buffers
        zplCopyVec (pfRefFFT, pChannelData->pfTimeSignalRef, _FFT_LENGTH);
        zplCopyVec (pfTestFFT, pChannelData->pfTimeSignalTest, _FFT_LENGTH);

        // calc inplace FFT (eq. (4))
        fmFFT(      pfRefFFT);
        fmFFT(      pfTestFFT);

        // scale input (eq. (6))
        fmScale(    pfRefFFT);
        fmScale(    pfTestFFT);
        
        // calc energy spectrum (eq. (11))
        fmCalcPower(pfRefFFT);
        fmCalcPower(pfTestFFT);

        // calc log spectrum for Bandwidth calculation
        fmLogSpec(  pfRefFFT, 
                    pChannelData->pfdBSpecRef, 
                    _HALF_FFT_LENGTH);
        fmLogSpec(  pfTestFFT, 
                    pChannelData->pfdBSpecTest, 
                    _HALF_FFT_LENGTH);
        
        // weighting with outer ear transfer function (eq. (9))
        pmOuterEar( pfRefFFT, 
                    pTables->pfOuterEar, 
                    _HALF_FFT_LENGTH);
        pmOuterEar( pfTestFFT, 
                    pTables->pfOuterEar, 
                    _HALF_FFT_LENGTH);
        
        
        // calc log spectrum for EHS    
        fmLogSpec(  pfRefFFT, 
                    pChannelData->pfdBSpecWRef, 
                    _HALF_FFT_LENGTH);
        fmLogSpec(  pfTestFFT, 
                    pChannelData->pfdBSpecWTest, 
                    _HALF_FFT_LENGTH);
        
        // calculate error (noise) spectrum (eq. (62))
        fmCalcNoise(pfRefFFT, 
                    pfTestFFT, 
                    pChannelData->pfSpecNoise);
        
        // grouping into critical bands (eq. (10))
        fmGroupInBands( pfRefFFT, 
                        pfRefBark,
                        iSampleRate,
                        iNumOfBands);
        fmGroupInBands( pfTestFFT, 
                        pfTestBark, 
                        iSampleRate,
                        iNumOfBands);
        fmGroupInBands( pChannelData->pfSpecNoise, 
                        pChannelData->pfBarkNoise, 
                        iSampleRate,
                        iNumOfBands);

        // add internal noise (eq. (14))
        pmAddNoise(     pfRefBark,
                        pTables->pfIntNoise,
                        iNumOfBands);
        pmAddNoise(     pfTestBark,
                        pTables->pfIntNoise,
                        iNumOfBands);
        
        // calculate unsmeared excitation patterns (eqs. (15-20))
        fmFreqSpread(   pfRefBark, 
                        pChannelData->pfFreqSpreadRef, 
                        pInstance->bModelType);
        fmFreqSpread(   pfTestBark, 
                        pChannelData->pfFreqSpreadTest, 
                        pInstance->bModelType);
        
        // calculate excitation patterns (eq. (23))
        fmTimeSpread(   pChannelData,
                        pTables->pfFiltFac1,
                        iNumOfBands);
        
        // calc log spectrum of excitation patterns for MFPD
        fmLogSpec(      pChannelData->pfSpreadRef, 
                        pChannelData->pfdBSpreadRef, 
                        iNumOfBands);
        fmLogSpec(      pChannelData->pfSpreadTest, 
                        pChannelData->pfdBSpreadTest, 
                        iNumOfBands);
        
        // calculate masking threshold (eq. (26))
        fmAddMask(      pChannelData->pfSpreadRef,
                        pChannelData->pfMaskRef,
                        iNumOfBands);
    } // end of channel loop

    return rErr;
}
