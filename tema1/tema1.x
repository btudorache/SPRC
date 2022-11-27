/* request_access_token primeste numele client-ului, token-ul de request si specifica daca doreste refresh automate
   si intoarce tokenii ceruti sau eroare in cazul in care au aparut probleme pe partea de server 
*/
struct request_access_params {
	string name<>;
	string request_token<>;
	unsigned char automatic_refresh;
};

struct request_access_result {
	string access_token<>;
	string refresh_token<>;
	string error_message<>;
	unsigned char has_errors;
};

/* validate_delegated_action primeste numele resursei, operatia executata si token-ul de acces 
   si intoarce rezultatul operatiei, si noul token de refresh in cazul reinoirii automate 
*/
struct validate_action_params {
	string operation<>;
	string resource<>;
	string access_token<>;
};

struct validate_action_result {
	string result<>;
	string new_access_token<>;
	unsigned char access_token_refreshed;
};

/* approve_request_result primeste tokenul de request si intoarce token-ul de request semnat */
struct approve_request_result {
	string request_token<>;
	unsigned char signed_token;
};

/* cele 4 functii ale interfetei. request_authorization primeste numele client-ului care cere autorizarea,
   si intoarce un token de request.
   
   ceilalti parametri si valori de return ale celorlalte functii sunt explicate mai sus 
*/
program TEMA1_PROG {
	version TEMA1_VERS {
		string REQUEST_AUTHORIZATION(string) = 1;
		struct request_access_result REQUEST_ACCESS_TOKEN(struct request_access_params) = 2;
		struct validate_action_result VALIDATE_DELEGATED_ACTION(struct validate_action_params) = 3;
		struct approve_request_result APPROVE_REQUEST_TOKEN(string) = 4;
	} = 1;
} = 123456789;
