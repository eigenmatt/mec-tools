/*
 * mec_csum_outer.c: check/fix outer checksum
 * Should be run on the encrypted image
 */
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "mmapfile.h"

int main(int argc, char *argv[])
{
    size_t length;
    unsigned char *base;
    unsigned char *p, *end;
    unsigned short csum = 0;
    unsigned short stored;
    int fix;

    if ((argc >= 3) && (strcmp(argv[1], "-f") == 0))
        fix = 1;
    else if ((argc >= 3) && (strcmp(argv[1], "-c") == 0))
        fix = 0;
    else
    {
        fprintf(stderr, "usage: %s {-f|-c} file\n", argv[0]);
        return 1;
    }

    base = (unsigned char *)mmapfile(argv[2], fix?(PROT_READ|PROT_WRITE):PROT_READ, MAP_SHARED, &length);
    if (base == MAP_FAILED)
    {
        perror("mmapfile");
        return 1;
    }

    if (*(unsigned int *)base == 0x0f802020)
    {
        fprintf(stderr, "you should run this on the encrypted image\n");
        return 1;
    }

    end = base + length;
    for (p = base; p < end-2; p+=2)
        csum += ntohs(*(unsigned short *)p);
    csum = -csum;
    csum = htons(csum);

    stored = *(unsigned short *)p;
    if (fix)
        *(unsigned short *)p = csum;

    printf("%04x %04x %s\n", stored, csum, (stored!=csum)?(fix?"FIXED":"FAIL"):"OK");
    return (stored!=csum)&&!fix;
}
