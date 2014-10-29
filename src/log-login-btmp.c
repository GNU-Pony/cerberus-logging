/**
 * cerberus-logging – Log-in logging extension for cerberus
 * 
 * Copyright © 2014  Mattias Andrée (maandree@member.fsf.org)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "common.h"
#include <utmp.h>


int do_log(void)
{
  struct utmp ut;
  
  if (!streq(action, "failed"))
    return 0;
  
  memset(&ut, 0, sizeof(struct utmp));
  
  ut.ut_type = LOGIN_PROCESS;
  ut.ut_pid = (pid_t)atoll(pid);
  xstrcpy(ut.ut_user, username);
  xstrcpy(ut.ut_host, hostname);
  xstrcpy(ut.ut_line, ttyname);
  xstrcpy(ut.ut_id, first_digit(ttyname));
  xmemcpy(ut.ut_addr_v6, get_hostaddress(hostname));
  
#ifdef _HAVE_UT_TV
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ut.ut_tv.tv_sec = (typeof(ut.ut_tv.tv_sec))(tv.tv_sec);
    ut.ut_tv.tv_usec = (typeof(ut.ut_tv.tv_usec))(tv.tv_usec);
  }
#else
  {
    time_t t;
    time(&t);
    ut.ut_time = t;
  }
#endif
  
  updwtmp(LOGDIR "/btmp", &ut);
  return 0;
}

