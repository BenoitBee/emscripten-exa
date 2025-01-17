#define _BSD_SOURCE
#include <sys/uio.h>
#include <unistd.h>
#include "syscall.h"

ssize_t preadv(int fd, const struct iovec *iov, int count, off_t ofs)
{
#if __EMSCRIPTEN__EXA
	size_t num;
	if (__wasi_syscall_ret(__wasi_fd_pread(fd, (struct __wasi_iovec_t*)iov, count, ofs, &num))) {
		return -1;
	}
	return num;
#else
	return syscall_cp(SYS_preadv, fd, iov, count,
			  (long)(ofs), 0/*(long)(ofs>>32)*/);
#endif
}

weak_alias(preadv, preadv64);
