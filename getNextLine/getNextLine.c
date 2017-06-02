#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "utils.h"

static bool	processReadString(char *carrier,
				  char *readString,
				  char *stringToReturn)
{
  unsigned int	i;
  unsigned int	j;

  for (i = ZERO, j = strlen(stringToReturn);
       readString[i] != NULL_BYTE;
       i++, j++)
    {
      if (readString[i] == NEW_LINE)
	{
	  (void)memcpy(carrier, readString + i + 1, SIZE_READ - (i + 1));
	  (void)memset(carrier + (SIZE_READ - (i + 1)), ZERO, i + 1);
	  return (true);
	}
      stringToReturn[j] = readString[i];
    }
  return (false);
}

static char	*myRealloc(char *oldStringToReturn)
{
  char		*newStringToReturn;
  unsigned int	len;

  len = strlen(oldStringToReturn) + SIZE_READ;
  if (!(newStringToReturn = calloc(len, sizeof(char))))
    return (NULL);
  (void)memcpy(newStringToReturn, oldStringToReturn, strlen(oldStringToReturn));
  free(oldStringToReturn);
  return (newStringToReturn);
}

char		*getNextLine(int const fd)
{
  static char	carrier[SIZE_READ] = {NULL_BYTE};
  char		readString[SIZE_READ] = {NULL_BYTE};
  char		*stringToReturn;

  if (!(stringToReturn = calloc(SIZE_READ, sizeof(char))))
    return (NULL);
  if (processReadString(carrier, carrier, stringToReturn))
    return (stringToReturn);
  while (read(fd, readString, SIZE_READ - 1) > ZERO)
    {
      if (!(stringToReturn = myRealloc(stringToReturn)))
	return (NULL);
      if (processReadString(carrier, readString, stringToReturn))
	return (stringToReturn);
      (void)memset(readString, NULL_BYTE, SIZE_READ);
    }
  free(stringToReturn);
  return (NULL);
}
