/////////////////////////////////////////////////////////////////////////////////////
// libEAQUAL.h: interface for libEAQUAL.
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


#if !defined(__LIBEAQUAL_HEADER_INCLUDED__)
#define __LIBEAQUAL_HEADER_INCLUDED__


// define C API for libEAQUAL
#ifdef WIN32

    #ifndef EAQUALAPI
        #define EAQUALAPI __stdcall
    #endif
#else
    #ifndef EAQUALAPI
        #define EAQUALAPI
    #endif
#endif

/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stCLOptions
//  @members:       - bool          bModelType          BASIC or ADVANCED model
//                  - bool          bForgetFactoEnabled forgetting factor for MFPD
//                  - float         fSPL                listening level in dBSPL
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         command line options
//  @creation_date: 14.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    bool    bModelType;
    bool    bForgetFactorEnabled;
    float   fSPL;
} stCLOptions;


/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        WaveFormat
//  @members:       - int           fSamplingRate           sampling rate of input signals
//                  - int           iNumOfBits              number of bits of input signals
//                  - int           iNumOfChannels          number of channels of input signals
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         waveformat of (both!) input signals
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    int     iNumOfBits,
            iNumOfChannel,
            iSamplingRate;
} WaveFormat;


/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stAudioData
//  @members:       - short         *pRefData[2]        pointer on ref audio stream (2 channels supported now)
//                  - short         *pTestData[2]       pointer on test audio stream (2 channels supported now)
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         audio data of both input signals (must be block of 2048 samples, up to 2 channels allowed)
//  @creation_date: 15.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    short   *pRefData[2],
            *pTestData[2];
    
} stAudioData;


/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stBlockResults
//  @members:       - bool          bModelType              basic of advanced analysis model
//                  - double        *pfMOVs                 pointer on array with MOVs
//                  - float         *pfNMRSpectral          pointer spectral distribution of NMR
//                  - float         *pfDetProbSpectral      pointer spectral distribution of detection probability
//                  - float         *pfModDiffSpectral      pointer spectral distribution of modulation difference
//                  - float         *pfNoiseLoudSpectral    pointer spectral distribution of noise loudness
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         resulting output values for one processing block
//  @creation_date: 06.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    int     iThisChannelPlease;
    bool    bModelType;
    double  *pfMOVs,
            *pfNMRSpectral,
            *pfDetProbSpectral,
            *pfModDiffSpectral,
            *pfNoiseLoudSpectral;

} stBlockResults;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
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
void*           EAQUALAPI   eaqualCreateInstance();


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualKillInstance
//  @parameter:     - void          *pInstance  handle to instance to delete
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         delete specified instance of EAQUAL 
//  @creation_date: 14.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void        EAQUALAPI   eaqualKillInstance( void    *pInstance);


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualGetBlockResults
//  @parameter:     - void                  *pInstance          handle to instance to delete
//                  - stBlockResults        *pstBlockResults    pointe ron structure with block results, to be written
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         get results for current processing block 
//  @creation_date: 06.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int EAQUALAPI  eaqualGetBlockResults(  void            *pInstance,
                                                stBlockResults  *pstBlockResults);


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
unsigned int EAQUALAPI  eaqualEndInstance(  void    *pInstance, 
                                            double  **pfResults, 
                                            int     *piResultLength);


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
unsigned int EAQUALAPI  eaqualProcessInstance(  void        *pInstance, 
                                                stAudioData *pAudioData);


/////////////////////////////////////////////////////////////////////////////////////
//  @function:      eaqualInitInstance
//  @parameter:     -void           *pInstance  handle to instance pointer
//                  - stCLOptions   *pCLOptions see libEAQUAL.h
//                  - WaveFormat    *pWFormat   pointer on audio format
//  @result:        - unsigned int  error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         process new block 
//  @creation_date: 15.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int EAQUALAPI  eaqualInitInstance(     void        *pInstance, 
                                                stCLOptions *pCLOptions, 
                                                WaveFormat  *pWFormat);

#ifdef __cplusplus
}
#endif /* __cplusplus */

    
#endif // !defined(__LIBEAQUAL_HEADER_INCLUDED__)
