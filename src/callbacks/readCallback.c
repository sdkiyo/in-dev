#include <mxTun.h>
#include <templateMessages.h>

#include "../../net-client/include/netInterface.h"


void parseMessageIdsCount(const char *const pResponse, const uint32_t responseLen, uint16_t* pMessageIdsCount)
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
		if (pResponse[i] != (char)0xD3)
		{
			continue;
		}

		i += 8;

		if (i > responseLen)
		{
			break;
		}

		(*pMessageIdsCount)++;
	}

	printf("parsing end\n");
	printf("итого: %d\n", *pMessageIdsCount);
}


void parseMessageIds(const char *const pResponse, const uint32_t responseLen, uint64_t* pMessageIds, uint16_t messageIdsCount)
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

	uint16_t temp = 0;

	for (uint32_t i = 0; i < responseLen; i++)
	{
		if (pResponse[i] != (char)0xD3)
		{
			continue;
		}

		if ((i + 8) > responseLen)
		{
			break;
		}

		i++;
		pMessageIds[temp] = ((uint64_t) pResponse[i++] & 0x00000000000000FF) << 56;
		pMessageIds[temp] |= ((uint64_t) pResponse[i++] & 0x00000000000000FF) << 48;
		pMessageIds[temp] |= ((uint64_t) pResponse[i++] & 0x00000000000000FF) << 40;
		pMessageIds[temp] |= ((uint64_t) pResponse[i++] & 0x00000000000000FF) << 32;
		pMessageIds[temp] |= ((uint64_t) pResponse[i++] & 0x00000000000000FF) << 24;
		pMessageIds[temp] |= ((uint64_t) pResponse[i++] & 0x00000000000000FF) << 16;
		pMessageIds[temp] |= ((uint64_t) pResponse[i++] & 0x00000000000000FF) << 8;
		pMessageIds[temp] |= ((uint64_t) pResponse[i] & 0x00000000000000FF);

		printf("......................:\n");
		printf("0x%.8x", (uint32_t)(pMessageIds[temp] >> 32));
		printf("%.8x", (uint32_t) pMessageIds[temp]);
		printf("\n......................\n");

		temp++;

		if (temp == messageIdsCount)
		{
			break;
		}
	}
}


uint8_t readCallback(const char *const pResponse, const uint16_t responseLen, char* pRequest, uint16_t* pCurrentRequestLen, const uint16_t requestMaxLen, void* pUserData, const uint8_t prevCallbackReturnValue)
{
	if (prevCallbackReturnValue == CALLBACK_FIRST_TRY)
	{
		memcpy(pRequest, connectRequestTemplate, sizeof(connectRequestTemplate));

		generateUUIDv4(pRequest + sizeof(connectRequestTemplate));

		*pCurrentRequestLen = sizeof(connectRequestTemplate) + 36;// FIXME

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
//		printf("\x1b[32mserver response start:\x1b[0m\n");
//		for (uint32_t i = 0; i < responseLen; i++)
//		{
//			printf("%c", pResponse[i]);
//		}
//		printf("\x1b[32m\nserver response end\x1b[0m\n");
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
		// ===== DEBUG DEBUG DEBUG =====

		static uint8_t loginInSession = 0;

		if (loginInSession == 0)
		{
			loginInSession = 1;
			makeSessionLoginRequest(((MxTunData*)pUserData)->pSession->pSessionToken, ((MxTunData*)pUserData)->pSession->sessionTokenLen, pRequest, pCurrentRequestLen);
			return 0;// next callback
		}

		if (loginInSession < 2)// FIXME FIXME FIXME
		{
			loginInSession++;
			*pCurrentRequestLen = 0;
			return 0;
		}// FIXME FIXME FIXME

		static uint8_t flag = 0;
		static uint16_t temp = 0;

		static uint16_t messageIdsCount = 0;
		static uint64_t* messageIds = nullptr;

		if (temp == messageIdsCount && flag == 0)
		{
			flag = 1;
			memcpy(pRequest, getChatHistoryTemplate, sizeof(getChatHistoryTemplate));

			pRequest[19] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0xFF00000000000000) >> 56);
			pRequest[20] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00FF000000000000) >> 48);
			pRequest[21] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000FF0000000000) >> 40);
			pRequest[22] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000FF00000000) >> 32);
			pRequest[23] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00000000FF000000) >> 24);
			pRequest[24] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000000000FF0000) >> 16);
			pRequest[25] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000000000FF00) >> 8);
			pRequest[26] = (char) (((MxTunData*)pUserData)->pSession->chatId & 0x00000000000000FF);

			*pCurrentRequestLen = sizeof(getChatHistoryTemplate);

			return 0;
		}

		if (flag == 1)
		{
			temp = 0;
			messageIdsCount = 0;
			flag = 0;

			free(messageIds);
			messageIds = nullptr;

			parseMessageIdsCount(pResponse, responseLen, &messageIdsCount);

			messageIds = (uint64_t*) malloc(sizeof(*messageIds) * messageIdsCount);

			parseMessageIds(pResponse, responseLen, messageIds, messageIdsCount);
			printf("итого result: %d\n", messageIdsCount);
		}

		memcpy(pRequest, getMessageTemplate, sizeof(getMessageTemplate));

		pRequest[19] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0xFF00000000000000) >> 56);
		pRequest[20] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00FF000000000000) >> 48);
		pRequest[21] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000FF0000000000) >> 40);
		pRequest[22] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000FF00000000) >> 32);
		pRequest[23] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x00000000FF000000) >> 24);
		pRequest[24] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x0000000000FF0000) >> 16);
		pRequest[25] = (char) ((((MxTunData*)pUserData)->pSession->chatId & 0x000000000000FF00) >> 8);
		pRequest[26] = (char) (((MxTunData*)pUserData)->pSession->chatId & 0x00000000000000FF);

		pRequest[40] = (char) ((messageIds[temp] & 0xFF00000000000000) >> 56);
		pRequest[41] = (char) ((messageIds[temp] & 0x00FF000000000000) >> 48);
		pRequest[42] = (char) ((messageIds[temp] & 0x0000FF0000000000) >> 40);
		pRequest[43] = (char) ((messageIds[temp] & 0x000000FF00000000) >> 32);
		pRequest[44] = (char) ((messageIds[temp] & 0x00000000FF000000) >> 24);
		pRequest[45] = (char) ((messageIds[temp] & 0x0000000000FF0000) >> 16);
		pRequest[46] = (char) ((messageIds[temp] & 0x000000000000FF00) >> 8);
		pRequest[47] = (char) (messageIds[temp] & 0x00000000000000FF);

		printf("кого: %d\n", messageIdsCount);
		temp++;

		*pCurrentRequestLen = sizeof(getMessageTemplate);


		// ===== DEBUG DEBUG DEBUG =====
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
