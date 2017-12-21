/////////////////////////////////////////////////////////////////////////////////////
// CalcTables.h: interface for the CCalcTables class.
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


#if !defined(__CALCTABLES_HEADER_INCLUDED__)
#define __CALCTABLES_HEADER_INCLUDED__

#include "BasicUtils.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CCalcTables
//  @derived_from:  CBasicUtils
//  @variables:     - zINT  m_aiNNSize          dimensions of neural network
//                  - zFLOAT    m_fScaleFac         scale factor for listening level (FFT model)
//                  - zFLOAT    m_fScaleFacFB       scale factor for listening level (FB model)
//                  - zFLOAT    *m_pfCenterFreq     center freqs of bands (FFT Model)
//                  - zFLOAT    *m_pfCenterFreqFB   center freqs of bands (FB Model)
//                  - zFLOAT    *m_pfFiltFac1       filter coeffs (FFT Model)
//                  - zFLOAT    *m_pfFiltFac1FB     filter coeffs (FB Model)
//                  - zFLOAT    *m_pfFiltFac2       filter coeffs (FFT Model)
//                  - zFLOAT    *m_pfFiltFac2FB     filter coeffs (FB Model)
//                  - zFLOAT    *m_pfHannWin        hanning window of size _FFT_LENGTH
//                  - zFLOAT    *m_pfIntNoise       internal noise of ear (fft model)
//                  - zFLOAT    *m_pfIntNoise2      internal noise of ear (fft model), different scaling
//                  - zFLOAT    *m_pfIntNoiseFB     internal noise of ear (fb model)
//                  - zFLOAT    *m_pfIntNoise2FB    internal noise of ear (fb model), different scaling
//                  - zFLOAT    *m_pfLoudS          table for calculating excitation patterns
//                  - zFLOAT    *m_pfLowerBound     freq table for lower band bounds
//                  - zFLOAT    *m_pfMaskWeight     masking index
//                  - zFLOAT    *m_fNNNormMax       input scaling for neural network
//                  - zFLOAT    *m_fNNNormMin       input scaling for neural network
//                  - zFLOAT    *m_fNNWeights       weights for neural network
//                  - zFLOAT    *m_pfNormSpread     normalization for freq spreading
//                  - zFLOAT    *m_pfOuterEar       transfer function of outer and middle ear (fft model)
//                  - zFLOAT    *m_pfOuterEarFB     transfer function of outer and middle ear (fb model)
//                  - zFLOAT    *m_pfSlopeSpread        table for freq spreading
//                  - zFLOAT    *m_pfUpperBound     freq table for upper band bounds
//  @methods:       - ctCalcLP
//                  - ctCalcSpreadTables
//                  - ctDelTables
//                  - ctGetBandLimits
//                  - ctGetCenterFreq
//                  - ctGetNNTables
//                  - ctHannWindow
//                  - cdInitTables
//                  - ctInternNoise
//                  - ctInternNoise2
//                  - ctLowPassCoeffs
//                  - cdNormSpread
//                  - ctOuterEar
//                  - ctScaleFac
//                  - ctSetTablePointers
//                  - ctSLoudness
//                  - ctSpreadSlopeUp
//                  - ctWeightFunc
//                  -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate all tables for later use
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CCalcTables : public CBasicUtils  
{
private:

    // member variables
    zFLOAT      m_fScaleFac,
                m_fScaleFacFB,
                // arrays
                *m_pfHannWin,
                *m_pfOuterEar,
                *m_pfOuterEarFB,
                *m_pfNormSpread,
                *m_pfSlopeSpread,
                *m_pfMaskWeight,
                
                *m_pfIntNoise,
                *m_pfIntNoiseFB,
                *m_pfIntNoise2,
                *m_pfIntNoise2FB,
                *m_pfFiltFac1,
                *m_pfFiltFac1FB,
                *m_pfFiltFac2,
                *m_pfFiltFac2FB,
                *m_pfLoudS,
                
                *m_pfCenterFreq,            //given table
                *m_pfCenterFreqFB,          //given table
                *m_pfLowerBound,            //given table
                *m_pfUpperBound,            //given table

                *m_pfNNWeights[_NUM_NN_LAYER-1][_NUM_HIDD_NEUR_ADVANCED],               //given table
                *m_pfNNNormMin,             //given table
                *m_pfNNNormMax;             //given table
                
    zINT        m_aiNNSize[_NUM_NN_LAYER];  //given table
        

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
    zERROR  ctInitTables(   zBOOL   bModelType);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        ctInitTables
    //  @parameter:     -
    //  @result:        zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         free table memory 
    //  @creation_date: 10.11.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR CCalcTables::ctDelTables();

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        ctHannWindow
    //  @parameter:     - zUINT32    uiHalfFFTLength    length of half hann window
    //  @result:        zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calc the the hanning window 
    //  @creation_date: 10.11.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  ctHannWindow(   zUINT32                 uiHalfFFTLength);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        ctabScaleFac
    //  @parameter:     - zFLOAT     fListLevel listening level
    //  @result:        zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calc the the hanning window according BS.1387, p.203, eq.(2)
    //  @creation_date: 27.06.2000
    //  @last_modified: 10.11.2001 AL: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  ctScaleFac(     zFLOAT                  fListLevel, 
                            zBOOL                   bModelType, 
                            zUINT32                 uiSampleRate);

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
    zERROR  ctGetCenterFreq(    zBOOL               bModelType);

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
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  ctOuterEar(         zBOOL               bModelType, 
                                zINT                iSampleRate);


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
    zERROR  ctGetBandLimits(    zBOOL               bModelType);


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
    zERROR  ctInternNoise(      zBOOL               bModelType);
    
    
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
    zERROR  ctInternNoise2(     zBOOL               bModelType);

    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        ctLowPassCoeffs
    //  @parameter:     zBOOL       bModelType  basic or advanced model
    //                  zINT        iSampleRate sampling rate of input audio    
    //  @result:        zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates filter coeffs 
    //  @creation_date: 11.11.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  ctLowPassCoeffs(    zBOOL               bModelType, 
                                zINT                iSampleRate);

    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        ctCalcLowPass
    //  @parameter:     zFLOAT      pfFiltFac   pointer on resulting coefficients
    //                  zFLOAT      fTau100     filter constant,
    //                  zFLOAT      fTaumin     filter constant,
    //                  zINT        iStepSize   1024 for FFT- based Model, 192 for FB- based model --> time resolution  
    //                  zINT        iSampleRate sampling rate of input audio    
    //                  zINT        iLength     length of vector pfFiltFac  
    //  @result:        zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates filter coeffs according BS.1387, eq (24) and (44)
    //  @creation_date: 25.03.2000
    //  @last_modified: 11.11.2001 AL: changed parameters
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  ctCalcLP(           zFLOAT              *pfFiltFac, 
                                zFLOAT              fTau100, 
                                zFLOAT              fTaumin, 
                                zINT                iStepSize, 
                                zINT                iSampleRate, 
                                zINT                iLength);

    
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
    zERROR  ctCalcSpreadTables( zBOOL               bModelType);


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
    zERROR  ctSpreadSlopeUp(    zFLOAT              fBarkRes, 
                                zINT                iLength);
        

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
    zERROR  ctNormSpread(       zFLOAT              fBarkRes, 
                                zINT                iLength);

    
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
    zERROR  ctWeightFunc(       zBOOL               bModelType);

    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        ctSLoudness
    //  @parameter:     - zBOOL     bModelType      basic or advanced model
    //  @result:        zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculate weighting function for masking threshold (BS.1387, p.214)
    //  @creation_date: 27.06.2000
    //  @last_modified: 13.07.2000
    //                  11.11.2001 AL: changed parameter
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  ctSLoudness(        zBOOL               bModelType);

    
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
    zERROR  ctGetNNTables(      zBOOL               bModelType);

        
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
    zERROR  ctSetTablePointers( zBOOL               bModelType, 
                                eaqsTables          *pstTables);

        
public:
    CCalcTables(            zBOOL bModelType);
    virtual ~CCalcTables();

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
    zERROR  Process (   eaqstEAQUALInstance     *pInstance);


};

#endif // !defined(__CALCTABLES_HEADER_INCLUDED__)
