#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pty.h>
#include <stdio.h>
#include <pthread.h>

#include <emscripten.h>

/* Nonstandard, but vastly superior to the standard functions */

int openpty(int *pm, int *ps, char *name, const struct termios *tio, const struct winsize *ws)
{
	int m, s, n=0, cs;
	char buf[20];

	m = open("/dev/ptmx", O_RDWR|O_NOCTTY);

	//emscripten_log(EM_LOG_CONSOLE, "!! openpty: m=%d", m);
	
	if (m < 0) return -1;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	//emscripten_log(EM_LOG_CONSOLE, "TIOCSPTLCK=%d TIOCGPTN=%d", TIOCSPTLCK, TIOCGPTN);

	if (ioctl(m, TIOCSPTLCK, &n) || ioctl (m, TIOCGPTN, &n))
		goto fail;

	//emscripten_log(EM_LOG_CONSOLE, "!! openpty: n=%d", n);

	if (!name) name = buf;
	snprintf(name, sizeof buf, "/dev/pts/%d", n);
	if ((s = open(name, O_RDWR|O_NOCTTY)) < 0)
		goto fail;

	if (tio) tcsetattr(s, TCSANOW, tio);
	if (ws) ioctl(s, TIOCSWINSZ, ws);

	*pm = m;
	*ps = s;

	//emscripten_log(EM_LOG_CONSOLE, "!! openpty: pm=%d ps=%d", m, s);

	pthread_setcancelstate(cs, 0);
	return 0;
fail:
	close(m);
	pthread_setcancelstate(cs, 0);
	return -1;
}
