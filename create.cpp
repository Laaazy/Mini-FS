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
char *buffer = (char*)malloc(700 * 1024 * 1024);

void creat(char fileName[])
{
	char str[50] = "D:/mini-FS/";
	//p = fopen(strcat(str,fileName), "rb");
	//if (p == NULL)
	{
		p = fopen(strcat(str,fileName), "wb");
		for (int i = 0; i < inode_count; i++)
		{
			disk->dirUnits[i].inodeNumber = -1;
			disk->dirUnits[i].fileName[0] = 0;
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
		//disk->superblock.iNodeNum = 0;
		disk->superblock.num1stDataBlock = reserved_block_mount + 1;
		disk->superblock.iMapBlockMount = sizeof(disk->inode_Map) / block_size;
		//kaitou
		//superblock
		addr_superblock_start = 0;
		fseek(p, addr_superblock_start, SEEK_SET);
		fwrite(&(disk->superblock), block_size, 1, p);


		//FAT
		addr_fat_start = block_size * 10;
		fseek(p, addr_fat_start, SEEK_SET);
		fwrite((disk->FAT), block_mount * 4, 1, p);

		//inode_map
		addr_inodemap_start = block_size * 300;
		fseek(p, addr_inodemap_start, SEEK_SET);
		fwrite(disk->inode_Map, inode_count * 4, 1, p);

		//inode_array
		addr_inodearray_start = block_size * 2000;
		fseek(p, addr_inodearray_start, SEEK_SET);
		fwrite((disk->inode_array), inode_count * 12, 1, p);

		//dirUnits
		addr_dirUnits_start = block_size * 8000;
		fseek(p, addr_dirUnits_start, SEEK_SET);
		fwrite(disk->dirUnits, 63 * inode_count, 1, p);
		addr = reserved_block_mount * 4096;

		//jiewei

		fseek(p, system_size - 1, SEEK_SET);
		char end = EOF;
		fwrite(&end, 1, 1, p);
		fclose(p);
	}
	/*int flag = 0;
	for (int i = 0; i < data_block_mount; i++) {
		if (disk->dirUnits[i].inodeNumber != -1) {
			flag++;
		}
	}
	printf("%d\n", flag);*/
}


void save_superblock(FILE *fp)
{
	fseek(fp, 0, SEEK_SET);
	fread(&disk->superblock,1, sizeof(disk->superblock), fp);
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
	fread(disk->FAT, 1, block_mount*4, fp);
}


void save_inode_map(FILE *fp)
{
	fseek(fp, block_size * 300, SEEK_SET);
	fread(disk->inode_Map, 1, inode_count*4, fp);
}


void save_inode_array(FILE *fp)
{
	fseek(fp, block_size * 2000, SEEK_SET);
	fread(disk->inode_array, 1, inode_count*12, fp);

}


void save_dirUnits(FILE *fp)
{
	fseek(fp, block_size * 8000, SEEK_SET);
	fread(disk->dirUnits, 1, inode_count*63, fp);
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
	//fseek(p, 4, SEEK_SET);
	//fwrite(&disk->superblock.iNodeNum, 4, 1, p);
	addr = reserved_block_mount * 4096;
	fclose(p);
}


void copyin(char spaceName[],const char*filename)//复制文件
{

	int nowaddr;
	long size = 0;
	int sum = 0;
	FILE *fp = fopen(filename, "rb");
	size = filesize(fp);
	int count = (size + block_size - 1) / block_size;//块数
	int countsum = count * block_size; //buffer大小
	//char *buffer = (char*)malloc(700*1024*1024);
	for (int i = 0; i <countsum; i++)
		buffer[i] = 0;
	//if (disk->superblock.blockMount*block_size < size)
	//	return;
	fread(buffer, 1, size, fp);
	fclose(fp);

	int filenumber = makeNewFile(filename, count);
	disk->inode_array[filenumber].fileSize = size;
	char str[50] = "D:/mini-FS/";
	p = fopen(strcat(str,spaceName), "rb+");
	//fseek(p, addr_inodearray_start, SEEK_SET);
	//rite(&size, size, 1, p);
	nowaddr = addr + disk->dirUnits[filenumber].inodeNumber * block_size;
	fseek(p, nowaddr, SEEK_SET);
	filenumber = reserved_block_mount + filenumber;
	int last = size%block_size;
	char*ch = buffer;
	//int now = addr_fat_start+65536+filenumber-reserved_block_mount;
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
		{
			fwrite(ch, 1, last, p);
		}
		else {
			fwrite(ch, 1, block_size, p);
			//fseek(p, now, SEEK_SET);
			//rite(&filenumber, 4, 1, p);
			//w = disk->FAT[filenumber] - reserved_block_mount + 65536 + addr_fat_start;
			nowaddr = disk->FAT[filenumber] * block_size;
			fseek(p, nowaddr, SEEK_SET);
			filenumber = disk->FAT[filenumber];
			ch = ch + block_size;
		}
	}
	//disk->superblock.iNodeNum++;
	fseek(p, addr_superblock_start, SEEK_SET);
	fwrite(&(disk->superblock), sizeof(disk->superblock), 1, p);
	fseek(p, addr_dirUnits_start, SEEK_SET);
	fwrite(disk->dirUnits, sizeof(disk->dirUnits), 1, p);
	fseek(p, addr_inodemap_start, SEEK_SET);
	fwrite(disk->inode_Map, sizeof(disk->inode_Map), 1, p);
	fseek(p, addr_inodearray_start, SEEK_SET);
	fwrite(disk->inode_array, sizeof(disk->inode_array), 1, p);
	fseek(p, addr_fat_start, SEEK_SET);
	fwrite(disk->FAT, sizeof(disk->FAT), 1, p);
	fclose(p);
	/*int flag = 0;
	for (int i = 0; i < data_block_mount; i++) {
		if (disk->dirUnits[i].inodeNumber != -1)
			flag++;
	}
	printf("%d\n", flag);*/
}


