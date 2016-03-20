/*
 * mec_csum_boot.c: check/fix boot-time checksums
 * Should be run on the decrypted image
 */
#include <stdio.h>
#include <string.h>
#include "mmapfile.h"

int checksum(unsigned char *base, unsigned int location, 
             unsigned int start, unsigned int end, int fix)
{
    unsigned char *p;
    unsigned int stored = *(unsigned int *)(base + location);
    unsigned int csum = 0;

    for (p = base+start; p < base+end; p += 4)
        csum += *(unsigned int *)p;
    csum = -csum;

    if (fix)
        *(unsigned int *)(base+location) = csum;

    printf("%08x %08x %s\n", stored, csum, (stored!=csum)?(fix?"FIXED":"FAIL"):"OK");
    return (stored!=csum)&&!fix;
}
 
int main(int argc, char *argv[])
{
    size_t length;
    unsigned char *base;
    int fix, r;

    if ((argc >= 3) && (strcmp(argv[1], "-f") == 0))
        fix = 1;
    else if ((argc >= 3) && (strcmp(argv[1], "-c") == 0))
        fix = 0;
    else
    {
        fprintf(stderr, "usage: %s {-f|-c} file\n", argv[0]);
        return 1;
    }

    base = mmapfile(argv[2], fix?(PROT_READ|PROT_WRITE):PROT_READ, MAP_SHARED, &length);
    if (base == MAP_FAILED)
    {
        perror("mmapfile");
        return 1;
    }

    if (*(unsigned int *)base != 0x0f802020)
    {
        fprintf(stderr, "you should run this on the unencrypted image\n");
        return 1;
    }

    r  = checksum(base, 0x2048, 0, 0x2048, fix);
    r |= checksum(base, 0x204c, 0x2080, 0x10000, fix);
    r |= checksum(base, 0x2050, 0x10000, 0x20000, fix);
    r |= checksum(base, 0x2054, 0x20000, 0x2e000, fix);
    munmap(base, length);
    return r;
}

