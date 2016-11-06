#ifndef RED_ECLIPSE_BASE_IDENT_H
#define RED_ECLIPSE_BASE_IDENT_H
#include <support/jsmn.h>
#include "engine.h"
#include "cube.h"
#include <string.h>
#include <sodium.h>
#include <support/base64.h>
extern int curuserident;
extern char* curuseridentname;
extern int has_sodium_init;
extern void loaduseridents();
extern int ident_index(const char* name);
#endif //RED_ECLIPSE_BASE_IDENT_H
