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
#include <syslog.h>
#include <pwd.h>


int do_log(void)
{
  static char empty[] = "";
  struct passwd* pwd;
  const char* prefix;
  const char* affix;
  
  if (!streq(action, "login"))
    return 0;
  
  if (pwd = getpwnam(username), pwd == NULL)  return -1;
  
  openlog("login", LOG_ODELAY, LOG_AUTHPRIV);
  
  if (strstr(ttyname, "ttyS") == ttyname)
    syslog(LOG_INFO, "DIALUP AT %s BY %s", ttyname, username);
  
  prefix = pwd->pw_uid ? "" : "ROOT ";
  affix = hostname ? " FROM " : "";
  hostname = hostname ? hostname : empty;
  
  syslog(pwd->pw_uid ? LOG_INFO : LOG_NOTICE, "%sLOGIN ON %s BY %s%s%s",
	 prefix, ttyname, username, affix, hostname);
  
  closelog();
  return 0;
}

