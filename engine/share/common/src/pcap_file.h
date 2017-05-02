/**
* @Function:
*  define pcap_file class to read pcap file.
* @Memo:
*  Create by hill, 4/16/2017
*/

#pragma once

#include "file_i.h"

namespace LabSpace
{
namespace Common
{
struct timestamp_t
{
    uint32_t    tv_sec;
    uint32_t    tv_nsec;
};

struct packet_header_t
{
    struct timestamp_t ts;	    // time stamp
    uint32_t    caplen;	            // length of portion present
    uint32_t    len;	            // length this packet (off wire)
};


class c_pcap_file : public IFile
{
public:
    c_pcap_file(FileType::FILE_TYPE_e _fileType = FileType::FILE_TYPE_PCAP);
    ~c_pcap_file();

    virtual int open(const tchar* _fileName, int _access, int _flag, bool _asyn, bool _directIO);
    virtual int close();
    virtual int read(PPER_FILEIO_INFO_t  _readBlock);
    virtual int write(PPER_FILEIO_INFO_t  _writeBlock);
    virtual int setFileSize(u_int64 _fileSize);
    virtual int rename(const tchar* _fileName);
    virtual int flush();

    virtual bool    needPadding()           { return false; }
    virtual bool    isValid()               { return m_data != NULL; }
    byte*           getData()               { return m_data; }

    int             getPacketHeaderSize()   { return sizeof(packet_header_t); }
    int             getNextPacket(packet_header_t& _header, byte*& _data);

protected:
    int createMapFile(const tchar* _fileName, u_int64 _fileSize);
    /**
    * @Function: write the pcap file header
    **/
    virtual int writeHeader();

    u_int32 getSize(u_int32* _high32);
    u_int64 getSize();

protected:
    PER_FILEIO_INFO_t   m_priWriteBlock;
    handle_t    m_fileHandle;
    handle_t    m_fileView;
    byte*       m_data;
};
}
}
