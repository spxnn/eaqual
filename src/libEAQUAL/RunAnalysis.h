/////////////////////////////////////////////////////////////////////////////////////
// RunAnalysis.h: interface for the CRunAnalysis class.
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


#if !defined(__RUNANALYSIS_HEADER_INCLUDED__)
#define __RUNANALYSIS_HEADER_INCLUDED__

#include "AvgBlock.h"
#include "AvgTime.h"
#include "CalcTables.h"
#include "FBModel.h"
#include "FFTModel.h"
#include "KogModel.h"
#include "MOVBlock.h"
#include "PreProc.h"
#include "FileIO.h"
#include "BasicUtils.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CRunAnalysis
//  @derived_from:  CBasicUtils
//  @variables:     - zBOOL                 bStopProcessing         should the processing being stopped?
//                  - eaqstEAQUALInstance   EAQUALInstance          instance data
//                  - zFLOAT                m_afTimeSignalRef       reference audio data buffer
//                  - zFLOAT                m_afTimeSignalTest      test audio data buffer
//                  - zBOOL                 m_bBeginOfFile          is it the first call?
//                  - zBOOL                 m_ProcessingBlockFinished is the block processing finished or not?
//                  -CAvgBlock              *m_pCAvgBlock           pointer on avgblock class
//                  -CAvgTime               *m_pCAvgTime            pointer on avgtime class
//                  -CCalcTables            *m_pCCalcTables         pointer on tables class
//                  -CFBModel               *m_pCFBModel            pointer on fbbased psychomodel
//                  -CFFTModel              *m_pCFFTModel           pointer on fft based psychomodel
//                  -CFileIO                *m_pCFileIO             pointer on file in/out class
//                  -CKogModel              *m_pCKogModel           pointer on neural network class
//                  -CMOVBlock              *m_pCMOVBlock           pointer on MOV calc class
//                  -CPreProc               *m_pCPreProc            pointer on preprocessing class
//  @methods:       - raFinalProcess
//                  - raInit
//                  - raProcess
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         base class for psychomodels
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CRunAnalysis  : public CBasicUtils
{
private:
    eaqstEAQUALInstance EAQUALInstance;

    CAvgBlock   *m_pCAvgBlock;
    CAvgTime    *m_pCAvgTime;
    CCalcTables *m_pCCalcTables;
    CFBModel    *m_pCFBModel;
    CFFTModel   *m_pCFFTModel;
    CKogModel   *m_pCKogModel;
    CMOVBlock   *m_pCMOVBlock;
    CPreProc    *m_pCPreProc;
    CFileIO     *m_pCFileIO;

    zBOOL       m_bBeginOfFile,
                m_ProcessingBlockFinished;
    zFLOAT      m_afTimeSignalRef[_NUM_CHANNELS_MAX][_FFT_LENGTH],
                m_afTimeSignalTest[_NUM_CHANNELS_MAX][_FFT_LENGTH];
    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        raCalcChannelMOVs
    //  @parameter:     - 
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculate block MOVs per channels   
    //  @creation_date: 06.01.2002
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR raCalcChannelMOVs();


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        raSetOutputPointers
    //  @parameter:     - stBlockResults    *pstResults     pointer on structure of results, to be written
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         set the pointers for block output   
    //  @creation_date: 06.01.2002
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR raSetOutputPointers( stBlockResults  *pstResults);
    
public:

    zBOOL       bStopProcessing;


    CRunAnalysis();
    
    virtual ~CRunAnalysis();

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        raProcess
    //  @parameter:     - stAudioData *pAudioData   input audio data ref and test
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         process of one block of data    
    //  @creation_date: 09.12.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  raProcess(  stAudioData *pAudioData);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        raFinalProcess
    //  @parameter:     - zFLOAT        **pResults      pointer on pointer on the results, to be written
    //                  - zINT      *pResultLength  pointer on the length of pResults, to be written
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         final processing of data and output of data 
    //  @creation_date: 16.12.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR raFinalProcess(  zFLOAT  **pResults, 
                            zINT    *pResultLength);
    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        raInit
    //  @parameter:     - stCLOptions       *pOptions       pointer struct with CL Options
    //                  - WaveFormat            *pWaveFormat    pointer Waveformat
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         initialize RunAnalysis class    
    //  @creation_date: 16.12.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  raInit(     stCLOptions *pOptions, 
                        WaveFormat  *pWaveFormat);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        raGetBlockResults
    //  @parameter:     - stBlockResults    *pstResults     pointer on structure of results, to be written
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         get results of one audio block  
    //  @creation_date: 06.01.2002
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR raGetBlockResults(   stBlockResults  *pstResults);
};

#endif // !defined(__RUNANALYSIS_HEADER_INCLUDED__)
