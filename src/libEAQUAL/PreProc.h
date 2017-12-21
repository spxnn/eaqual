/////////////////////////////////////////////////////////////////////////////////////
// PreProc.h: interface for the CPreProc class.
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


#if !defined(__PREPROC_HEADER_INCLUDED__)
#define __PREPROC_HEADER_INCLUDED__

#include "ProcessBlock.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CPreProc
//  @derived_from:  CProcessBlock
//  @variables:     - zFLOAT    *m_pfRCorrFacDenom      pointer on denominator of factor R (pattern adaptation)     
//                  - zFLOAT    *m_pfRCorrFacNum        pointer on numerator of factor R (pattern adaptation)
//                  - zFLOAT    *m_pfRefEnDeriv         pointer on low passed temporal derivation for modulation
//                  - zFLOAT    *m_pfRefEnLoud          pointer on loudness estimation for modulation
//                  - zFLOAT    *m_pfRefEnOld           pointer on low passed unsmeared excitation patterns for modulation
//                  - zFLOAT    *m_pfRefLastBlock       pointer on low passed excitation patterns for level adaptation
//                  - zFLOAT    *m_pfRefLevSpread       pointer on level adapted patterns
//                  - zFLOAT    *m_pfRefMod             pointer on modulation
//                  - zFLOAT    *m_pfRefPattCorrFac     pointer on correction factor for pattern adaptation
//                  - zFLOAT    *m_pfRefPattSpread      pointer on spectrally adapted patterns
//                  - zFLOAT    *m_pfTestEnDeriv        pointer on low passed temporal derivation for modulation
//                  - zFLOAT    *m_pfTestRefEnLoud      pointer on loudness estimation for modulation
//                  - zFLOAT    *m_pfTestEnOld          pointer on low passed unsmeared excitation patterns for modulation
//                  - zFLOAT    *m_pfTestLastBlock      pointer on low passed excitation patterns for level adaptation
//                  - zFLOAT    *m_pfTestLevSpread      pointer on level adapted patterns
//                  - zFLOAT    *m_pfTestMod            pointer on modulation
//                  - zFLOAT    *m_pfTestPattCorrFac    pointer on correction factor for pattern adaptation
//  @methods:       - ppLevAdapt
//                  - ppLoudness
//                  - ppLowPass
//                  - ppModulation
//                  - ppPattAdapt
//                  -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         preprocessing of excitation patterns
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CPreProc : public CProcessBlock  
{
private:
    zFLOAT  *m_pfRefLastBlock[_NUM_CHANNELS_MAX],
            *m_pfTestLastBlock[_NUM_CHANNELS_MAX],
            *m_pfRefLevSpread[_NUM_CHANNELS_MAX],
            *m_pfTestLevSpread[_NUM_CHANNELS_MAX],
            *m_pfRCorrFacNum[_NUM_CHANNELS_MAX],
            *m_pfRCorrFacDenom[_NUM_CHANNELS_MAX],
            *m_pfRefPattCorrFac[_NUM_CHANNELS_MAX],
            *m_pfTestPattCorrFac[_NUM_CHANNELS_MAX],
            *m_pfRefPattSpread[_NUM_CHANNELS_MAX],
            *m_pfTestPattSpread[_NUM_CHANNELS_MAX],
            *m_pfRefEnDeriv[_NUM_CHANNELS_MAX],
            *m_pfTestEnDeriv[_NUM_CHANNELS_MAX],
            *m_pfRefEnOld[_NUM_CHANNELS_MAX],
            *m_pfTestEnOld[_NUM_CHANNELS_MAX],
            *m_pfRefEnLoud[_NUM_CHANNELS_MAX],
            *m_pfTestEnLoud[_NUM_CHANNELS_MAX],
            *m_pfRefMod[_NUM_CHANNELS_MAX],
            *m_pfTestMod[_NUM_CHANNELS_MAX];


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
    zFLOAT      ppLowPass(  stChannelData   *pChannel, 
                            zFLOAT          *pfFiltFac, 
                            zINT            iNumOfBands);

    
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
    zERROR      ppLevAdapt( stChannelData   *pChannel, 
                            zINT            iNumOfBands);

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
    zERROR      ppPattAdapt(stChannelData   *pChannelData, 
                            zFLOAT          *pfFilFac, 
                            zINT            iNumOfBands);

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
    zERROR      ppModulation(stChannelData  *pChannelData, 
                            zFLOAT          *pfFiltFac, 
                            zINT            iSampleRate, 
                            zINT            iNumOfBands);

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
    zFLOAT      ppLoudness( zFLOAT          *pfEnSpec, 
                            eaqsTables      *pTables, 
                            zINT            iNumOfBands);

public:
    CPreProc(   eaqstEAQUALInstance     *pInstance);
    virtual ~CPreProc();

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
    zERROR      Process(    eaqstEAQUALInstance *pInstance);

};

#endif // !defined(__PREPROC_HEADER_INCLUDED__)
