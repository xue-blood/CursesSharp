/*
 * CursesSharp
 * 
 * Copyright 2009 Robert Konklewski
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * www.gnu.org/licenses/>.
 * 
 */

#include "wrapper.h"
#include "unicode.h"


WRAP_API int
wrap_winsnstr(WINDOW *win, uchar2 *str, int n)
{
#if defined(CURSES_WIDE) && SIZEOF_WCHAR_T == 2
	return wins_nwstr(win, str, n);
#elif defined(CURSES_WIDE)
	wchar_t stackbuf[BUFFER_SIZE];
	xbuffer xinput, xoutput;
	xreader reader;
	xwriter writer;
	int ret;

	xbuf_init_uc(&xinput, str, n, XBUF_FILL);
	xbuf_init_wc(&xoutput, stackbuf, BUFFER_SIZE, XBUF_EXPANDABLE);

	xrdr_init(&reader, &xinput);
	xwrtr_init(&writer, &xoutput);
	if (unicode_to_wchar(&reader, &writer) < 0) {
		xbuf_free(&xoutput);
		return -1;
	}
	ret = wins_nwstr(win, xbuf_data_wc(&xoutput), xbuf_len_wc(&xoutput));
	xbuf_free(&xoutput);
	return ret;
#else
#error Not implemented yet
#endif
}

#include <syslog.h>
#include <errno.h>
WRAP_API int
wrap_mvwinsnstr(WINDOW *win, int y, int x, uchar2 *str, int n)
{
#if defined(CURSES_WIDE) && SIZEOF_WCHAR_T == 2
	return mvwins_nwstr(win, y, x, str, n);
#elif defined(CURSES_WIDE)
	wchar_t stackbuf[BUFFER_SIZE];
	xbuffer xinput, xoutput;
	xreader reader;
	xwriter writer;
	int ret;

	xbuf_init_uc(&xinput, str, n, XBUF_FILL);
	xbuf_init_wc(&xoutput, stackbuf, BUFFER_SIZE, XBUF_EXPANDABLE);

	xrdr_init(&reader, &xinput);
	xwrtr_init(&writer, &xoutput);
	if (unicode_to_wchar(&reader, &writer) < 0) {
		openlog("curseswrapper", LOG_NDELAY|LOG_PERROR, LOG_USER);
		syslog(LOG_ERR, "unicode_to_wchar");
		closelog();
		xbuf_free(&xoutput);
		return -1;
	}
	ret = mvwins_nwstr(win, y, x, xbuf_data_wc(&xoutput), xbuf_len_wc(&xoutput));
	if (ret < 0) {
		int eno = errno;
		openlog("curseswrapper", LOG_NDELAY|LOG_PERROR, LOG_USER);
		syslog(LOG_ERR, "mvwins_nwstr: %d", eno);
		closelog();
	}
	xbuf_free(&xoutput);
	return ret;
#else
#error Not implemented yet
#endif
}

