/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TEMA1_H_RPCGEN
#define _TEMA1_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct request_access_params {
	char *name;
	char *request_token;
	u_char automatic_refresh;
};
typedef struct request_access_params request_access_params;

struct request_access_result {
	char *access_token;
	char *refresh_token;
	char *error_message;
	u_char has_errors;
};
typedef struct request_access_result request_access_result;

struct validate_action_params {
	char *operation;
	char *resource;
	char *access_token;
};
typedef struct validate_action_params validate_action_params;

struct validate_action_result {
	char *result;
	char *new_access_token;
	u_char access_token_refreshed;
};
typedef struct validate_action_result validate_action_result;

struct approve_request_result {
	char *request_token;
	u_char signed_token;
};
typedef struct approve_request_result approve_request_result;

#define TEMA1_PROG 123456789
#define TEMA1_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define REQUEST_AUTHORIZATION 1
extern  char ** request_authorization_1(char **, CLIENT *);
extern  char ** request_authorization_1_svc(char **, struct svc_req *);
#define REQUEST_ACCESS_TOKEN 2
extern  struct request_access_result * request_access_token_1(struct request_access_params *, CLIENT *);
extern  struct request_access_result * request_access_token_1_svc(struct request_access_params *, struct svc_req *);
#define VALIDATE_DELEGATED_ACTION 3
extern  struct validate_action_result * validate_delegated_action_1(struct validate_action_params *, CLIENT *);
extern  struct validate_action_result * validate_delegated_action_1_svc(struct validate_action_params *, struct svc_req *);
#define APPROVE_REQUEST_TOKEN 4
extern  struct approve_request_result * approve_request_token_1(char **, CLIENT *);
extern  struct approve_request_result * approve_request_token_1_svc(char **, struct svc_req *);
extern int tema1_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define REQUEST_AUTHORIZATION 1
extern  char ** request_authorization_1();
extern  char ** request_authorization_1_svc();
#define REQUEST_ACCESS_TOKEN 2
extern  struct request_access_result * request_access_token_1();
extern  struct request_access_result * request_access_token_1_svc();
#define VALIDATE_DELEGATED_ACTION 3
extern  struct validate_action_result * validate_delegated_action_1();
extern  struct validate_action_result * validate_delegated_action_1_svc();
#define APPROVE_REQUEST_TOKEN 4
extern  struct approve_request_result * approve_request_token_1();
extern  struct approve_request_result * approve_request_token_1_svc();
extern int tema1_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_request_access_params (XDR *, request_access_params*);
extern  bool_t xdr_request_access_result (XDR *, request_access_result*);
extern  bool_t xdr_validate_action_params (XDR *, validate_action_params*);
extern  bool_t xdr_validate_action_result (XDR *, validate_action_result*);
extern  bool_t xdr_approve_request_result (XDR *, approve_request_result*);

#else /* K&R C */
extern bool_t xdr_request_access_params ();
extern bool_t xdr_request_access_result ();
extern bool_t xdr_validate_action_params ();
extern bool_t xdr_validate_action_result ();
extern bool_t xdr_approve_request_result ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TEMA1_H_RPCGEN */
