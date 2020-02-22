#ifndef STRCTR
#define STRCTR

#include "header.h"

int replace(char *src, char* sub, char* target, int32_t n){
    src = strstr(src, sub);
    if(src != nullptr){
        for(int i=0; i<n; i++, src++, target++){
            *src = *target;
        }
        return 0;
    }
    return 1;
}

#endif