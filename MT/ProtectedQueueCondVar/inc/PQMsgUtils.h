#ifndef __PQ_MSG_UTILS_H__
#define __PQ_MSG_UTILS_H__


/*typedef void (*ReadMsg)(void* _msg, char* _callerName, char* _comMode);*/
typedef void (*ReadMsg)(void* _msg, char* _callerName, char* _comMode, int _callerID);



#endif /* #ifndef __PQ_MSG_UTILS_H__ */