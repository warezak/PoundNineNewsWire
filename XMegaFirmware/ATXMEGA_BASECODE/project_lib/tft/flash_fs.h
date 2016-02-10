/*
 * flash_fs.h
 *
 * Created: 8/9/2015 11:46:55 PM
 *  Author: ssenthil
 */ 


#ifndef FLASH_FS_H_
#define FLASH_FS_H_

#define INODE_SUMMARY 0
#define INODE_TABLE 1
#define INODE_SIZE 9 //10 sector for inode/ file list storage  //  this should ideally be 8 with 4096 files 
#define STORAGE_MEMORY_START	10

int ffs_clean_inode(void);
int ffs_write_icon_to_flash(gfx_color_t PROGMEM_PTR_T progmem_pixmap, int icon_length, int icon_width);
int ffs_clean_sectors(int start, int end);
void ffs_display_file_table( int start_file, int end_file, int selected_file );
int ffs_add_file_to_inode(int file_name, int start_sector, int icon_length, int icon_width);
int write_inode_summary(int num_files);
int read_inode_summary(void);
void ffs_read_icon_from_sector(uint16_t start_sector, int icon_length, int width, gfx_coord_t x, gfx_coord_t y);
int ffs_write_icon_to_sector(gfx_color_t PROGMEM_PTR_T progmem_pixmap, int start_sector, int icon_length, int icon_width);
int ffs_display_icon_from_flash(int file_name, gfx_coord_t x, gfx_coord_t y);
int ffs_delete_file_from_inode(int file_name);
int ffs_dumb_delete_file_from_inode(int file_name1);
#endif /* FLASH_FS_H_ */
