/////////////////////////////////////////////////////////////////////////////////////
// EAQUALMain.h - header for the frontend of EAQUAL
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


#if !defined(__EAQUALMAIN_HEADER_INCLUDED__)
#define __EAQUALMAIN_HEADER_INCLUDED__

/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stOutputPaths
//  @members:       - char          *pcResult               analysis output of whole file
//                  - char          *pcAll                  block output for all MOVs
//                  - char          *pcODG                  block output for ODG
//                  - char          *pcDI                   block output for DI
//                  - char          *pcBWRef                block output for bandwidth of reference signal
//                  - char          *pcBWTest               block output for bandwidth of test signal
//                  - char          *pcNMR                  block output for NMR
//                  - char          *pcADB                  block output for ADB
//                  - char          *pcEHS                  block output for EHS
//                  - char          *pcModDiff1             block output for modulation difference 1
//                  - char          *pcModDiff2             block output for modulation difference 2
//                  - char          *pcNoiseLoud            block output for noise loudness
//                  - char          *pcDetProb              block output for detection probability
//                  - char          *pcRDF                  block output for RDF
//                  - char          *pcNMRSpectral          block output for spectral distribution of NMR
//                  - char          *pcDetProbSpectral      block output for spectral distribution of detection probability
//                  - char          *pcModDiffSpectral      block output for spectral distribution of modulation difference 1
//                  - char          *pcNoiseLoudSpectral    block output for spectral distribution of noise loudness
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         paths for block results
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    char    *pcResult,
            // blockwise outputs
            *pcAll,
            *pcODG,
            *pcDI,
            *pcBWRef,
            *pcBWTest,
            *pcNMR,
            *pcADB,
            *pcEHS,
            *pcModDiff1,
            *pcModDiff2,
            *pcNoiseLoud,
            *pcDetProb,
            *pcRDF,

            *pcNMRSpectral,
            *pcDetProbSpectral,
            *pcModDiffSpectral,
            *pcNoiseLoudSpectral;
} stOutputPaths;

    
/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stOutputFiles
//  @members:       - FILE          *pFAll                  file for block output for all MOVs
//                  - FILE          *pFODG                  file for block output for ODG
//                  - FILE          *pFDI                   file for block output for DI
//                  - FILE          *pFBWRef                file for block output for bandwidth of reference signal
//                  - FILE          *pFBWTest               file for block output for bandwidth of test signal
//                  - FILE          *pFNMR                  file for block output for NMR
//                  - FILE          *pFADB                  file for block output for ADB
//                  - FILE          *pFEHS                  file for block output for EHS
//                  - FILE          *pFModDiff1             file for block output for modulation difference 1
//                  - FILE          *pFModDiff2             file for block output for modulation difference 2
//                  - FILE          *pFNoiseLoud            file for block output for noise loudness
//                  - FILE          *pFDetProb              file for block output for detection probability
//                  - FILE          *pFRDF                  file for block output for RDF
//                  - FILE          *pFNMRSpectral          file for block output for spectral distribution of NMR
//                  - FILE          *pFDetProbSpectral      file for block output for spectral distribution of detection probability
//                  - FILE          *pFModDiffSpectral      file for block output for spectral distribution of modulation difference 1
//                  - FILE          *pFNoiseLoudSpectral    file for block output for spectral distribution of noise loudness
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         pointers on files for one processing block
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    FILE    *pFAll,
            *pFODG,
            *pFDI,
            *pFBWRef,
            *pFBWTest,
            *pFNMR,
            *pFADB,
            *pFEHS,
            *pFModDiff1,
            *pFModDiff2,
            *pFNoiseLoud,
            *pFDetProb,
            *pFRDF,

            *pFNMRSpectral,
            *pFDetProbSpectral,
            *pFModDiffSpectral,
            *pFNoiseLoudSpectral;
} stOutputFiles;

    
/////////////////////////////////////////////////////////////////////////////////////
//  function:       InitArgs
//  @parameter:     - stCLOptions       *pOptions       pointer on struct for command line options, to be written
//                  - WaveFormat        *pWFormat       pointer on struct for wave format, to be written
//                  - SF_INFO           *sfRefInfo      pointer on struct for reference wave format, to be written
//                  - SF_INFO           *sfTestInfo     pointer on struct for test wave format, to be written
//                  - int               *piOffset       pointer on offset/delay between reference and test file, to be written
//                  - stOutputPaths     *pOutputPaths   pointer on structure with output paths, to be written
//                  - stOutputFiles     *pOutputFiles   pointer on structure with output files, to be written
//  @result:        int                                 Error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         initialize all command line args
//  @creation_date: 27.12.2001
//  @last_modified: 07.01.2002 AL: added outputpath structure
/////////////////////////////////////////////////////////////////////////////////////
int     InitArgs(           stCLOptions     *pOptions, 
                            WaveFormat      *pWFormat, 
                            SF_INFO         *sfRefInfo, 
                            SF_INFO         *sfTestInfo, 
                            int             *piOffset,
                            stOutputPaths   *pOutputPaths,
                            stOutputFiles   *pOutputFiles);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ParseArgs
