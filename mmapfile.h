/*
 * mmapfile.h: utility function for mmapping files
 */
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

static inline void *mmapfile(const char *pathname, int prot, int flags,
                             size_t *length)
{
	struct stat st;
	int openflags = (prot & PROT_WRITE) ? O_RDWR : O_RDONLY;
	int fd;
	void *p;

	fd = open(pathname, openflags);
	if (fd == -1)
		return MAP_FAILED;

	if (fstat(fd, &st) == -1)
	{
		close(fd);
		return MAP_FAILED;
	}

	*length = st.st_size;
	p = mmap(NULL, *length, prot, flags, fd, 0);

	close(fd);
	return p;
}

