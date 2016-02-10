/*
 * flash_fs.c
 *
 * Created: 8/9/2015 11:46:30 PM
 *  Author: ssenthil
 */ 
#include "asf.h"
#include "flash_fs.h"
#include "ui_objects.h"
#include <util/delay.h>
#include "screens.h"
// a simple file system implementation customized for icon storage and retrieval

// this file cleans/erases the file list stored in the flash memory. - quick delete 
// the first 10 sectors ( each 512bytes ) is called inode which contains the list of all files in the flash.

static	uint8_t flash_sector_buf[AT45DBX_SECTOR_SIZE]; //512 bytes
	
// this is not same as deleting a file. this blindly cleans any sector without involving the filesystem.	
int ffs_clean_sectors(int start, int end)
{
	int i, x;
	for (x=start; x<end; x++)
	{
		for(i=0;i<AT45DBX_SECTOR_SIZE;i++) {
			flash_sector_buf[i]	=0;
		}
		at45dbx_write_sector_open(x);
		at45dbx_write_sector_from_ram(flash_sector_buf);
		at45dbx_write_close();
	}
	return 0;	
}
	
int ffs_clean_inode(void)
{
	int i, x;
	for (x=INODE_SUMMARY; x<INODE_SIZE; x++)
	{
		for(i=0;i<AT45DBX_SECTOR_SIZE;i++) {
			flash_sector_buf[i]	=0;
		}
		at45dbx_write_sector_open(x);
		at45dbx_write_sector_from_ram(flash_sector_buf);
		at45dbx_write_close();
	}
	return 0;
} 

int ffs_display_icon(int file_number, int x, int y)
{
	
int file_sector, file_offset, read_fnum=0;
int icon_sector=0, icon_length=0, icon_width=0;
uint32_t icon_size=0;
int icon_rows_per_sector =0, icon_num_sectors=0, icon_rows_in_last_sector=0;
int i;

struct gfx_bitmap DATA_FLASH_ICON;
DATA_FLASH_ICON.type =GFX_BITMAP_RAM;

	// first check the presence of the file in the inode
	// if present get the file sector, 

	if (file_number > 640)
	{
		return(-1); // the requested file number is out of range
	}
	file_sector = file_number / 64; // number of files listed per sector is 64 , as each file entry requires 8 bytes.
	file_offset = file_number % 64;
	at45dbx_read_sector_open(file_sector);
	at45dbx_read_sector_to_ram(flash_sector_buf);
	at45dbx_read_close();
	read_fnum = (flash_sector_buf[file_offset]<<8)& flash_sector_buf[file_offset+1];
	if (read_fnum==0) {
		// the file location is empty or file number/name is not present
		return(1);
	}
	else if (read_fnum==file_number) // expected file is present
	{
		icon_sector = (flash_sector_buf[file_offset+2]<<8)& flash_sector_buf[file_offset+3];
		icon_length = (flash_sector_buf[file_offset+4]<<8)& flash_sector_buf[file_offset+5];
		icon_width  = (flash_sector_buf[file_offset+6]<<8)& flash_sector_buf[file_offset+7];
		DATA_FLASH_ICON.width =icon_length;
		DATA_FLASH_ICON.height =icon_width;
	}
	else
	return(2);
	
	icon_size = icon_length* icon_width;
	icon_rows_per_sector= AT45DBX_SECTOR_SIZE/icon_length;
	icon_num_sectors = icon_width / icon_rows_per_sector;
	icon_rows_in_last_sector = icon_width % icon_rows_per_sector;
	
	for (i=icon_sector;i<icon_sector+icon_num_sectors;i++)
	{
		// read the individuall sectors and display them one by one
		at45dbx_read_sector_open(file_sector);
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		DATA_FLASH_ICON.data.pixmap= flash_sector_buf;
		gfx_draw_bitmap(&DATA_FLASH_ICON, x,y+(i-icon_sector)*icon_rows_per_sector  );
		
	}
	if (icon_rows_in_last_sector)
	{
		DATA_FLASH_ICON.height =icon_rows_in_last_sector;
		// display the last sector chunk here if residue is present
		at45dbx_read_sector_open(file_sector);
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		
		DATA_FLASH_ICON.data.pixmap= flash_sector_buf;
		gfx_draw_bitmap(&DATA_FLASH_ICON, x,icon_num_sectors*icon_rows_per_sector  );		
	}
	
	
/*
	for (j=0;j<icon_size; j++)
	{
		for (i=0; i<AT45DBX_SECTOR_SIZE; i+=icon_length)
		{
		}
		i=i-icon_length;
		if (i)
		{
			for (;i<AT45DBX_SECTOR_SIZE;i++)
			{
			}	
		}*/
}
	
	
	
