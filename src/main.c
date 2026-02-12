#include <stdio.h>
#include <dlfcn.h>

#include <mxTun.h>

#include "../../net-client/include/netInterface.h"


int main(const int argc, const char *const *const argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "please set session token in arguments\n");
	}

	fprintf(stderr, "\x1b[2m" "test1 start" "\x1b[0m" "\n");


	void* netClientLib = dlopen("../../net-client/tests/test1/build/libnet-client.so", RTLD_LAZY);

	if (netClientLib == nullptr)
	{
		fprintf(stderr, "\x1b[31m" "failed to load library" "\x1b[0m" "\n");
		return -1;
	}

	PFN_clientConnectSsl clientConnectSsl = (PFN_clientConnectSsl) dlsym(netClientLib, "clientConnectSsl");

	MxTunSession session = {};
	session.pSessionToken		= argv[1];
	session.sessionTokenLen		= (uint16_t) strlen(argv[1]);// FIXME
	session.chatId			= 0x00;// FIXME

	MxTunSentData sentData = {};
	sentData.pSession		= &session;
	sentData.pData			= (void*) "teto";
	sentData.dataLength		= 4;


	if (clientConnectSsl("api.oneme.ru", 443, sendCallback, (uint16_t)2048, (uint16_t)4096, &sentData) != 0)
	{
		fprintf(stderr, "\x1b[31mclientConnectSsl() failed\x1b[0m\n");
		return -1;
	}



	dlclose(netClientLib);


	fprintf(stderr, "\x1b[2m" "test1 end" "\x1b[0m" "\n");
	return 0;
}
