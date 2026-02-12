#include <mxTun.h>
#include <templateMessages.h>


void makeMessageObject(const char *const pData, const uint32_t dataLen, const uint64_t chatId, char *const pResult, uint32_t *const pResultLen)
{
	const uint32_t payloadLen = dataLen + sizeof(sendMessageTemplate) - 10;

	memcpy(pResult, sendMessageTemplate, sizeof(sendMessageTemplate));

	pResult[6] = (char) ((payloadLen & 0xFF000000) >> 24);// i love x86_64 processor architecture
	pResult[7] = (char) ((payloadLen & 0x00FF0000) >> 16);
	pResult[8] = (char) ((payloadLen & 0x0000FF00) >> 8);
	pResult[9] = (char) (payloadLen & 0x000000FF);

	pResult[19] = (char) ((chatId & 0xFF00000000000000) >> 56);// i love x86_64 processor architecture
	pResult[20] = (char) ((chatId & 0x00FF000000000000) >> 48);
	pResult[21] = (char) ((chatId & 0x0000FF0000000000) >> 40);
	pResult[22] = (char) ((chatId & 0x000000FF00000000) >> 32);
	pResult[23] = (char) ((chatId & 0x00000000FF000000) >> 24);
	pResult[24] = (char) ((chatId & 0x0000000000FF0000) >> 16);
	pResult[25] = (char) ((chatId & 0x000000000000FF00) >> 8);
	pResult[26] = (char) (chatId & 0x00000000000000FF);

	struct timespec tp = {};
	clock_gettime(CLOCK_REALTIME, &tp);

	const uint64_t cid = (uint64_t)(((uint64_t)(tp.tv_sec * 1'000)) + ((uint64_t)(tp.tv_nsec / 1'000'000)));// timestamp in milliseconds

	pResult[41] = (char) ((cid & 0xFF00000000000000) >> 56);// i love x86_64 processor architecture
	pResult[42] = (char) ((cid & 0x00FF000000000000) >> 48);
	pResult[43] = (char) ((cid & 0x0000FF0000000000) >> 40);
	pResult[44] = (char) ((cid & 0x000000FF00000000) >> 32);
	pResult[45] = (char) ((cid & 0x00000000FF000000) >> 24);
	pResult[46] = (char) ((cid & 0x0000000000FF0000) >> 16);
	pResult[47] = (char) ((cid & 0x000000000000FF00) >> 8);
	pResult[48] = (char) (cid & 0x00000000000000FF);

	pResult[55] = (char) ((dataLen & 0xFF000000) >> 24);// i love x86_64 processor architecture
	pResult[56] = (char) ((dataLen & 0x00FF0000) >> 16);
	pResult[57] = (char) ((dataLen & 0x0000FF00) >> 8);
	pResult[58] = (char) (dataLen & 0x000000FF);

	memcpy(pResult + sizeof(sendMessageTemplate), pData, dataLen);


	*pResultLen = sizeof(sendMessageTemplate) + dataLen;
}
