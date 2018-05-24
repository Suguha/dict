#ifndef _DICT_H_
#define _DICT_H_

struct WORD
{
	char* word;
	int numOfTrans;
	char** trans;
};

int dictCount(char* file);
void initDict(char *file, struct WORD *words, int nums);
int transCount(char* trans);
void initTrans(struct WORD words, char *buf);
struct WORD* searchWord(struct WORD* words, char* SStr, int nums);
void showWord(struct WORD* word);
void freeWord(struct WORD* words, int nums);

void writeBin(struct WORD* words, int nums, char *f);
int readBin(struct WORD** words, char* file);//返回单词的个数
#endif