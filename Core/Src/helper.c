
#include "helper.h"

// returns true if `X` and `Y` are the same
bool compare(const char *X, const char *Y)
{
  while (*X && *Y)
  {
    if (*X != *Y) 
    {
      return false;
    }
 
    X++;
    Y++;
  }
 
  return true;
}
 
// Function to implement `strstr()` function
char* any_strstr(const char* X, const char* Y, uint32_t find_siz)
{
  while (find_siz)
  {
    if ((*X == *Y) && compare(X, Y)) 
    {
      return (char*) X;
    }
    
    X++;
    find_siz--;
  }

  return NULL;
}

