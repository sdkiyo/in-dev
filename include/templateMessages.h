#pragma once
#ifndef MX_TUNNEL_CLIENT_INCLUDE_TEMPLATE_MESSAGES_H_
#define MX_TUNNEL_CLIENT_INCLUDE_TEMPLATE_MESSAGES_H_


static const unsigned char connectRequestTemplate[] = {
	/* =======HEADERS======= */
	0x0B,			// protocolVersion: 11
	0x00, 0x00,		// cmd: 0
	0x00,			// sequence: 0
	0x00, 0x06,		// operationCode: 6 (SESSION_INIT)
	0x00, 0x00, 0x00, 0x4A,	// payloadLen: 74
	/* =======PAYLOAD=IN=MSGPACK======= */
	0x82,// fixmap
	0xA9,// fixstr
	'u','s','e','r','A','g','e','n','t',
	0x81,// fixmap
	0xAA,// fixstr
	'd','e','v','i','c','e','T','y','p','e',
	0xA3,// fixstr
	'I','O','S',
	0xA8,// fixstr
	'd','e','v','i','c','e','I','d',
	0xD9,// str 8
	0x24 // len
	/* then 36 bytes from random UUIDv4 */ };


static const unsigned char loginWithSessionToken_RequestTemplate[] = {
	/* =======HEADERS======= */
	0x0B,			// protocolVersion: 11
	0x00, 0x00,		// cmd: 0
	0x00,			// sequence: 0
	0x00, 0x13,		// operationCode: 19 (LOGIN)
	0x00, 0x00, 0x00, 0x00,	// payloadLen: (in template all zero)
	/* =======PAYLOAD=IN=MSGPACK======= */
	0x82,// fixmap
	0xA9,// fixstr
	'u','s','e','r','A','g','e','n','t',
	0x81,// fixmap
	0xAA,// fixstr
	'd','e','v','i','c','e','T','y','p','e',
	0xA3,// fixstr
	'I','O','S',

	0xA5,// fixstr
	't','o','k','e','n',
	0xDA,// str 16

	0x00,// len value (in template all zero)
	0x00 // len value (in template all zero)

	/* then from 0 to (2^16)-1 bytes from session token */ };


static const unsigned char sendMessageTemplate[] = {
	/* =======HEADERS======= */
	0x0B,			// protocolVersion: 11
	0x00, 0x00,		// cmd: 0
	0x00,			// sequence: 0
	0x00, 0x40,		// operationCode: 64 (MSG_EDIT)
	0x00, 0x00, 0x00, 0x00,	// payloadLen: (in template all zero)
	/* =======PAYLOAD=IN=MSGPACK======= */
	0x82,// fixmap
	0xA6,// fixstr
	'c','h','a','t','I','d',
	0xD3,// int 64

	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)

	0xA7,// fixstr
	'm','e','s','s','a','g','e',
	0x82,// fixmap
	0xA3,// fixstr
	'c','i','d',
	0xD3,// int 64

	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)

	0xA4,// fixstr
	't','e','x','t',
	0xDB,// str 32

	0x00,// len value (in template all zero)
	0x00,// len value (in template all zero)
	0x00,// len value (in template all zero)
	0x00,// len value (in template all zero)

	/* then from 0 to (2^32)-1 bytes from payload text for message */ };


static const unsigned char getChatHistoryTemplate[] = {
	/* =======HEADERS======= */
	0x0B,			// protocolVersion: 11
	0x00, 0x00,		// cmd: 0
	0x00,			// sequence: 0
	0x00, 0x40,		// operationCode: 64 (MSG_EDIT)
	0x00, 0x00, 0x00, 0x55,	// payloadLen: 85
	/* =======PAYLOAD=IN=MSGPACK======= */
	0x85,// fixmap
	0xA6,// fixstr
	'c','h','a','t','I','d',
	0xD3,// int 64

	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)

	0xA8,// fixstr
	'f','r','o','m','T','i','m','e',
	0x00,// fixint

	0xA7,// fixstr
	'f','o','r','w','a','r','d',
	0xD3,// int 64

	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)

	0xA8,// fixstr
	'b','a','c','k','w','a','r','d',
	0xD3,// int 64

	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)

	0xAB,// fixstr
	'g','e','t','M','e','s','s','a','g','e','s',
	0xC2,// false
};


static const unsigned char getMessageTemplate[] = {
	/* =======HEADERS======= */
	0x0B,			// protocolVersion: 11
	0x00, 0x00,		// cmd: 0
	0x00,			// sequence: 0
	0x00, 0x47,		// operationCode: 71 (MSG_GET)
	0x00, 0x00, 0x00, 0x26,	// payloadLen: 38
	/* =======PAYLOAD=IN=MSGPACK======= */
	0x82,// fixmap
	0xA6,// fixstr
	'c','h','a','t','I','d',
	0xD3,// int 64

	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)

	0xAA,// fixstr
	'm','e','s','s','a','g','e','I','d','s',
	0x91,// fixarray
	0xD3,// int 64

	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
	0x00,// value (in template all zero)
};


#endif // MX_TUNNEL_CLIENT_INCLUDE_TEMPLATE_MESSAGES_H_
