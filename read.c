#include "api.h"
/* Function to read a data from a file. If file_size not exceeds 4096 bytes then read only direct block */
int read_file(char *filePath)
{
  int i;
  int file_inode_index = get_inode(filePath, 0, strlen(filePath));
  inode * file_inode = &(inode_table_object->inode_index[file_inode_index]);
  int file_size = file_inode->size;

  char buff[4096];
  si_block * si_block_object ;
  if(file_inode_index == -1){
    printf("Error: File not found");
  }
  si_block_object = (si_block *) buff;
  my_read(si_block_object, BS, file_inode->single_indirect_block_offset);
#ifdef DEBUG_BUILD
  printf("in read.c read_file: *** File_inode: %d  di_db_offset: %d  si_block_offset:%d  ***\n", file_inode_index, file_inode->direct_block_offset, file_inode->single_indirect_block_offset);

  printf("\n--------------------------------------\n");
  for(i = 0;i < (BS / sizeof(int *));i++){
    printf("in read.c read_file(): si_block(%d) -> db_offset[%d]:%d\n",file_inode->single_indirect_block_offset,i,si_block_object->db_index[i]);
  }
  printf("\n--------------------------------------\n");
#endif
  if(file_size < sizeof(data_block))
    print_data_block(file_size, file_inode->direct_block_offset, file_inode->file_type);
  else
    print_data_block(sizeof(data_block), file_inode->direct_block_offset, file_inode->file_type);


  if(file_size > BS){
    //file_size -= BS;
    read_file_from_indirect_block(file_size-BS, file_inode);
  }
  return 0;
}
/* Function to read from a file from single indirect block */
int read_file_from_indirect_block(int file_size, inode * file_inode)
{
  int i,print_len;
  char buff[BS];
  si_block * si_block_object ;
  si_block_object = (si_block *) buff;

  my_read(si_block_object, BS, file_inode->single_indirect_block_offset);
  int remain_block = ceil((float) (file_size * 1.0)/ BS);

  print_len = BS;

  while(remain_block > 0){
    if(remain_block > BS / sizeof(int *)){
      for(i = 0;i < (BS / sizeof(int *)) - 1;i++ ){
	//printf("\n------------------- si_block[%d] at (%d)----------------------------\n",i,si_block_object->db_index[i]);
	print_data_block(print_len, si_block_object->db_index[i], file_inode->file_type);
	file_size -= BS;
      }
    }else{
      for(i = 0;i < remain_block ;i++ ){
	if(file_size < BS)
	  print_len = file_size;
	//printf("\n------------------- si_block[%d] at (%d)----------------------------\n",i,si_block_object->db_index[i]);
	print_data_block(print_len, si_block_object->db_index[i], file_inode->file_type);
	file_size -= BS;
      }
    }
    my_read(si_block_object, BS, si_block_object->db_index[(BS / sizeof(int *)) - 1]);
    remain_block -= (BS / sizeof(int *)) - 1;
  }

  return 0;
}

int print_data_block(int count, unsigned int offset,int t){
  int i;
  data_block data_block_object;
  dir dir_object;
  my_read(&data_block_object, count, offset);
  if(t==0){
    for(i=0; i < count ; i++)
      putc(data_block_object.data[i],stdout);
  }
  else{
    printf("Error: Is not file\n");
  }
  return 0;
}
