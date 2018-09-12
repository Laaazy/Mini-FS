//
//  creat.cpp
//  Test1
//
//  Created by 王岩 on 2018/9/9.
//  Copyright © 2018 王岩. All rights reserved.
//

#include <stdio.h>

#include "OS.h"

//全局变量
extern FILE *p; //外存起始位置
extern std::string cmd;
DISK *disk = new DISK;
extern char command[50]; //文件名标识符
extern char SName[59];
int makeNewFile(const char* filename, int size);
long filesize(FILE *fp);
int newfileout(char name[]);
int addr_superblock_start = 0;
int addr_fat_start = 0;
int addr_inodemap_start = 0;
int addr_inodearray_start = 0;
int addr_dirUnits_start = 0;
int addr;

void creat(char fileName[])
{

	//char absoluteAddr[100] = "/Users/Jetpack/Desktop/GOod GOOd sTudy./Mini_FS/";
	//strcat(absoluteAddr, SName);
	//FILE* fp = fopen("/Users/Jetpack/Desktop/GOod GOOd sTudy./Mini_FS/minifs.txt", "wb");
	//FILE* fp = fopen("E:/mini-FS/mini.fs", "rb");
	//if (fp == NULL)
	{
		char str[50] = "D:/mini-FS/";
		p = fopen(strcat(str,fileName), "wb");
		for (int i = 0; i < inode_count; i++)
		{
			disk->dirUnits[i].inodeNumber = -1;
			disk->dirUnits[i].fileName[0] = '\0';
		}
		for (int i = 0; i < reserved_block_mount; i++)
			disk->FAT[i] = 1;
		for (int i = reserved_block_mount; i < block_mount; i++)
			disk->FAT[i] = 0;
		for (int i = 0; i < inode_count; i++)
			disk->inode_Map[i] = 0;
		for (int i = 0; i < inode_count; i++)
		{
			disk->inode_array[i].fileSize = 0;
			disk->inode_array[i].startBlockNum = 0;
		}
		disk->superblock.blockMount = block_mount;
		disk->superblock.fatBlockMount = sizeof(disk->FAT) / block_size;
		disk->superblock.iNodeBlockMount = sizeof(sizeof(disk->inode_array)) / block_size;
		disk->superblock.iNodeMount = data_block_mount;
		disk->superblock.iNodeNum = 0;
		disk->superblock.num1stDataBlock = reserved_block_mount + 1;

		//kaitou
		//superblock
		addr_superblock_start = 0;
		fseek(p, addr_superblock_start, SEEK_SET);
		fwrite(&(disk->superblock), sizeof(disk->superblock), 1, p);


		//FAT
		addr_fat_start = block_size * 10;
		fseek(p, addr_fat_start, SEEK_SET);
		fwrite((disk->FAT), sizeof((disk->FAT)), 1, p);

		//inode_map
		addr_inodemap_start = block_size * 300;
		fseek(p, addr_inodemap_start, SEEK_SET);
		fwrite(disk->inode_Map, sizeof((disk->inode_Map)), 1, p);

		//inode_array
		addr_inodearray_start = block_size * 2000;
		fseek(p, addr_inodearray_start, SEEK_SET);
		fwrite((disk->inode_array), sizeof((disk->inode_array)), 1, p);

		//dirUnits
		addr_dirUnits_start = block_size * 8000;
		fseek(p, addr_dirUnits_start, SEEK_SET);
		fwrite(disk->dirUnits, sizeof(disk->dirUnits), 1, p);
		addr = reserved_block_mount * 4096;

		//jiewei

		fseek(p, system_size - 1, SEEK_SET);
		char end = EOF;
		fwrite(&end, 1, 1, p);
		fclose(p);
	}
}


void save_superblock(FILE *fp)
{
	fseek(fp, 0, SEEK_SET);
	fread(&disk->superblock, sizeof(disk->superblock), 1, fp);
	//fread(&disk->superblock.blockMount, 1, 4, fp);
	//fseek(fp, 4, SEEK_CUR);
	//fread(&disk->superblock.fatBlockMount, 1, 4, fp);
	//fseek(fp, 4, SEEK_CUR);
	//fread(&disk->superblock.iMapBlockMount, 1, 4, fp);
	//fseek(fp, 4, SEEK_CUR);
	//fread(&disk->superblock.iNodeBlockMount, 1, 4, fp);
	//fseek(fp, 4, SEEK_CUR);
	//fread(&disk->superblock.iNodeMount, 1, 4, fp);
	//fseek(fp, 4, SEEK_CUR);
	//fread(&disk->superblock.num1stDataBlock, 1, 4, fp);
}


void save_fat(FILE *fp)
{
	fseek(fp, block_size * 10, SEEK_SET);
	fread(disk->FAT, 1, block_mount * 4, fp);
}


void save_inode_map(FILE *fp)
{
	fseek(fp, block_size * 300, SEEK_SET);
	fread(disk->inode_Map, 1, inode_count * 4, fp);
}


void save_inode_array(FILE *fp)
{
	fseek(fp, block_size * 2000, SEEK_SET);
	fread(disk->inode_array, 1, inode_count * 12, fp);

}


void save_dirUnits(FILE *fp)
{
	fseek(fp, block_size * 8000, SEEK_SET);
	fread(disk->dirUnits, 1, 63 * inode_count, fp);
}


void mount(char fileName[])
{
	char str[50] = "D:/mini-FS/";
	p = fopen(strcat(str,fileName), "rb+");
	save_superblock(p);
	save_fat(p);
	save_inode_map(p);
	save_inode_array(p);
	save_dirUnits(p);
	fclose(p);
}