//  @parameter:     - stCLOptions       *pOptions       pointer on struct for command line options, to be written
//                  - WaveFormat            *pWFormat       pointer on struct for wave format, to be written
//                  - char              **pcRefPath     pointer on reference file, to be written
//                  - char              **pcTestPath    pointer on test file, to be written
//                  - stOutputPaths     *pOutputPaths   pointer on structure with output paths
//                  - SF_INFO           *sfRefInfo      pointer on struct for reference wave format, to be written
//                  - SF_INFO           *sfTestInfo     pointer on struct for test wave format, to be written
//                  - int               *piOffset       pointer on offset/delay between reference and test file, to be written
//                  - bool              *pbOutputMode   pointer on bool for decision stdio<->file, to be written
//                  - bool              *pbBlockOutput  pointer on bool for decision if block output or not, to be written
//                  - int               argc            number of command line args
//                  - char              *argv           command line
//  @result:        int                                 Error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         parse command line args
//  @creation_date: 27.12.2000
//  @last_modified: 07.01.2002 AL: added support for block output
/////////////////////////////////////////////////////////////////////////////////////
int     ParseArgs(          stCLOptions     *pOptions, 
                            WaveFormat      *pWFormat, 
                            char            **pcRefPath, 
                            char            **pcTestPath, 
                            stOutputPaths   *pOutputPaths, 
                            SF_INFO         *sfRefInfo, 
                            SF_INFO         *sfTestInfo, 
                            int             *piOffset, 
                            bool            *pbOutputMode, 
                            bool            *pbBlockOutput,
                            int             argc, 
                            char            *argv[]);



/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowProgramInfo
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program info
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowProgramInfo();


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowFrameNumber
//  @parameter:     - unsigned int      uiFrameNumber       number of current frame
//                  - int               iSampleRate         sample rate of audio
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print number of calculated frames
//  @creation_date: 27.12.2000
//  @last_modified: 08.01   changed output to seconds
/////////////////////////////////////////////////////////////////////////////////////
void    ShowFrameNumber(    unsigned int    uiFrameNumber,
                            int             iSampleRate);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowTime
//  @parameter:     - clock_t       clStartTime     time when processing started
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print elapsed time
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowTime(           clock_t         clStartTime);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowResults
//  @parameter:     - double        *pResults           pointer on program results
//                  - int           iLengthOfResults    number of results
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program results to stdio
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowResults(        double          *pResults, 
                            int             iLengthOfResults);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowCancel
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print cancel message
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowCancel();


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowConfiguration
//  @parameter:     - char      *pcRefPath      path of reference input file
//                  - char      *pcTestPath     path of test input file
//                  - WaveFormat    *pFormat        pointer on struct with the input wave format
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print current configuration
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowConfiguration(  char            *pcRefPath, 
                            char            *pcTestPath, 
                            WaveFormat      *pFormat);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowHelp
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print command line help
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowHelp();


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowVersion
//  @parameter:     -
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program version
//  @creation_date: 27.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowVersion();


/////////////////////////////////////////////////////////////////////////////////////
//  function:       PrintResults
//  @parameter:     - double        *pResults           pointer on program results
//                  - int           iLengthOfResults    number of results
//                  - char          *pcOutputPath       path to file to write
//                  - char          *pcRefPath          path to ref file 
//                  - char          *pcTestPath         path to test file 
//  @result:        - int                               error code
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print program results to file
//  @creation_date: 27.12.2001
//  @last_modified: 25.01.2002 AL: added ref and test file path
/////////////////////////////////////////////////////////////////////////////////////
int     PrintResults(       double          *pResults, 
                            int             iLengthOfResults, 
                            char            *pcOutputPath, 
                            char            *pcRefPath, 
							char			*pcTestPath);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       ShowError
//  @parameter:     - unsigned int      rError      error code
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         print the program error
//  @creation_date: 27.12.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void    ShowError (         unsigned int    rError);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       OpenOutputFiles
//  @parameter:     - stOutputPaths *pOutputPaths       pointer on structure with output file paths
//                  - stOutputFiles *pOutputFiles       pointer on structure with output files
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         open output files for block output
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
unsigned int OpenOutputFiles(   stOutputPaths   *pOutputPaths, 
                                stOutputFiles   *pOutputFiles);

/////////////////////////////////////////////////////////////////////////////////////
//  function:       CloseOutputFiles
//  @parameter:     - stOutputFiles *pOutputFiles       pointer on structure with output files
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         open output files for block output
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void CloseOutputFiles(  stOutputFiles   *pOutputFiles);


/////////////////////////////////////////////////////////////////////////////////////
//  function:       PrintOutputFiles
//  @parameter:     - stBlockResults    *pBlockResults      results of analyisis per block
//                  - stOutputFiles     *pOutputFiles       pointer on structure with output files
//  @result:        -
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         open output files for block output
//  @creation_date: 07.01.2002
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
void PrintOutputFiles(  stBlockResults  *pBlockResults, 
                        stOutputFiles   *pOutputFiles);

#endif // !defined(__EAQUALMAIN_HEADER_INCLUDED__)
