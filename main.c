#include "api.h"

int main(int argc, char *argv[])
{
  char buff1[10000];
  char buff2[10000];
  if(argc != 2){
    printf("E: please provide fs_name with ./a.out or too many arguments \n");
    return 0;
  }

  fd = createDisk(argv[1]); //create a disk image with file name
  sb_object = (sb *)malloc(sizeof(sb));
  db_bmap_object = (db_bmap *)malloc(sizeof(db_bmap));
  inode_bmap_object = (inode_bmap *)malloc(sizeof(inode_bmap));
  inode_table_object = (inode_table *)malloc(sizeof(inode_table));

  sb_object = get_super_block();
  db_bmap_object = get_db_bmap();
  inode_bmap_object = get_inode_bmap();
  inode_table_object = get_inode_table();
  initiate_file_system(fd,argv[1]);

  while(1){
    printf("\n$ ");
    scanf("%s",buff1);
    if(strcmp("write",buff1)==0){
      printf("enter local file to write:");
      scanf("%s", buff1);
      printf("enter name of file to write:");
      scanf("%s", buff2);
      printf("%s %s\n",buff1,buff2);
      printf("\n%d\n",make_entry(buff2,buff1));
    }
    else if(strcmp("read",buff1)==0){
      printf("enter file path to read:");
      scanf("%s", buff1);
      read_file(buff1);
    }
    else if(strcmp("help",buff1)==0){
      printf("Valid Commands: read | write | exit\n");
    }
    else if(strcmp("exit",buff1)==0){
      break;
    }
    else {
      printf("Invalid command \n");
      printf("Valid Commands: read | write | exit\n");
    }
  
  }
    
  return 0;
}

