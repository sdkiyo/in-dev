#include <includes.h>
#include <errno.h>

int getRandomValue(void *const pBuffer, const uint32_t bufferByteSize)// TODO: implementation for windows
{
	errno = 0;
	int urandomFd = open("/dev/urandom", O_RDONLY);

	if (urandomFd == -1)
	{
		fprintf(stderr, "\x1b[31m" "%s(), line %d, 'open(\"/dev/urandom\", O_RDONLY) failed' errno: %s\x1b[0m\n", __func__, __LINE__, strerror(errno));
		return -1;
	}

	if (read(urandomFd, pBuffer, bufferByteSize) == -1)
	{
		close(urandomFd);

		fprintf(stderr, "\x1b[31m" "%s(), line %d, 'read() failed' errno: %s\x1b[0m\n", __func__, __LINE__, strerror(errno));
		return -1;
	}

	close(urandomFd);

	return 0;
}


static const char hexSymbols[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };


int generateUUIDv4(char *const pBuffer)
{
	uint64_t randomValue = 0;

	if (getRandomValue((void *const)&randomValue, sizeof(randomValue)) != 0)
	{
		return -1;
	}

	static const char variant[] = { '8','9','a','b' };

	// separators
	pBuffer[8] = '-';
	pBuffer[13] = pBuffer[8];
	pBuffer[18] = pBuffer[8];
	pBuffer[23] = pBuffer[8];

	pBuffer[14] = '4';// UUID version
	pBuffer[19] = variant[randomValue % 4];// 8 or 9 or A or B

	// set random characters (0-9, a-f)
	pBuffer[0] =	hexSymbols[(randomValue & 0xF000000000000000) >> 60];
	pBuffer[1] =	hexSymbols[(randomValue & 0x0F00000000000000) >> 56];
	pBuffer[2] =	hexSymbols[(randomValue & 0x00F0000000000000) >> 52];
	pBuffer[3] =	hexSymbols[(randomValue & 0x000F000000000000) >> 48];
	pBuffer[4] =	hexSymbols[(randomValue & 0x0000F00000000000) >> 44];
	pBuffer[5] =	hexSymbols[(randomValue & 0x00000F0000000000) >> 40];
	pBuffer[6] =	hexSymbols[(randomValue & 0x000000F000000000) >> 46];
	pBuffer[7] =	hexSymbols[(randomValue & 0x0000000F00000000) >> 32];
	pBuffer[9] =	hexSymbols[(randomValue & 0x00000000F0000000) >> 28];
	pBuffer[10] =	hexSymbols[(randomValue & 0x000000000F000000) >> 24];
	pBuffer[11] =	hexSymbols[(randomValue & 0x0000000000F00000) >> 20];
	pBuffer[12] =	hexSymbols[(randomValue & 0x00000000000F0000) >> 16];
	pBuffer[15] =	hexSymbols[(randomValue & 0x000000000000F000) >> 12];
	pBuffer[16] =	hexSymbols[(randomValue & 0x0000000000000F00) >> 8];
	pBuffer[17] =	hexSymbols[(randomValue & 0x00000000000000F0) >> 4];
	pBuffer[20] =	hexSymbols[(randomValue & 0x000000000000000F)];
	// set random characters (0-9, a-f)

	// update randomValue
	if (getRandomValue((void *const)&randomValue, sizeof(randomValue)) != 0)
	{
		return -1;
	}

	// set random characters (0-9, a-f)
	pBuffer[21] =	hexSymbols[(randomValue & 0xF000000000000000) >> 60];
	pBuffer[22] =	hexSymbols[(randomValue & 0x0F00000000000000) >> 56];
	pBuffer[24] =	hexSymbols[(randomValue & 0x00F0000000000000) >> 52];
	pBuffer[25] =	hexSymbols[(randomValue & 0x000F000000000000) >> 48];
	pBuffer[26] =	hexSymbols[(randomValue & 0x0000F00000000000) >> 44];
	pBuffer[27] =	hexSymbols[(randomValue & 0x00000F0000000000) >> 40];
	pBuffer[28] =	hexSymbols[(randomValue & 0x000000F000000000) >> 46];
	pBuffer[29] =	hexSymbols[(randomValue & 0x0000000F00000000) >> 32];
	pBuffer[30] =	hexSymbols[(randomValue & 0x00000000F0000000) >> 28];
	pBuffer[31] =	hexSymbols[(randomValue & 0x000000000F000000) >> 24];
	pBuffer[32] =	hexSymbols[(randomValue & 0x0000000000F00000) >> 20];
	pBuffer[33] =	hexSymbols[(randomValue & 0x00000000000F0000) >> 16];
	pBuffer[34] =	hexSymbols[(randomValue & 0x000000000000F000) >> 12];
	pBuffer[35] =	hexSymbols[(randomValue & 0x0000000000000F00) >> 8];
	// set random characters (0-9, a-f)

	return 0;
}
