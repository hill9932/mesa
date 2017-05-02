/**
 * @Function:
 *  Use ndpi to decode packets flows
 * @Memo:
 *  Create by hill, 4/23/2017
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ndpi_api.h>

#ifdef __cplusplus
}
#endif



class c_ndpi_decoder
{
public:
    c_ndpi_decoder();
    ~c_ndpi_decoder();

    static bool setup_detection();

private:
    static void* memory_wrapper(size_t _size);
    static void  free_wrapper(void* _freeable);

private:
    static struct ndpi_detection_module_struct * g_module;

};
