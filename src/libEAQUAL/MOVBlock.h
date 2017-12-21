/////////////////////////////////////////////////////////////////////////////////////
// MOVBlock.h: interface for the CMOVBlock class.
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


#if !defined(__MOVBLOCK_HEADER_INCLUDED__)
#define __MOVBLOCK_HEADER_INCLUDED__

#include "ProcessBlock.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CMOVBlock
//  @derived_from:  CProcessBlock
//  @variables:     - zFLOAT    *m_afMOVs       temporarily MOVs per channel        
//                  - zFLOAT    *m_pfModDiff1   pointer on spectrum of modulation difference 1
//                  - zFLOAT    *m_pfModDiff2   pointer on spectrum of modulation difference 2
//                  - zFLOAT    *m_pfNMR        pointer on spectrum of NMR
//                  - zFLOAT    *m_pfNoiseLoud  pointer on spectrum of noise loudness
//                  - zFLOAT    *m_pfProb       pointer on spectrum of detection probability
//                  - zFLOAT    *m_StepsAbThr   pointer on spectrum of steps above threshold
//  @methods:       - mobBandwidth
//                  - mobDetProb
//                  - mobEHS
//                  - mobModDiff
//                  - mobNMR
//                  - mobNoiseLoud
//                  - mobRDF
//                  -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         transformation from MOVs to ODG
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CMOVBlock : public CProcessBlock  
{
private:

    zFLOAT      *m_pfModDiff1[_NUM_CHANNELS_MAX],
                *m_pfModDiff2[_NUM_CHANNELS_MAX],
                *m_pfNoiseLoud[_NUM_CHANNELS_MAX],
                *m_pfProb[_NUM_CHANNELS_MAX],
                *m_pfStepsAbThr[_NUM_CHANNELS_MAX],
                *m_pfNMR[_NUM_CHANNELS_MAX],
                m_afMOVs[_NUM_CHANNELS_MAX][_NUM_MOV_BASIC];


    
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
    //  @notes:         calculates Model Output Variable Bandwidth according to BS.1387, pp.228+229
    //  @creation_date: 06.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR      mobModDiff(     stChannelData       *pChannelData, 
                                zFLOAT              *pfIntNoise, 
                                zBOOL               bModelType, 
                                zINT                iType);

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
    zERROR      mobNoiseLoud(   stChannelData       *pChannelData, 
                                zFLOAT              *pfIntNoise, 
                                char                *cMOV);

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
    inline  void    mobNMR(     zFLOAT              *pfNMR, 
                                zFLOAT              *pfNoiseSpec, 
                                zFLOAT              *pfMaskSpec, 
                                zINT                iNumOfBands);
    
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
    zERROR          mobDetProb( stChannelData       *pChannelData);


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
    zERROR          mobBandwidth(stChannelData      *pChannelData, 
                                zINT                iSampleRate);

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
    inline zFLOAT   mobRDF(     zFLOAT              *pfNMRSpec, 
                                zINT                iNumOfBands);

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
    zERROR          mobEHS(     stChannelData       *pChannelData, 
                                zFLOAT              *pfHannWin, 
                                zINT                iSampleRate);


public:
    CMOVBlock(                  eaqstEAQUALInstance *pInstance);
    virtual ~CMOVBlock();

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
    zERROR          Process(    eaqstEAQUALInstance *pInstance);

};

#endif // !defined(__MOVBLOCK_HEADER_INCLUDED__)
