#include "ident.h"
#include "cube.h"

vector<identinfo *> useridents;
void getidents(int ident, int prop, int idx)
{
 intret(useridents.length());

}

ICOMMAND(0, getuserident, "bbb", (int *ident, int *prop, int *idx, int *numargs), getidents(*ident, *prop, *idx));



static identinfo *newuserident(const char *name)
{
    identinfo *ident = new identinfo(name);


    useridents.add(ident);
    return ident;
}

void adduserident(const char *name)
{
    loopv(useridents) if(!strcmp(useridents[i]->name, name)) return;
    if(newuserident(name) && verbose >= 2)
        conoutf("added identity %s", name);
}
ICOMMAND(0, adduserident, "s", (char *n), adduserident(n));
