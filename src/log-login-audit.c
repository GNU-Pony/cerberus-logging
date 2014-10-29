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
#include <libaudit.h>
#include <pwd.h>


int do_log(void)
{
  struct passwd* pwd;
  int fd, status, r;
  
  if ((!streq(action, "failed")) && (!streq(action, "login")))
    return 0;

  status = streq(action, "login");
  
  if (pwd = getpwnam(username), pwd == NULL)  return -1;
  if (fd = audit_open(), fd == -1)            return -1;
  
  r = audit_log_acct_message(fd, AUDIT_USER_LOGIN, NULL, "login", username,
			     pwd->pw_uid, hostname, NULL, ttyname, status);
  
  close(fd);
  return r;
}

