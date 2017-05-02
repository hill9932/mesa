/**
* @Function:
*  Define the structure for file operation
* @Memo:
*  Create by hill, 4/17/2017
*/
#pragma once
#include "common.h"


namespace LabSpace
{
namespace Common
{

#define PCAP_NG_MAGIC           0x0a0d0d0a
#define PCAP_NSEC_MAGIC         0xa1b23c4d
#define PCAP_MSEC_MAGIC         0xa1b2c3d4
#define PCAP_VERSION_MAJOR      2
#define PCAP_VERSION_MINOR      4

struct FileType
{
    enum FILE_TYPE_e
    {
        FILE_TYPE_UNSUPPORT = 0,
        FILE_TYPE_PCAP      = 1,
        FILE_TYPE_PCAP_NS   = 2,
        FILE_TYPE_PCAP_NG   = 3
    };
};

struct FileAccessMode
{
    enum FILE_ACCESS_e
    {
        ACCESS_NONE     = 0,
        ACCESS_READ     = 1,
        ACCESS_WRITE    = 2
    };
};

struct FileAccessOption
{
    enum FILE_CREATE_e
    {
        FILE_CREATE_NONE        = 0,
        FILE_CREATE_NEW         = 1,
        FILE_CREATE_ALWAYS      = 2,
        FILE_OPEN_EXISTING      = 4,
        FILE_OPEN_ALWAYS        = 8,
        FILE_TRUNCATE_EXISTING  = 0x10,
        FILE_APPEND             = 0x20,
    };
};

#pragma pack(push, 4)

typedef struct pcap_file_header_t
{
    u_int32 magic;
    u_int16 version_major;
    u_int16 version_minor;
    u_int32 thiszone;
    u_int32 sigfigs;
    u_int32 snaplen;
    u_int32 linktype;
} pcap_file_header_t;

#pragma pack(pop)


static const pcap_file_header_t PCAP_FILE_HEADER =
{
    PCAP_MSEC_MAGIC,
    PCAP_VERSION_MAJOR,
    PCAP_VERSION_MINOR,
    0,
    0,
    0xFFFF,
    FileType::FILE_TYPE_PCAP
};

static const pcap_file_header_t PCAP_NS_FILE_HEADER =
{
    PCAP_NSEC_MAGIC,
    PCAP_VERSION_MAJOR,
    PCAP_VERSION_MINOR,
    0,
    0,
    0xFFFF,
    FileType::FILE_TYPE_PCAP_NS
};

struct FileHeaderInfo_t
{
    void*   addr;
    size_t  size;
};

static const FileHeaderInfo_t FILE_HEADERS[] =
{
    { NULL, 0 },
    { (void*)&PCAP_FILE_HEADER,     sizeof(PCAP_FILE_HEADER)    },      // FILE_PCAP
    { (void*)&PCAP_NS_FILE_HEADER,  sizeof(PCAP_NS_FILE_HEADER) }       // FILE_PCAP_NS
};

struct FileOperation
{
    enum FILE_OPERATION_e
    {
        FILE_READ = 1,
        FILE_WRITE
    };
};

struct BlockDesc_t
{

};

//
// this struct is used for aio
//
typedef struct OVERLAPIOINFO_t : public OVERLAPPED
{
    handle_t                hFile;
    FileOperation::FILE_OPERATION_e     optType;
    u_int32                 index;
    u_int32                 flag;
    u_int32                 dataLen;
    byte*                   data;
    void*                   context;

    BlockDesc_t             dataDesc;
} PER_FILEIO_INFO_t, *PPER_FILEIO_INFO_t;

}
}
