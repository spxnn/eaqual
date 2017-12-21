/////////////////////////////////////////////////////////////////////////////////////
// EAQUALCore.h: constants/ structs/ macros for EAQUAL
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


#if !defined(__EAQUALCORE_HEADER_INCLUDED__)
#define __EAQUALCORE_HEADER_INCLUDED__

/////////////////////////////////////////////////////////////////////////////////////
// necessary include files
/////////////////////////////////////////////////////////////////////////////////////
#include <math.h>

#include "zGlobals.h"

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
#endif

/////////////////////////////////////////////////////////////////////////////////////
// definition of global flags
/////////////////////////////////////////////////////////////////////////////////////
#define OFF                     0
#define ON                      1

#define _BASIC                  0
#define _ADVANCED               1

#define _FFT_MODEL              0
#define _FB_MODEL               1

/////////////////////////////////////////////////////////////////////////////////////
// definition of global constants
/////////////////////////////////////////////////////////////////////////////////////
// length of FFT window
#define _FFT_LENGTH             2048

// length of half FFT window
#define _HALF_FFT_LENGTH        1024

// number of max allowd input channels
#define _NUM_CHANNELS_MAX       2

// maximal number of spectral bands
#define _NUM_BANDS_MAX          109

// number of spectral bands in basic FFT-Model
#define _NUM_BANDS_BASIC        109

// number of spectral bands in advanced FFT-Model
#define _NUM_BANDS_ADVANCED     55  

// number of spectral bands in advanced FB-Model
#define _NUM_BANDS_FB           40  

// number of basic model output variables
#define _NUM_MOV_BASIC          11
#define _NUM_MOVS_MAX           _NUM_MOV_BASIC

// number of advanced model output variables
#define _NUM_MOV_ADVANCED       5

// time resoluation
#define _STEPSIZE_FB            192
#define _STEPSIZE_FFT           1024

// band resolution
#define _BARKRES_BASIC          0.25F
#define _BARKRES_ADVANCED       0.5F

// neural network dimensions
#define _NUM_NN_LAYER           3
#define _NUM_HIDD_NEUR_BASIC    3
#define _NUM_HIDD_NEUR_ADVANCED 5
#define _NUM_HIDD_LAYER_MAX     _NUM_HIDD_NEUR_ADVANCED

/////////////////////////////////////////////////////////////////////////////////////
// definition of global compiler switches
/////////////////////////////////////////////////////////////////////////////////////
#define ATH_NEW                 OFF     // modified transfer function of outer and middle ear
#define TRUNC_MOV               ON      // truncate scaled MOVs between 0 and 1
#define OPTIMIZE_FREQ_GROUPING  ON      // optimized frequency grouping in bands
#define ADD_SILENCE_BEGIN       OFF     // should the first 1024 samples be zero or not?



/////////////////////////////////////////////////////////////////////////////////////
// definition of global macros
/////////////////////////////////////////////////////////////////////////////////////

// math functions (cast because of returning double...)
// pow(a,b)
#define zplPOW(a,b)             (static_cast<zFLOAT>(pow(a,b)))
// exp(a,b)
#define zplEXP(a)               (static_cast<zFLOAT>(exp(a)))
// log(a,b)
#define zplLOG(a)               (static_cast<zFLOAT>(log(a)))
// cos(a,b)
#define zplCOS(a)               (static_cast<zFLOAT>(cos(a)))
// sin(a,b)
#define zplSIN(a)               (static_cast<zFLOAT>(sin(a)))
// tan(a,b)
#define zplTAN(a)               (static_cast<zFLOAT>(tan(a)))
// atan(a,b)
#define zplATAN(a)              (static_cast<zFLOAT>(atan(a)))
// sqrt(a,b)
#define zplSQRT(a)              (static_cast<zFLOAT>(sqrt(a)))
// fabs(a,b)
#define zplFABS(a)              ((a > 0) ? (a) : -(a))
// abs(a,b)
#define zplIABS(a)              ((a > 0) ? (a) : -(a))
// floor(a,b)
#define zplFLOOR(a)             (static_cast<zFLOAT>(floor(a)))
// ceil(a,b)
#define zplCEIL(a)              (static_cast<zFLOAT>(ceil(a)))



