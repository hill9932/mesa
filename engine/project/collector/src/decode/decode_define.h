/**
 * @Function:
 *  Define some struct and definition for decode
 * @Memo:
 *  Create by hill, 4/11/2017
 */
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/queue.h>
 
typedef enum 
{
    CHAR_ASCII,
    CHAR_EBCDIC
} encoding_e;

typedef enum 
{
    PRINT_XML,
    PRINT_TEXT,
} print_type_e;

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
