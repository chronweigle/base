
#ifndef __BASE64_H_
#define __BASE64_H_
#include <string.h>
void base64_encode(char* dest,unsigned char const* , unsigned int len);
void base64_decode(unsigned char* dest,const char* s);


#endif /* __BASE64_H_ */