/////////////////////////////////////////////////////////////////////////////////////
// definition of global structures
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        eaqsTablesFFT
//  @members:       - zFLOAT        fScaleFac           scale factor depending on listening level       
//                  - zFLOAT        *pfIntNoise         pointer on the internal noise of the ear
//                  - zFLOAT        *pfIntNoise2        pointer on the second model of internal noise of the ear
//                  - zFLOAT        *pfFiltFac1         pointer on the first lp- filter coeffs
//                  - zFLOAT        *pfFiltFac2         pointer on the second lp- filter coeffs
//                  - zFLOAT        *pfLoudS            pointer needed for Loudness calculation
//                  - zFLOAT        *pfHannWin          pointer on hanning window
//                  - zFLOAT        *pfOuterEar         pointer on the transfer of function of outer and middle ear
//                  - zFLOAT        *pfNormSpread       pointer on table needed for spreading
//                  - zFLOAT        *pfSlopeSpread      pointer on table needed for spreading
//                  - zFLOAT        *pfMaskWeight       pointer on masking index model
//                  - zFLOAT        *pfCenterFreq       pointer on the center freqs of bands
//                  - zFLOAT        *pfLowerBound       pointer on lower band boundaries
//                  - zFLOAT        *pfUpperBound       pointer on upper band boundaries
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         pointers on all tables for the fft version, in work...
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct eaqsTablesFFT
{
    zFLOAT  fScaleFac;
    zFLOAT  *pfIntNoise,
            *pfIntNoise2,
            *pfFiltFac1,
            *pfFiltFac2,
            *pfLoudS,
            *pfHannWin,
            *pfOuterEar,
            *pfNormSpread,
            *pfSlopeSpread,
            *pfMaskWeight,
            *pfCenterFreq,
            *pfLowerBound,
            *pfUpperBound;
};

/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        eaqsTablesFB
//  @members:       - zFLOAT        fScaleFacFB         scale factor depending on listening level       
//                  - zFLOAT        *pfCenterFreqFB     pointer on the center freqs of bands
//                  - zFLOAT        *pfOuterEarFB       pointer on the transfer of function of outer and middle ear
//                  - zFLOAT        *pfIntNoiseFB       pointer on the internal noise of the ear
//                  - zFLOAT        *pfIntNoise2FB      pointer on the second model of internal noise of the ear
//                  - zFLOAT        *pfFiltFac1FB       pointer on the first lp- filter coeffs
//                  - zFLOAT        *pfFiltFac2FB       pointer on the second lp- filter coeffs
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         pointers on all tables for the filterbank version, in work...
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct eaqsTablesFB
{
    zFLOAT  fScaleFacFB;
    zFLOAT  *pfCenterFreqFB,
            *pfOuterEarFB,
            *pfIntNoise,
            *pfIntNoise2,
            *pfLoudSFB,
            *pfFiltFac1FB,
            *pfFiltFac2FB;
};

/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        eaqsTablesNN
//  @members:       - zFLOAT        *pfNNNormMin        minimum vectors for input scaling
//                  - zFLOAT        *pfNNNormMax        maximum vectors for input scaling
//                  - zFLOAT        **pfNNWeights       pointer on the network weights
//                  - zINT      *piNNSize           pointer on the size of network
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         pointers on all tables for the neural network
//  @creation_date: 11.11.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct eaqsTablesNN
{
    zFLOAT  *pfNNNormMin,
            *pfNNNormMax,
            *pfNNWeights[_NUM_NN_LAYER-1][_NUM_HIDD_NEUR_ADVANCED];
    
    zINT    *piNNSize;
};

/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        eaqsTables
//  @members:       - eaqsTablesFFT stTablesFFT     tables for fft model
//                  - eaqsTablesFB  stTablesFB      tables for fb model
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         pointers on all tables for the basic _and_ advanced version of BS.1387, in work...
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct eaqsTables
{
    eaqsTablesFFT   stTablesFFT;
    eaqsTablesFB    stTablesFB;
    eaqsTablesNN    stTablesNN;
};

/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stGlobalOptions
//  @members:       - zFLOAT            fSPL                    listening level in dB
//                  - zBOOL         bFogetFactorEnabled     Use of forgetting factor for MFPD?
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         options that can be choosen via command line
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct stGlobalOptions
{
    zBOOL   bForgetFactorEnabled;
    zFLOAT  fSPL;
};


