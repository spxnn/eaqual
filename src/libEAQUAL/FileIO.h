/////////////////////////////////////////////////////////////////////////////////////
// FileIO.h: interface for the CFileIO class.
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



#if !defined(__FILEIO_HEADER_INCLUDED_)
#define __FILEIO_HEADER_INCLUDED_

#include <stdio.h>

#define _FILE_OUT_BUFFER_LENGTH (_FFT_LENGTH<<3)

/////////////////////////////////////////////////////////////////////////////////////
//  @struct:        stMOVs
//  @members:       - zFLOAT            *pfTimeMOVs         pointer on the MOVs per channel and time
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         command line options
//  @creation_date: 14.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    zFLOAT  *pfTimeMOVs[_NUM_MOVS_MAX];
} stMOVs;


/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CFileIO
//  @derived_from:  -
//  @variables:     - zFLOAT        m_afFileOutBuffer   data output buffer
//                  - zBOOL     m_bWritingFinished  boolean flag for end of writing
//                  - FILE      *m_pFFile           pointer on temporary file
//                  - stMOVs        m_TimeMOVChannel    the read MOV per time
//  @methods:       - fioEndWrite
//                  - fioReadTmpFile
//                  - fioSortMOVs
//                  - fioWriteTmpFile
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         write and read temporary file
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CFileIO  
{
private:
    zFLOAT  m_afFileOutBuffer[_FILE_OUT_BUFFER_LENGTH];
    stMOVs  m_TimeMOVChannel[_NUM_CHANNELS_MAX];
    zINT    iNumFull;
    FILE    *m_pFFile;
    zBOOL   m_bWritingFinished;

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fioSortMOVs
    //  @parameter:     - eaqstEAQUALInstance   *pInstance      pointer on instance data
    //                  - zFLOAT                    *pfTempBuffer   pointer on buffer with MOVs over time
    //                  - zINT                  iNumOfMOVs      number of MOVs
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         sorts MOV data correct
    //  @creation_date: 16.12.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline zERROR   fioSortMOVs(    eaqstEAQUALInstance     *pInstance, 
                                    zFLOAT                  *pfTempBuffer, 
                                    zINT                    iNumOfMOVs);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fioInt2String
    //  @parameter:     - zINT              i       integer
    //  @result:        - zINT8             *buf    resulting string
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         converts integer value to string
    //  @creation_date: 15.01.2002
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline zINT8*   fioInt2String(  zINT i);
        
public:
    CFileIO();
    virtual ~CFileIO();

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fioWriteTmpFile
    //  @parameter:     - zFLOAT            *pfData         pointer on data to write
    //                  - zINT          iLength         length of data vector
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         writes file with temporary data
    //  @creation_date: 15.12.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  fioWriteTmpFile(    zFLOAT  *pfData, 
                                zINT    iLength);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fioEndWrite
    //  @parameter:     -
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         closes file for writing
    //  @creation_date: 15.12.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  fioEndWrite();

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        fioReadTmpFile
    //  @parameter:     - eaqstEAQUALInstance   *pInstance  pointer on instance data
    //  @result:        - zERROR
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         reads file with temporary MOV data
    //  @creation_date: 15.12.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR  fioReadTmpFile( eaqstEAQUALInstance     *pInstance);

};

#endif // !defined(__FILEIO_HEADER_INCLUDED_)
