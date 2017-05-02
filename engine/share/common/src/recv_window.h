/**
* @Function:
*  receive window to check the miss and disorder
* @Memo:
*  Create by hill, 4/25/2017
*/
#pragma once
#include "common.h"

const int BITS_PER_BYTE     = 8;
const int ARRAY_SIZE        = 2;
const int WINDOWS_SIZE      = ARRAY_SIZE * BITS_PER_BYTE;
const int VALID_SEQ_RANGE   = 2 * WINDOWS_SIZE;

class c_recv_window
{
public:
    c_recv_window();

    void insert(u_int64 _seq);
    void reset();
    void finalize();

    u_int64 get_disorder_count()    { return m_disorder_count;  }
    u_int64 get_miss_count()        { return m_miss_count;      }
    u_int64 get_invalid_count()     { return m_invalid_count;   }
    u_int64 get_total_count()       { return m_total_count;     }
    u_int64 get_dup_count()         { return m_dup_count;       }
    u_int64 get_first_seq()         { return m_first_seq;       }
    u_int64 get_last_seq()          { return m_last_seq;        }

private:
    void set_bit(u_int64 _seq);
    void move_forward(int _count = 0);

private:
    byte m_window[ARRAY_SIZE];

    u_int64 m_first_seq;        // the smallest number
    u_int64 m_last_seq;         // the largest number
    u_int64 m_low_seq;          // the next expected number
    u_int64 m_high_seq;
    u_int64 m_prev_seq;

    u_int64 m_disorder_count;
    u_int64 m_miss_count;
    u_int64 m_invalid_count;
    u_int64 m_total_count;
    u_int64 m_dup_count;
};