/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stWaveFormat
//  @members:       - zFLOAT            fSamplingRate           sampling rate of input signals
//                  - zINT          iNumOfBits              number of bits of input signals
//                  - zINT          iNumOfChannels          number of channels of input signals
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         waveformat of (both!) input signals
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct stWaveFormat
{
    zINT    iNumOfBits,
            iNumOfChannel,
            iSamplingRate;
};


/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stChannelData
//  @members:       - zFLOAT            *pfTimeSignalRef        pointer on reference time buffer
//                  - zFLOAT            *pfTimeSignalTest       pointer on test time buffer
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         pointer on different data arrays
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct stChannelData
{
    zFLOAT  *pfTimeSignalRef,
            *pfTimeSignalTest,

            // psychoacoustic model
            *pfFFTRef,
            *pfFFTTest,
            *pfdBSpecRef,
            *pfdBSpecTest,
            *pfdBSpecWRef,
            *pfdBSpecWTest,
            *pfSpecNoise,
            *pfBarkRef,
            *pfBarkTest,
            *pfBarkNoise,
            *pfFreqSpreadRef,
            *pfFreqSpreadTest,
            *pfSpreadRef,
            *pfSpreadTest,
            *pfdBSpreadRef,
            *pfdBSpreadTest,
            *pfMaskRef,
            *pfEnLastBlockRef,
            *pfEnLastBlockTest,

            // pre- processing
            fLevCorr,
            fLoudRef,
            fLoudTest,
            *pfPPLevSpreadRef,
            *pfPPLevSpreadTest,
            *pfPPLastBlockRef,
            *pfPPLastBlockTest,
            *pfPPRCorrFacNum,
            *pfPPRCorrFacDenom,
            *pfPPPattCorrFacRef,
            *pfPPPattCorrFacTest,
            *pfPattSpreadRef,
            *pfPattSpreadTest,
            *pfPPEnDerivRef,
            *pfPPEnDerivTest,
            *pfPPEnOldRef,
            *pfPPEnOldTest,
            *pfPPEnLoudRef,
            *pfPPEnLoudTest,
            *pfModRef,
            *pfModTest,
            
            // mov calculation
            fTempWt,
            fBWRef,
            fBWTest,
            fEHS,
            fRDF,
            *pfProb,            
            *pfModDiff1,
            *pfModDiff2,
            *pfNoiseLoud,
            *pfNMR,
            *pfStepsAbThr,
            
            // block averaging
            *pfChannelMOVs,
                        
            // time averaging
//          *pfTimeBWRef,
//          *pfTimeBWTest,
//          *pfTimeNMR,
//          *pfTimeModDiff1,
//          *pfTimeEHS,
//          *pfTimeModDiff2,
//          *pfTimeNL,
//          *pfTimeProb,
//          *pfTimeRDF,
//          *pfTimeStepsAbThr,
//          *pfTimeTempWt,
            *pfTimeMOVs[_NUM_MOVS_MAX];
            
};


/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        eaqstEAQUALInstance
//  @members:       - stGlobalOptions       *pGlobalOptions     pointer on the Global options
//                  - stWaveFormat          *pWaveFormat        pointer format of input files
//                  - stChannelData         *pChannelData[]     pointer array on the buffer data
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         pointer on different data arrays
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
struct eaqstEAQUALInstance
{
    // Advanced Model or Basic Model?
    zBOOL               bModelType;

    // Global assessment options
    stGlobalOptions     GlobalOptions;

    // wave format of (both) input files
    stWaveFormat        WaveFormat;

    // input data of input files
    stChannelData       ChannelData[_NUM_CHANNELS_MAX];

    // tables
    eaqsTables          Tables;

    zUINT32             uiFramesProcessed,
                        uiNumEnFrames,
                        uiThreshBoundBeg,
                        uiThreshBoundEnd,
                        uiThreshLoud,
                        uiThreshDel,
                        uiBlocksDistorted;
    
    zFLOAT              fSumStepsAbThr;
        

    zUINT32             *puiThreshEn;


    zFLOAT              *pfMOVs;

};



#endif // #if !defined(__EAQUALCORE_HEADER_INCLUDED__)

