/////////////////////////////////////////////////////////////////////////////////////
// PreProc.cpp: implementation of the CPreProc class.
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


#include "PreProc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreProc::CPreProc(eaqstEAQUALInstance *pInstance)
{
    zINT    iCh,
            iNumOfBands = ( pInstance->bModelType == _BASIC ) ? _NUM_BANDS_BASIC : _NUM_BANDS_FB;

    for ( iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++) 
    {
        stChannelData   *pChannel   = &(pInstance->ChannelData[iCh]);
        
        pChannel->pfPPLevSpreadRef  = m_pfRefLevSpread[iCh]     = new zFLOAT[iNumOfBands];
        pChannel->pfPPLevSpreadTest = m_pfTestLevSpread[iCh]    = new zFLOAT[iNumOfBands];
        
        // initialization for level adaption
        pChannel->pfPPLastBlockRef  = m_pfRefLastBlock[iCh]     = new zFLOAT[iNumOfBands];
        pChannel->pfPPLastBlockTest = m_pfTestLastBlock[iCh]    = new zFLOAT[iNumOfBands];
        
        zplSetZero ( m_pfRefLastBlock[iCh], iNumOfBands );
        zplSetZero ( m_pfTestLastBlock[iCh], iNumOfBands );
        
        // initialization for pattern adaption
        pChannel->pfPPRCorrFacNum       = m_pfRCorrFacNum[iCh]      = new zFLOAT[iNumOfBands];
        pChannel->pfPPRCorrFacDenom     = m_pfRCorrFacDenom[iCh]    = new zFLOAT[iNumOfBands];
        pChannel->pfPPPattCorrFacRef    = m_pfRefPattCorrFac[iCh]   = new zFLOAT[iNumOfBands];
        pChannel->pfPPPattCorrFacTest   = m_pfTestPattCorrFac[iCh]  = new zFLOAT[iNumOfBands];
        pChannel->pfPattSpreadRef       = m_pfRefPattSpread[iCh]    = new zFLOAT[iNumOfBands];
        pChannel->pfPattSpreadTest      = m_pfTestPattSpread[iCh]   = new zFLOAT[iNumOfBands];
        
        zplSetZero ( m_pfRCorrFacNum[iCh], iNumOfBands );
        zplSetZero ( m_pfRCorrFacDenom[iCh], iNumOfBands );
        zplSetOne ( m_pfRefPattCorrFac[iCh], iNumOfBands );             // no comment in standard about initialization
        zplSetOne ( m_pfTestPattCorrFac[iCh], iNumOfBands );            // no comment in standard about initialization
        
        // initialization for pattern adaption
        pChannel->pfPPEnDerivRef        = m_pfRefEnDeriv[iCh]       = new zFLOAT[iNumOfBands];
        pChannel->pfPPEnDerivTest       = m_pfTestEnDeriv[iCh]      = new zFLOAT[iNumOfBands];
        pChannel->pfPPEnOldRef          = m_pfRefEnOld[iCh]         = new zFLOAT[iNumOfBands];
        pChannel->pfPPEnOldTest         = m_pfTestEnOld[iCh]        = new zFLOAT[iNumOfBands];
        pChannel->pfPPEnLoudRef         = m_pfRefEnLoud[iCh]        = new zFLOAT[iNumOfBands];
        pChannel->pfPPEnLoudTest        = m_pfTestEnLoud[iCh]       = new zFLOAT[iNumOfBands];
        pChannel->pfModRef              = m_pfRefMod[iCh]           = new zFLOAT[iNumOfBands];
        pChannel->pfModTest             = m_pfTestMod[iCh]          = new zFLOAT[iNumOfBands];
        
        zplSetZero ( m_pfRefEnDeriv[iCh], iNumOfBands );
        zplSetZero ( m_pfTestEnDeriv[iCh], iNumOfBands );
        zplSetZero ( m_pfRefEnOld[iCh], iNumOfBands );
        zplSetZero ( m_pfTestEnOld[iCh], iNumOfBands );
        zplSetZero ( m_pfRefEnLoud[iCh], iNumOfBands );
        zplSetZero ( m_pfTestEnLoud[iCh], iNumOfBands );
    }
    for ( iCh = pInstance->WaveFormat.iNumOfChannel; iCh < _NUM_CHANNELS_MAX; iCh++) 
    {
        m_pfRefLevSpread[iCh]   = 0;
        m_pfTestLevSpread[iCh]  = 0;
        
        // initialization for level adaption
        m_pfRefLastBlock[iCh]   = 0;
        m_pfTestLastBlock[iCh]  = 0;
        
        // initialization for pattern adaption
        m_pfRCorrFacNum[iCh]    = 0;
        m_pfRCorrFacDenom[iCh]  = 0;
        m_pfRefPattCorrFac[iCh] = 0;
        m_pfTestPattCorrFac[iCh]= 0;
        m_pfRefPattSpread[iCh]  = 0;
        m_pfTestPattSpread[iCh] = 0;
        
        // initialization for pattern adaption
        m_pfRefEnDeriv[iCh]     = 0;
        m_pfTestEnDeriv[iCh]    = 0;
        m_pfRefEnOld[iCh]       = 0;
        m_pfTestEnOld[iCh]      = 0;
        m_pfRefEnLoud[iCh]      = 0;
        m_pfTestEnLoud[iCh]     = 0;
        m_pfRefMod[iCh]         = 0;
        m_pfTestMod[iCh]        = 0;
    }
}


