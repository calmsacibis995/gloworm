
#include <unistd.h>
#include <kernel/syscall.h>


size_t read(int fd, char *buf, size_t nbytes)
{
	return SYSCALL3(SYS_READ, fd, (int) buf, (int) nbytes);
}
