#include <stdio.h> 
#include <time.h> 
#include <rpc/rpc.h> 

#include "utils.h"
#include "tema1.h"
#include "sc_map.h"
#include "sc_array.h"
#include "structs.h"

char** request_authorization_1_svc(char** name, struct svc_req *cl) {
	char** var = calloc(sizeof(char*), 1);
	var[0] = calloc(sizeof(char), VARIOUS_TOKENS_LEN + 1);
	extern struct sc_map_sv client_map;
	
	printf("BEGIN %s AUTHZ\n", *name);

	client_data* data = (client_data*)sc_map_get_sv(&client_map, *name);
	if (sc_map_found(&client_map) == true) {
		data->request_token = generate_access_token(*name);
		printf("  RequestToken = %s\n", data->request_token);
		strcpy(var[0], data->request_token);
	} else {
		strcpy(var[0], USER_NOT_FOUND);
	}

	return var;
}

struct request_access_result* request_access_token_1_svc(struct request_access_params* req_access_params, struct svc_req *cl) {
	extern struct sc_map_sv client_map;
	extern struct sc_map_str access_token_to_name;
	extern int max_token_duration;
	struct request_access_result* resp = calloc(sizeof(struct request_access_result), 1);

	client_data* data = (client_data*)sc_map_get_sv(&client_map, req_access_params->name);
	if (sc_map_found(&client_map) == true) {
		if (data->signed_request_token == 1) {
			resp->access_token = generate_access_token(req_access_params->request_token);
			data->access_token = strdup(resp->access_token);
			printf("  AccessToken = %s\n", resp->access_token);
			if (req_access_params->automatic_refresh) {
				resp->refresh_token = generate_access_token(resp->access_token);
				data->refresh_token = strdup(resp->refresh_token);
				printf("  RefreshToken = %s\n", resp->refresh_token);
			} else {
				resp->refresh_token = strdup(NO_REFRESH_TOKEN);
			}
			resp->error_message = strdup(NO_ERROR);
			resp->has_errors = 0;
			
			data->token_duration = max_token_duration;
			data->automatic_refresh = req_access_params->automatic_refresh;
			sc_map_put_str(&access_token_to_name, strdup(resp->access_token), strdup(req_access_params->name));
		} else {
			resp->access_token = strdup(ERROR_VALUE);
			resp->refresh_token = strdup(ERROR_VALUE);
			resp->error_message = strdup(REQUEST_DENIED);
			resp->has_errors = 1;
		}
	} else {
		resp->access_token = strdup(ERROR_VALUE);
		resp->refresh_token = strdup(ERROR_VALUE);
		resp->error_message = strdup(REQUEST_DENIED);
		resp->has_errors = 1;
	}

	return resp;
}

struct validate_action_result* validate_delegated_action_1_svc(struct validate_action_params* params, struct svc_req *cl){
	extern struct sc_map_sv client_map;
	extern struct sc_map_str access_token_to_name;
	extern struct sc_array_str resources;
	extern int max_token_duration;

	struct validate_action_result* res = calloc(sizeof(struct validate_action_result), 1);
	res->access_token_refreshed = 0;
	res->new_access_token = strdup(NO_ACCESS_TOKEN);

	char* user_name = sc_map_get_str(&access_token_to_name, params->access_token);
	if (sc_map_found(&access_token_to_name) == false) {
		printf("DENY (%s,%s,,0)\n", params->operation, params->resource);
		res->result = strdup(PERMISSION_DENIED);
		return res;
	}


	client_data* data = (client_data*)sc_map_get_sv(&client_map, user_name);
	if (sc_map_found(&client_map) == false) {
		printf("DENY (%s,%s,%s,%d)\n", params->operation, params->resource, params->access_token, data->token_duration);
		res->result = strdup(PERMISSION_DENIED);
		return res;
	} 

	if (data->token_duration == 0 && data->automatic_refresh == 0) {
		printf("DENY (%s,%s,,%d)\n", params->operation, params->resource, data->token_duration);
		res->result = strdup(TOKEN_EXPIRED);
		return res;
	}

