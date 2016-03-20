###Tools for modifying Lenovo embedded controller firmware

Run make to build (requires OpenSSL).

To extract an EC firmware image from the BIOS update file:

```sh
dd if=\$01DA000.FL2 of=mec.bin bs=4096 skip=1280 count=48
```

To decrypt an EC firmware image:

```sh
./mec_encrypt -d mec.bin >mec-decrypted.bin
```

To reencrypt the image after modification, fixing checksums:

```sh
./mec_repack mec-decrypted.bin mec-new.bin
```

To reembed into the BIOS update file:

```sh
dd if=mec-new.bin of=\$01DA000.FL2 conv=notrunc bs=4096 seek=1280
```

Before carrying out your own modifications, follow the above steps on
the unmodified image and check that, in the third step, the checksums
all report as OK.  If any checksum shows FIXED or FAIL, do not proceed
until you understand why.  In particular, the checksum locations in
mec_csum_boot.c may need to be modified for different laptop models.

Modifying embedded controller firmware carries the serious risk of
bricking your laptop.  These tools are provided in the hope they are
useful, without warranty; use them at your own risk.

