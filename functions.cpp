#include"functions.h"
#include<math.h>
////文件重命名
//int changeName(char oldName[], char newName[])
//{
//	int i;
//	for (i = 0; i < strlen(newName); i++) {
//		if (newName[i] == '.')
//			break;
//	}
//	//规定文件名不超过8位
//	if (i > 8) {
//		printf("file name too long!\n");
//		return -1;
//	}
//	//扩展名不能超过3位
//	if (strlen(newName) - i - 1>3) {
//		printf("extend name too long!\n");
//		return -1;
//	}
//	for (int i = 0; i < disk->superblock.iNodeNum; i++)
//	{
//		if (strcmp(oldName, disk->dirUnits[i].fileName) == 0)
//		{
//			strcpy(disk->dirUnits[i].fileName, newName);
//			return 0;//0表示更名成功
//		}
//	}
//	return -1;//-1表示文件不存在
//}


//显示帮助信息
void help() {
	printf("Below are some help information:\n\n");
	printf("To clear screen:                   \t\t    cls\n");
	printf("To show command tips information:  \t\t    help\n");
	printf("To list all files in Mini-FS:      \t\t    ls\n");
	printf("To exit Mini-FS and return to operating system:     close\n");
	printf("To create a Mini-FS storeage space:\t\t    create     space_name\n");
	printf("To load Mini-FS	in run-time memory:\t\t    mount      space_name\n");
	printf("To format Mini-FS storage space:   \t\t    fmt        space_name\n");
	printf("To show which blocks are file storaged in           map        file_name\n");
	printf("To delete a file from Mini-FS:     \t\t    delete     file_name\n");
	printf("To display a file`s attribution:   \t\t    att        file_name\n");
	printf("To copy a file from operating system into Mini-FS:  copyin     file_path\n");
	printf("To copy a file from Mini-FS into operating system:  copyout    file_name path_outside\n");
	printf("To rename an existing file in Mini-FS:              rename     file_name new_name\n");
}

//显示空间文件属性
int Att(char filename[]) {
	for (int i = 0; i <data_block_mount; i++) {
		if (strcmp(disk->dirUnits[i].fileName, filename) == 0&&disk->dirUnits[i].inodeNumber!=-1) {
			printf("%s ", filename);
			printf("%d Bytes\n\n", disk->inode_array[disk->dirUnits[i].inodeNumber].fileSize);
			return 0;//0表示成功
		}
	}
	return -1;//-1表示文件不存在
}



////删除文件内容
//int releaseFile(int inodeNum) {
//	iNode* Myinode = &disk->inode_array[inodeNum];
//	int fileBlock[data_block_mount];
//	memset(fileBlock, 0, sizeof(fileBlock));
//	for (int i = 0, j = Myinode->startBlockNum; i < ceil(Myinode->fileSize/block_size); i++, j = disk->FAT[j]) {
//		fileBlock[i] = disk->FAT[j];
//	}
//	int i = 0;
//	while (fileBlock[i] != 0) {
//		disk->FAT[fileBlock[i]] = 0;
//		i++;
//	}
//	Myinode->fileSize = 0;
//	return 0;
//}
//

////删除目录表中文件对应目录项
//int deleteDirUnit(int unitIndex) {
//	int dirAmount = disk->superblock.iNodeNum;
//	for (int i = unitIndex; i < dirAmount; i++) {
//		disk->dirUnits[i] = disk->dirUnits[i + 1];
//	}
//	disk->superblock.iNodeNum--;
//	return 0;
//}

////删除文件
//int deleteFile(char fileName[]) {
//
//	int fileNum = disk->superblock.iNodeNum;
//	int i;
//	int flag = 0;
//	for (i = 0; i<fileNum; i++) {
//		if (strcmp(disk->dirUnits[i].fileName, fileName) == 0) {
//			//找到文件
//			flag = 1;
//			break;
//		}
//	}
//	if (flag == 0) {
//		return -1;
//	}
//	dirUnit myUnit = disk->dirUnits[i];
//	int inodeNum = myUnit.inodeNumber;
//	releaseFile(inodeNum);
//	deleteDirUnit(i);
//}

