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
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>



#ifdef __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif



#ifndef VARDIR
#define VARDIR "/var"
#endif
#ifndef LOGDIR
#define LOGDIR VARDIR "/log"
#endif


/* ttyname is defined in <unistd.h> */
#define ttyname ttyname_


#define LIST_ARGUMENTS  X(action) X(username) X(ttyname) X(pid) X(hostname)
#define streq(a, b)     (((a == NULL) == (b == NULL)) || (a && b && !strcmp(a, b)))
#define xstrcpy(d, s)   (s ? snprintf(d, sizeof(d) / sizeof(char), "%s", s) : 0)
#define xmemcpy(d, s)   (s ? memcpy(d, s, sizeof(d)) : NULL)



#define X(A)  extern const char* A;
  LIST_ARGUMENTS
#undef X


static inline const char* first_digit(const char* str)
{
  for (; str && *str; str++)
    if (('0' <= *str) && (*str <= '9'))
      return str;
  return NULL;
}


static const char* get_hostaddress(const char* host)
{
  static char rc[32 * 4 / 8];
  struct addrinfo hints;
  struct addrinfo *info = NULL;
  
  if (host == NULL)
    return NULL;
  
  memset(rc, 0, sizeof(rc));
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_ADDRCONFIG;
  
  if (getaddrinfo(host, NULL, &hints, &info))  return NULL;
  if (info == NULL)                                return NULL;
  
  if (info->ai_family == AF_INET)
    {
      struct sockaddr_in* addr = (struct sockaddr_in*)(info->ai_addr);
      memcpy(rc, &(addr->sin_addr), sizeof(addr->sin_addr));
    }
  else if (info->ai_family == AF_INET6)
    {
      struct sockaddr_in6* addr = (struct sockaddr_in6*)(info->ai_addr);
      memcpy(rc, &(addr->sin6_addr), sizeof(addr->sin6_addr));
    }
  
  freeaddrinfo(info);
  return rc;
}


static inline int write_all(int fd, char* data, size_t n)
{
  ssize_t wrote;
  
  while (n > 0)
    if (wrote = write(fd, data, n), wrote >= 0)
      data += wrote, n -= (size_t)wrote;
    else if (errno != EINTR)
      return -1;
  
  return 0;
}


int do_log(void);