/*
	for(;i<AT45DBX_SECTOR_SIZE;i++) {
		if (ram_buf[i]!=0x55) {
			test_ko();
		}
	}
*/

//}

int ffs_is_file_present(int file_name)
{
	int read_file_name, read_start_sector, read_length , read_width;
	int rlocation;
	if (file_name<576)
	{
		at45dbx_read_sector_open(INODE_TABLE + (file_name/64));
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		rlocation = 8*((file_name-1)%64);
		read_file_name		= (flash_sector_buf[rlocation+0] << 8 ) | flash_sector_buf[rlocation+1];
		if (read_file_name == file_name)
		{
			return(0);
		}
		else
		{
			return(-1);
		}
	}
	else
	return(-1);
		
}

int ffs_display_icon_from_flash(int file_name, gfx_coord_t x, gfx_coord_t y)
{
	int read_file_name, read_start_sector, read_length , read_width;
	int rlocation;
	if (file_name<576)
	{
		at45dbx_read_sector_open(INODE_TABLE + (file_name/64));
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		rlocation = 8*((file_name-1)%64);
		read_file_name		= (flash_sector_buf[rlocation+0] << 8 ) | flash_sector_buf[rlocation+1];
//			gfx_draw_string("file name: ",10, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//			gfx_draw_int(file_name,100, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//			gfx_draw_int(read_file_name,150, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);

		if (read_file_name == file_name)
		{
//			gfx_draw_string("match",10, 210, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
			
			read_start_sector	= (flash_sector_buf[rlocation+2] << 8 ) | flash_sector_buf[rlocation+3];
			read_length			= (flash_sector_buf[rlocation+4] << 8 ) | flash_sector_buf[rlocation+5];
			read_width			= (flash_sector_buf[rlocation+6] << 8 ) | flash_sector_buf[rlocation+7];
			ffs_read_icon_from_sector(read_start_sector, read_length, read_width, x, y);
			return(0);
		}	
		else
		{
			return(-1);		
		}
	}
	else
	return(-1);					
}

/*

int ffs_create_icon_file(gfx_bitmap *bmp)
{
	
	
}*/


// this file will write an icon to flash after splitting it up into smaller chunks and then update the file list with the information
/*
1. from the flash get the icon progmem pointer.
get the icon length and width.

2. calculate the no of bytes to be used in each sector and the number of rows of the icon that can be accommodated in it.

3. calculate the no of sectors required.

4. check the file list if the memory location is already written based on the file number.

5. if not present already or if overwrite is passed, write the file no, length and breadth to the file list location.
( how will it know what is the next free start sector ?)

6. repeatedly write the chunks of the icon to the flash sectors starting from the start sector.
*/



int get_num_sectors_required(int length, int width)
{
	int icon_rows_per_sector, icon_num_full_sectors, icon_rows_in_last_sector;
	icon_rows_per_sector= AT45DBX_SECTOR_SIZE/(length * 2);
	icon_num_full_sectors = width / icon_rows_per_sector;
	icon_rows_in_last_sector = width % icon_rows_per_sector;
	
	if (icon_rows_in_last_sector)
		return(icon_num_full_sectors+1);
	else
		return(icon_num_full_sectors);
}

