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


    if(curuserident >= useridents.length() || curuserident < 0)
    {
        conoutf("That Identity doesn't exist");
        curuserident = -1;
        strcpy(curuseridentname,"\0");
    } else {
        conoutf("User Identity Switched");
        strcpy(curuseridentname,useridents[curuserident]->name);
    }

}
void switchuseridentbyname(const char *ident_name){

    curuserident = ident_index(ident_name);
    if(curuserident >= useridents.length() || curuserident < 0)
    {
        conoutf("That Identity doesn't exist");
        curuserident = -1;
    } else {
        conoutf("User Identity Switched");
    }

}
VARF(0, curuserident, -1, -1,5000,switchuserident());
SVARF(IDF_PERSIST,curuseridentname,"",switchuseridentbyname(curuseridentname));
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
    base64_encode(buf,ident->sk,crypto_sign_SECRETKEYBYTES);
    f->printf("\"secret_key\":\"%s\",\n",buf);
    buf[0] = '\0';
    base64_encode(buf,ident->pk,crypto_sign_PUBLICKEYBYTES);
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
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}
void loaduserident(const char *name)
{
    loopv(useridents) if(!strcmp(useridents[i]->name, name)) return;

    identinfo *ident = new identinfo(name);
    string fname;

    sprintf(fname, "idents/%s.json",name);

    stream *f = openfile(fname, "r");
    char buf[f->size()];
    size_t siz  =f->size();
    f->read(buf,siz);


    jsmn_parser parser;

    //tokens
    jsmntok_t t[7];

    jsmn_init(&parser);
    int r = jsmn_parse(&parser, buf, siz, t, 7);

    if (r < 1 || t[0].type != JSMN_OBJECT) {
        conoutf("Object expected");
        return;
    }
    string buf2;
    for (int i = 1; i < r; i++) {
        if (jsoneq(buf, &t[i], "name") == 0) {

            sprintf(ident->name,"%.*s", t[i+1].end-t[i+1].start,
                    buf + t[i+1].start);
            i++;
        } else if (jsoneq(buf, &t[i], "secret_key") == 0) {

            sprintf(buf2,"%.*s", t[i+1].end-t[i+1].start,
                    buf + t[i+1].start);

            base64_decode(ident->sk,buf2);
            i++;
        } else if (jsoneq(buf, &t[i], "public_key") == 0) {

            sprintf(buf2,"%.*s", t[i+1].end-t[i+1].start,
                    buf + t[i+1].start);

            base64_decode(ident->pk,buf2);
            i++;
        }  else {
            conoutf("Unexpected key: %.*s\n", t[i].end-t[i].start,
                   buf + t[i].start);
        }
    }
    delete f;
    useridents.add(ident);


//    conoutf("Loaded ident with name: %s", ident->name);
//    base64_encode(buf,ident->pk,crypto_sign_PUBLICKEYBYTES);
//    conoutf("Loaded ident with pk: %s",buf);
//    base64_encode(buf,ident->sk,crypto_sign_SECRETKEYBYTES);
//    conoutf("Loaded ident with sk: %s", buf);


}
int ident_index(const char* name)
{
    loopv(useridents) if(!strcmp(useridents[i]->name, name)) return i;
    return -1;
}

void ident_index_command(const char* name)
{
    intret(ident_index(name));
}
ICOMMAND(0, useridentindex, "s", (const char* name), ident_index_command(name));
/**
 * Loads the user identities into the useridents vector. Should run once on startup
 */
void loaduseridents(){
    vector<char *> files;
    listfiles("idents","json",files);
    for(int i = 0; i<files.length();i++)
    {
        string ident_name;
        strcpy(ident_name, files[i]);
        int len = strlen(ident_name);
        conoutf("found ident: %s",ident_name);
        loaduserident(ident_name);
    }



}

ICOMMAND(0, loaduseridents, "", (void), loaduseridents());
