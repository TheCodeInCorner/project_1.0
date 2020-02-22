#ifndef SIO
#define SIO
#include "header.h"

#define BUFSIZE 8192

struct sio
{
    int32_t fd;
    int32_t cnt;
    char *bp;
    char buf[8192];

    int reset()
    {
        memset(&buf, '\0', BUFSIZE);
        fd = -1;
        bp = buf;
        return 0;
    }
};

ssize_t str_read(int32_t fd, void *usrbuf, size_t n)
{
    size_t left = n;
    ssize_t readnum = 0;
    char *bufp = (char*)usrbuf;

    while(left > 0){
        if((readnum = read(fd, bufp, left)) < 0){
            if(errno == EINTR){
                readnum = 0;
            }else{
                return -1; // error
            }
        }else if(readnum == 0){
            break; // short count
        }
        bufp += readnum;
        left -= readnum;
    }
    return (n-left);
};

ssize_t str_write(int32_t fd, void *usrbuf, size_t n)
{
    size_t left = n;
    ssize_t writenum = 0;
    char *bufp = (char*)usrbuf;

    while(left > 0){
        if((writenum = write(fd, bufp, left)) < 0){
            if(errno == EINTR){
                writenum = 0;
            }else{
                return -1;
            }
        }
        bufp += writenum;
        left -= writenum;
    }
    return n;
};


ssize_t str_readb(struct sio* buffer, void *usrbuf, size_t expect)
{
    int32_t cnt = 0;
    while(buffer->cnt <= 0){
        buffer->cnt = str_read(buffer->fd, buffer->buf, BUFSIZE);
        if(buffer->cnt < 0){
            return -1; //error
        }else if(buffer->cnt == 0){
            break;
        }
        buffer->bp = buffer->buf;
    }
    cnt = expect;
    if(buffer->cnt < expect) cnt = buffer->cnt;
    memcpy(usrbuf, buffer->bp, cnt);
    buffer->bp += cnt;
    buffer->cnt -= cnt;
    return cnt;
};

ssize_t str_readline(struct sio* buffer, void *usrbuf, size_t limit)
{
    ssize_t ret = 0,count = 0;
    char c;
    char *bufp = (char*)usrbuf;
    int32_t i=0;
    for(i = 1; i < limit; i++){
        if((ret = str_readb(buffer, &c, 1)) == -1 || ret == 0)return ret;
        *bufp++ = c;
        count++;
        if(c == '\n')break;
    }
    return count;
}

#endif