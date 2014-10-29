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


#define X(A)  const char* A = NULL;
  LIST_ARGUMENTS
#undef X


int main(int argc, char** argv)
{
  int i;
  
  for (i = 1; i < argc; i++)
    {
#define X(A)  if (strstr(argv[i], "--" #A "="))  A = strchr(argv[i], '=') + 1;
      LIST_ARGUMENTS
#undef X
    }
  
  if (NULL == username)  return 1;
  if (NULL == ttyname)   return 1;
  if (NULL == pid)       return 1;
  
  return -do_log();
}

