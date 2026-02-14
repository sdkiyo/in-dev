#include <mxTun.h>
#include <templateMessages.h>

#include "../../net-client/include/netInterface.h"


uint8_t readCallback(const char *const pResponse, const uint16_t responseLen, char* pRequest, uint16_t* pCurrentRequestLen, const uint16_t requestMaxLen, void* pUserData, const uint8_t prevCallbackReturnValue)
{
	if (prevCallbackReturnValue == CALLBACK_FIRST_TRY)
	{
		memcpy(pRequest, connectRequestTemplate, sizeof(connectRequestTemplate));

		generateUUIDv4(pRequest + sizeof(connectRequestTemplate));

		*pCurrentRequestLen = sizeof(connectRequestTemplate) + 36;// FIXME

		return 0;
	}
//	else if (prevCallbackReturnValue != CALLBACK_FIRST_TRY && responseLen == 0)
//	{
//		printf("\x1b[33m" "NOT first try + no response" "\x1b[0m\n");
//		return CALLBACK_CLOSE_CONNECTION;
//	}
	else if (prevCallbackReturnValue != CALLBACK_FIRST_TRY & responseLen >= 0)
	{
		// ===== DEBUG DEBUG DEBUG =====
//		printf("\x1b[32mserver response start:\x1b[0m\n");
//		for (uint32_t i = 0; i < responseLen; i++)
//		{
//			printf("%c", pResponse[i]);
//		}
//		printf("\x1b[32m\nserver response end\x1b[0m\n");
		// ===== DEBUG DEBUG DEBUG =====

		static uint8_t loginInSession = 0;

		if (loginInSession == 0)
		{
			loginInSession = 1;
			makeSessionLoginRequest(((MxTunData*)pUserData)->pSession->pSessionToken, ((MxTunData*)pUserData)->pSession->sessionTokenLen, pRequest, pCurrentRequestLen);
			return 0;// next callback
		}
		if (loginInSession < 2)
		{
			loginInSession++;
			*pCurrentRequestLen = 0;
			return 0;
		}

		if (loginInSession == 2)
		{
			loginInSession++;
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
			return 0;
		}

		static uint16_t messageIdsCount = 0;
		static uint64_t* messageIds = nullptr;

		if (messageIdsCount == 0)// FIXME переписать чтобы была отправка нового getChatHistory, и вообще нормальный код сделать
		{
			printf("\x1b[32mserver response start:\x1b[0m\n");
			for (uint32_t i = 0; i < responseLen; i++)
			{
				printf("%c", pResponse[i]);
			}
			printf("\x1b[32m\nserver response end\x1b[0m\n");
			printf("\x1b[32mserver response start:\x1b[0m\n");
			for (uint32_t i = 0; i < responseLen; i++)
			{
				printf("0x%.2x ", pResponse[i]);
			}
			printf("\x1b[32m\nserver response end\x1b[0m\n");
			printf("parsing start\n");
			for (uint32_t i = 0; i < responseLen; i++)
			{
				if (pResponse[i] == (char)0xD3)
				{
					messageIdsCount++;
					i += 8;// FIXME
				}
			}
			printf("parsing end\n");
			printf("итого: %d\n", messageIdsCount);
			messageIds = malloc(sizeof(*messageIds) * messageIdsCount);// FIXME

			messageIdsCount = 0;// FIXME

			for (uint32_t i = 0; i < responseLen; i++)// FIXME not memory-safety realization FIXME
			{
				if (pResponse[i] == (char)0xD3)
				{
					i++;
					messageIds[messageIdsCount] = 0;// FIXME
					memcpy(messageIds + messageIdsCount, pResponse + i, sizeof(*pResponse) * 8);
					i += 7;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++] << 56;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++] << 48;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++] << 40;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++] << 32;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++] << 24;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++] << 16;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++] << 8;
//					messageIds[messageIdsCount] |= (uint64_t) pResponse[i++];

					messageIdsCount++;
				}
			}
			*pCurrentRequestLen = 0;
			return SKIP_NEXT_SERVER_RESPONSE;
		}


		memcpy(pRequest, getMessageTemplate, sizeof(getMessageTemplate));

		pRequest[19] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0xFF00000000000000) >> 56);// i love x86_64 processor architecture
		pRequest[20] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00FF000000000000) >> 48);
		pRequest[21] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000FF0000000000) >> 40);
		pRequest[22] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000FF00000000) >> 32);
		pRequest[23] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00000000FF000000) >> 24);
		pRequest[24] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000000000FF0000) >> 16);
		pRequest[25] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000000000FF00) >> 8);
		pRequest[26] = (char) (((MxTunData*)pUserData)->pSession->chatId & 0x00000000000000FF);

		pRequest[47] = (char) ((messageIds[messageIdsCount] & 0xFF00000000000000) >> 56);
		pRequest[46] = (char) ((messageIds[messageIdsCount] & 0x00FF000000000000) >> 48);
		pRequest[45] = (char) ((messageIds[messageIdsCount] & 0x0000FF0000000000) >> 40);
		pRequest[44] = (char) ((messageIds[messageIdsCount] & 0x000000FF00000000) >> 32);
		pRequest[43] = (char) ((messageIds[messageIdsCount] & 0x00000000FF000000) >> 24);
		pRequest[42] = (char) ((messageIds[messageIdsCount] & 0x0000000000FF0000) >> 16);
		pRequest[41] = (char) ((messageIds[messageIdsCount] & 0x000000000000FF00) >> 8);
		pRequest[40] = (char) (messageIds[messageIdsCount] & 0x00000000000000FF);

		printf("кого: %d\n", messageIdsCount);
		messageIdsCount--;
		printf("\x1b[32mserver response start:\x1b[0m\n");
		for (uint32_t i = 0; i < responseLen; i++)
		{
			printf("%c", pResponse[i]);
		}
		printf("\x1b[32m\nserver response end\x1b[0m\n");
		printf("\x1b[32mserver response start:\x1b[0m\n");
		for (uint32_t i = 0; i < responseLen; i++)
		{
			printf("0x%.2x ", pResponse[i]);
		}
		printf("\x1b[32m\nserver response end\x1b[0m\n");

		*pCurrentRequestLen = sizeof(getMessageTemplate);
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

		if (messageIdsCount == 0)
		{
			free(messageIds);// FIXME
			messageIds = nullptr;
		}

		// ===== DEBUG DEBUG DEBUG =====
		// ===== DEBUG DEBUG DEBUG =====

		// сообщить другому потоку что можно давать следующие данные


		return 0;
	}

	return CALLBACK_CLOSE_CONNECTION;
}
