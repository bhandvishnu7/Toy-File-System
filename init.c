#include "api.h"

/* function to write data block bitmap on fs */
int my_write(void *buff, int n, int offset)
{
  lseek(fd,offset,SEEK_SET); //setting position for db_bmap on fs
  write(fd, buff, n);
  return 0;
}

// function to format file system
int initiate_file_system()
{

  init_superblock(); //initialize super block
  init_db_bmap(); // initiate data block bit map (all 0's (free))
  init_inode_bmap(); // inititate inode block map (all 0's (free))
  init_inode_table(); // inititiate inode table on disk
  init_root_dir(); //initialize root directory

}


/* function to inititate filesystem by writting superblock
   to filesystem */

int init_superblock()
{
  sb_object->total_no_inodes = FS_SIZE / BS;
  sb_object->no_free_blocks = FS_SIZE / BS;
  sb_object->no_free_inodes = (FS_SIZE / BS) ; // root dir will have 0th
  sb_object->next_free_inode_index = 1;// Oth to root dir

  sb_object->data_block_bmap_offset = sizeof(sb); //1 data block bitmap starts
  sb_object->inode_bmap_offset = sizeof(db_bmap) + sizeof(sb); //2 inode bitmap starts
  sb_object->inode_table_offset = sb_object->inode_bmap_offset + sizeof(inode_bmap); //3 inode table starts
  sb_object->data_block_offset = sb_object->inode_table_offset + sizeof(inode_table); //4 data block starts
  sb_object->next_free_data_block_offset = sb_object->data_block_offset;

  my_write(sb_object,sizeof(sb),0);
  return 0;
}


int init_db_bmap()
{
  int i;
  for (i = 0; i < FS_SIZE/BS; i++) //all free datablock on initialization
    db_bmap_object->db_bmap_index[i] = 0;
  my_write(db_bmap_object,sizeof(db_bmap),sb_object->data_block_bmap_offset);
  return 0;
}

int init_inode_bmap()
{
  int i;
  for (i = 0; i < FS_SIZE/BS; i++) //all free inode on initialization
    inode_bmap_object->inode_bmap_index[i] = 0;
  my_write(inode_bmap_object,sizeof(*inode_bmap_object),sb_object->inode_bmap_offset);
  return 0;

}

int init_inode_table()
{
	my_write(inode_table_object,sizeof(*inode_table_object),sb_object->inode_table_offset);
	return 0;
}

int init_root_dir()
{
  db_bmap_object->db_bmap_index[0] = 1;
  //sb_object->next_free_inode_index = 1;
  sb_object->no_free_inodes -= 1;
  my_write(db_bmap_object,sizeof(*db_bmap_object),sb_object->data_block_bmap_offset);

  inode_bmap_object->inode_bmap_index[0] = 1;
  my_write(inode_bmap_object,sizeof(*inode_bmap_object),sb_object->inode_bmap_offset);

  inode_table_object->inode_index[0].file_type = 1;
  inode_table_object->inode_index[0].size = 2;
  inode_table_object->inode_index[0].direct_block_offset = sb_object-> next_free_data_block_offset;
  my_write(inode_table_object, sizeof(*inode_table_object), sb_object->inode_table_offset);

  dir *dir_object = (dir *)malloc(sizeof(dir));
  dir_object->child[0].inode_index = 0;
  dir_object->child[1].inode_index = 0;
  strcpy(dir_object->child[0].file_name, ".");
  strcpy(dir_object->child[1].file_name, "..");
  my_write(dir_object, sizeof(*dir_object), sb_object->next_free_data_block_offset);
  set_next_free_block();
  return 0;
}