//检查是否有足够数量的block
int getBlock(int blockNumber) {
	//第一个可用块
	int startBlock = -1;
	int num = 0;
	for (int i = block_mount / 16; i < block_mount; i++) {
		//FAT表并不从0号block开始,前1/16的block作为保留块
		if (disk->FAT[i] == 0 && startBlock == -1)
			startBlock == i;
		//找到一块可用块
		if (disk->FAT[i] == 0) {
			++num;
		}
	}
	if (num >= blockNumber)
		return startBlock;
	else
		return -1;
}


//为文件创建inode
int createInode(int iNodeNum, int fileBlockNum, int fileSize) {
	iNode *currentInode = &disk->inode_array[iNodeNum];
	currentInode->fileSize = fileSize;
	currentInode->startBlockNum = fileBlockNum;
	return 0;
}

////为文件创建目录项
//int addDirUnit(char fileName[], int inodeNum) {
//	int fileNum = disk->superblock.iNodeNum;
//	if (fileNum == inode_count) {
//		printf("Dir is full,please delete some files!\n");
//		return -1;
//	}
//	for (int i = 0; i<fileNum; i++) {
//		if (strcmp(disk->dirUnits[i].fileName, fileName) == 0) {
//			printf("File already exists!\n");
//			return -1;
//		}
//	}
//	dirUnit* newDirUnit = &disk->dirUnits[fileNum];
//	disk->superblock.iNodeNum++;
//	strcpy(disk->dirUnits[fileNum].fileName, fileName);
//	disk->dirUnits[fileNum].inodeNumber = inodeNum;
//	return 0;
//}
//
//
////创建新文件,fileSize是文件需占用盘块数
//int newFile(char fileName[], int fileSize) {
//	int i;
//	for (i = 0; i < strlen(fileName); i++) {
//		if (fileName[i] == '.')
//			break;
//	}
//	//规定文件名不超过8位
//	if (i > 8) {
//		printf("file name too long!\n");
//		return -1;
//	}
//	//扩展名不能超过3位
//	if (strlen(fileName) - i - 1>3) {
//		printf("extend name too long!\n");
//		return -1;
//	}
//
//	int inodeBlock = -1;
//	for (int i = 0; i < inode_count; i++) {
//		if (disk->inode_Map[i] == 0)
//			//找到空闲inode
//			inodeBlock = i;
//	}
//	if (inodeBlock == -1) {
//		printf("No more block could be used!\n");
//		return -1;
//	}
//	//申请数据块
//	int fileBlock = getBlock(fileSize);
//	if (fileBlock == -1) {
//		printf("No enough block could be used!\n");
//		return -1;
//	}
//	//修改目录项
//	createInode(inodeBlock, fileBlock, fileSize);
//	for (int i = fileBlock, j = block_mount / 16, k = 0; j<block_mount&&k<fileSize; i = j, j++, k++) {
//		if (disk->FAT[j] == 0) {
//			disk->FAT[i] = j;
//		}
//	}
//	//修改目录
//	if (addDirUnit(fileName, inodeBlock) == -1)
//		return -1;
//	return 0;
//}

int listFile() {
	int flag = 0;
	for (int i = 0; i < 200000; i++) {
		if (disk->dirUnits[i].inodeNumber != -1) {
			flag++;
			//printf("%d  ", i);
			printf("%s\n", disk->dirUnits[i].fileName);
		}
	}
	if (flag == 0)
		return -1;
	return 0;
}


//打印存储文件的block
int map(char fileName[]) {
	int eachLine = 0;
	for (int i = 0; i < data_block_mount; i++) {
		if (strcmp(fileName, disk->dirUnits[i].fileName) == 0) {
			if (disk->inode_array[disk->dirUnits[i].inodeNumber].fileSize == 0)
				return 1;//1表示文件为空
			printf("%s are storaged in blocks below:\n",fileName);
			for (int j = disk->inode_array[disk->dirUnits[i].inodeNumber].startBlockNum; 
				j!= -1; j = disk->FAT[j]) {
				printf("%d  ",j);
				eachLine++;
				if (eachLine % 5 == 0){
					printf("\n");
					eachLine = 0;
				}
			}
			printf("\n");
			return 0;//0表示文件存在且不为空
		}
	}
	return -1;//-1表示文件不存在
}
