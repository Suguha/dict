#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

int dictCount(char *file)
{
	int count = 0;
	FILE *f = NULL;
	char buf[4096];

	f = fopen(file, "r");
	if (f == NULL) {
		printf("打开文件有误\n");
		exit(1);
	}
	while (fgets(buf, sizeof(buf), f) != NULL)
		count++;
	fclose(f);
	return count / 2;
}

void initDict(char *file, struct WORD *words, int nums)
{
	FILE *f = NULL;
	char buf[4096];

	f = fopen(file, "r");
	if (f == NULL) {
		printf("打开文件有误\n");
		exit(1);
	}
	for (int i = 0; i < nums; i++) {
		fgets(buf, sizeof(buf), f);
		int wordLen = strlen(buf);

		buf[wordLen - 1] = '\0';
		words[i].word = (char*)malloc(wordLen - 1);
		strcpy(words[i].word, buf + 1);
		
		fgets(buf, sizeof(buf), f);
		buf[strlen(buf) - 1] = '\0';
		words[i].numOfTrans = transCount(buf);
		words[i].trans = (char**)malloc(words[i].numOfTrans * sizeof(char*));
		initTrans(words[i], buf);
	}
}

int transCount(char* trans)
{
	int res = 1;
	char *p = trans;
	while (1) {
		p = strchr(p, '@');
		if (p == NULL)
			break;
		while (*(p+1) == '@')
			p++;
		if (*(p+1) == '\0')
			break;
		p++;
		res++;
	}
	return res;
}

void initTrans(struct WORD words, char *buf)
{
	int count = words.numOfTrans;
	char *head = buf+6, *rear = buf;

	for (int i = 0; i < count; i++) {
		rear = strchr(head, '@');
		if (rear == NULL) {
			words.trans[i] = (char*)malloc(strlen(head) + 1);
			strcpy(words.trans[i], head);
			break;
		}
		*rear = '\0';
		int len = strlen(head);
		words.trans[i] = (char*)malloc(len + 1);
		strcpy(words.trans[i], head);
		while (*(rear + 1) == '@')
			rear++;
		head = rear + 1;
	}
}

struct WORD* searchWord(struct WORD* words, char* SStr, int nums)
{
	struct WORD* res = NULL;
	for (int i = 0; i < nums; i++) {
		if (strcmp(SStr, words[i].word) == 0) {
			res = &words[i];
			break;
		}
	}
	return res;
}

void showWord(struct WORD* word)
{
	printf("你要搜的词是：%s\n", word->word);
	printf("翻译有：\n");
	for (int i = 0; i < word->numOfTrans; i++) {
		printf("%d.%s\n", i + 1, word->trans[i]);
	}
}

void freeWord(struct WORD* words, int nums)
{
	for (int i = 0; i < nums; i++) {
		free(words[i].word);
		for (int j = 0; j < words[i].numOfTrans; j++)
			free(words[i].trans[j]);
		free(words[i].trans);
	}
	free(words);
}

void writeBin(struct WORD* words, int nums, char* f)
{
	FILE* data = fopen(f, "wb");
	if (data == NULL) {
		printf("打开文件有误\n");
		exit(1);
	}
	fwrite(&nums, sizeof(int), 1, data);
	for (int i = 0; i < nums; i++) {
		int wordLen = strlen(words[i].word);
		wordLen++;
		fwrite(&wordLen, sizeof(int), 1, data);//word的长度
		fwrite(words[i].word, sizeof(char) * wordLen, 1, data);

		fwrite(&words[i].numOfTrans, sizeof(int), 1, data);//解释的个数

		for (int j = 0; j < words[i].numOfTrans; j++) {//逐个解释
			int transLen = strlen(words[i].trans[j]);
			transLen++;
			fwrite(&transLen, sizeof(int), 1, data);//解释的长度
			fwrite(words[i].trans[j], sizeof(char) * transLen, 1, data);
		}
	}
	//fwrite(words, sizeof(struct WORD), nums, data);
	fclose(data);
}

int readBin(struct WORD** words, char* file)
{
	FILE* data = fopen(file, "rb");
	if (data == NULL) {
		printf("打开文件有误\n");
		exit(1);
	}
	int nums;
	fread(&nums, sizeof(int), 1, data);
	(*words) = (struct WORD*)malloc(sizeof(struct WORD) * nums);

	for (int i = 0; i < nums; i++) {
		//读取word
		int wordLen;
		fread(&wordLen, sizeof(int), 1, data);
		(*words)[i].word = (char*)malloc(wordLen);
		fread((*words)[i].word, sizeof(char) * wordLen, 1, data);
		//读取解释个数
		fread(&(*words)[i].numOfTrans, sizeof(int), 1, data);
		//读取各个解释
		(*words)[i].trans = (char**)malloc(sizeof(char*) * (*words)[i].numOfTrans);
		for (int j = 0; j < (*words)[i].numOfTrans; j++) {
			int transLen;
			fread(&transLen, sizeof(int), 1, data);

			(*words)[i].trans[j] = (char*)malloc(sizeof(char) * transLen);
			fread((*words)[i].trans[j], sizeof(char) * transLen, 1, data);
		}
	}
	fclose(data);
	return nums;
}