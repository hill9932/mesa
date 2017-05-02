#include "recv_window.h"

c_recv_window::c_recv_window()
{
    reset();
}

void c_recv_window::reset()
{
    bzero(m_window, sizeof(m_window));

    m_low_seq       = 0;
    m_high_seq      = 0;
    m_prev_seq      = 0;
    m_miss_count    = 0;
    m_invalid_count = 0;
    m_total_count   = 0;
    m_dup_count     = 0;
    m_disorder_count= 0;
    m_first_seq     = 0;
    m_last_seq      = 0;
}

void c_recv_window::finalize()
{
    int topSeq = 0;
    for (int i = 0; i < WINDOWS_SIZE; ++i)
    {
        int bytePos = i / BITS_PER_BYTE;
        int bitPos  = i % BITS_PER_BYTE;

        if (m_window[bytePos] & (1 << bitPos))
            topSeq = i;
    }

    for (int i = 0; i < topSeq; ++i)
    {
        int bytePos = i / BITS_PER_BYTE;
        int bitPos  = i % BITS_PER_BYTE;

        if (!(m_window[bytePos] & (1 << bitPos)))
            ++m_miss_count;
    }
}

void c_recv_window::insert(u_int64 _seq)
{
    bool valid = true;
    ++m_total_count;

    if (m_low_seq == m_high_seq)    // the first seq
    {
        m_low_seq   = _seq;
        m_high_seq  = m_low_seq + WINDOWS_SIZE - 1;
        m_first_seq = _seq;
    }
    else if (_seq - m_low_seq >= VALID_SEQ_RANGE)
    {
        ++m_invalid_count;
        valid = false;
    }
    else if (_seq != m_prev_seq + 1)
    {
        ++m_disorder_count;
    }

    if (valid)
    {
        bool forceMoveForward = _seq > m_high_seq;

        if (!forceMoveForward)
        {
            set_bit(_seq);

            if (_seq == m_low_seq)  // seq is in sequence
                move_forward();
        }
        else
        {
            move_forward(_seq - m_high_seq);   // skip the missing seq
            set_bit(_seq);
        }

        m_last_seq = max(m_last_seq, _seq);
    }

    m_prev_seq = _seq;
}

void c_recv_window::set_bit(u_int64 _seq)
{
    assert(_seq <= m_high_seq);

    int pos     = _seq - m_low_seq;
    int bytePos = pos / BITS_PER_BYTE;
    int bitPos  = pos % BITS_PER_BYTE;

    if (m_window[bytePos] & (1 << bitPos))
        ++m_dup_count;

    m_window[bytePos] |= 1 << bitPos;
}

void c_recv_window::move_forward(int _count)
{
    int count   = _count;
    int bytePos = 0;
    int bitPos  = 0;

    //
    // check the missing seq
    //
    for (int i = 0; i < count; ++i)
    {
        bytePos = i / BITS_PER_BYTE;
        bitPos  = i % BITS_PER_BYTE;

        if (!(m_window[bytePos] & (1 << bitPos)))
        {
            ++m_miss_count;
        }
    }

    //
    // get the continuous seq
    //
    if (0 == count)
    {
        while (m_window[bytePos] & (1 << bitPos))
        {
            ++count;
            bytePos = count / BITS_PER_BYTE;
            bitPos  = count % BITS_PER_BYTE;
        }
    }

    if (0 == count)
        return;

    //
    // move the bytes
    //
    int bytes = count / BITS_PER_BYTE;
    if (bytes > 0)
    {
        int i = 0;
        for (; i <= ARRAY_SIZE - 1 - bytes; ++i)
        {
            m_window[i] = m_window[i + bytes];
            m_window[i + bytes] = 0;
        }
    }

    //
    // move the bits
    //
    byte carry = 0;
    for (int i = ARRAY_SIZE - 1; i >= 0; --i)
    {
        byte tmp = m_window[i] << (BITS_PER_BYTE - count % BITS_PER_BYTE);
        m_window[i] >>= count % BITS_PER_BYTE;
        m_window[i] |= carry;
        carry = tmp;
    }

    m_low_seq   += count;
    m_high_seq  += count;
}
