#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned long ulong;


void TEA_encrypt_a_block(ulong *v, ulong *k) {
    ulong sum = 0;
    ulong delta = 0x9e3779b9;
    for (int i = 0; i < 32; i++) {
        sum += delta;
        v[0] += ((v[1] << 4)+k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5)+k[1]);
        v[1] += ((v[0] << 4)+k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5)+k[3]);
    }
}


void TEA_decrypt_a_block(ulong *v, ulong *k) {
    ulong delta = 0x9e3779b9;
    ulong sum = 0xc6ef3720;
    for (int i = 0; i < 32; i++) {
        v[1] -= ((v[0] << 4)+k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5)+k[3]);
        v[0] -= ((v[1] << 4)+k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5)+k[1]);
        sum -= delta;
    }
}

uchar *TEA_encrypt(uchar *plaintext, ulong text_len, uchar *key) {
    /*key length : 128bit / 16bytes*/
    ulong buf_len = 0;
    if (text_len % 8 != 0) buf_len = (text_len / 8 + 1) * 8;
    else buf_len = text_len;
    uchar * buf = (ulong *)malloc(buf_len);
    memset(buf, 0, buf_len);
    for (int i = 0; i < text_len; i++) buf[i] = plaintext[i];
    for (int i = 0; i < text_len ; i += 8) {
        TEA_encrypt_a_block((ulong *)&buf[i], (ulong *)key);
    }
    return buf;
}

void TEA_decrypt(uchar *encrypt, ulong text_len, uchar *key) {
    /*key length : 128bit / 16bytes*/
    ulong buf_len = 0;
    if (text_len % 8 != 0) buf_len = (text_len / 8 + 1) * 8;
    else buf_len = text_len;
    uchar * buf = (ulong *)malloc(buf_len);
    memset(buf, 0, buf_len);
    for (int i = 0; i < text_len; i++) buf[i] = encrypt[i];
    for (int i = 0; i < text_len ; i += 8) {
        TEA_decrypt_a_block((ulong *)&buf[i], (ulong *)key);
    }
    for (int i = 0; i < text_len; i++) encrypt[i] = buf[i];
    free(buf);
}



int main() {
    /*a demo of hoe to use it*/
    uchar s[125];
    uchar key[16];
    memset(s, 'i', sizeof(s));
    memset(key, 'k', sizeof(key));
    key[2] = 'c';key[3] = 'b';
    s[2] = 'b';
    uchar *encry = TEA_encrypt(s, 125, key);
    //for (int i = 0; i < 125; i++) printf("%02X ", s[i]);printf("\n");
    TEA_decrypt(encry, 128, key);
    for (int i = 0; i < 125; i++) printf("%d:%c|",i,s[i]);
    return 0;
}