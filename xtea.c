#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef unsigned char uchar;
typedef unsigned long ulong;

ulong textlen_to_cryptlen(ulong textlength) {
    return textlength % 8 == 0 ? textlength : (textlength / 8 + 1) * 8;
}


void XTEA_encrypt_a_block(ulong *v, ulong *k) {
    ulong sum = 0;
    ulong delta = 0x9e3779b9;
    for (int i = 0; i < 32; i++) {
        v[0] += (((v[1] << 4)^(v[1] >> 5)) + v[1]) ^ (sum + k[sum&3]);
        sum += delta;
        v[1] += (((v[0] << 4)^(v[0] >> 5)) + v[0]) ^ (sum + k[(sum>>11)&3]);
    }
}

void XTEA_decrypt_a_block(ulong *v, ulong *k) {
    ulong sum = 0xc6ef3720;
    ulong delta = 0x9e3779b9;
    for (int i = 0; i < 32; i++) {
        v[1] -= (((v[0] << 4)^(v[0] >> 5)) + v[0]) ^ (sum + k[(sum>>11)&3]);
        sum -= delta;
        v[0] -= (((v[1] << 4)^(v[1] >> 5)) + v[1]) ^ (sum + k[sum&3]);
    }
}


uchar * XTEA_encrypt(uchar *plaintext, ulong text_len, uchar *key) {
    /*key length : 128bit / 16bytes*/
    ulong buf_len = 0;
    if (text_len % 8 != 0) buf_len = (text_len / 8 + 1) * 8;
    else buf_len = text_len;
    uchar * buf = (ulong *)malloc(buf_len);
    memset(buf, 0, buf_len);
    for (int i = 0; i < text_len; i++) buf[i] = plaintext[i];
    for (int i = 0; i < buf_len ; i += 8) {
        XTEA_encrypt_a_block((ulong *)&buf[i], (ulong *)key);
    }
    return buf;
}

void XTEA_decrypt(uchar *encrypt, ulong text_len, uchar *key) {
    /*key length : 128bit / 16bytes*/

    if (text_len % 8 != 0) {
        printf("\r\t[ERROR]Invalid length of text\n");
        return ;
    }
    for (int i = 0; i < text_len ; i += 8) {
        XTEA_decrypt_a_block((ulong *)&encrypt[i], (ulong *)key);
    }
}


int main() {
    uchar s[129] = {0};
    uchar key[16] = "this_is_a_key!!!";
    strcpy(s, "I am a plaintext");
    s[127] = 'T';
    uchar *s_ = XTEA_encrypt(s, 129,key);
    for (int i = 0; i < textlen_to_cryptlen(129); i++) printf("%02X ", s_[i]);printf("\n");
    XTEA_decrypt(s_, textlen_to_cryptlen(129),key);
    for (int i = 0; i < 129; i++) printf("%d:%c|", i,s_[i]);printf("\n");
    return 0;
}