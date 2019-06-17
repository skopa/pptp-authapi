#define MAXINT 4294967295

#include "pppd/pppd.h"
//#include "pppd/chap_ms.h"
#include "pppd/chap-new.h"
//#include "pppd/md4.h"

// LIST OF AVAILABLE OPTIONS
#include "options.h"


#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "string.c"
#include "nxjson.c"
#include "request.c"

char pppd_version[] = VERSION;


static int UserID = 0;
static u_int32_t ClientIP = 0;

static unsigned int lastUsageIn = 0;
static unsigned int lastUsageOut = 0;



/**
 * Check if api is ready for requests.
 */
int _chap_check(void) {
    dbglog("ApiAuth: Checking verification ready");

    char *url = getRequestUrl("action=check");
    const nx_json *json = nx_json_parse(
            getChar(makeRequest(url)), 0
    );

    if (json) {
        int v = (int) nx_json_get(nx_json_get(json, "data"), "is_ready")->int_value;
        if (v == 1) {
            dbglog("ApiAuth: Checking ready successful");
            return 1;
        } else {
            dbglog("ApiAuth: Checking unsuccessful! Api not ready!");
            return -1;
        }
    }

    error("ApiAuth: Check verification ready error! Response error!");
    return 0;
}


/**
 * Verify password.
 */
int _chap_verify(char *name, char *ourname, int id,
                 struct chap_digest_type *digest,
                 unsigned char *challenge, unsigned char *response,
                 char *message, int message_space) {
    dbglog("ApiAuth: Verification peer start");

    char *clientip;
    char *secret;
    int secret_len;
    int ok = 0;

    struct string *t, *p = concat(newString("action=auth&user="), newString(name));
    
    char *url = getRequestUrl(getChar(p));

    dbglog("ApiAuth: Request url:");
    dbglog(url);

    const nx_json *json = nx_json_parse(
            getChar(makeRequest(url)), 0
    );

    if (json) {
        int code = (int) nx_json_get(json, "code")->int_value;
        if (code == 200) {
            dbglog("ApiAuth: Verification response get successful");
            secret = (char *) nx_json_get(nx_json_get(json, "data"), "password")->text_value;
            secret_len = strlen(secret);
            UserID = (int) nx_json_get(nx_json_get(json, "data"), "id")->int_value;
            clientip = (char *) nx_json_get(nx_json_get(json, "data"), "ip")->text_value;
            inet_aton(clientip, (struct in_addr *) &ClientIP);
            ok = digest->verify_response(id, name, secret, secret_len, challenge, response, message, message_space);
            memset(secret, 0, sizeof(secret));
            return ok;
        }
    }

    error("ApiAuth: Verify peer error! Response error!");
    return 0;
}

void _ip_choose(u_int32_t * address) {
    dbglog("ApiAuth: Set peer ip address");
    *address = ClientIP;
    return;
}


int _allowed_address(u_int32_t address) {
    dbglog("ApiAuth: Checking ip address");
    return (address == ClientIP);
}

void plugin_init(void) {

    dbglog("ApiAuth: Plugin init");
    add_options(options);
    dbglog("Options setted");

    chap_check_hook = _chap_check;
    chap_verify_hook = _chap_verify;

    ip_choose_hook = _ip_choose;
    allowed_address_hook = _allowed_address;
    //idle_time_hook=mysql_idle_time;

    //add_notifier(&ip_down_notifier, mysql_ip_down_notifier, NULL);

    return;
}
