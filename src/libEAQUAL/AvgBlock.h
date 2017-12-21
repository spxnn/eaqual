/////////////////////////////////////////////////////////////////////////////////////
// AvgBlock.h: interface for the CAvgBlock class.
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


#if !defined(__AVGBLOCK_HEADER_INCLUDED__)
#define __AVGBLOCK_HEADER_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Avg.h"


#define _DATA_BOUND_THRESH  200
#define _NUM_ADD_SAMPLES    5
#define _ENERGY_THRESHOLD   16000.0F


/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CAvgBlock
//  @derived_from:  CAvg
//  @variables:     - zBOOL     m_bBeginOfFile      first block or not?
//                  - zUINT32   *m_puiThreshEn      pointer for valid EHS averaging
//                  - zFLOAT        *m_pfChannelMOVs    pointer on temporary results
//  @methods:       - avDataBound
//                  - avEnThresh
//                  - avLoudThresh
//                  - avProbAvg
//                  - avStepAvg
//                  -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         all for averaging block data
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CAvgBlock : public CAvg  
{
private:

    zBOOL   m_bBeginOfFile;

    zUINT32 *m_puiThreshEn;

    zFLOAT  *m_pfChannelMOVs[_NUM_CHANNELS_MAX];

    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avProbAvg
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //                  - zINT  iNumOfBands     length of vector    
    //  @result:        - zFLOAT    fProb           resulting average of probability
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates binaural spectral average according BS.1387, eqs. (79),(81) 
    //  @creation_date: 14.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avProbAvg(      eaqstEAQUALInstance     *pInstance, 
                                    zINT                    iNumOfBands);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avStepAvg
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //                  - zINT  iNumOfBands     length of vector    
    //  @result:        - zFLOAT    fSteps          resulting average of steps above threshold
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates binaural spectral average according BS.1387, eqs. (80),(82) 
    //  @creation_date: 14.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avStepAvg(      eaqstEAQUALInstance     *pInstance, 
                                    zINT                    iNumOfBands);


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avDataBound
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //                  - zINT  iLength         length of time buffer   
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         set flags for Data boundary according BS.1387, p.238, 5.2.4.4 
    //  @creation_date: 12.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zERROR  avDataBound(    eaqstEAQUALInstance     *pInstance, 
                                    zINT                    iLength);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avEnThresh
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //                  - zUINT32               uiLength    length of time buffer   
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         set Flags for Energy threshold according BS.1387, p.238, 5.2.4.3
    //  @creation_date: 18.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zERROR  avEnThresh(     eaqstEAQUALInstance     *pInstance, 
                                    zUINT32                 uiLength);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avLoudThresh
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         set Flags for Loudness threshold according BS.1387, p.238, 5.2.4.2
    //  @creation_date: 13.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zERROR  avLoudThresh(   eaqstEAQUALInstance     *pInstance);
                
public:
    CAvgBlock(eaqstEAQUALInstance *pInstance);
    virtual ~CAvgBlock();

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        Process
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates the MOVs per block and other needed variables 
    //  @creation_date: 15.02.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zERROR  Process(        eaqstEAQUALInstance     *pInstance);
        
};

#endif // !defined(__AVGBLOCK_HEADER_INCLUDED__)
