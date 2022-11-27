/**
	Sisteme de programe pentru retele de calculatoare
	
	Copyright (C) 2008 Ciprian Dobre & Florin Pop
	Univerity Politehnica of Bucharest, Romania

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 */

#include <stdio.h> 
#include <time.h> 
#include <rpc/rpc.h> 
#include <string.h>

#include "sc_map.h"
#include "tema1.h" 
#include "structs.h"

int main(int argc, char *argv[]){
	CLIENT *handle;
	
	handle=clnt_create(
		argv[1],
		TEMA1_PROG,
		TEMA1_VERS,
		"tcp");
	
	if(handle == NULL) {
		perror("");
		return -1;
	}

	struct sc_map_str name_to_access_token;
	sc_map_init_str(&name_to_access_token, 0, 0);

	char line[LINE_SIZE];
	char** res;
	char* user_name;
	char* command;
	char* last_arg;

	FILE* fp;
	fp = fopen(argv[2], READ_ONLY);
	while(fgets(line, LINE_SIZE, fp) != NULL) {
		user_name = strtok(line, TOKEN_DELIMS);
		command = strtok(NULL, TOKEN_DELIMS);

		if (strncmp(command, REQUEST, strlen(REQUEST)) == 0) {
			last_arg = strtok(NULL, TOKEN_DELIMS);
			int automatic_refresh = atoi(last_arg);

			res = request_authorization_1(&user_name, handle);
			if (strncmp(*res, USER_NOT_FOUND, strlen(USER_NOT_FOUND)) == 0) {
				printf("%s\n", USER_NOT_FOUND);
				continue;
			}
	
			struct approve_request_result* approve_request_res;
			approve_request_res = approve_request_token_1(res, handle);
	
			struct request_access_result* request_access_res;
			struct request_access_params request_access_parm;
			request_access_parm.name = user_name;
			request_access_parm.request_token = approve_request_res->request_token;
			request_access_parm.automatic_refresh = automatic_refresh;

			request_access_res = request_access_token_1(&request_access_parm, handle);
			if (request_access_res->has_errors == 1) {
				printf("%s\n", request_access_res->error_message);
			} else {
				sc_map_put_str(&name_to_access_token, strdup(request_access_parm.name), strdup(request_access_res->access_token));

				if (strncmp(request_access_res->refresh_token, NO_REFRESH_TOKEN, strlen(NO_REFRESH_TOKEN)) == 0) {
					printf("%s -> %s\n", approve_request_res->request_token, request_access_res->access_token);	
				} else {
					printf("%s -> %s,%s\n", approve_request_res->request_token, request_access_res->access_token, request_access_res->refresh_token);
				}
			}
		} else {
			last_arg = strtok(NULL, TOKEN_DELIMS);
			struct validate_action_params validate_action_param;
			validate_action_param.operation = command;
			validate_action_param.resource = last_arg;

			char* access_token_mapping = sc_map_get_str(&name_to_access_token, user_name);
			if (sc_map_found(&name_to_access_token) == true) {
				validate_action_param.access_token = access_token_mapping;
			} else {
				validate_action_param.access_token = strdup(NO_ACCESS_TOKEN);
			}

			struct validate_action_result* validate_action_res;
			validate_action_res = validate_delegated_action_1(&validate_action_param, handle);

			if (validate_action_res->access_token_refreshed) {
				sc_map_put_str(&name_to_access_token, strdup(user_name), strdup(validate_action_res->new_access_token));
			}

			printf("%s\n", validate_action_res->result);
		}
	}

	sc_map_term_str(&name_to_access_token);

	return 0;
}
