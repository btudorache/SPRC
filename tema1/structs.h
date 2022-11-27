#ifndef STRUCTS_H
#define STRUCTS_H

#include "sc_map.h"

#define VARIOUS_TOKENS_LEN 15
#define LINE_SIZE 120
#define MEDIUM_BUFFER_SIZE 60

#define READ_ONLY "r"
#define TOKEN_DELIMS ",\r\n"

#define REQUEST "REQUEST"
#define USER_NOT_FOUND "USER_NOT_FOUND"
#define NO_REFRESH_TOKEN "NO_REFRESH_TOKEN"
#define NO_ACCESS_TOKEN "NO_ACCESS_TOKEN"
#define NO_ERROR "NO_ERROR"
#define ERROR_VALUE "ERROR_VALUE"
#define REQUEST_DENIED "REQUEST_DENIED"
#define PERMISSION_DENIED "PERMISSION_DENIED"
#define PERMISSION_GRANTED "PERMISSION_GRANTED"
#define TOKEN_EXPIRED "TOKEN_EXPIRED" 
#define RESOURCE_NOT_FOUND "RESOURCE_NOT_FOUND"
#define OPERATION_NOT_PERMITTED "OPERATION_NOT_PERMITTED"
#define READ "READ"
#define INSERT "INSERT"
#define MODIFY "MODIFY"
#define DELETE "DELETE"
#define EXECUTE "EXECUTE"

struct client_data {
	char* name;
	char* request_token;
	char* access_token;
	char* refresh_token;
    unsigned char signed_request_token;
	unsigned char automatic_refresh;
	int token_duration;
	struct sc_map_str permissions;
};
typedef struct client_data client_data;

#endif /* STRUCTS_H */