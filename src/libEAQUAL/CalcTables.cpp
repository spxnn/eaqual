/////////////////////////////////////////////////////////////////////////////////////
// CalcTables.cpp: implementation of the CCalcTables class.
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
#include "EAQUALCore.h"
#include "CalcTables.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcTables::CCalcTables(zBOOL bModelType)
{
    ctInitTables(bModelType);
}

CCalcTables::~CCalcTables()
{
    // dealloc memory
    ctDelTables();

}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctInitTables
//  @parameter:     - zBOOL bModelType  switch basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         initialize the class CCalcTables, alloc memory 
//  @creation_date: 10.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::ctInitTables(zBOOL bModelType)
{
    zUINT32 uiNumBandsFFT,
            uiNumMOV,
            uiNumHiddenLayer,
            i;


    uiNumBandsFFT           = ( bModelType == _ADVANCED ) ? _NUM_BANDS_ADVANCED : _NUM_BANDS_BASIC;
    uiNumMOV                = ( bModelType == _ADVANCED ) ? _NUM_MOV_ADVANCED : _NUM_MOV_BASIC;
    uiNumHiddenLayer        = ( bModelType == _ADVANCED ) ? _NUM_HIDD_NEUR_ADVANCED : _NUM_HIDD_NEUR_BASIC;
    
    // alloc memory for the tables used by FFT_MODEL and _FB_MODEL

    // alloc memory for the FFT_MODEL tables
    if (( m_pfHannWin       = new zFLOAT[_HALF_FFT_LENGTH] )== 0) return _MEM_ALLOC_FAILED;
    if (( m_pfOuterEar      = new zFLOAT[_HALF_FFT_LENGTH] )== 0) return _MEM_ALLOC_FAILED;
    if (( m_pfIntNoise      = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfIntNoise2     = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfFiltFac1      = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfFiltFac2      = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfLoudS         = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfNormSpread    = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfSlopeSpread   = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfMaskWeight    = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfCenterFreq    = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfLowerBound    = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfUpperBound    = new zFLOAT[uiNumBandsFFT] )   == 0) return _MEM_ALLOC_FAILED;

    if (( m_pfNNNormMin     = new zFLOAT[uiNumMOV])         == 0) return _MEM_ALLOC_FAILED;
    if (( m_pfNNNormMax     = new zFLOAT[uiNumMOV])         == 0) return _MEM_ALLOC_FAILED;
    for ( i = 0; i < uiNumHiddenLayer; i++)
        if (( m_pfNNWeights [0][i]  = new zFLOAT[uiNumMOV+1])           == 0) return _MEM_ALLOC_FAILED;
    for ( i= uiNumHiddenLayer; i < _NUM_HIDD_LAYER_MAX; i++)
        m_pfNNWeights[0][i]         = 0;
    if (( m_pfNNWeights     [1][0]  = new zFLOAT[uiNumHiddenLayer+1])   == 0) return _MEM_ALLOC_FAILED;
    for ( i= 1; i < _NUM_HIDD_LAYER_MAX; i++)
        m_pfNNWeights[1][i]         = 0;
    
    
    // alloc memory for the FB_MODEL tables (TODO)
    // ...
    if ( bModelType == _ADVANCED)
    {
        zUINT32 uiNumBandsFB    = _NUM_BANDS_FB;
        if (( m_pfCenterFreqFB  = new zFLOAT[uiNumBandsFB] )        == 0) return _MEM_ALLOC_FAILED;
        if (( m_pfOuterEarFB    = new zFLOAT[uiNumBandsFB] )        == 0) return _MEM_ALLOC_FAILED;
        if (( m_pfIntNoiseFB    = new zFLOAT[uiNumBandsFB] )        == 0) return _MEM_ALLOC_FAILED;
        if (( m_pfIntNoise2FB   = new zFLOAT[uiNumBandsFB] )        == 0) return _MEM_ALLOC_FAILED;
        if (( m_pfFiltFac1FB    = new zFLOAT[uiNumBandsFB] )        == 0) return _MEM_ALLOC_FAILED;
        if (( m_pfFiltFac2FB    = new zFLOAT[uiNumBandsFB] )        == 0) return _MEM_ALLOC_FAILED;
    }
    else
    {
        m_pfCenterFreqFB    = 0;
        m_pfOuterEarFB      = 0;
        m_pfIntNoiseFB      = 0;
        m_pfIntNoise2FB     = 0;
        m_pfFiltFac1FB      = 0;
        m_pfFiltFac2FB      = 0;
    }
                    
    return _NO_ERROR;
}

