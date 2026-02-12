#include <mxTun.h>
#include <templateMessages.h>


void makeSessionLoginRequest(const char *const pSessionToken, const uint16_t sessionTokenLen, char *const pResult, uint32_t *const pResultLen)
{
	const uint32_t payloadLen = sessionTokenLen + sizeof(loginWithSessionToken_RequestTemplate) - 10;

	memcpy(pResult, loginWithSessionToken_RequestTemplate, sizeof(loginWithSessionToken_RequestTemplate));

	pResult[6] = (char) ((payloadLen & 0xFF000000) >> 24);
	pResult[7] = (char) ((payloadLen & 0x00FF0000) >> 16);
	pResult[8] = (char) ((payloadLen & 0x0000FF00) >> 8);
	pResult[9] = (char) (payloadLen & 0x000000FF);

	pResult[44] = (char) ((sessionTokenLen & 0xFF00) >> 8);
	pResult[45] = (char) (sessionTokenLen & 0x00FF);

	memcpy(pResult + sizeof(loginWithSessionToken_RequestTemplate), pSessionToken, sessionTokenLen);

	*pResultLen = sizeof(loginWithSessionToken_RequestTemplate) + sessionTokenLen;
}
