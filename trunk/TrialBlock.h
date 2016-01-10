#ifndef BLOCKPAIR__H
#define BLOCKPAIR__H


#include <vector>
#include <fstream>

#include "TrialMan.h"


class TrialBlock
{
public:
	TrialBlock(int blockpairnum, int startind, int endind);

	~TrialBlock();

public:

	void MakeList(std::vector<Trial*> mainlist);

	void FillList(int newstart, int newend, std::vector<Trial*> &mainlist);

	void ShuffleList();

public:

	int BlockPairNum;
	int Start;
	int End;
	std::vector<Trial*> TrialList;
};


#endif
