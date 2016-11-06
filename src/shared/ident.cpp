#include "ident.h"



vector<identinfo *> useridents;
int has_sodium_init = 0;
void check_sodium_init()
{
    if(has_sodium_init == 0)
    {
        int result = sodium_init();
        if(result == 1)
        {
            conoutf("Sodium Already Init. Should not be seeing this.");
        } else if(result == -1) {
            conoutf("Error starting sodium");
        } else if(result == 0) {
            conoutf("Started sodium Successfully");
            has_sodium_init = 1;
        } else {

           conoutf("Unknown Sodium Status: %d. Should not be seeing this. (may be impossible actually)",result);


        }
    }

}


void getidents(int ident, int prop, int idx)
{

    if(ident < 0)
        intret(useridents.length());
    else {
        switch(prop){
            case(0): {
                size_t len = strlen(useridents[ident]->name);
                char *tstr = newstring(len);
                memcpy(tstr, useridents[ident]->name, len);
                tstr[len] = '\0';
                stringret(tstr);
                }
                break;
            default:
                conoutf("Invalid Property. 0=name. Passed in: %d",prop);
                intret(0);
                break;
        }
    }


}

void switchuserident(){

    if(curuserident >= useridents.length())
    {
        conoutf("That Identity doesn't exist");
        curuserident = -1;
    } else {
        conoutf("User Identity Switched");
    }


}
VARF(IDF_PERSIST, curuserident, -1, -1,5000 ,switchuserident());

ICOMMAND(0, getuserident, "bbb", (int *ident, int *prop, int *idx, int *numargs), getidents(*ident, *prop, *idx));


/**
 * called by adduserident, creates a new identity file
 * @param name
 * @return
 */
static identinfo *newuserident(const char *name)
{
    check_sodium_init();
    if(strlen(name) < 1
       || strlen(name) > 128
       || strspn(name,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_") != strlen(name))
    {
        conoutf("Identity name is invalid. It must be between 1 and 128 characters and can only contain letters, numbers, underscores and dashes.");
        return NULL;
    }

    string fname;

    sprintf(fname, "idents/%s.json",name);

    identinfo *ident = new identinfo(name);

    crypto_sign_keypair(ident->pk, ident->sk);


    conoutf("Attempting to open file '%s'", fname);
    stream *f = openfile(fname, "w");
    if(!f) return NULL;
    string buf;

    f->printf("{\n\"name\":\"%s\",\n",ident->name);
    base64_encode(buf,ident->sk,crypto_sign_PUBLICKEYBYTES);
    f->printf("\"secret_key\":\"%s\",\n",buf);
    buf[0] = '\0';
    base64_encode(buf,ident->pk,crypto_sign_SECRETKEYBYTES);
    f->printf("\"public_key\":\"%s\"\n}",buf);
    delete f;
    useridents.add(ident);

    return ident;
}

void adduserident(const char *name)
{
    loopv(useridents) if(!strcmp(useridents[i]->name, name)) return;
    if(newuserident(name) /*&& verbose >= 2*/)
        conoutf("added identity %s", name);
}
ICOMMAND(0, adduserident, "s", (char *n), adduserident(n));