void copyin(const char*filename)//复制文件
{

	int nowaddr;
	long size = 0;
	int sum = 0;
	FILE *fp = fopen(filename, "rb");
	size = filesize(fp);
	int count = (size + block_size - 1) / block_size;//块数
	int countsum = count * block_size; //buffer大小
	char *buffer = (char*)malloc(countsum);
	for (int i = 0; i <countsum; i++)
		buffer[i] = 0;
	//if (disk->superblock.blockMount*block_size < size)
	//	return;
	fread(buffer, 1, size, fp);
	fclose(fp);

	int filenumber = makeNewFile(filename, count);
	disk->inode_array[filenumber].fileSize = size;
	p = fopen("D:/mini-FS/mini.fs", "rb+");
	//fseek(p, addr_inodearray_start, SEEK_SET);
	//rite(&size, size, 1, p);
	nowaddr = addr + filenumber * block_size;
	fseek(p, nowaddr, SEEK_SET);
	filenumber = reserved_block_mount + filenumber;

	char*ch = buffer;
	//int now = addr_fat_start+65536+filenumber-reserved_block_mount;
	for (int i = 0; i < count; i++)
	{
		fwrite(ch, 1, block_size, p);
		//fseek(p, now, SEEK_SET);
		//rite(&filenumber, 4, 1, p);
		//w = disk->FAT[filenumber] - reserved_block_mount + 65536 + addr_fat_start;
		nowaddr = disk->FAT[filenumber] * block_size;
		fseek(p, nowaddr, SEEK_SET);
		filenumber = disk->FAT[filenumber];
		ch = ch + block_size;
	}
	fclose(p);
	disk->superblock.iNodeNum++;
}


void copyout(const char*filename, char SName[])
{

	p = fopen("D:/mini-FS/mini.fs", "rb");
	long size;

	int nowaddr;
	int nowinode;
	nowinode = newfileout(SName);
	int startnumber = disk->inode_array[disk->dirUnits[nowinode].inodeNumber].startBlockNum;
	nowaddr = addr + nowinode * block_size;
	size = disk->inode_array[nowinode].fileSize;
	fseek(p, nowaddr, SEEK_SET);
	char *buffer = (char*)malloc(64 * 1024 * 1024);
	nowinode += reserved_block_mount;
	int blockcount = (size + block_size - 1) / block_size;
	char *ch = buffer;
	for (int i = 0; i < size; i++)
		buffer[i] = 0;
	for (int i = 0; i < blockcount; i++)
	{
		fread(ch, 1, block_size, p);
		nowaddr = disk->FAT[nowinode] * block_size;
		fseek(p, nowaddr, SEEK_SET);
		nowinode = disk->FAT[nowinode];
		ch = ch + block_size;
	}
	fclose(p);

	FILE *fpp = fopen(filename, "wb");
	fwrite(buffer, 1, size, fpp);
	fclose(fpp);
	disk->superblock.iNodeNum--;
}



//格式化系统
int format(char fileName[]) {
	{

		char str[50] = "D:/mini-FS/";
		p = fopen(strcat(str,fileName), "wb");
		for (int i = 0; i < inode_count; i++)
		{
			disk->dirUnits[i].inodeNumber = -1;
			disk->dirUnits[i].fileName[0] = '\0';
		}
		for (int i = 0; i < reserved_block_mount; i++)
			disk->FAT[i] = 1;
		for (int i = reserved_block_mount; i < block_mount; i++)
			disk->FAT[i] = 0;
		for (int i = 0; i < inode_count; i++)
			disk->inode_Map[i] = 0;
		for (int i = 0; i < inode_count; i++)
		{
			disk->inode_array[i].fileSize = 0;
			disk->inode_array[i].startBlockNum = 0;
		}
		disk->superblock.blockMount = block_mount;
		disk->superblock.fatBlockMount = sizeof(disk->FAT) / block_size;
		disk->superblock.iNodeBlockMount = sizeof(sizeof(disk->inode_array)) / block_size;
		disk->superblock.iNodeMount = data_block_mount;
		disk->superblock.iNodeNum = 0;
		disk->superblock.num1stDataBlock = reserved_block_mount + 1;

		//kaitou
		//superblock
		addr_superblock_start = 0;
		fseek(p, addr_superblock_start, SEEK_SET);
		fwrite(&(disk->superblock), sizeof(disk->superblock), 1, p);


		//FAT
		addr_fat_start = block_size * 10;
		fseek(p, addr_fat_start, SEEK_SET);
		fwrite((disk->FAT), sizeof((disk->FAT)), 1, p);

		//inode_map
		addr_inodemap_start = block_size * 300;
		fseek(p, addr_inodemap_start, SEEK_SET);
		fwrite(disk->inode_Map, sizeof((disk->inode_Map)), 1, p);

		//inode_array
		addr_inodearray_start = block_size * 2000;
		fseek(p, addr_inodearray_start, SEEK_SET);
		fwrite((disk->inode_array), sizeof((disk->inode_array)), 1, p);

		//dirUnits
		addr_dirUnits_start = block_size * 8000;
		fseek(p, addr_dirUnits_start, SEEK_SET);
		fwrite(disk->dirUnits, sizeof(disk->dirUnits), 1, p);
		addr = reserved_block_mount * 4096;

		//jiewei

		fseek(p, system_size - 1, SEEK_SET);
		char end = EOF;
		fwrite(&end, 1, 1, p);
		fclose(p);
	}
	return 0;
}

