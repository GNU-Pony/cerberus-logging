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
  struct utmp *entry;
  pid_t pid_;
  const char* ttyno;
  
  if (!streq(action, "login"))
    return 0;

  pid_ = (pid_t)atoll(pid);
  ttyno = first_digit(ttyname);
  memset(&ut, 0, sizeof(struct utmp));
  
  utmpname(_PATH_UTMP);
  setutent();
  
  while ((entry = getutent()))
    if (entry->ut_pid == pid_)
      if ((INIT_PROCESS <= entry->ut_type) && (entry->ut_type <= DEAD_PROCESS))
	break;
  
  if ((entry == NULL) && ttyname)
    {
      setutent();
      ut.ut_type = LOGIN_PROCESS;
      xstrcpy(ut.ut_line, ttyname);
      entry = getutline(&ut);
    }
  if ((entry == NULL) && ttyno)
    {
      setutent();
      ut.ut_type = DEAD_PROCESS;
      xstrcpy(ut.ut_id, ttyno);
      entry = getutline(&ut);
    }
  
  if (entry)  memcpy(&ut, entry, sizeof(struct utmp));
  else        memset(&ut, 0,     sizeof(struct utmp));
  
  ut.ut_type = USER_PROCESS;
  ut.ut_pid = pid_;
  xstrcpy(ut.ut_user, username);
  xstrcpy(ut.ut_host, hostname);
  xstrcpy(ut.ut_line, ttyname);
  xstrcpy(ut.ut_id, first_digit(ttyname));
  xmemcpy(ut.ut_addr_v6, get_hostaddress(hostname));
  
#ifdef _HAVE_UT_TV
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ut.ut_tv.tv_sec = tv.tv_sec;
    ut.ut_tv.tv_usec = tv.tv_usec;
  }
#else
  {
    time_t t;
    time(&t);
    ut.ut_time = t;
  }
#endif

  pututline(&ut);
  endutent();
  updwtmp(_PATH_WTMP, &ut);
  return 0;
}

