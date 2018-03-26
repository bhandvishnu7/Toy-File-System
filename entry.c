#include "api.h"
char *strrev(char *str)
{
	int i;
	char temp;
	for (i = 0; i < strlen(str)/2; ++i) {
		temp = str[i];
		str[i] = str[strlen(str)-1-i];
		str[strlen(str)-1-i] = temp;
	}
	return str;
}

int make_entry(char *file_path,char *inp_file_name)
{
  int file_inode_index;
  int i, j;
  int dir_inode_index;
  inode *dir_inode;
  dir *dir_object = (dir *)malloc(sizeof(dir));
  char dir_path[strlen(file_path)];
  char tmp_path[strlen(file_path)];
  char file_name[strlen(file_path)];

  strcpy(tmp_path,strrev(file_path));
  //printf("%s\n",tmp_path);
  for(i=0;tmp_path[i]!='/';i++)
	  file_name[i]=tmp_path[i];
  file_name[i]=0;
  strcpy(file_name,strrev(file_name));
  //printf("%s\n",file_name);
  for(;i<strlen(tmp_path);i++){
	  dir_path[j]=tmp_path[i];
	  j++;
  }
  dir_path[j]=0;
  //printf("%s\n",dir_path);

  file_inode_index = filewrite(inp_file_name);
  if(file_inode_index == -1)
    return -1;
  dir_inode_index = get_inode(dir_path, 0,strlen(dir_path));
  
#ifdef DEBUG_BUILD
  printf("in make_entry: ***get path dir inode***  dir_i_index -> %d\n",dir_inode_index);
#endif
  dir_inode = &inode_table_object->inode_index[dir_inode_index];
  if(dir_inode->file_type != 1){
	  printf("E:Not a valid path. Parent is not directoty\n");
	  return -1;
  }
 
  my_read(dir_object, BS, dir_inode->direct_block_offset);
  dir_object->child[dir_inode->size].inode_index = file_inode_index;
#ifdef DEBUG_BUILD
  printf("in make_entry: ***child write*** dir_i_index[%d] -> child[%d] -> %d_:_%s\n",dir_inode_index,dir_inode->size,file_inode_index,file_name);
#endif
  strcpy(dir_object->child[dir_inode->size].file_name, file_name);
  my_write(dir_object, sizeof(*dir_object), dir_inode->direct_block_offset);
  inode_table_object->inode_index[dir_inode_index].size +=  1;
  my_write(inode_table_object, sizeof(*inode_table_object), sb_object->inode_table_offset);

  return 0;
}
