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
#include <lastlog.h>
#include <pwd.h>


int do_log(void)
{
  struct passwd* pwd;
  struct lastlog ll;
  time_t t;
  int fd;
  
  if (!streq(action, "login"))
    return 0;
  
  if (pwd = getpwnam(username), pwd == NULL)                         return -1;
  if (fd = open(_PATH_LASTLOG, O_RDWR, 0), fd < 0)                   return -1;
  if (lseek(fd, (off_t)(pwd->pw_uid * sizeof(ll)), SEEK_SET) == -1)  goto fail;
  
  memset(&ll, 0, sizeof(ll));
  
  time(&t), ll.ll_time = t;
  xstrcpy(ll.ll_line, ttyname);
  xstrcpy(ll.ll_host, hostname);
  
  if (write_all(fd, (char*)&ll, sizeof(ll)))
    goto fail;
  
  return close(fd), 0;
 fail:
  return close(fd), -1;
}