CPreProc::~CPreProc()
{
    for (zINT iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++)
    {
        if ( m_pfRefLastBlock[iCh]      != 0) delete [] m_pfRefLastBlock[iCh];
        if ( m_pfTestLastBlock[iCh]     != 0) delete [] m_pfTestLastBlock[iCh];
        if ( m_pfRefLevSpread[iCh]      != 0) delete [] m_pfRefLevSpread[iCh];
        if ( m_pfTestLevSpread[iCh]     != 0) delete [] m_pfTestLevSpread[iCh];
        if ( m_pfRCorrFacNum[iCh]       != 0) delete [] m_pfRCorrFacNum[iCh];
        if ( m_pfRCorrFacDenom[iCh]     != 0) delete [] m_pfRCorrFacDenom[iCh];
        if ( m_pfRefPattCorrFac[iCh]    != 0) delete [] m_pfRefPattCorrFac[iCh];
        if ( m_pfTestPattCorrFac[iCh]   != 0) delete [] m_pfTestPattCorrFac[iCh];
        if ( m_pfRefPattSpread[iCh]     != 0) delete [] m_pfRefPattSpread[iCh];
        if ( m_pfTestPattSpread[iCh]    != 0) delete [] m_pfTestPattSpread[iCh];
        if ( m_pfRefEnDeriv[iCh]        != 0) delete [] m_pfRefEnDeriv[iCh];
        if ( m_pfTestEnDeriv[iCh]       != 0) delete [] m_pfTestEnDeriv[iCh];
        if ( m_pfRefEnOld[iCh]          != 0) delete [] m_pfRefEnOld[iCh];
        if ( m_pfTestEnOld[iCh]         != 0) delete [] m_pfTestEnOld[iCh];
        if ( m_pfRefEnLoud[iCh]         != 0) delete [] m_pfRefEnLoud[iCh];
        if ( m_pfTestEnLoud[iCh]        != 0) delete [] m_pfTestEnLoud[iCh];
        if ( m_pfRefMod[iCh]            != 0) delete [] m_pfRefMod[iCh];
        if ( m_pfTestMod[iCh]           != 0) delete [] m_pfTestMod[iCh];
    }
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ppLowPass
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zFLOAT            *pfFiltFac      filtering factors
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        fLevCorr (level correction factor according eq. (45))
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         low pass filtering as preprocessing for calculation of MOVs (BS.1387, p.224)
//  @creation_date: 07.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CPreProc::ppLowPass(stChannelData *pChannel, zFLOAT *pfFiltFac, zINT iNumOfBands)
{
    zINT        iIdx;

    zFLOAT      *pfEnRefLastBlock,
                *pfEnTestLastBlock,
                *pfFilRef,
                *pfFilTest,
                *pfEnRef,
                *pfEnTest;
    zFLOAT      fLevCorr                = 0.0F,
                fTmp                    = 0.0F;

    pfEnRefLastBlock        = pChannel->pfPPLastBlockRef;
    pfEnTestLastBlock       = pChannel->pfPPLastBlockTest;
    pfFilRef                = pChannel->pfPPLevSpreadRef;
    pfFilTest               = pChannel->pfPPLevSpreadTest;
    pfEnRef                 = pChannel->pfSpreadRef;
    pfEnTest                = pChannel->pfSpreadTest;

    for (iIdx = 0; iIdx < iNumOfBands; iIdx++)
    {
        // reference
        pfEnRefLastBlock[iIdx]  *= pfFiltFac[iIdx];
        pfEnRefLastBlock[iIdx]  += (1.0F - pfFiltFac[iIdx]) * pfEnRef[iIdx];

        // test
        pfEnTestLastBlock[iIdx] *= pfFiltFac[iIdx];
        pfEnTestLastBlock[iIdx] += (1.0F - pfFiltFac[iIdx]) * pfEnTest[iIdx];

        // update output
        pfFilRef[iIdx]          = pfEnRefLastBlock[iIdx];
        pfFilTest[iIdx]         = pfEnTestLastBlock[iIdx];

        // eq. (45)
        fLevCorr               += zplSQRT(pfFilRef[iIdx] * pfFilTest[iIdx] );
        fTmp                   += pfFilTest[iIdx];
    }

    if (fTmp)
    {
        fLevCorr           /= fTmp;
        fLevCorr           *= fLevCorr;
    }
    else
        fLevCorr            = 0.0F;



    return fLevCorr;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ppLevAdapt
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         level adaptation as preprocessing for calculation of MOVs (BS.1387, p.224)
//  @creation_date: 07.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CPreProc::ppLevAdapt(stChannelData *pChannel, zINT iNumOfBands)
{
    zFLOAT      fCorr   = pChannel->fLevCorr;
    
    if ( fCorr > 1 )
    {
        zplMultVecConst (   pChannel->pfPPLevSpreadRef, 
                            pChannel->pfSpreadRef, 
                            1.0F / fCorr, 
                            iNumOfBands);
        zplCopyVec (        pChannel->pfPPLevSpreadTest, 
                            pChannel->pfSpreadTest, 
                            iNumOfBands);
    }
    else
    {
        zplMultVecConst (   pChannel->pfPPLevSpreadTest, 
                            pChannel->pfSpreadTest, 
                            fCorr, 
                            iNumOfBands);
        zplCopyVec (        pChannel->pfPPLevSpreadRef, 
                            pChannel->pfSpreadRef, 
                            iNumOfBands);
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ppPattAdapt
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zFLOAT            *pFilFac        pointer on lowpass filter factors
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         spectral pattern adaptation as preprocessing for calculation of MOVs (BS.1387, p.225)
//  @creation_date: 07.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CPreProc::ppPattAdapt(stChannelData *pChannelData, zFLOAT *pfFilFac, zINT iNumOfBands)
{
    zINT        iIdx,
                i,
                iM1,
                iM2;

    zFLOAT      fInvM,
                fMInv,
                fSumTmpRef,
                fSumTmpTest;

    zFLOAT      afRCorrFacRef   [_NUM_BANDS_MAX],
                afRCorrFacTest  [_NUM_BANDS_MAX],
                *pfRCorrNum     = pChannelData->pfPPRCorrFacNum,
                *pfRCorrDenom   = pChannelData->pfPPRCorrFacDenom,
                *pfLevRef       = pChannelData->pfPPLevSpreadRef,
                *pfLevTest      = pChannelData->pfPPLevSpreadTest,
                *pfPattFacRef   = pChannelData->pfPPPattCorrFacRef,
                *pfPattFacTest  = pChannelData->pfPPPattCorrFacTest,
                *pfPattRef      = pChannelData->pfPattSpreadRef,
                *pfPattTest     = pChannelData->pfPattSpreadTest;


    switch ( iNumOfBands )
    {
    case _NUM_BANDS_BASIC:
        {
            iM1     = 3;
            iM2     = 4;
            fMInv   = 0.125F;
            break;
        }
    case _NUM_BANDS_FB:
        {
            iM1     = 1;
            iM2     = 1;
            fMInv   = 3.33333333333E-01F;
            break;
        }
    default:
        return _INVALID_FUNCTION_ARGS;
    }

    // initialization for case pfRCorrFacDenom[0] = 0
    afRCorrFacRef[0]        = 1.0F;                     
    afRCorrFacTest[0]       = 1.0F;


    for (iIdx = 0; iIdx < iNumOfBands; iIdx++ )
    {
        // numerator of eq.(48)
        pfRCorrNum[iIdx]   *= pfFilFac[iIdx];
        pfRCorrNum[iIdx]   += pfLevRef[iIdx] * pfLevTest[iIdx];

        // denominator of eq.(48)
        pfRCorrDenom[iIdx] *= pfFilFac[iIdx];
        pfRCorrDenom[iIdx] += pfLevRef[iIdx] * pfLevRef[iIdx];

        // eq. (49)
        if ((pfRCorrNum[iIdx] > pfRCorrDenom[iIdx]))
        {
            afRCorrFacRef[iIdx]     = 1.0F;
            afRCorrFacTest[iIdx]    = (pfRCorrDenom[iIdx] / pfRCorrNum[iIdx]);
        }
        else
        {
            if (pfRCorrDenom[iIdx] == 0)
            {
                if (iIdx > 0)
                {
                    afRCorrFacRef[iIdx]     = afRCorrFacRef[iIdx - 1];
                    afRCorrFacTest[iIdx]    = afRCorrFacTest[iIdx - 1];
                }
            }
            else
            {
                afRCorrFacRef[iIdx]         = (pfRCorrNum[iIdx] / pfRCorrDenom[iIdx]);
                afRCorrFacTest[iIdx]        = 1.0F;
            }
        }           
    }

    for (iIdx = 0; iIdx < iNumOfBands; iIdx++ )
    {
        // realization of eq. (51)
        fSumTmpRef                  = 0.0F;
        fSumTmpTest                 = 0.0F;
        if ( iIdx >= iM1)
        {
            if ( iIdx < iNumOfBands - iM2)
            {
                for ( i = iIdx - iM1; i <= iIdx + iM2; i++)
                {
                    fSumTmpRef     += afRCorrFacRef[i];
                    fSumTmpTest    += afRCorrFacTest[i];
                }
                fInvM               = fMInv;
            }
            else
            {
                for ( i = iIdx - iM1; i < iNumOfBands; i++)
                {
                    fSumTmpRef     += afRCorrFacRef[i];
                    fSumTmpTest    += afRCorrFacTest[i];
                }
                fInvM               = 1.0F / (iNumOfBands - iIdx + iM1);
            }
        }
        else
        {
            for ( i = 0; i <= iIdx + iM2; i++)
            {
                fSumTmpRef         += afRCorrFacRef[i];
                fSumTmpTest        += afRCorrFacTest[i];
            }
            fInvM                   = 1.0F / (iIdx + iM2 + 1);
        }

        // eq. (50)
        pfPattFacRef[iIdx]     *= pfFilFac[iIdx];
        pfPattFacTest[iIdx]    *= pfFilFac[iIdx];
        pfPattFacRef[iIdx]     += (1.0F - pfFilFac[iIdx]) * fInvM * fSumTmpRef;
        pfPattFacTest[iIdx]    += (1.0F - pfFilFac[iIdx]) * fInvM * fSumTmpTest;

        pfPattRef[iIdx]         = pfLevRef[iIdx]  * pfPattFacRef[iIdx];
        pfPattTest[iIdx]        = pfLevTest[iIdx] * pfPattFacTest[iIdx];
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ppModulation
//  @parameter:     - stChannelData *pChannelData   pointer on all channel-dependent data
//                  - zFLOAT            *pFilFac        pointer on lowpass filter factors
//                  - zINT          iSampleRate     sample rate of input signals
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculation of modulation as preprocessing for calculation of MOVs (BS.1387, p.226)
//  @creation_date: 07.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CPreProc::ppModulation(stChannelData *pChannelData, zFLOAT *pfFiltFac, zINT iSampleRate, zINT iNumOfBands)
{
    zINT        iIdx;

    zFLOAT      fLoudRef,
                fLoudTest,
                fFac        = static_cast<zFLOAT> ( iSampleRate )/ (( iNumOfBands == _NUM_BANDS_BASIC ) ? _STEPSIZE_FFT :  _STEPSIZE_FB ),
                *pfEnRef    = pChannelData->pfFreqSpreadRef,
                *pfEnTest   = pChannelData->pfFreqSpreadTest,
                *pfDerRef   = pChannelData->pfPPEnDerivRef,
                *pfDerTest  = pChannelData->pfPPEnDerivTest,
                *pfLoudRef  = pChannelData->pfPPEnLoudRef,
                *pfLoudTest = pChannelData->pfPPEnLoudTest,
                *pfOldRef   = pChannelData->pfPPEnOldRef,
                *pfOldTest  = pChannelData->pfPPEnOldTest,
                *pfModRef   = pChannelData->pfModRef,
                *pfModTest  = pChannelData->pfModTest;

    for ( iIdx = 0; iIdx < iNumOfBands; iIdx++)
    {
        // eq. (54)
        fLoudRef        = zplPOW(pfEnRef[iIdx], .3F); 
        pfDerRef[iIdx]  *= pfFiltFac[iIdx];
        pfDerRef[iIdx]  += (1.0F - pfFiltFac[iIdx]) * fFac * zplFABS( (fLoudRef - pfOldRef[iIdx]) );

        fLoudTest       = zplPOW(pfEnTest[iIdx], .3F); 
        pfDerTest[iIdx] *= pfFiltFac[iIdx];
        pfDerTest[iIdx] += (1.0F - pfFiltFac[iIdx]) * fFac * zplFABS( fLoudTest - pfOldTest[iIdx] );

        
        // eq. (55)
        pfLoudRef[iIdx]   *= pfFiltFac[iIdx];
        pfLoudRef[iIdx]   += (1.0F - pfFiltFac[iIdx]) * fLoudRef;

        pfLoudTest[iIdx]  *= pfFiltFac[iIdx];
        pfLoudTest[iIdx]  += (1.0F - pfFiltFac[iIdx]) * fLoudTest;

        // store for lp filtering
        pfOldRef[iIdx]  = fLoudRef;
        pfOldTest[iIdx] = fLoudTest;

        // eq. (57)
        pfModRef[iIdx]      = pfDerRef[iIdx] / (.3F + pfLoudRef[iIdx]);
        pfModTest[iIdx]     = pfDerTest[iIdx] / (.3F + pfLoudTest[iIdx]);
        pfModRef[iIdx]     *= .3F;
        pfModTest[iIdx]    *= .3F;
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ppLoudness
//  @parameter:     - zFLOAT            *pfEnSpec       vector of size cNumBands on excitation spectrum
//                  - eaqsTables        *pTables        pointer on tables
//                  - zINT          iNumOfBands     number of frequency bands
//  @result:        - zFLOAT            fLoudness       calculated loudness
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         loudness calculation (BS.1387, p.227)
//  @creation_date: 13.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT  CPreProc::ppLoudness(zFLOAT *pfEnSpec, eaqsTables *pTables, zINT iNumOfBands)
{
    zINT        iIdx;
    
    zFLOAT      fTmp,
                fConst,
                fLoudness   = 0,
                *pfNoise2,
                *pfLoudS;

    if ( iNumOfBands == _NUM_BANDS_BASIC )
    {
        pfNoise2        = pTables->stTablesFFT.pfIntNoise2;
        pfLoudS         = pTables->stTablesFFT.pfLoudS;
        fConst          = 1.07664F;
    }
    else
    {
        pfNoise2        = pTables->stTablesFB.pfIntNoise2;
        pfLoudS         = pTables->stTablesFB.pfLoudSFB;
        fConst          = 1.26539F;
    }

    for ( iIdx = 0; iIdx < iNumOfBands; iIdx++)
    {
        fTmp            = fConst;
        fTmp           *= zplPOW(.0001F * pfNoise2[iIdx] / pfLoudS[iIdx], .23F);
        fTmp           *= (zplPOW(1.0F - pfLoudS[iIdx] + (pfLoudS[iIdx] * pfEnSpec[iIdx] / pfNoise2[iIdx]), .23F) - 1);

        fLoudness      += (fTmp > 0) ? fTmp : 0.0F;
    }

    return (24.0F / iNumOfBands * fLoudness);
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        Process
//  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance of EAQUAL
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         Pre- processing of excitation patterns acc. BS.1387, Chap. 3, pp.224    
//  @creation_date: 16.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CPreProc::Process(eaqstEAQUALInstance *pInstance)
{
    zFLOAT      *pfFiltFac;
    zINT        iCh,
                iNumOfBands;
    zERROR      rErr    = _NO_ERROR;


    if ( pInstance->bModelType == _BASIC )
    {
        pfFiltFac   = pInstance->Tables.stTablesFFT.pfFiltFac2;
        iNumOfBands = _NUM_BANDS_BASIC;
    }
    else
    {
        pfFiltFac   = pInstance->Tables.stTablesFB.pfFiltFac2FB;
        iNumOfBands = _NUM_BANDS_ADVANCED;
    }
    
    ////////////////////////////////////////////////////////////
    // preprocessing for model output variables
    for (iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
    {
        stChannelData *pChannel = &(pInstance->ChannelData[iCh]);

        // low pass filtering (eq. (42+43))
        pChannel->fLevCorr  = ppLowPass(pChannel, 
                                        pfFiltFac, 
                                        iNumOfBands);

        // level adaption (eq. (46+47))
        rErr            = ppLevAdapt(   pChannel, 
                                        iNumOfBands);
        if ( rErr ) return rErr;

        // pattern adaption (eq. (52+53))
        rErr            = ppPattAdapt(  pChannel, 
                                        pfFiltFac, 
                                        iNumOfBands);
        if ( rErr ) return rErr;

        // modulation (eq. (57))
        rErr            = ppModulation( pChannel, 
                                        pfFiltFac, 
                                        pInstance->WaveFormat.iSamplingRate, 
                                        iNumOfBands);
        if ( rErr ) return rErr;

        // loudness (eq. (58 + 59))
        pChannel->fLoudRef= ppLoudness( pChannel->pfSpreadRef, 
                                        &(pInstance->Tables), 
                                        iNumOfBands);
        pChannel->fLoudTest= ppLoudness(pChannel->pfSpreadTest, 
                                        &(pInstance->Tables), 
                                        iNumOfBands);
    }

    return _NO_ERROR;
}
