CFLAGS=-Wall -O2
TOOLS=mec_csum_boot mec_csum_flasher mec_csum_outer mec_encrypt
LD_LIBS_mec_encrypt = -lcrypto

all: $(TOOLS)

%: %.c mmapfile.h
	$(CC) $(CFLAGS) $^ -o $@ $(LD_LIBS_$@)

clean:
	rm -f $(TOOLS)

