#include "api.h"

int my_read(void *buff, int n,int offset)
{
	lseek(fd,offset,SEEK_SET); //setting position for db_bmap on fs
	read(fd, buff, n);
	return 0;
}
/* function to get super block data from fs to RAM */
sb *get_super_block()
{
	my_read(sb_object,sizeof(sb),0);
	return sb_object;
}

/* function to get db_bmap data from fs to RAM */
db_bmap *get_db_bmap()
{
	my_read(db_bmap_object,sizeof(db_bmap),sb_object->data_block_bmap_offset);
	return db_bmap_object;
}

/* function to get inode_bmap data from fs to RAM */
inode_bmap *get_inode_bmap()
{
	my_read(inode_bmap_object,sizeof(*inode_bmap_object),sb_object->inode_bmap_offset);
	return inode_bmap_object;
}

/* function to get inode_table data from fs to RAM */
inode_table *get_inode_table()
{
	my_read(inode_table_object,sizeof(*inode_table_object),sb_object->inode_table_offset);
	return inode_table_object;
}

int get_inode(char *file, int i_index, int size)
{

	char temp[size];
	int i, j = 0;
	int old_size;
	if(strlen(file)==1)
		return 0;
	if(file[0] == '/')
		i = 1;
	else
		i = 0;

	for (; i < size; i++){
		if(file[i] == '/')
			break;
		temp[j] = file[i];
		j++;
	}
	temp[j] = '\0';

	//inode_table_object = get_inode_table();
	if(inode_table_object->inode_index[i_index].file_type != 1){
		printf("in get_inode: i_index -> %d is not dir\n",i_index);
		return -1;
	}
	old_size = i;

	dir *dir_object = (dir *)malloc(sizeof(dir));
	my_read(dir_object,sizeof(*dir_object), inode_table_object->inode_index[i_index].direct_block_offset);
	for (i = 0; i < inode_table_object->inode_index[i_index].size; i++) {

#ifdef DEBUG_BUILD
		printf("in get_inode: inode[%d]-> child[%d] = %d_:_%s_:_%s\n",i_index,i,dir_object->child[i].inode_index,dir_object->child[i].file_name,temp);
#endif
		if(strcmp(dir_object->child[i].file_name, temp) == 0){
			if(size - old_size == 0){
#ifdef DEBUG_BUILD
				printf("in get_inode: ***return*** inode_index -> %d \n\n",dir_object->child[i].inode_index);
#endif
				return dir_object->child[i].inode_index;
			}else
				get_inode(&file[old_size],dir_object->child[i].inode_index, size - old_size);
		}
	}
	return -1;
}

void update()
{
}

