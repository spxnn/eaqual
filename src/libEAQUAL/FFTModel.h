/////////////////////////////////////////////////////////////////////////////////////
// FFTModel.h: interface for the CFFTModel class.
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


#if !defined(__FFTMODEL_HEADER_INCLUDED__)
#define __FFTMODEL_HEADER_INCLUDED__

#include "PsychoModel.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CFFTModel
//  @derived_from:  CPsychoModel
//  @variables:     - zFLOAT        *m_pfNoiseBark      buffer for noise bark spectrum
//                  - zFLOAT        *m_pfNoiseSpec      buffer for difference spectrum
//                  - zFLOAT        *m_pfRefBark        buffer for reference bark spectrum
//                  - zFLOAT        *m_pfRefdBSpec      buffer for log reference spectrum
//                  - zFLOAT        *m_pfRefdBSpecW     buffer for log reference spectrum, weighted with outer and middle ear transfer function
//                  - zFLOAT        *m_pfRefdBSpread    buffer for log smeared reference spectrum
//                  - zFLOAT        *m_pfRefFFT         buffer for fft'd reference audio buffer
//                  - zFLOAT        *m_pfRefFreqSpread  buffer for frequency smeared reference spectrum
//                  - zFLOAT        *m_pfRefMask        buffer for masking threshold
//                  - zFLOAT        *m_pfRefSpread      buffer for smeared reference spectrum (excitation pattern)
//                  - zFLOAT        *m_pfTestBark       buffer for test bark spectrum
//                  - zFLOAT        *m_pfTestdBSpec     buffer for log test spectrum
//                  - zFLOAT        *m_pfTestdBSpecW    buffer for log test spectrum, weighted with outer and middle ear transfer function
//                  - zFLOAT        *m_pfTestdBSpread   buffer for log smeared test spectrum
//                  - zFLOAT        *m_pfTestFFT        buffer for fft'd test audio buffer
//                  - zFLOAT        *m_pfTestFreqSpread buffer for frequency smeared test spectrum
//                  - zFLOAT        *m_pfTestSpread     buffer for smeared test spectrum (excitation pattern)
//  @methods:       - fmAddMask
//                  - fmCalcNoise
//                  - fmCalcNoise
//                  - fmCalcPower
//                  - fmFFT
//                  - fmFreqSpread
//                  - fmGroupInBands
//                  - fmLogSpec
//                  - fmScale
//                  - fmTimeSpread
//                  -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         implements FFT- based psychomodel
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CFFTModel : public CPsychoModel  
{
private:

    zFLOAT  *m_pfRefFFT[_NUM_CHANNELS_MAX],
            *m_pfTestFFT[_NUM_CHANNELS_MAX],
            *m_pfRefdBSpec[_NUM_CHANNELS_MAX],
            *m_pfTestdBSpec[_NUM_CHANNELS_MAX],
            *m_pfRefdBSpecW[_NUM_CHANNELS_MAX],
            *m_pfTestdBSpecW[_NUM_CHANNELS_MAX],
            *m_pfNoiseSpec[_NUM_CHANNELS_MAX],
            *m_pfRefBark[_NUM_CHANNELS_MAX],
            *m_pfTestBark[_NUM_CHANNELS_MAX],
            *m_pfNoiseBark[_NUM_CHANNELS_MAX],
            *m_pfTestFreqSpread[_NUM_CHANNELS_MAX],
            *m_pfRefFreqSpread[_NUM_CHANNELS_MAX],
            *m_pfRefSpread[_NUM_CHANNELS_MAX],
            *m_pfTestSpread[_NUM_CHANNELS_MAX],
            *m_pfRefdBSpread[_NUM_CHANNELS_MAX],
            *m_pfTestdBSpread[_NUM_CHANNELS_MAX],
            *m_pfRefMask[_NUM_CHANNELS_MAX];
    
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
    inline  void    fmFFT(          zFLOAT      *pfFFTData);

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
    inline  void    fmScale(        zFLOAT      *pfAudioData);

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
    inline  void    fmCalcPower(    zFLOAT      *pfSpecData);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fmLogSpec
    //  @parameter:     - zFLOAT    *pfSpecData     pointer on spectral data with length iLength
    //                  - zFLOAT    *pfdBData       pointer on resulting buffer for log spectrum
    //                  - zINT  iLength         length of vectors
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates dB spectrum of FFT values
    //  @creation_date: 09.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    fmLogSpec(      zFLOAT      *pfEnData, 
                                    zFLOAT      *pfdBData, 
                                    zINT        iLength);

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
    inline  void    fmCalcNoise(    zFLOAT      *pfRefSpec, 
                                    zFLOAT      *pfTestSpec, 
                                    zFLOAT      *pfNoiseSpec);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fmGroupInBands
    //  @parameter:     - zFLOAT    *pfSpecData     pointer on buffer with power spectrum (length: half FFT length)
    //                  - zFLOAT    *pfBandData     output: pointer on buffer with grouped freq values
    //                  - zINT  iSampleRate     Sample rate of audio signal
    //                  - zINT  iNumOfBands     number of resulting freq bands
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         grouping power spectrum in bands (BS.1387, pp.204-211)
    //  @creation_date: 26.06.2000
    //  @last_modified: 21.07.2000  AL: optimized (compiler switch)
    /////////////////////////////////////////////////////////////////////////////////////
            void    fmGroupInBands( zFLOAT      *pfSpecData, 
                                    zFLOAT      *pfBandData, 
                                    zINT        iSampleRate, 
                                    zINT        iNumOfBands);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fmGroupInBands
    //  @parameter:     - zFLOAT        *pfEn           energy spectrum to be spreaded
    //                  - zFLOAT        *pfEnSpread     resulting spreaded spectrum
    //                  - zBOOL     bModelType      basic or advanced model
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         computes SpreadSpectrum (frequency spreading) of energy
    //  @creation_date: 02.04.2000
    //  @last_modified: 12.10.2001: cosmetic changes in table usage (tables->...)
    /////////////////////////////////////////////////////////////////////////////////////
            void    fmFreqSpread(   zFLOAT      *pfEn, 
                                    zFLOAT      *pfEnSpread, 
                                    zBOOL       bModelType);

            
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
            void    fmTimeSpread(   stChannelData   *pChannelData, 
                                    zFLOAT          *pfFiltFac, 
                                    zINT            iNumOfBands);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fmAddMask
    //  @parameter:     - zFLOAT        *pfSpecData     pointer on threshold
    //                  - zFLOAT        *pfMask         pointer on masking threshold
    //                  - zINT      iNumOfBands     number of freq bands
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         adding weighting function for masking to threshold (BS.1387, p.214)
    //  @creation_date: 06.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    fmAddMask(      zFLOAT      *pfSpecData, 
                                    zFLOAT      *pfMask,
                                    zINT        iNumOfBands);

public:
    CFFTModel(          eaqstEAQUALInstance     *pInstance);
    virtual ~CFFTModel();

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
    zERROR          Process(        eaqstEAQUALInstance     *pInstance);
        
};

#endif // !defined(__FFTMODEL_HEADER_INCLUDED__)