void copyout(char spaceName[],const char*filename, char SName[])
{
	char str[50] = "D:/mini-FS/";
	p = fopen(strcat(str,spaceName), "rb");
	long size;

	int nowaddr;
	int nowinode;
	nowinode = newfileout(SName);
	int startnumber = disk->inode_array[disk->dirUnits[nowinode].inodeNumber].startBlockNum;
	nowaddr = addr + nowinode * block_size;
	size = disk->inode_array[nowinode].fileSize;
	fseek(p, nowaddr, SEEK_SET);
	
	nowinode += reserved_block_mount;
	int blockcount = (size + block_size - 1) / block_size;
	char *ch = buffer;
	int last = size%block_size;
	for (int i = 0; i < size; i++)
		buffer[i] = 0;
	for (int i = 0; i < blockcount; i++)
	{
		if (i == blockcount - 1)
		{
			fread(ch, 1, last, p);
		}
		else
		{
			fread(ch, 1, block_size, p);
			nowaddr = disk->FAT[nowinode] * block_size;
			fseek(p, nowaddr, SEEK_SET);
			nowinode = disk->FAT[nowinode];
			ch = ch + block_size;
		}
	}
	fclose(p);

	FILE *fpp = fopen(filename, "wb");
	fwrite(buffer, 1, size, fpp);
	fclose(fpp);
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
		//disk->superblock.iNodeNum = 0;
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

//删除文件内容
int releaseFile(int inodeNum) {
	iNode* Myinode = &disk->inode_array[inodeNum];
	int fileBlock[data_block_mount];
	memset(fileBlock, 0, sizeof(fileBlock));
	for (int i = 0, j = Myinode->startBlockNum; i < Myinode->fileSize/block_size; i++, j = disk->FAT[j]) {
		fileBlock[i] = disk->FAT[j];
	}
	int i = 0;
	while (fileBlock[i] != 0) {
		disk->FAT[fileBlock[i]] = 0;
		i++;
	}
	Myinode->fileSize = 0;
	return 0;
}

//删除目录表中文件对应目录项
int deleteDirUnit(int unitIndex) {
	//int dirAmount = disk->superblock.iNodeNum;
	
	//memset(disk->dirUnits[unitIndex].fileName, 0, sizeof(disk->dirUnits[unitIndex].fileName));
	disk->dirUnits[unitIndex].inodeNumber = -1;
	disk->dirUnits[unitIndex].fileName[0] = 0;
	//disk->superblock.iNodeNum--;
	return 0;
}


//删除文件
int deleteFile(char spaceName[],char fileName[]) {
	char str[50] = "D:/mini-FS/";
	p = fopen(strcat(str,spaceName), "rb+");
	//int fileNum = disk->superblock.iNodeNum;
	int i;
	int flag = 0;
	for (i = 0; i < data_block_mount; i++) {
		if (strcmp(disk->dirUnits[i].fileName, fileName) == 0) {
			//找到文件
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		return -1;
	}
	dirUnit myUnit = disk->dirUnits[i];
	int inodeNum = myUnit.inodeNumber;

	releaseFile(inodeNum);
	disk->inode_array[disk->dirUnits[i].inodeNumber].fileSize = 0;
	disk->inode_array[disk->dirUnits[i].inodeNumber].startBlockNum = 0;
	disk->inode_Map[disk->dirUnits[i].inodeNumber] = 0;
	deleteDirUnit(i);
	int now = addr_dirUnits_start + i * 63;
	fseek(p, now, SEEK_SET);
	fwrite(disk->dirUnits, 63, 1, p);
	now = addr_inodearray_start + i * 12;
	fseek(p, now, SEEK_SET);
	fwrite(disk->inode_array, 12, 1, p);
	now = addr_inodemap_start + i * 4;
	fseek(p, now, SEEK_SET);
	fwrite(disk->inode_Map, 4, 1, p);
	now = addr_superblock_start;
	fseek(p, now, SEEK_SET);
	fwrite(&disk->superblock, sizeof(disk->superblock), 1, p);

	fclose(p);
	return 0;
}


//文件重命名
int changeName(char spaceName[],char oldName[], char newName[])
{
	int i;
	for (i = 0; i < strlen(newName); i++) {
		if (newName[i] == '.')
			break;
	}
	//规定文件名不超过8位
	if (i > 8) {
		printf("file name too long!\n");
		return -1;
	}
	//扩展名不能超过3位
	if (strlen(newName) - i - 1>3) {
		printf("extend name too long!\n");
		return -1;
	}
	char str[50] = "D:/mini-FS/";
	p = fopen(strcat(str,spaceName), "rb+");
	for (int i = 0; i < data_block_mount; i++)
	{
		if (strcmp(oldName, disk->dirUnits[i].fileName) == 0)
		{
			strcpy(disk->dirUnits[i].fileName, newName);
			int now = addr_dirUnits_start;
			fseek(p, now, SEEK_SET);
			fwrite(disk->dirUnits, sizeof(disk->dirUnits), 1, p);
			return 0;
		}
	}
	fclose(p);
	return -1;
}


