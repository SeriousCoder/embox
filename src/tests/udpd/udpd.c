/**
 * \file udpd.c
 *
 * \date 08.05.09
 * \author sikmir
 */

#include "types.h"
#include "common.h"
#include "udpd.h"
#include "shell.h"
#include "socket.h"

#define COMMAND_NAME "udpd"
static char available_keys[] = {
    #include "udpd_keys.inc"
};

static void show_help() {
	printf(
    #include "udpd_help.inc"
	);
}

int udpd_shell_handler(int argsc, char **argsv) {
	SHELL_KEY keys[MAX_SHELL_KEYS];
	char *key_value;
	int keys_amount;
	unsigned char addr[4];
	int port;

	keys_amount = parse_arg(COMMAND_NAME, argsc, argsv, available_keys,
			sizeof(available_keys), keys);

	if (get_key('a', keys, keys_amount, &key_value)) {
		if (NULL == ipaddr_scan(key_value, addr)) {
	                ERROR("wrong ip addr format (%s)\n", key_value);
	                show_help();
	                return -1;
	        }
	}
        if (!get_key('p', keys, keys_amount, &key_value)) {
                port = 666;
        } else if (1 != sscanf(key_value, "%d", &port)) {
                ERROR("enter validly port '-p'\n");
                show_help();
                return -1;
        }
	if (get_key('h', keys, keys_amount, &key_value)) {
		show_help();
		return 0;
	}
	int sk, n;
	char buf[1024];
	sk = socket(SOCK_DGRAM, UDP);
	if(sk < 0) {
		ERROR("opening socket\n");
		return -1;
	}
	if(bind(sk, addr, port)) {
		ERROR("binding socket\n");
		return -1;
	}
	while (1) {
		n = recv(sk, buf, 1024);
		if(n) break;
		sleep(1);
	}
	close(sk);
	return 0;
}
