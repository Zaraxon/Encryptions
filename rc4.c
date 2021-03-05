#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned long ulong;


uchar * RC4_KSA(uchar * key,ulong key_len) {

    uchar * sbox = (uchar *)malloc(256);
    ulong i = 0, j = 0;
    char k[256];
    for (i = 0; i < 256; i++) {
        sbox[i] = i;
        k[i] = key[i % key_len];
    }
    for (i = 0; i < 256; i++) {
        j = (j + sbox[i] + k[i]) % 256;
        uchar tmp;
        tmp = sbox[i];
        sbox[i] = sbox[j];
        sbox[j] = tmp;
    }
    return sbox;
}

void RC4_CRYPT(uchar * Data, ulong Data_len,uchar * key, ulong key_len) {
    /*
        key : no longer than 256bytes
        DATA : no longer than 256bytes
    */
    uchar * sbox = RC4_KSA(key, key_len);
    ulong i = 0, j = 0, k = 0;
    ulong t = 0;
    for (k = 0; k < Data_len; k++) {
        i = (i + 1) % 256;
        j = (j + sbox[i]) % 256;
        uchar tmp = sbox[i];
        sbox[i] = sbox[j];
        sbox[j] = tmp;
        t = (sbox[i] + sbox[j]) % 256;
        Data[k] ^= sbox[t];
    }
}


void RC4_encrypt(uchar * Data, ulong Data_len,uchar * key, ulong key_len) {
    /*API for Encrypt*/
    /*key no longer than 256bytes, or this will just use first 256bytes if key has more than 256bytes*/
    for (int i = 0; i < Data_len; i += 256) {
        if (i + 256 > Data_len)
            RC4_CRYPT(Data, Data_len - i, key, key_len);
        else 
            RC4_CRYPT(Data, 256, key, key_len);
    }
}


void RC4_decrypt(uchar * Data, ulong Data_len,uchar * key, ulong key_len) {
    /*API for Decrypt*/
    /*key no longer than 256bytes, or this will just use first 256bytes if key has more than 256bytes*/
    for (int i = 0; i < Data_len; i += 256) {
        if (i + 256 > Data_len)
            RC4_CRYPT(Data, Data_len - i, key, key_len);
        else 
            RC4_CRYPT(Data, 256, key, key_len);
    }
}

int main() {
    /*a demo of how to use it*/
    uchar s[259] = {0};
    uchar key[256];
    memset(s, 'i', sizeof(s));
    memset(key, 'k', sizeof(key));
    RC4_encrypt(s, 259, key, 256);
    for (int i = 0; i < 259; i++) printf("%02X ", s[i]);printf("\n");
    RC4_decrypt(s, 259, key, 256);
    for (int i = 0; i < 259; i++) printf("%02X ", s[i]);printf("\n");
    return 0;
}