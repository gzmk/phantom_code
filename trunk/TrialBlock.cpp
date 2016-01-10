#ifndef BLOCKPAIR__H
	#include "TrialBlock.h"
#endif



TrialBlock::TrialBlock(int blockpairnum, int startind, int endind)
{
	BlockPairNum = blockpairnum;
	Start = startind;
	End = endind;

	int tempint;

	if (End < Start)
	{
		tempint = End;
		End = Start;
		Start = tempint;
	}

	TrialList.clear();
}

TrialBlock::~TrialBlock()
{
}


void TrialBlock::MakeList(std::vector<Trial*> mainlist)
{
	int counter1;

	TrialList.clear();

	if ((int)mainlist.size() > End)
	{
		for (counter1=Start;counter1<=End;counter1++)
		{
			TrialList.push_back(mainlist[counter1]);
		}
	}
}


void TrialBlock::FillList(int newstart, int newend, std::vector<Trial*> &mainlist)
{
	int counter1;
	int counter2 = 0;

	int tempint;

	if (newend < newstart)
	{
		tempint = newend;
		newend = newstart;
		newstart = tempint;
	}

	for (counter1=newstart;counter1<=newend;counter1++)
	{
		if (counter2 < (int)TrialList.size())
		{
			mainlist[counter1] = TrialList[counter2];
		}
		else
			break;

		counter2++;
	}
}


void TrialBlock::ShuffleList()
{
	TrialSetupParams::RandomPermute(TrialList.begin(), TrialList.size());
}
