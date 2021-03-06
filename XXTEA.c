
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))
 

typedef unsigned long ulong;
typedef unsigned char uchar;


void XXTEA(ulong *v, int n, ulong const key[4])
{
    ulong y, z, sum;
    unsigned p, rounds, e;
    rounds = 6 + 52/n;
    sum = 0;
    z = v[n-1];
    do
    {
        sum += DELTA;
        e = (sum >> 2) & 3;
        for (p=0; p<n-1; p++)
        {
            y = v[p+1];
            v[p] += MX;
            z = v[p];
        }
        y = v[0];
        z = v[n-1] += MX;
    }
    while (--rounds);

}

void XXTEA_encrypt_a_block(ulong n, ulong *v, ulong *key) {
    /*n represent length of v */
    ulong rounds = 6 + 52/n;
    ulong y, z, e;
    ulong sum = 0;
    z = v[n-1];
    while (rounds--) {
        sum += DELTA;
        e = sum >> 2;
        for (int i = 0; i < n-1; i++) {
            y = v[i+1];
            v[i] += (((z>>5)^(y<<2)) + ((z<<4)^(y>>3))) ^ ((z^key[(i^e)&3]) + (y^sum));
            z = v[i];
        }
        y = v[0];
        v[n-1] += (((z>>5)^(y<<2)) + ((z<<4)^(y>>3))) ^ ((z^key[((n-1)^e)&3]) + (y^sum));
        z = v[n-1];
    }
}

void XXTEA_decrypt_a_block(ulong n, ulong *v, ulong *key) {
    ulong rounds = 6 + 52/n;
    ulong y, z, e;
    ulong sum = rounds * DELTA;
    y = v[0];
    while(rounds--) {
        e = sum >> 2;
        for (int i = n-1; i>0; i--) {
            z = v[i-1];
            v[i] -= (((z>>5)^(y<<2)) + ((z<<4)^(y>>3))) ^ ((z^key[(i^e)&3]) + (y^sum));
            y = v[i];
        }
        z = v[n-1];
        v[0] -= (((z>>5)^(y<<2)) + ((z<<4)^(y>>3))) ^ ((z^key[(0^e)&3]) + (y^sum));
        y = v[0];
        sum -= DELTA;
    }    
}

uchar * XXTEA_encrypt(uchar *plaintext, ulong text_len, uchar *key) {
    ulong buf_len = text_len % 8 == 0? text_len : (text_len / 8 + 1) * 8;
    uchar *buf = (uchar *)malloc(buf_len);
    for (int i = 0; i < buf_len; i += 8) {
        XXTEA_encrypt_a_block(2, (ulong*)&buf[i], (ulong*)key);
    }
    return buf;
}

void XXTEA_decrypt(uchar *encrypt, ulong enc_len, uchar *key) {
    if (enc_len % 8) {
        printf("[ERROR]Invalid encrypt length\n");
        return;
    }
    for (int i = 0; i < enc_len; i += 8) {
        XXTEA_decrypt_a_block(2, (ulong*)&encrypt[i], (ulong*)key);
    }    
}

int main() {
    uchar s[] =   "I love study!!!!"; //22
    uchar key[] = "0123456789ABCDEF";
    XXTEA_encrypt_a_block(4, (ulong*)s, (ulong*)key);
    XXTEA_decrypt_a_block(4, (ulong*)s, (ulong*)key);
    for (int i = 0; i < 22; i++) printf("%d:%c|", i, s[i]);
    return 0;
}

