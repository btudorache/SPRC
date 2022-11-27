/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "tema1.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

char **
request_authorization_1(char **argp, CLIENT *clnt)
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, REQUEST_AUTHORIZATION,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct request_access_result *
request_access_token_1(struct request_access_params *argp, CLIENT *clnt)
{
	static struct request_access_result clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, REQUEST_ACCESS_TOKEN,
		(xdrproc_t) xdr_request_access_params, (caddr_t) argp,
		(xdrproc_t) xdr_request_access_result, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct validate_action_result *
validate_delegated_action_1(struct validate_action_params *argp, CLIENT *clnt)
{
	static struct validate_action_result clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, VALIDATE_DELEGATED_ACTION,
		(xdrproc_t) xdr_validate_action_params, (caddr_t) argp,
		(xdrproc_t) xdr_validate_action_result, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct approve_request_result *
approve_request_token_1(char **argp, CLIENT *clnt)
{
	static struct approve_request_result clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, APPROVE_REQUEST_TOKEN,
		(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
		(xdrproc_t) xdr_approve_request_result, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}