// this function takes the flash pointer to the icon
int ffs_write_icon_to_flash(gfx_color_t PROGMEM_PTR_T progmem_pixmap, int icon_length, int icon_width)
{
	gfx_draw_string("write to flash started",10, 10, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	
	gfx_color_t color;
	uint8_t color_temp;
	int icon_length_bytes;

//1.  calculate the size of the image and the number of sectors that it will require.

uint32_t icon_size=0;
int icon_num_sectors=0, icon_rows_in_last_sector=0, icon_num_full_sectors=0, icon_last_sector=0;

////////// assign the icon size here

/*
// each pixel is 2 bytes so multiply the length by 2
	icon_length_bytes = icon_length * 2; // 48*2 = 96

	icon_size = icon_length_bytes* icon_width; 
	icon_rows_per_sector= AT45DBX_SECTOR_SIZE/icon_length_bytes;
//	icon_num_sectors = icon_size /(icon_rows_per_sector*icon_length);
	icon_num_full_sectors = icon_width / icon_rows_per_sector;
//	icon_rows_in_last_sector = (icon_size %(icon_rows_per_sector*icon_length))/icon_length;
	icon_rows_in_last_sector = icon_width % icon_rows_per_sector;
	
	if (icon_rows_in_last_sector)
	{
		icon_last_sector=1;
	}
	icon_num_sectors=icon_num_full_sectors + icon_last_sector;*/
	
	icon_num_sectors = get_num_sectors_required(icon_length, icon_width);
	
	gfx_draw_string("num sectors:",10, 20, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_int(icon_num_sectors,100, 20, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG	gfx_draw_int(icon_num_full_sectors,150, 20, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG	gfx_draw_int(icon_rows_per_sector,200, 20, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG	gfx_draw_int(icon_rows_in_last_sector,250, 20, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//_delay_ms(1000);

//2.  read the INODE summary to know the total number of files in the disk. ( it also contains the occupied size and free space.)
// everything is stored in no of sectors. while displaying it shall be multiplied by sector size.

	uint16_t num_files=0;
	num_files= read_inode_summary();
/*
gfx_draw_string("num files:",10, 20, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
gfx_draw_int(num_files,100, 20, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
_delay_ms(2000);*/

// read all inode sectors to check the earliest available space which can be filled with the new icon
// read the inode sector 1 to check the available file name/number, if not go to the next sector and so on.
	int i,j;
	int file_name=0, file_name_temp=0;
	uint8_t file_name_8bit;
	int earliest_start_sector_temp, available_size_in_sectors;
	int start_sector, end_sector;
	uint32_t read_start_sector_temp=0, read_length_temp, read_width_temp, read_end_sector_temp=0, read_num_sectors_temp=0;
	uint8_t x;
	int num_files_temp=0;
	

///////////////////////////////////////////////////////////////////////	
// go through the complete inode table to find file name availability
	for (x=INODE_TABLE; x<INODE_SIZE && !file_name ; x++)
	{
		at45dbx_read_sector_open(x);
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		
		for (i=0,j=1; i<AT45DBX_SECTOR_SIZE && !file_name; i+=8,j++)
		{
			file_name_temp = (flash_sector_buf[i] << 8 ) | flash_sector_buf[i+1];
			if (file_name_temp==((x-1)*64)+j)
			{
				//gfx_draw_string("present:",200, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
				//gfx_draw_int(file_name_temp,290, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
				//	_delay_ms(50);
			}
			else
			{
				//gfx_draw_string("available:",200, 210, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG//				gfx_draw_int(file_name_temp,290, 210, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
				file_name = ((x-1)*64)+j;
			//	gfx_draw_int(file_name,290, 220, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
				//_delay_ms(2000);
			}
		//	_delay_ms(100);
		}
	}
////////////////////////////////////////////////////////////////////remove this after adding sector search	
/*
	start_sector=(file_name-1)*10;
	end_sector = start_sector + icon_num_sectors;	
*/
/////////////////////////////////////////////////////////////////////////////////////////	
// go through the complete inode table again to find the unused space
int space_found=0, overlap_detected=0;
num_files_temp=0;
start_sector=0;
end_sector = start_sector + icon_num_sectors;
	
//num_files_temp < num_files
	for (x=INODE_TABLE; x<INODE_SIZE && !space_found && num_files ; x++)
	{
//DEBUG		gfx_draw_string("inode:",100, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG		gfx_draw_int(x,160, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG		//TBD_delay_ms(500);
//DEBUG		gfx_draw_string("      ",100, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG		gfx_draw_string("      ",160, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	//	gfx_draw_int(x,160, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
				
		at45dbx_read_sector_open(x);
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		
		for (i=0,j=1; i<AT45DBX_SECTOR_SIZE && !space_found; i+=8,j++)
		{
			file_name_temp = (flash_sector_buf[i] << 8 ) | flash_sector_buf[i+1];
			//TBD_delay_ms(2000);
//DEBUG				gfx_draw_string("file temp:",10, 150, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				gfx_draw_int(file_name_temp,100, 150, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);			
				
			if (file_name_temp==((x-INODE_TABLE)*64)+j)
			{				
//DEBUG				gfx_draw_string("fexist:",150, 150, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);		
					
				// file is present
				num_files_temp++;
//DEBUG				gfx_draw_int(file_name_temp,200, 150, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);	
				// check the file occupied location here
				read_start_sector_temp	= (flash_sector_buf[i+2] << 8 ) | flash_sector_buf[i+3];
				read_length_temp		= (flash_sector_buf[i+4] << 8 ) | flash_sector_buf[i+5];
				read_width_temp			= (flash_sector_buf[i+6] << 8 ) | flash_sector_buf[i+7];
			
			
/*
								
				if (icon_width % (AT45DBX_SECTOR_SIZE/icon_length_bytes))
				read_num_sectors_temp = (4*read_length_temp*read_width_temp/AT45DBX_SECTOR_SIZE) ;
				//	read_end_sector_temp = read_start_sector_temp +(2*read_length_temp*read_width_temp/AT45DBX_SECTOR_SIZE);
				else
				read_num_sectors_temp =  1+ (4*read_length_temp*read_width_temp/AT45DBX_SECTOR_SIZE);
				//	read_end_sector_temp = 1+ read_start_sector_temp +(2*read_length_temp*read_width_temp/AT45DBX_SECTOR_SIZE);
				
*/
				
				
				read_num_sectors_temp = get_num_sectors_required(read_length_temp, read_width_temp);
				
			//	read_end_sector_temp = read_num_sectors_temp + read_start_sector_temp- STORAGE_MEMORY_START;
				read_end_sector_temp = read_num_sectors_temp + read_start_sector_temp;
				



				
					gfx_draw_int(read_start_sector_temp,10, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);		
					gfx_draw_int(read_num_sectors_temp,50, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);		
//DEBUG				gfx_draw_int(read_width_temp,100, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);		
					gfx_draw_int(read_end_sector_temp,150, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);		
//DEBUG				gfx_draw_int(icon_num_sectors	,190, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//				_delay_ms(1000);		
//DEBUG
//DEBUG				gfx_draw_int(start_sector	,10, 190, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG			//	gfx_draw_int(read_length_temp		,50, 190, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG			//	gfx_draw_int(read_width_temp		,100, 190, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				gfx_draw_int(end_sector	,150, 190, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				
//DEBUG				//TBD_delay_ms(2000);
//DEBUG				gfx_draw_string("     ",10, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				gfx_draw_string("     ",50, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				gfx_draw_string("     ",100, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				gfx_draw_string("     ",150, 180, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
				
				// this logic checks for occupied space
				if ((start_sector >= read_start_sector_temp) && (start_sector <= read_end_sector_temp) ||
				(end_sector   >= read_start_sector_temp) && (end_sector   <= read_end_sector_temp) ||
				(read_start_sector_temp >= start_sector) && (read_start_sector_temp <= end_sector) ) // also check if max sector is reached TBD
				{ // overlap is detected
					overlap_detected=1;
					start_sector=read_end_sector_temp+1;
					end_sector = start_sector + icon_num_sectors;
					
/*
					if (num_files_temp==num_files)
					{//checked all the files again to make sure overlap is not set again
*/
				//		overlap_detected=0;
						num_files_temp =0;
						x=INODE_TABLE-1; // this will start the outer loop
						i=AT45DBX_SECTOR_SIZE; // this will break the inner loop

//DEBUG				gfx_draw_string("cleared",150, 170, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);		
				//TBD_delay_ms(2000);
/*
					}
*/
				}
			}
			else // no file is present
			{
//DEBUG				gfx_draw_string("Nofile",10, 140, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				gfx_draw_int(j,100, 140, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
//DEBUG				gfx_draw_int(file_name_temp,100, 160, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);			
//DEBUG				gfx_draw_int(file_name,150, 160, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);	
				//TBD_delay_ms(2000);
														
				if (num_files_temp==num_files)
				{
					if (!overlap_detected)
					{
						// search is over
						space_found=1;
					}
				}					
			}
		}
	}


/*
	
	for (x=INODE_TABLE; x<INODE_SIZE && num_files_temp < num_files; x++)
	{	
		gfx_draw_string("inode:",100, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_int(x,160, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		//TBD_delay_ms(500);
		
		at45dbx_read_sector_open(x);
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		
		for (i=0,j=1; i<AT45DBX_SECTOR_SIZE && ( num_files_temp < num_files || !file_name); i+=8,j++)
		{
			file_name_temp = (flash_sector_buf[i] << 8 ) | flash_sector_buf[i+1];
			gfx_draw_string("   ",200, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
			//TBD_delay_ms(100);
			gfx_draw_int(file_name_temp,200, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);		
//			if (file_name_temp == (x*64)+j)
			if (file_name_temp == ((x-INODE_TABLE)*64)+j)
			{
				gfx_draw_string("match",100, 210, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
				
				// file is present
				num_files_temp++;
				// check the file occupied location here
				read_start_sector_temp	= (flash_sector_buf[i+2] << 8 ) | flash_sector_buf[i+3];
				read_length_temp		= (flash_sector_buf[i+4] << 8 ) | flash_sector_buf[i+5];
				read_width_temp			= (flash_sector_buf[i+6] << 8 ) | flash_sector_buf[i+7];
				if (icon_width % (AT45DBX_SECTOR_SIZE/icon_length_bytes))
					read_end_sector_temp = read_start_sector_temp +(read_length_temp*read_width_temp/AT45DBX_SECTOR_SIZE);
				else
					read_end_sector_temp = 1+ read_start_sector_temp +(read_length_temp*read_width_temp/AT45DBX_SECTOR_SIZE);
				
				// this logic checks for occupied space
				if ((start_sector >= read_start_sector_temp) && (start_sector <= read_end_sector_temp) || 
				    (end_sector   >= read_start_sector_temp) && (end_sector   <= read_end_sector_temp) ||
					(read_start_sector_temp >= start_sector) && (read_start_sector_temp <= end_sector) ) // also check if max sector is reached TBD
				{
					start_sector=read_end_sector_temp+1;
					end_sector = start_sector + icon_num_sectors;
					if (num_files_temp<num_files)
					{
						num_files_temp =0;
						x=INODE_TABLE; // this will start the outer loop
						i=AT45DBX_SECTOR_SIZE; // this will break the inner loop
					}
				}			
			}
		}	
	}

	*/
	if (!file_name)
	{
		file_name =1;
	}
	
	
	// if start_sector is still 0 then the entire disk is free - check with num files
	// we should have a special variable to check against the max space and also the full file name space
	
////////////////////////	// if both file_name is valid and start_sector and end_sector are present then we shall write the icon file to that location and update the inode
	gfx_draw_string("file name:",10, 40, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_int(file_name,100, 40, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);

	gfx_draw_string("start sector:",10, 50, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_int(start_sector,100, 50, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("end sector:",150, 50, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_int(end_sector,210, 50, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);


//_delay_ms(2000);

	if (file_name && ( (!start_sector && !num_files) || (start_sector && num_files) ) && end_sector)
	{
	ffs_write_icon_to_sector(progmem_pixmap, start_sector, icon_length, icon_width);
//	ffs_read_icon_from_sector(				 start_sector, icon_length, icon_width, 10,150);
	}
/////////////////////	// update the inode page with the new file details
	
	ffs_add_file_to_inode(file_name, start_sector, icon_length, icon_width);

/////////////////////	// update the inode summary with the file addition
	num_files++; // increment the number of files

	write_inode_summary(num_files);

	
// from the read sectors( from the beginning ) check if the required no of sectors are free right from the beginning and store the available space.
// recursively store the image onto the start sector.
// write back the inode information with the written sector and size.


////////////// read and display the icon from the written sector 
//start_sector = 10;
	return(file_name);
	
}

void ffs_display_file_table( int start_file, int end_file, int selected_file )
{
	int sector;
	int num_columns_t;
	num_columns_t = 8;
	char* headers[8] = {"BAdd","F No","S Sect", "End S","NumS","Len","Wid","Siz"    };
	ili9341_color_t selectoion_color=GFX_COLOR_WHITE;
	uint32_t size_on_disk;

	int start_sector, end_sector;
	uint32_t read_start_sector_temp=0, read_length_temp, read_width_temp, read_end_sector_temp=0,file_name_temp,num_files_temp, read_num_sectors_temp=0;

	
	uint32_t	icon_size;
	int	icon_rows_per_sector, icon_num_sectors , icon_rows_in_last_sector;
	
	int temp_difference;
	temp_difference = 	end_file - start_file;
	sector = 1+start_file/64;
	start_file = start_file%64;
	end_file = start_file + temp_difference;
			
	int i,j,k;
	at45dbx_read_sector_open(sector);
	at45dbx_read_sector_to_ram(flash_sector_buf);
	at45dbx_read_close();	
	num_files_temp =0;

	gfx_draw_filled_rect(10,5,300,130,GFX_COLOR_WHITE);
	gfx_draw_rect(10,5,300,110,GFX_COLOR_BLACK);
//	gfx_draw_rect(10,5,300,20,GFX_COLOR_BLACK);
	
	for (i=0;i<num_columns_t;i++)
	{
		gfx_draw_rect(10+i*(300/num_columns_t),5,(300/num_columns_t),110,GFX_COLOR_BLACK);
		gfx_draw_rect(10+i*(300/num_columns_t),5,(300/num_columns_t),20,GFX_COLOR_BLACK);	
		gfx_draw_string(headers[i],14+i*(300/num_columns_t), 10, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);	
	}
	

			
	for (i=start_file*8 ,j=start_file; i<AT45DBX_SECTOR_SIZE && j <= end_file; i+=8,j++)
	{
		if (j==(1+(selected_file-1)%64))
			selectoion_color=GFX_COLOR_YELLOW;
		else
			selectoion_color=GFX_COLOR_WHITE;	
		
			file_name_temp			= (flash_sector_buf[i] << 8 ) | flash_sector_buf[i+1];
			read_start_sector_temp	= (flash_sector_buf[i+2] << 8 ) | flash_sector_buf[i+3];
			read_length_temp		= (flash_sector_buf[i+4] << 8 ) | flash_sector_buf[i+5];
			read_width_temp			= (flash_sector_buf[i+6] << 8 ) | flash_sector_buf[i+7];
/*
		
			read_num_sectors_temp =  1+ (4*read_length_temp*read_width_temp/AT45DBX_SECTOR_SIZE);
*/
			
			read_num_sectors_temp = get_num_sectors_required(read_length_temp, read_width_temp);			
			read_end_sector_temp    = read_start_sector_temp + 	read_num_sectors_temp ;	
		
//		gfx_draw_int(file_name_temp, 10, 200, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		k=0;
		gfx_draw_int(i									,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;																		  
		gfx_draw_int(file_name_temp						,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;																		 
		gfx_draw_int(read_start_sector_temp				,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;			
		gfx_draw_int(read_end_sector_temp				,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;
		gfx_draw_int(read_num_sectors_temp				,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;
		gfx_draw_int(read_length_temp					,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;																		 
		gfx_draw_int(read_width_temp					,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;																		 
		gfx_draw_int(read_length_temp*read_width_temp*2	,14+k*(300/num_columns_t), 30+(j-start_file)		*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
		k++;
		
		
			icon_size = 2*read_length_temp* read_width_temp;
			icon_rows_per_sector= AT45DBX_SECTOR_SIZE/(read_length_temp*2);
			icon_num_sectors = read_width_temp / icon_rows_per_sector;
			icon_rows_in_last_sector = read_width_temp % icon_rows_per_sector;
			if (icon_rows_in_last_sector)
			{
				icon_num_sectors++;
			}
			size_on_disk = icon_num_sectors*AT45DBX_SECTOR_SIZE;
			
//		gfx_draw_int(size_on_disk,14+k*(300/num_columns_t), 30+(j-start_file)*10, &sysfont,selectoion_color, GFX_COLOR_BLACK);
				
	//	//TBD_delay_ms(500);
	}
}




int ffs_add_file_to_inode(int file_name, int start_sector, int icon_length, int icon_width)
{
	uint8_t temp_name_8bit;

	at45dbx_read_sector_open(INODE_TABLE+((file_name-1)/64));
	at45dbx_read_sector_to_ram(flash_sector_buf);
	at45dbx_read_close();
		

	
	// file name update
	temp_name_8bit = file_name>>8;
	flash_sector_buf[0+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	temp_name_8bit = file_name;
	flash_sector_buf[1+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	
	// start sector
	temp_name_8bit = start_sector>>8;
	flash_sector_buf[2+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	temp_name_8bit = start_sector;
	flash_sector_buf[3+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	
	// icon_length
	temp_name_8bit = icon_length>>8;
	flash_sector_buf[4+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	temp_name_8bit = icon_length;
	flash_sector_buf[5+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	
	// icon_width
	temp_name_8bit = icon_width>>8;
	flash_sector_buf[6+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	temp_name_8bit = icon_width;
	flash_sector_buf[7+ ((file_name-1)%64)*8]		=	temp_name_8bit;
	
	
	at45dbx_write_sector_open(INODE_TABLE+((file_name-1)/64));
	at45dbx_write_sector_from_ram(flash_sector_buf);
	at45dbx_write_close();
/*
	
	gfx_draw_string("written inode:",10, 70, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_int(file_name,100, 70, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);*/
}

int ffs_dumb_delete_file_from_inode(int file_name1)
{
	
	uint8_t temp_name_8bit;
		temp_name_8bit = 0;
		flash_sector_buf[0+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		temp_name_8bit = 0;
		flash_sector_buf[1+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		
		// start sector
		temp_name_8bit = 0;
		flash_sector_buf[2+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		temp_name_8bit = 0;
		flash_sector_buf[3+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		
		// icon_length
		temp_name_8bit = 0;
		flash_sector_buf[4+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		temp_name_8bit = 0;
		flash_sector_buf[5+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		
		// icon_width
		temp_name_8bit = 0;
		flash_sector_buf[6+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		temp_name_8bit = 0;
		flash_sector_buf[7+ ((file_name1-1)%64)*8]		=	temp_name_8bit;
		
		
		at45dbx_write_sector_open(INODE_TABLE+(file_name1/64));
		at45dbx_write_sector_from_ram(flash_sector_buf);
		at45dbx_write_close();	
}

int ffs_delete_file_from_inode(int file_name1)
{

	uint8_t temp_name_8bit;
	int read_fnum, file_offset1,num_files;	
	int file_presence =-1;
	clr_scr();

	file_presence =ffs_is_file_present(file_name1);
	
	if (file_presence ==0) // file present
	{
		gfx_draw_string("file found:",10, 120, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_int(file_name1,80, 120, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		ffs_dumb_delete_file_from_inode(file_name1);
		gfx_draw_string("Deleted",10, 130, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		
		_delay_ms(1000);
		
		// check if already file was present or not
		num_files= read_inode_summary();
		num_files--;
		write_inode_summary(num_files);
		return(0);
	}
	else
	if (file_presence ==-1) // file missing
	{
		gfx_draw_string("file not found!",10, 120, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		_delay_ms(1000);
		return(-1);
		
	}
}


int read_inode_summary(void)
{
	uint16_t num_files=0;
	at45dbx_read_sector_open(INODE_SUMMARY);
	at45dbx_read_sector_to_ram(flash_sector_buf);
	at45dbx_read_close();
	num_files = (flash_sector_buf[0] << 8 ) | flash_sector_buf[1];

/*
	gfx_draw_string("num files:",10, 30, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_int(num_files,100, 30, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);*/
	return(num_files);
}

int write_inode_summary(int num_files)
{
	uint8_t temp_name_8bit;	
	at45dbx_read_sector_open(INODE_SUMMARY);
	at45dbx_read_sector_to_ram(flash_sector_buf);
	at45dbx_read_close();
	
	temp_name_8bit = num_files>>8;
	flash_sector_buf[0]	=	temp_name_8bit;
	temp_name_8bit = num_files;
	flash_sector_buf[1]	=	temp_name_8bit;
	
	at45dbx_write_sector_open(INODE_SUMMARY);
	at45dbx_write_sector_from_ram(flash_sector_buf);
	at45dbx_write_close();	
}

void ffs_read_icon_from_sector(uint16_t start_sector, int icon_length, int icon_width, gfx_coord_t x, gfx_coord_t y)
{
	ili9341_color_t ram_buf_temp[0x200];
	int i, j;
	uint8_t u;


	int icon_length_bytes, icon_rows_per_sector, icon_num_full_sectors, icon_rows_in_last_sector;
	uint32_t icon_size=0;
	icon_length_bytes = icon_length * 2; // 48*2 = 96
	icon_size = icon_length_bytes* icon_width;
	icon_rows_per_sector= AT45DBX_SECTOR_SIZE/icon_length_bytes;
	icon_num_full_sectors = icon_width / icon_rows_per_sector;
	icon_rows_in_last_sector = icon_width % icon_rows_per_sector;

	struct gfx_bitmap DATA_FLASH_ICON;
	DATA_FLASH_ICON.width =icon_length;
	DATA_FLASH_ICON.height =icon_rows_per_sector;
	DATA_FLASH_ICON.type =GFX_BITMAP_RAM;

	for (u=0; u<icon_num_full_sectors; u++)
	{
			
		at45dbx_read_sector_open(STORAGE_MEMORY_START+start_sector+u);
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		for(i=0, j=0;i<icon_rows_per_sector*icon_length_bytes;i+=2, j++) {
			ram_buf_temp[j] = (flash_sector_buf[i] << 8 ) | flash_sector_buf[i+1];
		}
	DATA_FLASH_ICON.data.pixmap= ram_buf_temp;
//		gfx_draw_string("sector:        ",10, 160, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
//		gfx_draw_string("sector:",10, 160, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
//		gfx_draw_int(STORAGE_MEMORY_START+start_sector+u,80, 160, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);	
	gfx_draw_bitmap(&DATA_FLASH_ICON, x,y+(u*icon_rows_per_sector));
//	_delay_ms(50);
	}
	if (icon_rows_in_last_sector)
	{
		DATA_FLASH_ICON.height =icon_rows_in_last_sector;
		at45dbx_read_sector_open(STORAGE_MEMORY_START+start_sector+u);
		at45dbx_read_sector_to_ram(flash_sector_buf);
		at45dbx_read_close();
		for(i=0, j=0;i<icon_rows_in_last_sector*icon_length_bytes;i+=2, j++) {
			ram_buf_temp[j] = (flash_sector_buf[i] << 8 ) | flash_sector_buf[i+1];
		}
		DATA_FLASH_ICON.data.pixmap= ram_buf_temp;
		gfx_draw_bitmap(&DATA_FLASH_ICON, x,y+(u*icon_rows_per_sector));
	}
}



int ffs_write_icon_to_sector(gfx_color_t PROGMEM_PTR_T progmem_pixmap, int start_sector, int icon_length, int icon_width)
{
	int x, i;
	gfx_color_t color;
	uint8_t color_temp;
	
	
	int icon_length_bytes, icon_rows_per_sector, icon_num_full_sectors, icon_rows_in_last_sector; 
	uint32_t icon_size=0;
	icon_length_bytes = icon_length * 2; // 48*2 = 96
	icon_size = icon_length_bytes* icon_width;
	icon_rows_per_sector= AT45DBX_SECTOR_SIZE/icon_length_bytes;
	icon_num_full_sectors = icon_width / icon_rows_per_sector;
	icon_rows_in_last_sector = icon_width % icon_rows_per_sector;	
	
//		_delay_ms(1000);	
	for (x=0; x<icon_num_full_sectors; x++)
	{
		// copy the icon buffer to the flash buffer
		for(i=0;i<icon_rows_per_sector*icon_length_bytes;i+=2)
		{
			color = PROGMEM_READ_WORD(progmem_pixmap);
			color_temp = (color >> 8);
			flash_sector_buf[i]	=color_temp;
			color_temp = color;
			flash_sector_buf[i+1]	=color_temp;
			progmem_pixmap++;
		}
		at45dbx_write_sector_open(STORAGE_MEMORY_START+start_sector+x);
		at45dbx_write_sector_from_ram(flash_sector_buf);
		at45dbx_write_close();
		
//		gfx_draw_string("sector:        ",10, 160, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
//		gfx_draw_int(STORAGE_MEMORY_START+start_sector+x,80, 160, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
//		gfx_draw_bitmap(&DATA_FLASH_ICON, x,y+(u*icon_rows_per_sector));
//		_delay_ms(50);		
/*
		gfx_draw_string("written sectors:",10, 60, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_int(STORAGE_MEMORY_START+start_sector+x,130, 60, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);*/
//		//TBD_delay_ms(100);
//	return(STORAGE_MEMORY_START+start_sector);
	}
	

	if (icon_rows_in_last_sector)
	{
		for(i=0;i<icon_rows_in_last_sector*icon_length_bytes;i+=2)
		{
			color = PROGMEM_READ_WORD(progmem_pixmap);
			color_temp = (color >> 8);
			flash_sector_buf[i]	=color_temp;
			color_temp = color;
			flash_sector_buf[i+1] =color_temp;
			progmem_pixmap++;
		}
		at45dbx_write_sector_open(STORAGE_MEMORY_START+start_sector+x);
		at45dbx_write_sector_from_ram(flash_sector_buf);
		at45dbx_write_close();
/*
		gfx_draw_string("+",180, 60, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_int(icon_rows_in_last_sector,200, 60, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string("(",220, 60, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_int(STORAGE_MEMORY_START+start_sector+x,240, 60, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(")",270, 60, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);*/
	}

}

