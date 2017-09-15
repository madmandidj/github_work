#ifndef __MSG_TYPE_H__
#define __MSG_TYPE_H__


#include "../ChannelDefs/ChannelDefs.h"

typedef struct Msg Msg;




struct Msg
{
    long 	m_channel;
    void* 	m_data; /* changed this to void* for CDR project becaue should handle CDR messages and UI messages */
};





#endif /*#ifndef __MSG_TYPE_H__*/






