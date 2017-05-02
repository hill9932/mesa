#include "log_.h"
#include "pcap_file.h"


namespace LabSpace
{
namespace Common
{
c_pcap_file::c_pcap_file(FileType::FILE_TYPE_e _fileType)
{
    m_fileType      = _fileType;
    m_fileHandle    = INVALID_HANDLE_VALUE;
    m_fileView      = NULL;
    m_data          = NULL;
    m_offset        = 0;
    m_fileSize      = 0;
    bzero(&m_priWriteBlock, sizeof(m_priWriteBlock));
}

c_pcap_file::~c_pcap_file()
{
    close();
}

int c_pcap_file::open(const tchar* _fileName, int _access, int _flag, bool _asyn, bool _directIO)
{
    close();
    m_fileName = _fileName;

    int z = createMapFile(_fileName, (_access & FileAccessMode::ACCESS_WRITE) ? m_fileSize : 0);
    if (z != 0) return z;

    if (_access & FileAccessMode::ACCESS_READ && !(_access & FileAccessMode::ACCESS_WRITE))
    {
        m_fileSize = getSize();
        return 0;
    }

    if ((_access & FileAccessMode::ACCESS_WRITE) &&
        !(_flag & FileAccessOption::FILE_OPEN_EXISTING))
        return writeHeader();

    return 0;
}

int c_pcap_file::writeHeader()
{
    m_offset = 0;
    m_priWriteBlock.data = (byte*)FILE_HEADERS[m_fileType].addr;
    m_priWriteBlock.dataLen = FILE_HEADERS[m_fileType].size;
    m_priWriteBlock.Offset = m_offset;

    return write(&m_priWriteBlock);
}

int c_pcap_file::write(PPER_FILEIO_INFO_t  _writeBlock)
{
    if (!_writeBlock || !_writeBlock->data || !isValid())
        return -1;

    u_int64 offset = (_writeBlock->Offset == (u_int64)-1) ? m_offset : (m_offset = _writeBlock->Offset, _writeBlock->Offset);
    if (_writeBlock->dataLen + offset > m_fileSize)
        return -1;

    memcpy(m_data + offset, _writeBlock->data, _writeBlock->dataLen);
    m_offset += _writeBlock->dataLen;

    return 0;
}

int c_pcap_file::read(PPER_FILEIO_INFO_t  _readBlock)
{
    if (!_readBlock || !_readBlock->data || !isValid())
        return -1;

    if ((u_int64)_readBlock->dataLen > m_fileSize)
        return -1;

    memcpy(_readBlock->data, m_data + _readBlock->Offset, _readBlock->dataLen);

    return 0;
}

u_int32 c_pcap_file::getSize(u_int32* _high32)
{
    u_int32 high32 = 0;
    u_int32 low32 = 0;

#ifdef WIN32
    low32 = GetFileSize(m_fileHandle, (LPDWORD)&high32);
#else
    struct stat statbuf;
    if (fstat(m_fileHandle, &statbuf) == -1)
    {
        LOG_LAST_ERRORMSG();
        return 0;
    }

    if (S_ISREG(statbuf.st_mode))
    {
        if (_high32)    *_high32 = statbuf.st_size >> 32;
        return statbuf.st_size;
    }
#endif

    if (_high32)    *_high32 = high32;
    return low32;
}

u_int64 c_pcap_file::getSize()
{
    u_int64 fileSize = 0;
    u_int32 high32 = 0, low32 = 0;
    low32 = getSize(&high32);

    fileSize = high32;
    fileSize <<= high32;
    fileSize += low32;
    return fileSize;
}

int c_pcap_file::rename(const tchar* _newFileName)
{
    if (!_newFileName || !isValid())  return -1;
    if (m_fileName == _newFileName)     return 0;

    int z = ::rename_t(m_fileName.c_str(), _newFileName);
    ON_ERROR_PRINT_LAST_ERROR(z, != , 0);
    if (0 == z)
        m_fileName = _newFileName;
    return z;
}

int c_pcap_file::getNextPacket(packet_header_t& _header, byte*& _data)
{
    if (m_offset >= m_fileSize) return -1;
    if (m_offset == 0)  m_offset = getFileHeaderSize();

    int offset = m_offset;
    packet_header_t* pktHdr = (packet_header_t*)(m_data + m_offset);
    m_offset += getPacketHeaderSize() + pktHdr->caplen;

    if (m_offset > m_fileSize) return -1;

    _header.caplen = pktHdr->caplen;
    _header.len = pktHdr->len;
    _header.ts.tv_sec = pktHdr->ts.tv_sec;
    _header.ts.tv_nsec = pktHdr->ts.tv_nsec;

    _data = m_data + offset + getPacketHeaderSize();
    return getPacketHeaderSize() + pktHdr->caplen;
}

#ifdef WIN32
#include "pcap_file_win32.hxx"
#else
#include "pcap_file_linux.hxx"
#endif
}
}