/**
 * cerberus-logging – Log-in logging extension for cerberus
 * 
 * Copyright © 2014, 2015  Mattias Andrée (maandree@member.fsf.org)
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
  int status;
  
  if ((!streq(action, "failed")) && (!streq(action, "login")))
    return 0;
  
  status = streq(action, "login");
  
  if (pwd = getpwnam(username), pwd == NULL)  return -1;
  
  openlog("log-login-syslog", LOG_ODELAY, LOG_AUTHPRIV);

  if (status)
    {
      if (strstr(ttyname, "ttyS") == ttyname)
	syslog(LOG_INFO, "DIALUP AT %s BY %s", ttyname, username);
      
      prefix = pwd->pw_uid ? "" : "ROOT ";
      affix = hostname ? " FROM " : "";
      hostname = hostname ? hostname : empty;
      
      syslog(pwd->pw_uid ? LOG_INFO : LOG_NOTICE, "%sLOGIN ON %s BY %s%s%s",
	     prefix, ttyname, username, affix, hostname);
    }
  else
    {
      affix = hostname ? " FROM " : "";
      
      syslog(LOG_NOTICE, "FAILED LOGIN SESSION%s%s FOR %s ON %s",
	     affix, hostname, username, ttyname);
    }
  
  closelog();
  return 0;
}

