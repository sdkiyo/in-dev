#pragma once
#ifndef MX_TUN_CLIENT_INCLUDE_MX_TUN_H_
#define MX_TUN_CLIENT_INCLUDE_MX_TUN_H_


#include "includes.h"


typedef struct MxTunSession {
	const char*	pSessionToken;
	uint64_t	chatId;
	uint16_t	sessionTokenLen;
} MxTunSession;

typedef struct MxTunData {
	const MxTunSession*	pSession;
	const void*		pData;
	uint32_t		dataLength;
} MxTunData;


#define MX_UNREAD	0
#define MX_READ		1


int generateUUIDv4(char *const pBuffer);


void makeSessionLoginRequest(const char *const pSessionToken, const uint16_t sessionTokenLen, char *const pResult, uint32_t *const pResultLen);

void makeMessageObject(const char *const pData, const uint32_t dataLen, const uint64_t chatId, char *const pResult, uint32_t *const pResultLen);


uint8_t sendCallback(const char *const pResponse, const uint16_t responseLen, char* pRequest, uint16_t* pCurrentRequestLen, const uint16_t requestMaxLen, void* pUserData, const uint8_t prevCallbackReturnValue);
uint8_t readCallback(const char *const pResponse, const uint16_t responseLen, char* pRequest, uint16_t* pCurrentRequestLen, const uint16_t requestMaxLen, void* pUserData, const uint8_t prevCallbackReturnValue);




#endif // MX_TUN_CLIENT_INCLUDE_MX_TUN_H_
