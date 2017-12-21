/////////////////////////////////////////////////////////////////////////////////////
// libEAQUAL.cpp: API functions for EAQUAL library.
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


#include "EAQUALCore.h"
#include "libEAQUAL.h"
#include "RunAnalysis.h"


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualCreateInstance
//  @parameter:     -
//  @result:        -void           *pInstance  handle to new instance
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         create new instance of EAQUAL 
//  @creation_date: 14.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void* EAQUALAPI eaqualCreateInstance()
{
    CRunAnalysis    *pRunAnalysis = new CRunAnalysis();
    
    return ((void*)(pRunAnalysis));
}

/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualInitInstance
//  @parameter:     -void           *pInstance  handle to instance pointer
//                  - stCLOptions   *pCLOptions see libEAQUAL.h
//                  - WaveFormat        *pWFormat   pointer on audio format
//  @result:        - unsigned int  error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         process new block 
//  @creation_date: 15.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int    EAQUALAPI   eaqualInitInstance(void *pInstance, stCLOptions *pCLOptions, WaveFormat *pWFormat)
{
    CRunAnalysis *pInstancePointer  = (CRunAnalysis*)(pInstance);
    
    return (pInstancePointer->raInit(pCLOptions, pWFormat));
}
/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualKillInstance
//  @parameter:     -void           *pInstance  handle to instance to delete
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         delete specified instance of EAQUAL 
//  @creation_date: 14.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void EAQUALAPI  eaqualKillInstance(void *pInstance)
{
    delete ((CRunAnalysis*)(pInstance));
    return;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualEndInstance
//  @parameter:     -void           *pInstance  handle to instance pointer
//                  - float         **pfResults pointer on results
//                  - int           length of pfResults, to be written
//  @result:        - unsigned int  error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         get the overall process results and quits processing
//  @creation_date: 14.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int EAQUALAPI  eaqualEndInstance(void *pInstance, double **pfResults, int *pResultLength)
{
    CRunAnalysis *pInstancePointer  = (CRunAnalysis*)(pInstance);
    
    pInstancePointer->bStopProcessing = _TRUE;
    
    return (pInstancePointer->raFinalProcess (pfResults, pResultLength));
}


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualProcess
//  @parameter:     -void           *pInstance  handle to instance pointer
//                  - stAudioData   *pAudioData pointer on audio data of both files, ref and test
//  @result:        - unsigned int  error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         process new block 
//  @creation_date: 14.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int EAQUALAPI  eaqualProcessInstance(void *pInstance, stAudioData *pAudioData)
{
    CRunAnalysis *pInstancePointer  = (CRunAnalysis*)(pInstance);
    
    return (pInstancePointer->raProcess(pAudioData));
}


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualGetBlockResults
//  @parameter:     - void                  *pInstance          handle to instance to delete
//                  - stBlockResults        *pstBlockResults    pointe ron structure with block results, to be written
//  @result:        - unsigned int          error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         get results for current processing block 
//  @creation_date: 06.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int EAQUALAPI  eaqualGetBlockResults(  void            *pInstance, stBlockResults  *pstBlockResults)
{
    CRunAnalysis *pInstancePointer  = (CRunAnalysis*)(pInstance);
    
    return (pInstancePointer->raGetBlockResults(pstBlockResults));
}
