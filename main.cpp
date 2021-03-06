//
//  main.cpp
//  Test1
//
//  Created by 王岩 on 2018/9/8.
//  Copyright © 2018 王岩. All rights reserved.
//

#include "OS.h"
#include"functions.h"
using namespace std;

//函数声明

//帮助信息

int deleteFile(char spaceName[], char fileName[]);

int changeName(char SpaceName[], char oldName[], char newName[]);

void copyout(char spaceName[],const char*filename, char SName[]);//拉出文件

void help();

void creat(char fileName[]);

void mount(char fileName[]);

int format(char fileName[]);

int close();

void copyin(char spaceName[],const char*filename);//拉入文件

int rm(char *filename);

void exitsys();

void save_inode_array(FILE *fp);

void save_dirUnits(FILE *fp);


void ls();
//全局变量

void save_superblock(FILE *fp);

void save_fat(FILE *fp);

void save_inode_map(FILE *fp);

FILE *p; //外存起始位置
string cmd;
char command[50]; //文件名标识符
char SName[30];
char filename[50];

int main()
{
	char Mini[20];
	printf("Welcome to Mini-FS file system.\nFor help,use command: ""help"".\nOr type in your command directly.\n\n");
	while (1) {
		printf("Mini-FS->");//???
		cin >> cmd;

		if (cmd=="cls") {
			system("cls");
		}
		else if (cmd == "create") {
			printf("\n");
			cin >> Mini;
			creat(Mini);
			printf("Create Mini-FS succeed!\n\n");//???
		}
		else if (cmd == "mount"){
			printf("\n");
			cin >> Mini;
			mount(Mini);
			printf("Mount Mini-FS succeed!\n\n");//???
		}
		else if (cmd == "copyin"){
			printf("\n");
			cin >> SName;
			copyin(Mini,SName);
			printf("Copy file %s in succeed!\n\n",SName);//???
		}
		else if (cmd == "copyout"){
			printf("\n");
			cin >> filename >> SName;//???
			copyout(Mini,filename, SName);
			printf("Copy file %s out succeed!\n\n",SName);
		}
		else if (cmd == "rename") {
			printf("\n");
			char oldName[20];
			char newName[20];
			cin >> oldName >> newName;
			if (changeName(Mini,oldName, newName)==0)
				printf("Rename file %s succeed\n\n", oldName);
			else
				printf("No such file!\n\n");
		}
		else if (cmd == "help") {
			printf("\n");
			help();
			printf("\n");
		}
		else if (cmd == "att") {
			printf("\n");
			char fileName[20];
			cin >> fileName;
			if(Att(fileName)==-1)
				printf("No such file\n\n");
		}
		else if (cmd == "fmt") {
			printf("\n");
			char fileName[20];
			cin >> fileName;
			if(format(fileName)==0)
			printf("Format Mini-FS succeed!\n\n");
		}
		//else if (cmd == "newfile") {
		//	printf("\n");
		//	char fileName[20];
		//	int fileSize;//输入的是文件字节数
		//	printf("Please input new file`s name.\n");
		//	scanf("%s", fileName);
		//	printf("Please input file size.\n");
		//	scanf("%d", fileSize);
		//	newFile(fileName, fileSize / block_size);
		//	printf("\n");
		//}
		else if (cmd == "delete") {
			printf("\n");
			char fileName[20];
			cin >> fileName;
			if (deleteFile(Mini,fileName) == 0)
				printf("Delete file %s succeed!\n\n", fileName);
			else
				printf("No such file!\n\n");
		}
		else if (cmd == "ls") {
			printf("\n");
			if (listFile() == 0) {
				printf("Finished.\n\n");
			}
			else{
				printf("No file in Mini-FS\n\n");
			}
		}
		else if (cmd == "map") {
			printf("\n");
			char fileName[20];
			cin >> fileName;
			if (map(fileName) == 0) {
				printf("Finished.\n\n");
			}
			else if (map(fileName) == 1)
				printf("File is empty!\n\n");
			else
				printf("No such file!\n\n");
		}
		else if (cmd == "close") {
			return 0;
		}
		else {
			cout << cmd;
			printf(":\nillegal command!\n\n");
		}
	}
}
