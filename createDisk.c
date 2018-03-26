#include "api.h"
/* creates a disk by opening a file by open system call
   and return a file descriptor
*/
int createDisk(char *file_name)
{
  return open(file_name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
}
