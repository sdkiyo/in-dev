#include <mxTun.h>
#include <templateMessages.h>

#include "../../net-client/include/netInterface.h"


uint8_t sendCallback(const char *const pResponse, const uint16_t responseLen, char* pRequest, uint16_t* pCurrentRequestLen, const uint16_t requestMaxLen, void* pUserData, const uint8_t prevCallbackReturnValue)
{
	if (prevCallbackReturnValue == CALLBACK_FIRST_TRY)
	{
		memcpy(pRequest, connectRequestTemplate, sizeof(connectRequestTemplate));

		generateUUIDv4(pRequest + sizeof(connectRequestTemplate));

		*pCurrentRequestLen = 84;// FIXME

		// ===== DEBUG DEBUG DEBUG =====
		printf("client request start:\n");
		for (uint8_t i = 0; i < *pCurrentRequestLen; i++)
		{
			printf("%c", pRequest[i]);
		}
		printf("\nclient request end\n");
		printf("client request start:\n");
		for (uint8_t i = 0; i < *pCurrentRequestLen; i++)
		{
			printf("0x%.2x ", pRequest[i]);
		}
		printf("\nclient request end\n");
		// ===== DEBUG DEBUG DEBUG =====

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
			makeSessionLoginRequest(((MxTunSentData*)pUserData)->pSession->pSessionToken, ((MxTunSentData*)pUserData)->pSession->sessionTokenLen, pRequest, pCurrentRequestLen);

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
			// ===== DEBUG DEBUG DEBUG =====

			return 0;// next callback
		}

		makeMessageObject(((MxTunSentData*)pUserData)->pData, ((MxTunSentData*)pUserData)->dataLength, ((MxTunSentData*)pUserData)->pSession->chatId, pRequest, pCurrentRequestLen);

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
		// ===== DEBUG DEBUG DEBUG =====

		// сообщить другому потоку что можно давать следующие данные


		return 0;
	}

	return CALLBACK_CLOSE_CONNECTION;
}
