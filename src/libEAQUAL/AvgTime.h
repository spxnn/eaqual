/////////////////////////////////////////////////////////////////////////////////////
// AvgTime.h: interface for the CAvgTime class.
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


#if !defined(__AVGTIME_HEADER_INCLUDED__)
#define __AVGTIME_HEADER_INCLUDED__

#include "Avg.h"

#define _AVG_WIN_LENGTH     4


/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CAvgTime
//  @derived_from:  CAvg
//  @variables:     - zFLOAT    *m_pfMOVs   final Model Output Variables
//  @methods:       - avADB
//                  - avBWAvg
//                  - avChannelAvg
//                  - avEHSAvg
//                  - avLinAvg (overloaded)
//                  - avMFPD
//                  - avNLAvg
//                  - avWinAvg
//                  -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         all for averaging time data
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CAvgTime : public CAvg  
{
private:

    zFLOAT  *m_pfMOVs;


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avLinAvg
    //  @parameter:     - zFLOAT *pfMOV pointer on vector on which the average is calulated
    //                  - zINT  iBegIdx     begin index for averaging   
    //                  - zINT  iEndIdx     end index for averaging
    //  @result:        - zFLOAT    fMOVAvg     result of averaging
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates linear average according BS.1387, eq. (89), overloaded
    //  @creation_date: 13.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  zFLOAT  avLinAvg(       zFLOAT      *pfMOV, 
                                    zINT        iBegIdx,    
                                    zINT        iEndIdx);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avLinAvg
    //  @parameter:     - zFLOAT *pfMOV     pointer on vector on which the average is calulated
    //                  - zFLOAT    *pfWeight   vector with Weights for temporal averaging of length this->uiFramesProcessed
    //                  - zINT  iBegIdx     begin index for averaging   
    //                  - zINT  iEndIdx     end index for averaging
    //  @result:        - zFLOAT    fMOVAvg     result of averaging
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates linear average according BS.1387, eq. (90), overloaded
    //  @creation_date: 13.07.2000
    //  @last_modified: 01.11.2001 AL: possible division by zero removed
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avLinAvg(       zFLOAT      *pfMOV, 
                                    zFLOAT      *pfWeight, 
                                    zINT        iBegIdx, 
                                    zINT        iEndIdx);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avEHSAvg
    //  @parameter:     - zFLOAT        *pfEHS          pointer on vector on which the average is calulated
    //                  - zUINT32   *puiThreshEn    which values are not valid for averaging?   
    //                  - zUINT32   uiBegIdx        begin index for averaging   
    //                  - zUINT32   uiEndIdx        end index for averaging
    //                  - zUINT32   uiNumEnFrames   number of Frames to be averaged
    //  @result:        - zFLOAT        fEHS            result of averaging
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates linear average of EHS according BS.1387, eq. (89) and chapter 5.2.4.3
    //  @creation_date: 18.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avEHSAvg(       zFLOAT      *pfEHS, 
                                    zUINT32     *puiThreshEn, 
                                    zUINT32     uiBegIdx, 
                                    zUINT32     uiEndIdx, 
                                    zUINT32     uiNumEnFrames);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avWinAvg
    //  @parameter:     - zFLOAT        *pfMOV      pointer on vector on which the average is calulated
    //                  - zUINT32   uiBegIdx    begin index for averaging   
    //                  - zUINT32   uiEndIdx    end index for averaging
    //  @result:        - zFLOAT    fMOVAvg     resulting window- averaged MOV
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates windowed average according BS.1387, eq. (93)
    //  @creation_date: 13.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avWinAvg(       zFLOAT      *pfMOV, 
                                    zUINT32     uiBegIdx, 
                                    zUINT32     uiEndIdx);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avNLAvg
    //  @parameter:     - zFLOAT *pfMOV     pointer on vector on which the average is calulated
    //                  - zINT  iBegIdx     begin index for averaging   
    //                  - zINT  iEndIdx     end index for averaging
    //                  - zBOOL bModelType  basic or advanced model
    //  @result:        - zFLOAT    fMOVAvg     resulting rms- averaged MOV
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates rms of NL according BS.1387, eq. (91) and table (12)
    //  @creation_date: 07.08.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avNLAvg(        zFLOAT      *pfMOV, 
                                    zINT        iBegIdx, 
                                    zINT        iEndIdx,    
                                    zBOOL       bModelType);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avNLAvg
    //  @parameter:     - zFLOAT *pfBWRef       pointer on reference Bandwidth over time vector
    //                  - zFLOAT *pfBWTest      pointer on test bandwidth over time vector
    //                  - zFLOAT *pBW           pointer on resulting bandwidths
    //                  - zINT  iSampleRate     sampling rate of input audio files
    //                  - zINT  iBegIdx         begin index for averaging   
    //                  - zINT  iEndIdx         end index for averaging
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates linear average of Bandwidth according BS.1387, eq. (89) and chapter 4.4.2
    //  @creation_date: 18.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zERROR  avBWAvg(        zFLOAT      *pfBWRef, 
                                    zFLOAT      *pfBWTest, 
                                    zFLOAT      *pBW, 
                                    zINT        iSampleRate, 
                                    zINT        iBegIdx, 
                                    zINT        iEndIdx);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avADB
    //  @parameter:     - zINT      iNumDistorted   number of distorted blocks
    //                  - zFLOAT        fStepsAbThr     number of steps above threshold 
    //  @result:        - zFLOAT                        resulting ADB
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates Model Output Variable ADB according to BS.1387, p.235
    //  @creation_date: 18.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avADB(          zINT        iNumDistorted, 
                                    zFLOAT      fStepsAbThr);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avMFPD
    //  @parameter:     - zFLOAT *pfMOV                 pointer on detection probability
    //                  - zINT  iBegIdx                 begin index for averaging   
    //                  - zINT  iEndIdx                 end index for averaging
    //                  - zBOOL bForgetFactorEnabled    is the forgetting factor (c_1, eq. (85)) enabled?
    //  @result:        - zFLOAT    fPDet                   MFPD
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates Model Output Variable MFPD according to BS.1387, pp.234+235
    //  @creation_date: 20.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zFLOAT  avMFPD(         zFLOAT              *pfMOV, 
                                    zINT                iBegIdx, 
                                    zINT                iEndIdx, 
                                    zBOOL               bForgetFactorEnabled);

                
public:
    CAvgTime(eaqstEAQUALInstance *pInstance);
    virtual ~CAvgTime();


                
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avBWAvg
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates linear average of Bandwidth according BS.1387, eq. (89) and chapter 4.4.2
    //  @creation_date: 09.12.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zERROR  avChannelAvg(   eaqstEAQUALInstance *pInstance);

            
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        Process
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data of EAQUAL
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates the MOVs averaged over time and channels 
    //  @creation_date: 15.02.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
            zERROR  Process(    eaqstEAQUALInstance *pInstance);

};

#endif // !defined(__AVGTIME_HEADER_INCLUDED__)
