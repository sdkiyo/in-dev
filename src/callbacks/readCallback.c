#include <mxTun.h>
#include <templateMessages.h>

#include "../../net-client/include/netInterface.h"


uint8_t readCallback(const char *const pResponse, const uint16_t responseLen, char* pRequest, uint16_t* pCurrentRequestLen, const uint16_t requestMaxLen, void* pUserData, const uint8_t prevCallbackReturnValue)
{
	if (prevCallbackReturnValue == CALLBACK_FIRST_TRY)
	{
		memcpy(pRequest, connectRequestTemplate, sizeof(connectRequestTemplate));

		generateUUIDv4(pRequest + sizeof(connectRequestTemplate));

		*pCurrentRequestLen = 84;// FIXME

		return 0;
	}
	else if (prevCallbackReturnValue != CALLBACK_FIRST_TRY && responseLen == 0)
	{
		printf("\x1b[33m" "NOT first try + no response" "\x1b[0m\n");
		return CALLBACK_CLOSE_CONNECTION;
	}
	else if (prevCallbackReturnValue != CALLBACK_FIRST_TRY & responseLen > 0)
	{
		// ===== DEBUG DEBUG DEBUG =====
		printf("\x1b[32mserver response start:\x1b[0m\n");
		for (uint16_t i = 0; i < responseLen; i++)
		{
			printf("%c", pResponse[i]);
		}
		printf("\x1b[32m\nserver response end\x1b[0m\n");
		// ===== DEBUG DEBUG DEBUG =====

		static uint8_t loginInSession = 0;

		if (loginInSession == 0)
		{
			loginInSession = 1;
			makeSessionLoginRequest(((MxTunData*)pUserData)->pSession->pSessionToken, ((MxTunData*)pUserData)->pSession->sessionTokenLen, pRequest, pCurrentRequestLen);
			return 0;// next callback
		}


		memcpy(pRequest, getChatHistoryTemplate, sizeof(getChatHistoryTemplate));

		pRequest[19] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0xFF00000000000000) >> 56);// i love x86_64 processor architecture
		pRequest[20] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00FF000000000000) >> 48);
		pRequest[21] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000FF0000000000) >> 40);
		pRequest[22] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000FF00000000) >> 32);
		pRequest[23] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00000000FF000000) >> 24);
		pRequest[24] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000000000FF0000) >> 16);
		pRequest[25] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000000000FF00) >> 8);
		pRequest[26] = (char) (((MxTunData*)pUserData)->pSession->chatId & 0x00000000000000FF);

		*pCurrentRequestLen = sizeof(getChatHistoryTemplate);
		printf("pCurrentRequestLen: %d\n", *pCurrentRequestLen);


		// ===== DEBUG DEBUG DEBUG =====
		printf("client request start:\n");
		for (uint32_t i = 0; i < *pCurrentRequestLen; i++)
		{
			printf("%c", pRequest[i]);
		}
		printf("\nclient request end\n");
		printf("client request start:\n");
		for (uint32_t i = 0; i < *pCurrentRequestLen; i++)
		{
			printf("0x%.2x ", pRequest[i]);
		}
		printf("\nclient request end\n");
		printf("\x1b[32mserver response start:\x1b[0m\n");
		for (uint16_t i = 0; i < responseLen; i++)
		{
			printf("0x%.2x ", pResponse[i]);
		}
		printf("\x1b[32m\nserver response end\x1b[0m\n");
		// ===== DEBUG DEBUG DEBUG =====

		// сообщить другому потоку что можно давать следующие данные


		return 0;
	}

	return CALLBACK_CLOSE_CONNECTION;
}
