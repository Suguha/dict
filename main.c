#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "dict.h"

int main(int argc, char** argv)
{
	//setlocale(LC_CTYPE, "zh_CN.GBK");
	struct WORD *words, *res;
	int nums = 0;
	char *textOrder = "-text";
	char *binOrder = "-bin";
	char *fileName = "dict.txt";
	char *binFile = "dict.dat";
	char *exitStr = "qqq";

	if (strcmp(argv[1], textOrder) == 0) {
		nums = dictCount(fileName);
		words = (struct WORD*) malloc(nums * sizeof(struct WORD));
		initDict(fileName, words, nums);
	} else if (strcmp(argv[1], binOrder) == 0) {
		if (strcmp(argv[2], "-w") == 0) {
			nums = dictCount(fileName);
			words = (struct WORD*) malloc(nums * sizeof(struct WORD));
			initDict(fileName, words, nums);
			writeBin(words, nums, binFile);
		} else if (strcmp(argv[2], "-r") == 0) {
			nums = readBin(&words, binFile);
		} else {
			printf("指令有误！即将退出\n");
			exit(1);
		}
	} else {
		printf("指令有误！即将退出\n");
		exit(1);
	}

	while (1) {
		printf("Search:");
		//fflush(stdout);
		char SStr[256];
		gets(SStr);
		if (strcmp(SStr, exitStr) == 0) {
			printf("再见\n");
			break;
		}
		res = searchWord(words, SStr, nums);
		if (res == NULL) {
			printf("你要搜的不存在.\n");
			continue;
		}
		showWord(res);
		printf("\n");
	}

	freeWord(words, nums);
	return 1;
}