zERROR CCalcTables::ctDelTables()
{
    if ( m_pfIntNoise       != 0) delete [] m_pfIntNoise;
    if ( m_pfIntNoise2      != 0) delete [] m_pfIntNoise2;
    if ( m_pfFiltFac1       != 0) delete [] m_pfFiltFac1;
    if ( m_pfFiltFac2       != 0) delete [] m_pfFiltFac2;
    if ( m_pfLoudS          != 0) delete [] m_pfLoudS;
    if ( m_pfHannWin        != 0) delete [] m_pfHannWin;
    if ( m_pfOuterEar       != 0) delete [] m_pfOuterEar;
    if ( m_pfNormSpread     != 0) delete [] m_pfNormSpread;
    if ( m_pfSlopeSpread    != 0) delete [] m_pfSlopeSpread;
    if ( m_pfMaskWeight     != 0) delete [] m_pfMaskWeight;
    if ( m_pfCenterFreq     != 0) delete [] m_pfCenterFreq;
    if ( m_pfLowerBound     != 0) delete [] m_pfLowerBound;
    if ( m_pfUpperBound     != 0) delete [] m_pfUpperBound;

    if ( m_pfCenterFreqFB   != 0) delete [] m_pfCenterFreqFB;
    if ( m_pfOuterEarFB     != 0) delete [] m_pfOuterEarFB;
    if ( m_pfIntNoiseFB     != 0) delete [] m_pfIntNoiseFB;
    if ( m_pfIntNoise2FB    != 0) delete [] m_pfIntNoise2FB;
    if ( m_pfFiltFac1FB     != 0) delete [] m_pfFiltFac1FB;
    if ( m_pfFiltFac2FB     != 0) delete [] m_pfFiltFac2FB;

    if ( m_pfNNNormMin      != 0) delete [] m_pfNNNormMin;
    if ( m_pfNNNormMax      != 0) delete [] m_pfNNNormMax;
    
    for ( zINT i = 0; i < _NUM_NN_LAYER-1; i++)
        for (zINT k = 0; k < _NUM_HIDD_NEUR_ADVANCED; k++)
            if ( m_pfNNWeights[i][k] != 0) delete [] m_pfNNWeights[i][k];
        
                    
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctProcess
//  @parameter:     - eaqstEAQUALInstance *pInstance    pointer on instance data
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calc the tables needed 
//  @creation_date: 10.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::Process(eaqstEAQUALInstance *pInstance)
{
    zERROR  rErr    = _NO_ERROR;
    zBOOL   bModelType  = pInstance->bModelType;

    
    
    /////////////////////////////////////////////////////////////////////////////////////
    // now calculate the tables!

    // calc hann window
    if (( rErr = ctHannWindow (     _HALF_FFT_LENGTH )))                    return rErr;

    // calc scale fac (listening level)
    if (( rErr = ctScaleFac (       pInstance->GlobalOptions.fSPL, 
                                    bModelType, 
                                    pInstance->WaveFormat.iSamplingRate ))) return rErr;

    // calc transfer function or outer and middle ear
    if (( rErr = ctOuterEar(            bModelType,
                                    pInstance->WaveFormat.iSamplingRate ))) return rErr;

    // get center freqs of the bands
    if (( rErr = ctGetCenterFreq(   bModelType)))                           return rErr;

    // get the frequency boundaries of the bands
    if (( rErr = ctGetBandLimits(   bModelType)))                           return rErr;
    
    // calc intern noise
    if (( rErr = ctInternNoise(     bModelType)))                           return rErr;
    
    // calc intern noise 2
    if (( rErr = ctInternNoise2(        bModelType)))                       return rErr;
    
    // calc lowpass coefficients
    if (( rErr = ctLowPassCoeffs(   bModelType,
                                    pInstance->WaveFormat.iSamplingRate ))) return rErr;
    
    // calc tables for spreading
    if (( rErr = ctCalcSpreadTables(    bModelType)))                       return rErr;
    
    // calc masking threshold offset
    if (( rErr = ctWeightFunc(      bModelType)))                           return rErr;
    
    // calc table for loudness calculation
    if (( rErr = ctSLoudness(       bModelType)))                           return rErr;
    
    // get all the stuff needed for the neural network
    if (( rErr = ctGetNNTables(     bModelType)))                           return rErr;
    
    
    /////////////////////////////////////////////////////////////////////////////////////
    // set the table pointers on the calculated tables
    
    // set the pointer on the tables
    if (( rErr = ctSetTablePointers(    bModelType,
                                    &(pInstance->Tables))))                 return rErr;
    
        
    return rErr;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctHannWindow
//  @parameter:     - zUINT32    uiHalfFFTLength    length of half hann window
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calc the the hanning window according BS.1387, p.203, eq.(2)
//  @creation_date: 23.03.2000
//  @last_modified: 10.11.2001 AL: length of hann win = fftlength/2
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctHannWindow(zUINT32 uiHalfFFTLength)
{
    zUINT32     uiTime;                                 // increment
    zUINT32     uiFFTLength = (uiHalfFFTLength)<<1;

    zFLOAT      fScaleFac,                              // scale factor for Hann Window
                fInvFFTLen  = 1.0F/uiFFTLength;
    
    fScaleFac               = zplSQRT(0.666666666666666666666666666666F);

    for (uiTime = 0; uiTime < (uiHalfFFTLength); uiTime++)
    {       
        // HannWindow according BS.1387, p.203, eq.(2)
        m_pfHannWin[uiTime]     = (1.0F - zplCOS(_2PI * uiTime / (uiFFTLength - 1.0F)));    
        m_pfHannWin[uiTime]    *= fScaleFac * fInvFFTLen;
    }
    
    
    return _NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctScaleFac
//  @parameter:     - zFLOAT     fListLevel listening level
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calc the the hanning window according BS.1387, p.203, eq.(2)
//  @creation_date: 27.06.2000
//  @last_modified: 10.11.2001 AL: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctScaleFac(zFLOAT   fListLevel, zBOOL bModelType, zUINT32 uiSampleRate)
{
    zFLOAT      afSin[_FFT_LENGTH];
    
    zUINT32     uiIdx,
                uiRange,
                uiBitDepth  = 0;

    m_fScaleFac = 0;
    
    
    uiRange     =  (0x1L<<15) - 1;

    for (uiIdx = 0; uiIdx < _HALF_FFT_LENGTH; uiIdx++)
    {
        afSin[uiIdx]        = zplSIN( _2PI * 1019.5F * uiIdx/uiSampleRate);
        afSin[uiIdx]       *= uiRange;
        afSin[uiIdx]       *= *m_pfHannWin++;
    }
    for (uiIdx = _HALF_FFT_LENGTH; uiIdx < _FFT_LENGTH; uiIdx++)
    {
        afSin[uiIdx]        = zplSIN( _2PI * 1019.5F * uiIdx/uiSampleRate);
        afSin[uiIdx]       *= uiRange;
        afSin[uiIdx]       *= *(-- m_pfHannWin);
    }

    // find log dualis of _FFT_LENGTH (note: _FFT_LENGTH must be power of two
    // hack; check for errors!!
    uiIdx           = _FFT_LENGTH;
    while (uiIdx)
    {
        uiIdx     >>= 1;
        uiBitDepth++;
        if ( uiIdx & 0x00000001 )   
            break;
    }

    zplRealFFT(afSin, uiBitDepth);

    for (uiIdx  = 1; uiIdx < _HALF_FFT_LENGTH; uiIdx++)
    {
        afSin[uiIdx]                       *= afSin[uiIdx];                     // Real part
        afSin[_FFT_LENGTH - uiIdx]         *= afSin[_FFT_LENGTH - uiIdx];           // Imag part
        afSin[uiIdx]                        = zplSQRT(afSin[uiIdx] + afSin[_FFT_LENGTH - uiIdx]);
        if (afSin[uiIdx-1] < afSin[uiIdx])
            m_fScaleFac                     = (afSin[uiIdx]>m_fScaleFac) ? afSin[uiIdx] : m_fScaleFac;
    }


    m_fScaleFac     = zplPOW(10, .05F * fListLevel)/m_fScaleFac;

    
    // scale factor for FB model    
    if (bModelType == _ADVANCED)
    {
        m_fScaleFacFB   = zplPOW(10, .05F * fListLevel);
        m_fScaleFacFB  /= 32767;
    }

    return _NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctGetCenterFreq
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         reads table with center frequencies of frequency bands (BS.1387, pp. 205-209, TABLE 7+8)
//  @creation_date: 24.03.2000
//  @last_modified: 13.06.2000 AL: 
//                  10.11.2001 AL: tables now in the function and nor global
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctGetCenterFreq(zBOOL bModelType)
{

    if (bModelType == _BASIC)
    {
        zFLOAT  afFreqTmp[_NUM_BANDS_BASIC]=
        {
           91.708F,   115.216F,   138.870F,   162.702F,   186.742F,   211.019F,   235.566F,
          260.413F,   285.593F,   311.136F,   337.077F,   363.448F,   390.282F,   417.614F,
          445.479F,   473.912F,   502.950F,   532.629F,   562.988F,   594.065F,   625.899F,
          658.533F,   692.006F,   726.362F,   761.644F,   797.898F,   835.170F,   873.508F,
          912.959F,   953.576F,   995.408F,  1038.511F,  1082.938F,  1128.746F,  1175.995F,
         1224.744F,  1275.055F,  1326.992F,  1380.623F,  1436.014F,  1493.237F,  1552.366F,
         1613.474F,  1676.641F,  1741.946F,  1809.474F,  1879.310F,  1951.543F,  2026.266F,
         2103.573F,  2183.564F,  2266.340F,  2352.008F,  2440.675F,  2532.456F,  2627.468F, 
         2725.832F,  2827.672F,  2933.120F,  3042.309F,  3155.379F,  3272.475F,  3393.745F,
         3519.344F,  3649.432F,  3784.176F,  3923.748F,  4068.324F,  4218.090F,  4373.237F,
         4533.963F,  4700.473F,  4872.978F,  5051.700F,  5236.866F,  5428.712F,  5627.484F,
         5833.434F,  6046.825F,  6267.931F,  6497.031F,  6734.420F,  6980.399F,  7235.284F,
         7499.397F,  7773.077F,  8056.673F,  8350.547F,  8655.072F,  8970.639F,  9297.648F,
         9636.520F,  9987.683F, 10351.586F, 10728.695F, 11119.490F, 11524.470F, 11944.149F,
        12379.066F, 12829.775F, 13294.850F, 13780.887F, 14282.503F, 14802.338F, 15341.057F,
        15899.345F, 16477.914F, 17077.504F, 17690.045F
        };

        zplCopyVec (m_pfCenterFreq, afFreqTmp, _NUM_BANDS_BASIC);
    }
    else
    {
        zFLOAT  afFreqTmp[_NUM_BANDS_ADVANCED] =
        {
          103.445F,   150.762F,   198.849F,   247.950F,   298.317F,   350.207F,   403.884F,
          459.622F,   517.707F,   578.434F,   642.114F,   709.071F,   779.647F,   854.203F,
          933.119F,  1016.797F,  1105.666F,  1200.178F,  1300.816F,  1408.094F,  1522.559F,
         1644.795F,  1775.427F,  1915.121F,  2064.590F,  2224.597F,  2395.959F,  2579.551F,
         2776.309F,  2987.238F,  3213.415F,  3455.993F,  3716.212F,  3995.399F,  4294.979F,
         4616.482F,  4961.548F,  5331.939F,  5729.545F,  6156.396F,  6614.671F,  7106.708F,
         7635.020F,  8202.302F,  8811.450F,  9465.574F, 10168.013F, 10922.351F, 11732.438F,
        12606.412F, 13536.710F, 14540.103F, 15617.710F, 16775.035F, 17690.045F
        };

        zFLOAT  afFreqTmpFB[_NUM_BANDS_FB] =
        {
           50.00F,   116.19F,   183.57F,   252.82F,   324.64F,   399.79F,   479.01F,   563.11F,
          652.97F,   749.48F,   853.65F,   966.52F,  1089.25F,  1223.10F,  1369.43F,  1529.73F,
         1705.64F,  1898.95F,  2111.64F,  2345.88F,  2604.05F,  2888.79F,  3203.01F,  3549.90F,
         3933.02F,  4356.27F,  4823.97F,  5340.88F,  5912.30F,  6544.03F,  7242.54F,  8014.95F,
         8869.13F,  9813.82F, 10858.63F, 12014.24F, 13292.44F, 14706.26F, 16270.13F, 18000.02F 
        };

        zplCopyVec (m_pfCenterFreq, afFreqTmp, _NUM_BANDS_ADVANCED);
        zplCopyVec (m_pfCenterFreqFB, afFreqTmpFB, _NUM_BANDS_FB);
    }


    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctOuterEar
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//                  - zINT      iSampleRate     sampling rate of input audio
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         computes transfer function of outer and middle ear according BS.1387, p.204, eq.(7)
//  @creation_date: 23.03.2000
//  @last_modified: 13.06.2000 AL: 
//                  10.11.2001 AL: changed parameters
//                  25.01.2002 AL: fixed bug for m_pfOuterEar[0]
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctOuterEar(zBOOL bModelType, zINT iSampleRate)
{
    zINT        iFreq;

    zFLOAT      fEarTmp;

    zFLOAT      fFreq,
                fFreqRes    = (iSampleRate) / (1000.0F * _FFT_LENGTH);                          // frequency resolution in kHz      
    
    // table for the FFT Model
    m_pfOuterEar[0] = 0.0F;
    for (iFreq = 1; iFreq < _HALF_FFT_LENGTH; iFreq++)
    {
        fFreq               = iFreq*fFreqRes;                                                   // frequency index in kHz

        // transfer function for outer and middle ear according BS.1387, p.204, eq.(7)
        #if (ATH_NEW == OFF)
            fEarTmp             =   -.2184F * zplPOW(fFreq, -0.8F);                             // all factors /10 see eq.(9), but in energy domain
            fEarTmp            +=   .65F    * zplPOW(_EULER, -0.6F * zplPOW(fFreq - 3.3F, 2));
            fEarTmp            -=   .0001F  * zplPOW(fFreq, 3.6F);
            
        #else // modified transfer function for test purposes
            fEarTmp             =   -.2184F * zplPOW(fFreq, -0.8F);                             // all factors /10 see eq.(9), but in energy domain
            fEarTmp            +=   .65F    * zplPOW(_EULER, -0.6F * zplPOW(fFreq - 3.3F, 2));
            fEarTmp            -=   .7F     * zplPOW(_EULER, -0.6F * zplPOW(fFreq - 9.0F, 2));
            fEarTmp            +=   .45F    * zplPOW(_EULER, -0.6F * zplPOW(fFreq - 11.0F, 2));
            fEarTmp            -=   .0001F  * zplPOW(fFreq, 3.6F);
        #endif
            m_pfOuterEar[iFreq] = zplPOW(10, fEarTmp);
    } 

    // table for the advanced model 
    if (bModelType == _ADVANCED)
    {
        for (iFreq = 0; iFreq < _NUM_BANDS_FB; iFreq++)
        {
            // transfer function for outer and middle ear according BS.1387, p.220, eq.(32)
            fEarTmp                 =   -.1092F * zplPOW(m_pfCenterFreqFB[iFreq], -0.8F) +                              
                                        .325F   * zplPOW(_EULER, -0.6F * zplPOW(m_pfCenterFreqFB[iFreq] - 3.3F, 2)) -
                                        .00005F * zplPOW(m_pfCenterFreqFB[iFreq], 3.6F);

            m_pfOuterEarFB[iFreq]   = zplPOW(10.0F, fEarTmp);
        } 
    }
    
    return _NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctGetBandLimits
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         reads table with center frequencies of frequency bands (BS.1387, pp. 205-209, TABLE 7+8)
//  @creation_date: 04.07.2000
//  @last_modified: 10.11.2001 AL: changed parameters, integrated tables into function
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctGetBandLimits(zBOOL bModelType)
{


    if (bModelType == _BASIC)
    {
        zFLOAT  afFreqTmp1[_NUM_BANDS_BASIC] = 
        {
           80.000F,   103.445F,   127.023F,   150.762F,   174.694F,   198.849F,   223.257F,
          247.950F,   272.959F,   298.317F,   324.055F,   350.207F,   376.805F,   403.884F,
          431.478F,   459.622F,   488.353F,   517.707F,   547.721F,   578.434F,   609.885F,
          642.114F,   675.161F,   709.071F,   743.884F,   779.647F,   816.404F,   854.203F,
          893.091F,   933.119F,   974.336F,  1016.797F,  1060.555F,  1105.666F,  1152.187F,
         1200.178F,  1249.700F,  1300.816F,  1353.592F,  1408.094F,  1464.392F,  1522.559F,
         1582.668F,  1644.795F,  1709.021F,  1775.427F,  1844.098F,  1915.121F,  1988.587F,
         2064.590F,  2143.227F,  2224.597F,  2308.806F,  2395.959F,  2486.169F,  2579.551F,
         2676.223F,  2776.309F,  2879.937F,  2987.238F,  3098.350F,  3213.415F,  3332.579F,
         3455.993F,  3583.817F,  3716.212F,  3853.817F,  3995.399F,  4142.547F,  4294.979F,
         4452.890F,  4616.482F,  4785.962F,  4961.548F,  5143.463F,  5331.939F,  5527.217F,
         5729.545F,  5939.183F,  6156.396F,  6381.463F,  6614.671F,  6856.316F,  7106.708F,
         7366.166F,  7635.020F,  7913.614F,  8202.302F,  8501.454F,  8811.450F,  9132.688F, 
         9465.574F,  9810.536F, 10168.013F, 10538.460F, 10922.351F, 11320.175F, 11732.438F,
        12159.670F, 12602.412F, 13061.229F, 13536.710F, 14029.458F, 14540.103F, 15069.295F,
        15617.710F, 16186.049F, 16775.035F, 17385.420F
        };
        zplCopyVec (m_pfLowerBound, afFreqTmp1, _NUM_BANDS_BASIC);

        zFLOAT  afFreqTmp2[_NUM_BANDS_BASIC] = 
        {
          103.445F,   127.023F,   150.762F,   174.694F,   198.849F,   223.257F,   247.950F,   272.959F,
          298.317F,   324.055F,   350.207F,   376.805F,   403.884F,   431.478F,   459.622F,   488.353F,
          517.707F,   547.721F,   578.434F,   609.885F,   642.114F,   675.161F,   709.071F,   743.884F,
          779.647F,   816.404F,   854.203F,   893.091F,   933.113F,   974.336F,  1016.797F,  1060.555F,
         1105.666F,  1152.187F,  1200.178F,  1249.700F,  1300.816F,  1353.592F,  1408.094F,  1464.392F,
         1522.559F,  1582.668F,  1644.795F,  1709.021F,  1775.427F,  1844.098F,  1915.121F,  1988.587F,
         2064.590F,  2143.227F,  2224.597F,  2308.806F,  2395.959F,  2486.169F,  2579.551F,  2676.223F,
         2776.309F,  2879.937F,  2987.238F,  3098.350F,  3213.415F,  3332.579F,  3455.993F,  3583.817F, 
         3716.212F,  3853.348F,  3995.399F,  4142.547F,  4294.979F,  4452.890F,  4643.482F,  4785.962F, 
         4961.548F,  5143.463F,  5331.939F,  5527.217F,  5729.545F,  5939.183F,  6156.396F,  6381.463F, 
         6614.671F,  6856.316F,  7106.708F,  7366.166F,  7635.020F,  7913.614F,  8202.302F,  8501.454F, 
         8811.450F,  9132.688F,  9465.574F,  9810.536F, 10168.013F, 10538.460F, 10922.351F, 11320.175F, 
        11732.438F, 12159.670F, 12602.412F, 13061.229F, 13536.710F, 14029.458F, 14540.103F, 15069.295F, 
        15617.710F, 16186.049F, 16775.035F, 17385.420F, 18000.000F
        };
        zplCopyVec (m_pfUpperBound, afFreqTmp2, _NUM_BANDS_BASIC);

    }
    else // bModelType == _ADVANCED
    {
        zFLOAT  afFreqTmp1[_NUM_BANDS_ADVANCED] = 
        {
           80.000F,   127.023F,   174.694F,   223.257F,   272.959F,   324.055F,   376.805F, 
          431.478F,   488.353F,   547.721F,   609.885F,   675.161F,   743.884F,   816.404F, 
          893.091F,   974.336F,  1060.555F,  1152.187F,  1249.700F,  1353.592F,  1464.392F, 
         1582.668F,  1709.021F,  1844.098F,  1988.587F,  2143.227F,  2308.806F,  2486.169F, 
         2676.223F,  2879.937F,  3098.350F,  3332.579F,  3583.817F,  3853.348F,  4142.547F, 
         4452.890F,  4785.962F,  5143.463F,  5527.217F,  5939.183F,  6381.463F,  6856.316F, 
         7366.166F,  7913.614F,  8501.454F,  9132.688F,  9810.536F, 10538.460F, 11320.175F, 
        12159.670F, 13061.229F, 14029.458F, 15069.295F, 16186.049F, 17385.420F
        };
        zplCopyVec (m_pfLowerBound, afFreqTmp1, _NUM_BANDS_ADVANCED);

        zFLOAT  afFreqTmp2[_NUM_BANDS_ADVANCED] = 
        {
          127.023F,   174.694F,   223.257F,   272.959F,   324.055F,   376.805F,   431.478F,   488.353F, 
          547.721F,   609.885F,   675.161F,   743.884F,   816.404F,   893.091F,   974.336F,  1060.555F, 
         1152.187F,  1249.700F,  1353.592F,  1464.392F,  1582.668F,  1709.021F,  1844.098F,  1988.587F, 
         2143.227F,  2308.806F,  2486.169F,  2676.223F,  2879.937F,  3098.350F,  3332.579F,  3583.817F, 
         3853.348F,  4142.547F,  4452.890F,  4785.962F,  5143.463F,  5527.217F,  5939.183F,  6381.463F, 
         6856.316F,  7366.166F,  7913.614F,  8501.454F,  9132.688F,  9810.536F, 10538.460F, 11320.175F, 
        12159.670F, 13061.229F, 14029.458F, 15069.295F, 16186.042F, 17385.420F, 18000.000F
        };
        zplCopyVec (m_pfUpperBound, afFreqTmp2, _NUM_BANDS_ADVANCED);
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctInternNoise
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Internal noise according BS.1387, eq. (36)
//  @creation_date: 25.03.2000
//  @last_modified: 10.11.2001 AL: changed parameters, included calculation for filterbank model
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctInternNoise(zBOOL bModelType)
{

    zINT        iBark;

    if ( bModelType == _BASIC)
    {
        for (iBark = 0; iBark < _NUM_BANDS_BASIC; iBark++)
        {
            m_pfIntNoise[iBark]     = .1456F * zplPOW(.001F * m_pfCenterFreq[iBark], -0.8F);    // noise in dB
            m_pfIntNoise[iBark]     = zplPOW(10, m_pfIntNoise[iBark]);                          // -> in energy domain
        }
    }
    else // model type == advanced
    {
        // FFT table
        for (iBark = 0; iBark < _NUM_BANDS_ADVANCED; iBark++)
        {
            m_pfIntNoise[iBark]     = .1456F * zplPOW(.001F * m_pfCenterFreq[iBark], -0.8F);    // noise in dB
            m_pfIntNoise[iBark]     = zplPOW(10, m_pfIntNoise[iBark]);                          // -> in energy domain
        }
        // filterbank table
        for (iBark = 0; iBark < _NUM_BANDS_FB; iBark++)
        {
            m_pfIntNoiseFB[iBark]   = .1456F * zplPOW(.001F * m_pfCenterFreqFB[iBark], -0.8F);  // noise in dB
            m_pfIntNoiseFB[iBark]   = zplPOW(10, m_pfIntNoiseFB[iBark]);                        // -> in energy domain
        }
    }
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctInternNoise2
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates Internal noise according BS.1387, eq. (60)
//  @creation_date: 13.07.2000
//  @last_modified: 11.11.2001 AL: changed parameters, included calculation for filterbank model
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctInternNoise2( zBOOL bModelType)
{

    zINT        iBark;


    if ( bModelType == _BASIC)
    {
        for (iBark = 0; iBark < _NUM_BANDS_BASIC; iBark++)
        {
            m_pfIntNoise2[iBark]    = .364F * zplPOW(.001F * m_pfCenterFreq[iBark], -0.8F);     // noise in dB
            m_pfIntNoise2[iBark]    = zplPOW(10.0F, m_pfIntNoise2[iBark]);                      // -> in energy domain
        }
    }
    else
    {
        for (iBark = 0; iBark < _NUM_BANDS_ADVANCED; iBark++)
        {
            m_pfIntNoise2[iBark]    = .364F * zplPOW(.001F * m_pfCenterFreq[iBark], -0.8F);     // noise in dB
            m_pfIntNoise2[iBark]    = zplPOW(10.0F, m_pfIntNoise2[iBark]);                      // -> in energy domain
        }
        for (iBark = 0; iBark < _NUM_BANDS_FB; iBark++)
        {
            m_pfIntNoise2FB[iBark]  = .364F * zplPOW(.001F * m_pfCenterFreqFB[iBark], -0.8F);   // noise in dB
            m_pfIntNoise2FB[iBark]  = zplPOW(10.0F, m_pfIntNoise2FB[iBark]);                    // -> in energy domain
        }
    }
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctLowPassCoeffs
//  @parameter:     - zBOOL     bModelType  basic or advanced model
//                  - zINT      iSampleRate sampling rate of input audio    
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates filter coeffs 
//  @creation_date: 11.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::ctLowPassCoeffs( zBOOL bModelType, zINT iSampleRate)
{
    zERROR  rErr    = _NO_ERROR;

    zINT    iNumOfBands;

    if ( bModelType == _BASIC)
        iNumOfBands = _NUM_BANDS_BASIC;
    else
    {
        iNumOfBands = _NUM_BANDS_ADVANCED;
        rErr       |= ctCalcLP ( m_pfFiltFac1FB, 0.02F, 0.004F, _STEPSIZE_FB, iSampleRate, _NUM_BANDS_FB);
        rErr       |= ctCalcLP ( m_pfFiltFac2FB, 0.05F, 0.008F, _STEPSIZE_FB, iSampleRate, _NUM_BANDS_FB);
    }

    rErr   |= ctCalcLP ( m_pfFiltFac1, 0.03F, 0.008F, _STEPSIZE_FFT, iSampleRate, iNumOfBands);
    rErr   |= ctCalcLP ( m_pfFiltFac2, 0.05F, 0.008F, _STEPSIZE_FFT, iSampleRate, iNumOfBands);
    
    return rErr;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctCalcLowPass
//  @parameter:     - zFLOAT        pfFiltFac   pointer on resulting coefficients
//                  - zFLOAT        fTau100     filter constant,
//                  - zFLOAT        fTaumin     filter constant,
//                  - zINT      iStepSize   1024 for FFT- based Model, 192 for FB- based model --> time resolution  
//                  - zINT      iSampleRate sampling rate of input audio    
//                  - zINT      iLength     length of vector pfFiltFac  
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates filter coeffs according BS.1387, eq (24) and (44)
//  @creation_date: 25.03.2000
//  @last_modified: 11.11.2001 AL: changed parameters
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::ctCalcLP( zFLOAT *pfFiltFac, zFLOAT fTau100, zFLOAT fTaumin, zINT iStepSize, zINT iSampleRate, zINT iLength)
{
    zFLOAT  fTau;

    zINT    iBark;

    for (iBark=0; iBark < iLength; iBark++)
    {
        fTau                = fTaumin + 100.0F * (fTau100 - fTaumin)/ m_pfCenterFreq[iBark];
        pfFiltFac[iBark]    = zplEXP(- iStepSize/(iSampleRate * fTau));
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctCalcSpreadTables
//  @parameter:     - zBOOL     bModelType  basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculates filter coeffs (do I need this function for the FB_Model, too?)
//  @creation_date: 11.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::ctCalcSpreadTables(zBOOL bModelType)
{
    zFLOAT  fBarkRes;
    zINT    iLength;


    if ( bModelType == _BASIC)
    {
        fBarkRes    = _BARKRES_BASIC;
        iLength     = _NUM_BANDS_BASIC;
    }
    else // model == advanced
    {
        fBarkRes    = _BARKRES_ADVANCED;
        iLength     = _NUM_BANDS_ADVANCED;
    }

        
    ctSpreadSlopeUp ( fBarkRes, iLength);
    ctNormSpread    ( fBarkRes, iLength);   

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctSpreadSlopeUp
//  @parameter:     - zFLOAT    fBarkRes        Frequency resolution of model in Bark   
//                  - zINT  iLength         Length of table
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate levewl independent part of upper slope (BS.1387, pp.212) 
//  @creation_date: 21.06.2000
//  @last_modified: 11.11.2001
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::ctSpreadSlopeUp(zFLOAT fBarkRes, zINT iLength)
{

    zINT    iBark;


    for( iBark = 0; iBark < iLength; iBark++)
    {
        m_pfSlopeSpread[iBark]  = 24.0F + 230.0F/m_pfCenterFreq[iBark];

        m_pfSlopeSpread[iBark]  = zplPOW(10, -0.1F * fBarkRes * m_pfSlopeSpread[iBark]);
    }
    
    
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctNormSpread
//  @parameter:     - zFLOAT    fBarkRes        Frequency resolution of model in Bark   
//                  - zINT  iLength         Length of table
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate level independent part of spreading function (BS.1387, pp.212,213, eq. (19))
//  @creation_date: 20.06.2000
//  @last_modified: 11.11.2001
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::ctNormSpread(zFLOAT fBarkRes, zINT iLength)
{

    zFLOAT              *pfSpreadTmp;

    zFLOAT              fAtt,
                        fNorm,
                        fSlope;

    zINT                cBarkj,                 // Masker
                        cBarkk;                 // Maskee


    pfSpreadTmp                     = new zFLOAT[iLength];

    // initialization
    zplSetZero(m_pfNormSpread, iLength);


    fSlope                      = zplPOW(10, - fBarkRes * 2.7F);                        // lower slope is const (27dB)

    for (cBarkj = 0; cBarkj < iLength; cBarkj++)
    {
        // initialize
        fAtt                        = 1.0F;
        fNorm                       = 1.0F;                                             // not 0, because case cBarkk==cBarkj
        pfSpreadTmp[cBarkj]         = 1.0F;                                             // case cBarkk==cBarkj


        // lower slope
        for (cBarkk = cBarkj; cBarkk > 0; cBarkk--)                                     
        {
            fAtt                   *= fSlope;                                           // attenuation for band cBarkk
            pfSpreadTmp[cBarkk-1]   = fAtt;                                             // nonnormalized masking threshold
            fNorm                  += fAtt;                                             // normalization factor
        }


        // initialize new
        fAtt                        = 1.0F;
        
        // upper slope
        for (cBarkk = cBarkj; cBarkk < iLength-1; cBarkk++)                                 
        {
            fAtt                   *= m_pfSlopeSpread[cBarkj];
            pfSpreadTmp[cBarkk+1]   = fAtt;
            fNorm                  += fAtt;                                             // normalization factor
        }

        
        fNorm                       = 1.0F/fNorm;

        // nonlinear superposition
        for (cBarkk = 0; cBarkk < iLength; cBarkk++)
        {
            pfSpreadTmp[cBarkk]    *= fNorm;
            m_pfNormSpread[cBarkk] += zplPOW(pfSpreadTmp[cBarkk], 0.4F);                
        }

    }


    // normalization
    for (cBarkk = 0; cBarkk < iLength; cBarkk++)
    {
        m_pfNormSpread[cBarkk]      = zplPOW(m_pfNormSpread[cBarkk], -2.5F);            // resulting normalization pattern (eq. 19)
    }

    
    delete [] pfSpreadTmp;
    
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctWeightFunc
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate weighting function for masking threshold (BS.1387, p.214)
//  @creation_date: 27.06.2000
//  @last_modified: 03.07.2000
//                  11.11.2001 AL:
/////////////////////////////////////////////////////////////////////////////////////
zERROR CCalcTables::ctWeightFunc(zBOOL bModelType)
{
    zINT        iFreq,
                iNumBands;

    zFLOAT      fBarkRes,
                fTmp    = zplPOW(.1F, .3F);

    if ( bModelType == _BASIC)
    {
        iNumBands   = _NUM_BANDS_BASIC;
        fBarkRes    = _BARKRES_BASIC;
    }
    else
    {
        iNumBands   = _NUM_BANDS_ADVANCED;
        fBarkRes    = _BARKRES_ADVANCED;
    }
    
    for ( iFreq = 0; iFreq < 12.0F/fBarkRes; iFreq++ )
        m_pfMaskWeight[iFreq]   = fTmp;


    while (iFreq < iNumBands)
    {
        m_pfMaskWeight[iFreq]   = zplPOW(.1F, .025F * fBarkRes * iFreq);
        iFreq++;
    }

    
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctSLoudness
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate weighting function for excitation patterns (BS.1387, p.214)
//  @creation_date: 27.06.2000
//  @last_modified: 13.07.2000
//                  11.11.2001 AL: changed parameter
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctSLoudness(zBOOL bModelType)
{
    zINT    iBandIdx,
            iNumBands = ( bModelType == _BASIC) ? _NUM_BANDS_BASIC : _NUM_BANDS_ADVANCED;


    for (iBandIdx = 0; iBandIdx < iNumBands; iBandIdx++)
    {
        m_pfLoudS[iBandIdx]     = -.2F;
        m_pfLoudS[iBandIdx]    -= .205F * zplATAN(m_pfCenterFreq[iBandIdx] * 0.00025F);
        m_pfLoudS[iBandIdx]    -= .075F * zplATAN(m_pfCenterFreq[iBandIdx] * m_pfCenterFreq[iBandIdx] * 3.90625E-07F);
        m_pfLoudS[iBandIdx]     = zplPOW(10, m_pfLoudS[iBandIdx]);
    }

    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctGetNNTables
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         reads tables for the neural network, tables 19-22
//  @creation_date: 11.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctGetNNTables(zBOOL bModelType)
{
    zINT i;

    if ( bModelType == _BASIC )
    {
        zFLOAT  afNNNormMin[]   =
                {       
                    393.916656F,
                    361.965332F,
                    -24.045116F,
                      1.110661F,
                      -.206623F,
                       .074318F,
                      1.113683F,
                       .950345F,
                       .029985F,
                       .000101F,
                      0.000000F
                };
        

        zFLOAT  afNNNormMax[]   =
                {       
                     921.000000F,
                     881.131226F,
                      16.212030F,
                     107.137772F,
                       2.886017F,
                      13.933351F,
                      63.257874F,
                    1145.018555F,
                      14.819740F,
                       1.000000F,
                       1.000000F
                };
        
        
        
        zFLOAT  afNNWeights[][_NUM_MOV_BASIC+1] =
        {   
            { -.502657F, 4.307481F, 4.984241F, .051056F, 2.321580F, -5.303901F, 2.730991F, .624950F, 3.102889F, -1.051468F, -1.804679F, -2.518254F},
            { 0.436333F, 3.246017F, -2.211189F, -1.762424F, 1.789971F, -3.452257F, -6.111805F, -1.331523F, .871260F, -.939882F, -.503610F, .654841F},
            { 1.219602F, 1.123743F, -.192096F, 4.331315F, -.754560F, -10.814982F, 1.519223F, -5.955151F, -5.922878F, -.142913F, -.620456F, -2.207228F},
            { -3.817048F, 4.107138F, 4.629582F, -.307594F, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        m_aiNNSize[0]   = _NUM_MOV_BASIC;
        m_aiNNSize[1]   = _NUM_HIDD_NEUR_BASIC;
        m_aiNNSize[2]   = 1;
        
        zplCopyVec (m_pfNNNormMin, afNNNormMin, m_aiNNSize[0]);
        zplCopyVec (m_pfNNNormMax, afNNNormMax, m_aiNNSize[0]);

        for (i = 0; i < _NUM_HIDD_NEUR_BASIC; i++)
            zplCopyVec(m_pfNNWeights[0][i], afNNWeights[i], _NUM_MOV_BASIC+1);
        zplCopyVec(m_pfNNWeights[1][0], afNNWeights[_NUM_HIDD_NEUR_BASIC], _NUM_HIDD_NEUR_BASIC+1);
    }
    else // bModelType == _ADVANCED
    {
        zFLOAT  afNNNormMin[]   =
                {       
                    13.299F, 0.041F, 0.025F, -25.019F, 0.062F
                };
        
        
        zFLOAT  afNNNormMax[]   =
                {       
                    2166.500F, 13.243F, 14.225F, 13.467F, 10.227F
                };
        
        zFLOAT  afNNWeights[][_NUM_MOV_ADVANCED+1] =
        {
            { 21.212F, -8.982F, 11.556F, 1.634F, 6.104F, 1.331F },
            { -39.913F, 19.956F, 3.892F, -2.878F, 19.587F, 2.686F },
            { -1.383F, .935F, 9.72F, -7.443F, -.240F, 2.097F },
            { -14.545F, -1.687F, -3.287F, 5.607F, 1.088F, -1.328F },
            { -.321F, -3.239F, -11.031F, -1.783F, -.511F, 3.087F},
            { -4.697F, -3.29F, 7.005F, 6.652F, 4.009F, -1.360F }
        };
            
        m_aiNNSize[0]   = _NUM_MOV_ADVANCED;
        m_aiNNSize[1]   = _NUM_HIDD_NEUR_ADVANCED;
        m_aiNNSize[2]   = 1;

        zplCopyVec (m_pfNNNormMin, afNNNormMin, m_aiNNSize[0]);
        zplCopyVec (m_pfNNNormMax, afNNNormMax, m_aiNNSize[0]);

        for (i = 0; i < _NUM_HIDD_NEUR_ADVANCED; i++)
            zplCopyVec(m_pfNNWeights[0][i], afNNWeights[i], _NUM_MOV_ADVANCED+1);
        zplCopyVec(m_pfNNWeights[1][0], afNNWeights[_NUM_HIDD_NEUR_ADVANCED], _NUM_HIDD_NEUR_ADVANCED+1);
        
    }
    
    return _NO_ERROR;
}



/////////////////////////////////////////////////////////////////////////////////////
//  @method:        ctSetTablePointers
//  @parameter:     - zBOOL     bModelType      basic or advanced model
//                  - eaqsTables *pstTables     tables for calculation
//  @result:        zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         set the pointers of the global table structure to the allocated memory
//  @creation_date: 11.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CCalcTables::ctSetTablePointers(zBOOL bModelType, eaqsTables *pstTables)
{
    eaqsTablesFFT   *pFFTTables = &pstTables->stTablesFFT;
    eaqsTablesNN    *pNNTables  = &pstTables->stTablesNN;

    // tables for the FFTbased model
    pFFTTables->fScaleFac       = m_fScaleFac;
    pFFTTables->pfCenterFreq    = m_pfCenterFreq;
    pFFTTables->pfFiltFac1      = m_pfFiltFac1;
    pFFTTables->pfFiltFac2      = m_pfFiltFac2;
    pFFTTables->pfHannWin       = m_pfHannWin;
    pFFTTables->pfIntNoise      = m_pfIntNoise;
    pFFTTables->pfIntNoise2     = m_pfIntNoise2;
    pFFTTables->pfLoudS         = m_pfLoudS;
    pFFTTables->pfMaskWeight    = m_pfMaskWeight;
    pFFTTables->pfNormSpread    = m_pfNormSpread;
    pFFTTables->pfOuterEar      = m_pfOuterEar;
    pFFTTables->pfSlopeSpread   = m_pfSlopeSpread;
    pFFTTables->pfLowerBound    = m_pfLowerBound;
    pFFTTables->pfUpperBound    = m_pfUpperBound;
    
    if ( bModelType == _ADVANCED)
    {
        eaqsTablesFB    *pFBTables  = &pstTables->stTablesFB;
        
        // tables for the FBbased model
        pFBTables->fScaleFacFB      = m_fScaleFacFB;
        pFBTables->pfCenterFreqFB   = m_pfCenterFreqFB;
        pFBTables->pfFiltFac1FB     = m_pfFiltFac1FB;
        pFBTables->pfFiltFac2FB     = m_pfFiltFac2FB;
        pFBTables->pfIntNoise2      = m_pfIntNoise2FB;
        pFBTables->pfIntNoise       = m_pfIntNoiseFB;
        pFBTables->pfOuterEarFB     = m_pfOuterEarFB;
    }

    // tables for the neural network
    pNNTables->pfNNNormMin      = m_pfNNNormMin;
    pNNTables->pfNNNormMax      = m_pfNNNormMax;
    pNNTables->piNNSize         = m_aiNNSize;
    for ( zINT i = 0; i < _NUM_NN_LAYER-1; i++)
        for (zINT k = 0; k < _NUM_HIDD_NEUR_ADVANCED; k++)
            pNNTables->pfNNWeights[i][k]    = m_pfNNWeights[i][k];

    return _NO_ERROR;
}
