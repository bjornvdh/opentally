#ifndef __OSCCLIENT_H
#define __OSCCLIENT_H

void oscclient_task(void*);
void oscclient_server_resolver_task(void*);
void oscclient_request_resubscribe();
bool oscclient_server_is_known();
#endif