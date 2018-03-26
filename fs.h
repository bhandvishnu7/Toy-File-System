#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<math.h>

#include "debug.h"

#define FS_SIZE (1024*1024*1024)
#define BS 4096


/* superblock structure */
typedef struct superblock{
  unsigned int total_no_inodes;
  unsigned int no_free_inodes;
  unsigned int no_free_blocks;
  unsigned int next_free_inode_index; //index in inode table list
  unsigned int inode_bmap_offset; // start position of inode bmap
  unsigned int data_block_bmap_offset; // start position of data_block  bmap
  unsigned int inode_table_offset; // start position of inode_table
  unsigned int data_block_offset; // start position of data_block
  unsigned int next_free_data_block_offset;
}sb;

/* block map for data block  */
typedef struct db_bmap_struct {
  char db_bmap_index[FS_SIZE/BS];
}db_bmap;

/* block  map for inode  */
typedef struct inode_bmap_struct {
  char inode_bmap_index[FS_SIZE/BS];
}inode_bmap;

/* data_block struct */
typedef struct data_block_struct{
  char data[BS]; //block size buffer string
}data_block;

/* single indirect block struct */
typedef struct si_block_struct{
  unsigned int db_index[BS/(sizeof(int *))]; // 4096/4 = 1024 adress
}si_block;
  
/* inode strcuture */
typedef struct inode_struct{
  char file_type; // file_type: d - directory | f - file
  unsigned int size; // size of file or directory
  unsigned int direct_block_offset; //address of direct block, will typecasted on f or d;
  unsigned int single_indirect_block_offset; // address of single indirect block
}inode;

/* inode_table strcuture */
typedef struct inode_table_struct{
  inode inode_index[FS_SIZE/BS]; //FS_SIZE/Block_size;
}inode_table;

/* dir_record_struct */
typedef struct dir_record_struct{
  unsigned int inode_index;
  char file_name[28];
}dir_record;

/* dir structure */
typedef struct dir_struct {
  dir_record child[BS/sizeof(dir_record)]; //first index = current, second index = parent
}dir;

/* global objects for all data structures 
   No need to pass to any functions as 
   defined globally */
int fd;
sb *sb_object; 
db_bmap *db_bmap_object; 
inode_bmap *inode_bmap_object; 
inode_table *inode_table_object; 
