#include <gtest/gtest.h>
#include "../src/recv_window.h"

#define TEST_CASE_NAME  Test_recv_window_


class c_recv_window_tester : public testing::Test
{
public:
    static void SetUpTestCase()     {}
    static void TearDownTestCase()  {}

    virtual void SetUp()            {}
    virtual void TearDown()         {}

protected:
    c_recv_window   m_recv_window;
};


TEST_F(c_recv_window_tester, insert1)
{
    m_recv_window.insert(1000);
    m_recv_window.insert(1001);
    m_recv_window.insert(1003);
    m_recv_window.insert(1004);
    m_recv_window.insert(1200);
    m_recv_window.insert(1008);
    m_recv_window.insert(1007);
    m_recv_window.insert(1005);
    m_recv_window.insert(1004);
    m_recv_window.insert(1010);
    m_recv_window.insert(1002);
    m_recv_window.insert(1009);
    m_recv_window.insert(1006);
    m_recv_window.finalize();

    EXPECT_EQ(m_recv_window.get_total_count(),    13);
    EXPECT_EQ(m_recv_window.get_dup_count(),      1);
    EXPECT_EQ(m_recv_window.get_invalid_count(),  1);
    EXPECT_EQ(m_recv_window.get_disorder_count(), 9);
    EXPECT_EQ(m_recv_window.get_miss_count(),     0);
}


TEST_F(c_recv_window_tester, insert2)
{
    m_recv_window.reset();
    m_recv_window.insert(1000);
    m_recv_window.insert(1001);
    m_recv_window.insert(1003);
    m_recv_window.insert(1004);
    m_recv_window.insert(1200);
    m_recv_window.insert(1008);
    m_recv_window.insert(1007);
    m_recv_window.insert(1005);
    m_recv_window.insert(1004);
    m_recv_window.insert(1010);
    m_recv_window.insert(1020);
    m_recv_window.insert(1002);
    m_recv_window.insert(1009);
    m_recv_window.insert(1006);
    m_recv_window.finalize();

    EXPECT_EQ(m_recv_window.get_total_count(),    14);
    EXPECT_EQ(m_recv_window.get_dup_count(),      1);
    EXPECT_EQ(m_recv_window.get_invalid_count(),  2);
    EXPECT_EQ(m_recv_window.get_disorder_count(), 9);
    EXPECT_EQ(m_recv_window.get_miss_count(),     10);

    u_int64 total_count_1 = m_recv_window.get_last_seq() - m_recv_window.get_first_seq() + 1 
                          + m_recv_window.get_dup_count()
                          + m_recv_window.get_invalid_count();
    u_int64 total_count_2 = m_recv_window.get_total_count() + m_recv_window.get_miss_count();
    EXPECT_EQ(total_count_1, total_count_2);
}


/**
 * @Function: test all numbers are in sequence
 */
TEST_F(c_recv_window_tester, insert3)
{
    m_recv_window.reset();
    int total = 100;
    for (int i = 0; i < total; ++i)
        m_recv_window.insert(i);

    m_recv_window.finalize();

    EXPECT_EQ(m_recv_window.get_total_count(),      total);
    EXPECT_EQ(m_recv_window.get_dup_count(),        0);
    EXPECT_EQ(m_recv_window.get_invalid_count(),    0);
    EXPECT_EQ(m_recv_window.get_disorder_count(),   0);
    EXPECT_EQ(m_recv_window.get_miss_count(),       0);

    u_int64 total_count_1 = m_recv_window.get_last_seq() - m_recv_window.get_first_seq() + 1
        + m_recv_window.get_dup_count()
        + m_recv_window.get_invalid_count();
    u_int64 total_count_2 = m_recv_window.get_total_count() + m_recv_window.get_miss_count();
    EXPECT_EQ(total_count_1, total_count_2);
}


/**
 * @Function: test all numbers are in reverse sequence
 */
TEST_F(c_recv_window_tester, insert4)
{
    m_recv_window.reset();
    int total = 100;
    for (int i = total - 1; i >= 0; --i)
        m_recv_window.insert(i);

    m_recv_window.finalize();

    EXPECT_EQ(m_recv_window.get_total_count(),      total);
    EXPECT_EQ(m_recv_window.get_dup_count(),        0);
    EXPECT_EQ(m_recv_window.get_invalid_count(),    total-1);
    EXPECT_EQ(m_recv_window.get_disorder_count(),   0);
    EXPECT_EQ(m_recv_window.get_miss_count(),       0);

    u_int64 total_count_1 = m_recv_window.get_last_seq() - m_recv_window.get_first_seq() + 1
        + m_recv_window.get_dup_count()
        + m_recv_window.get_invalid_count();
    u_int64 total_count_2 = m_recv_window.get_total_count() + m_recv_window.get_miss_count();
    EXPECT_EQ(total_count_1, total_count_2);
}


/**
 * @Function: test with miss sequence
 */
TEST_F(c_recv_window_tester, insert5)
{
    m_recv_window.reset();

    int total = 0;
    int first_seq = 1005;
    int last_seq  = 1200;
    for (int i = first_seq; i < last_seq; ++i)
        m_recv_window.insert(i);
    total = last_seq - first_seq;

    m_recv_window.insert(last_seq + VALID_SEQ_RANGE);   // invalid, low = 1200
    ++total;

    m_recv_window.insert(last_seq + WINDOWS_SIZE);      // force move forward, low = 1201, miss += 1
    ++total;

    EXPECT_EQ(m_recv_window.get_miss_count(), 1);

    m_recv_window.insert(last_seq + VALID_SEQ_RANGE - 1);   // force move forward, low = 1216, miss += 15
    ++total;

    EXPECT_EQ(m_recv_window.get_miss_count(), 16);

    m_recv_window.finalize();       // miss += 15

    EXPECT_EQ(m_recv_window.get_total_count(),      total);
    EXPECT_EQ(m_recv_window.get_dup_count(),        0);
    EXPECT_EQ(m_recv_window.get_invalid_count(),    1);
    EXPECT_EQ(m_recv_window.get_disorder_count(),   2);
    EXPECT_EQ(m_recv_window.get_miss_count(),       30);

    u_int64 total_count_1 = m_recv_window.get_last_seq() - m_recv_window.get_first_seq() + 1
        + m_recv_window.get_dup_count()
        + m_recv_window.get_invalid_count();
    u_int64 total_count_2 = m_recv_window.get_total_count() + m_recv_window.get_miss_count();
    EXPECT_EQ(total_count_1, total_count_2);
}