/////////////////////////////////////////////////////////////////////////////////////
// FileIO.cpp: implementation of the CFileIO class.
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

#include <stdlib.h>
#include <string.h>
#include "EAQUALCore.h"
#include "FileIO.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileIO::CFileIO()
{
    zINT8   cHandle[12],
            *pcHandle = cHandle;

    iNumFull            = 0;
    m_bWritingFinished  = _FALSE;

    pcHandle = fioInt2String(reinterpret_cast<zINT32>(this));
    strcat(pcHandle, ".tmp");
    m_pFFile            = fopen(pcHandle, "wb");

}

CFileIO::~CFileIO()
{
    zINT8   cHandle[8],
            *pcHandle = cHandle,
            cFile[16];
    
    if (m_pFFile) fclose(m_pFFile);

    for (zINT iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++)
        for (zINT iIdx = 0; iIdx < _NUM_MOVS_MAX; iIdx++)
            delete m_TimeMOVChannel[iCh].pfTimeMOVs[iIdx];

    //delete temporary files
    #if defined(_MSC_VER)       
        strcpy(cFile, "del ");
        pcHandle = fioInt2String(reinterpret_cast<zINT32>(this));
        strcat(cFile, pcHandle);
        strcat(cFile, ".tmp");
        system(cFile);
    #elif defined (__unix__)
        strcpy(cFile, "rm ");
        pcHandle = fioInt2String(reinterpret_cast<zINT32>(this));
        strcat(cFile, pcHandle);
        strcat(cFile, ".tmp");
        system(cFile);
    #endif

}

/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fioWriteTmpFile
//  @parameter:     - zFLOAT        *pfData         pointer on data to write
//                  - zINT          iLength         length of data vector
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         writes file with temporary data
//  @creation_date: 15.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CFileIO::fioWriteTmpFile(zFLOAT *pfData, zINT iLength)
{
    zINT iIdx;

    if (iNumFull + iLength >= _FILE_OUT_BUFFER_LENGTH)
    {
        if (iNumFull != (iIdx = fwrite( m_afFileOutBuffer, 
                                        sizeof(zFLOAT),
                                        iNumFull, 
                                        m_pFFile))) return _FILE_WRITE_ERROR;

        iNumFull    = 0;
    }
    for ( iIdx = iNumFull; iIdx < iNumFull + iLength; iIdx++)
        m_afFileOutBuffer[iIdx] = pfData[iIdx- iNumFull];
    iNumFull   += iLength;

    return _NO_ERROR;
}

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
zERROR  CFileIO::fioEndWrite()
{
    if (iNumFull)
        if ( iNumFull != zINT(fwrite(   m_afFileOutBuffer, 
                                        sizeof(zFLOAT),
                                        iNumFull, 
                                        m_pFFile))) return _FILE_WRITE_ERROR;

    fclose(m_pFFile);
    m_bWritingFinished  = _TRUE;
    return _NO_ERROR;
}


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
zERROR  CFileIO::fioReadTmpFile(eaqstEAQUALInstance *pInstance)
{
    zFLOAT  *pfTempBuffer;
    zINT    iIdx,
            iCh,
            iNumOfMOVs      = (pInstance->bModelType == _BASIC) ? _NUM_MOV_BASIC : _NUM_MOV_ADVANCED,
            iNumOfChannels  = pInstance->WaveFormat.iNumOfChannel;
    zUINT32 uiNumOfFrames   = pInstance->uiFramesProcessed;
    zINT8   cHandle[12],
            *pcHandle = cHandle;
    
    if (!m_bWritingFinished)
        return _ILLEGAL_FUNCTION_CALL;

    if ( (pfTempBuffer  = new zFLOAT[uiNumOfFrames * iNumOfMOVs * iNumOfChannels]) == 0)
        return _MEM_ALLOC_FAILED;

    for ( iCh = 0; iCh < iNumOfChannels; iCh++)
    {
        stChannelData *pChannelData = &pInstance->ChannelData[iCh];
        for ( iIdx = 0; iIdx < iNumOfMOVs; iIdx++)
            if (!(pChannelData->pfTimeMOVs[iIdx]    = m_TimeMOVChannel[iCh].pfTimeMOVs[iIdx]    = new zFLOAT[uiNumOfFrames]))   return _MEM_ALLOC_FAILED;
        for (iIdx = iNumOfMOVs; iIdx < _NUM_MOVS_MAX; iIdx++)
            m_TimeMOVChannel[iCh].pfTimeMOVs[iIdx]  = 0;
    }

    for ( iCh = iNumOfChannels; iCh < _NUM_CHANNELS_MAX; iCh++)
        for ( iIdx = 0; iIdx < _NUM_MOVS_MAX; iIdx++)
            m_TimeMOVChannel[iCh].pfTimeMOVs[iIdx]  = 0;
            
        

    // open temporary file
    pcHandle = fioInt2String(reinterpret_cast<zINT32>(this));
    strcat(pcHandle, ".tmp");
    m_pFFile        = fopen(pcHandle, "rb");

    // read the temporary data
    if ( fread(pfTempBuffer, sizeof(zFLOAT), uiNumOfFrames * iNumOfMOVs * iNumOfChannels,m_pFFile) != uiNumOfFrames * iNumOfMOVs * iNumOfChannels) return _UNKNOWN_ERROR;

    // sort the MOVs
    fioSortMOVs(pInstance, pfTempBuffer, iNumOfMOVs);

    // close file
    fclose(m_pFFile); m_pFFile = 0;

    // free memory
    delete [] pfTempBuffer;
    
    return _NO_ERROR;
}




/////////////////////////////////////////////////////////////////////////////////////
//  @method:        fioSortMOVs
//  @parameter:     - eaqstEAQUALInstance   *pInstance      pointer on instance data
//                  - zFLOAT                *pfTempBuffer   pointer on buffer with MOVs over time
//                  - zINT                  iNumOfMOVs      number of MOVs
//  @result:        - zERROR
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         sorts MOV data correct
//  @creation_date: 16.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
inline zERROR   CFileIO::fioSortMOVs(eaqstEAQUALInstance *pInstance, zFLOAT *pfTempBuffer, zINT iNumOfMOVs)
{
    zINT    iCh,
            iIdx,
            iInc,
            iMOVIdx,
            iOffset,
            iFramesProcessed    = (zINT) pInstance->uiFramesProcessed;

    iInc        = iNumOfMOVs * pInstance->WaveFormat.iNumOfChannel;
    for (iCh = 0; iCh < pInstance->WaveFormat.iNumOfChannel; iCh++)
    {
        stChannelData   *pChannel   = &pInstance->ChannelData[iCh];

        iOffset     = iNumOfMOVs * iCh;

        for (iIdx = 0; iIdx < iFramesProcessed; iIdx++)
        {
            for (iMOVIdx = 0; iMOVIdx < iNumOfMOVs; iMOVIdx++)
                pChannel->pfTimeMOVs[iMOVIdx][iIdx] = pfTempBuffer[iOffset + iInc * iIdx + iMOVIdx];
        }
    }
    return _NO_ERROR;
}


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
inline zINT8* CFileIO::fioInt2String(zINT32 i)
{
    static char buf[32];
    
    sprintf(buf, "%x", i);
    return buf;
}