	if (data->token_duration == 0 && data->automatic_refresh == 1) {
		data->access_token = generate_access_token(data->refresh_token);
		data->refresh_token = generate_access_token(data->access_token);
		data->token_duration = max_token_duration;

		res->access_token_refreshed = 1;
		res->new_access_token = data->access_token;
		sc_map_put_str(&access_token_to_name, strdup(data->access_token), strdup(user_name));

		printf("BEGIN %s AUTHZ REFRESH\n", user_name);
		printf("  AccessToken = %s\n", data->access_token);
		printf("  RefreshToken = %s\n", data->refresh_token);
	}
	
	data->token_duration -= 1;

	const char* resource;
	unsigned char found_resource = 0;
	sc_array_foreach (&resources, resource) {
		if (strncmp(params->resource, resource, strlen(resource)) == 0) {
			found_resource = 1;
		}
	}

	if (!found_resource) {
		printf("DENY (%s,%s,%s,%d)\n", params->operation, params->resource, data->access_token, data->token_duration);
		res->result = strdup(RESOURCE_NOT_FOUND);
		return res;
	}

	char* permission = sc_map_get_str(&data->permissions, params->resource);
	if (sc_map_found(&data->permissions) == false) {
		printf("DENY (%s,%s,%s,%d)\n", params->operation, params->resource, data->access_token, data->token_duration);
		res->result = strdup(OPERATION_NOT_PERMITTED);
		return res;
	}

	if ((strncmp(params->operation, READ, strlen(READ)) == 0 && strstr(permission, "R") != NULL) ||
		(strncmp(params->operation, INSERT, strlen(INSERT)) == 0 && strstr(permission, "I") != NULL) ||
		(strncmp(params->operation, MODIFY, strlen(MODIFY)) == 0 && strstr(permission, "M") != NULL) ||
		(strncmp(params->operation, DELETE, strlen(DELETE)) == 0 && strstr(permission, "D") != NULL) ||
		(strncmp(params->operation, EXECUTE, strlen(EXECUTE)) == 0 && strstr(permission, "X") != NULL)) {
		res->result = strdup(PERMISSION_GRANTED);
		printf("PERMIT (%s,%s,%s,%d)\n", params->operation, params->resource, data->access_token, data->token_duration);
		return res;
	} else {
		res->result = strdup(OPERATION_NOT_PERMITTED);
		printf("DENY (%s,%s,%s,%d)\n", params->operation, params->resource, data->access_token, data->token_duration);
		return res;
	}
}

struct approve_request_result* approve_request_token_1_svc(char** request_token, struct svc_req *cl){
	extern struct sc_array_str approvals;
	extern int approvals_pointer;
	extern struct sc_map_sv client_map;

	struct approve_request_result* resp = calloc(sizeof(struct approve_request_result), 1);

	const char* key;
	const client_data* data;
	client_data* found_data;
	sc_map_foreach (&client_map, key, data) {
		if (data->request_token != NULL && strncmp(*request_token, data->request_token, strlen(*request_token)) == 0) {
			found_data = data;
		}
	}

	if (found_data == NULL) {
		resp->request_token = strdup(*request_token);
		return resp;
	}

	const char* client_approval_line = sc_array_at(&approvals, approvals_pointer);
	approvals_pointer++;
	if (approvals_pointer >= sc_array_size(&approvals)) {
		approvals_pointer = 0;
	}
	
	if (client_approval_line[0] == '*') {
		found_data->signed_request_token = 0;
		resp->signed_token = 0;
	} else {
		found_data->signed_request_token = 1;
		resp->signed_token = 1;

		if (sc_map_size_str(&found_data->permissions) > 0) {
			sc_map_clear_str(&found_data->permissions);
		}

		char* approval_line_copy = strdup(client_approval_line);
		char* permission_file;
		char* permission_level;
		permission_file = strtok(approval_line_copy, TOKEN_DELIMS);
		permission_level = strtok(NULL, TOKEN_DELIMS);

		sc_map_put_str(&found_data->permissions, permission_file, permission_level);
		while (permission_file != NULL && permission_level != NULL) {
			permission_file = strtok(NULL, TOKEN_DELIMS);
			permission_level = strtok(NULL, TOKEN_DELIMS);
			if (permission_file != NULL && permission_level != NULL) {
				sc_map_put_str(&found_data->permissions, permission_file, permission_level);
			}
		}
	}

	resp->request_token = strdup(*request_token);
	return resp;
}
