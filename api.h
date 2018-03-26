#include "fs.h"

/* #define DEBUG_BUILD*/


int createDisk(char *file_name);
int initiate_file_system();
int my_write(void *buff, int n,int offset);
int init_inode_table();
int init_db_bmap();
int init_inode_bmap();
int init_superblock();
int initiate_file_system();

int init_root_dir();

/*------------------- get.c ---------------------*/
int my_read(void *buff, int n,int offset);
sb *get_super_block();
db_bmap *get_db_bmap();
inode_bmap *get_inode_bmap();
inode_table *get_inode_table();
inode_bmap *get_inode_bmap();
int get_inode(char *file, int i_index, int size);

/*------------------- entry.c ---------------------*/
int make_entry(char *file_path,char *inp_file_name);
char *strrev(char *str);

/*------------------- write.c ---------------------*/
int filewrite();
unsigned int get_next_offset_to_write(inode *file_inode);
int write_fs();
int set_next_free_block();
int set_next_free_inode_index();

/*------------------- read.c ---------------------*/
int read_file(char *filePath);
int read_file_from_indirect_block(int file_size, inode * file_inode);
int print_data_block(int count, unsigned int offseti,int t);





