/***********************************************************
 *
 * AppState.cpp
 *
 * Project:
 *
 * GlutGLSL
 *
 * Original Author:
 *
 * Matthew Jones, 2008
 *
 * Description:
 *
 * This file declares the variables representing the state
 * of the application.  This is an alternative method of
 * sharing data with classes (instead of using global
 * variables), by providing the class with a pointer to
 * this class.  Classes can also affect the application
 * state by changing these variables, as they are made
 * public.
 *
 ***********************************************************/

#ifndef APPSTATE__H
	#include "AppState.h"
#endif


int AppState::SpecialParticipantNumber = 0;


AppState::AppState()
{
	LeftDown = false;
	FirstFrame = false;
	CancelGravity = false;
	InitX = 0;
	InitY = 0;
	CurrentX = 0;
	CurrentY = 0;

	ParticipantNumber = 0;

#ifndef NO_HAPTICS
	LastPosition = hduVector3Dd(0, 0, 0);
	Anchor = hduVector3Dd(76.5988, 184.559, 23.9805);
#endif

	ScreenWidth = 1024;
	ScreenHeight = 768;

	SliderGravForce = 0.004f;

	ActiveShader = -1;

	CurrentTrial = -1;
	InIntroTrial = false;
	InPracticeTrial = -1;
	Trials.clear();
}


AppState::~AppState()
{
}


// Functions to save and restore state (crash recovery)
bool AppState::SaveState()
{
	int crashinstruction = 0;
	std::ofstream outputfile;

	std::string filename = GenerateFilename(0);
	outputfile.open(filename.c_str());

	if (outputfile.good())
	{
		int counter1;
		float rad2deg = 180.0f / 3.14159265358979323846f;

		for (counter1=0;counter1<(int)Trials.size();counter1++)
		{
		
			if(Trials[counter1]->GetTrialNum() == -1)		// do not write instruction trial caused by crash to file
				{
					crashinstruction++;
				}
		}

		outputfile << "numtrials " << Trials.size()- crashinstruction << "\n";
		outputfile << "currenttrial " << CurrentTrial << "\n";
		outputfile << "introtrial " << InIntroTrial << "\n";
		outputfile << "practicetrial " << InPracticeTrial << "\n";
		outputfile << "------------------------\n";
		outputfile << "\n";

		for (counter1=0;counter1<(int)Trials.size();counter1++)
		{
			if (Trials[counter1])
			{
				if(Trials[counter1]->GetTrialNum() == -1)		// do not write instruction trial caused by crash to file
				{
				}
				else
				{
					outputfile << "trial " << Trials[counter1]->GetTrialNum() << "\n";
					outputfile << "visualpresence " << Trials[counter1]->VisualPresence << "\n";
					outputfile << "hapticpresence " << Trials[counter1]->HapticPresence << "\n";
					outputfile << "environment " << Trials[counter1]->Environment << "\n";
					outputfile << "glossiness " << Trials[counter1]->Glossiness << "\n";
					outputfile << "depth " << Trials[counter1]->Depth << "\n";
					outputfile << "conflict " << Trials[counter1]->Conflict << "\n";
					outputfile << "timer1 " << Trials[counter1]->Timer1 << "\n";
				
					outputfile << "block " << Trials[counter1]->Block << "\n";

					outputfile << "instructtrial " << Trials[counter1]->InstructionTrial << "\n";
					outputfile << "instructtext " << Trials[counter1]->Text << "\n";

					outputfile << "glossresult " << Trials[counter1]->GlossResult << "\n";
					outputfile << "depthresult " << Trials[counter1]->DepthResult << "\n";
				
					outputfile << "------------------------\n";
					outputfile << "\n";
				}
			}
		}

		outputfile.close();
	}

	return false;
}


bool AppState::RestoreState(TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier)
{
	return RestoreState(notifier, stagenotifier, 0);
}

bool AppState::RestoreState(TrialEndNotifier notifier, TrialStageEndNotifier stagenotifier, int state)
{
	int counter1;
	int counter2;
	int trialnum = -1;
	int numtrials = 0;
	std::ifstream inputfile;
	std::string line;
	std::string param1;
	int param2;
	int lastblock = 0;
	std::vector<int> instructioninds;
	
	ClearTrials();

	bool isspecial = false;//(ParticipantNumber == SpecialParticipantNumber);

	if (isspecial)
		state = 2;

	std::string filename = GenerateFilename(state);

	inputfile.open(filename.c_str());

	if (inputfile.is_open())
	{
		while (std::getline(inputfile,line))
		{		
			std::stringstream sstr(line);

			if (sstr >> param1)
			{
				if (param1.compare("numtrials")==0)
				{
					if (sstr >> param2)
					{
						numtrials = param2;

						for (counter1=0;counter1<numtrials;counter1++)
						{
							Trials.push_back(new Trial(counter1, notifier, stagenotifier));
						}
					}
				}
				else if (param1.compare("currenttrial")==0)
				{
					if (state == 2)
						CurrentTrial = 0;
					else
					{
						if (sstr >> param2)
						{
							CurrentTrial = param2;
						}
					}
				}
				else if (param1.compare("introtrial")==0)
				{
					bool intro = false;

					if (sstr >> intro)
					{
						InIntroTrial = intro;
					}
				}
				else if (param1.compare("practicetrial")==0)
				{
					int temptrial = -1;

					if (sstr >> temptrial)
					{
						InPracticeTrial = temptrial;
					}
				}
				else if (param1.compare("trial")==0)
				{
					trialnum++;

					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						int temptrial = trialnum;

						if (sstr >> temptrial)
						{
							Trials[trialnum]->SetTrialNum(temptrial);
						}
					}
				}
				else if (param1.compare("visualpresence")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						bool pres = true; 

						if (sstr >> pres)
						{
							Trials[trialnum]->VisualPresence = pres;
						}
					}
				}
				else if (param1.compare("hapticpresence")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						bool pres = true;

						if (sstr >> pres)
						{
							Trials[trialnum]->HapticPresence = pres;
						}
					}
				}
				else if (param1.compare("environment")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						int env = 0;

						if (sstr >> env)
						{
							Trials[trialnum]->Environment = env;
						}
					}
				}
				else if (param1.compare("glossiness")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						int val = 0;

						if (sstr >> val)
						{
							Trials[trialnum]->Glossiness = val;
						}
					}
				}
				else if (param1.compare("depth")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						int val = 0;

						if (sstr >> val)
						{
							Trials[trialnum]->Depth = val;
						}
					}
				}
				
				else if (param1.compare("conflict")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						int val = 0;

						if (sstr >> val)
						{
							Trials[trialnum]->Conflict = val;
						}
					}
				}
				
				else if (param1.compare("timer1")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						float timer = 0.0f;

						if (sstr >> timer)
						{
							Trials[trialnum]->Timer1 = timer;
						}
					}
				}
				/*else if (param1.compare("block")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						int block = 0;

						if (sstr >> block)
						{
							Trials[trialnum]->Block = block;

							if ((block != lastblock) && (state == 2))
							{
								// Compile vector of trials with old block number
								// then randomly permute them (according to Participant Number)
								// and replace these in the list with the new ordered vector
								// (ONLY DO THIS IF LOADING MAIN FILE)

								std::vector<Trial*> temptriallist;
								temptriallist.clear();

								for (counter1=(trialnum-1);counter1>=0;counter1--)
								{
									if (Trials[counter1])
									{
										if (Trials[counter1]->Block == lastblock)
										{
											temptriallist.push_back(Trials[counter1]);
										}
										else
											break;
									}
								}

								if (temptriallist.size() > 1)
								{
									srand(ParticipantNumber+(lastblock*200));

									TrialSetupParams::RandomPermute(temptriallist.begin(), temptriallist.size());
									
									counter2 = 0;

									for (counter1=(trialnum-1);counter1>=(trialnum-((int)temptriallist.size()));counter1--)
									{
										if (temptriallist[counter2] && Trials[counter1])
											Trials[counter1] = temptriallist[counter2];
										else
											break;

										counter2++;
									}
								}
							}

							lastblock = block;
						}
					}
				}*/
				else if (param1.compare("instructtrial")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						bool instructiontrial = false;

						if (sstr >> instructiontrial)
						{
							Trials[trialnum]->InstructionTrial = instructiontrial;

							if (instructiontrial)
								instructioninds.push_back(trialnum);

							//if (!instructiontrial)
							//	Trials[trialnum]->LoadImages();
						}
					}
				}
				else if (param1.compare("instructtext")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						strcpy_s(Trials[trialnum]->Text,100,line.substr(13).c_str());
					}
				}
				else if (param1.compare("glossresult")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						double result = 0;

						if (sstr >> result)
						{
							Trials[trialnum]->GlossResult = result;
						}
					}
				}
				else if (param1.compare("depthresult")==0)
				{
					if ((trialnum < numtrials) && (trialnum < (int)Trials.size()))
					{
						double result = 0;

						if (sstr >> result)
						{
							Trials[trialnum]->DepthResult = result;
						}
					}
				}

			}
		}

		inputfile.close();

		// Randomly permute within the blocks too
		/*int numblocks = (int)instructioninds.size();
		std::vector<TrialBlock*> blocks;
		TrialBlock* newblock;
		int newstart;
		int newend;

		srand(ParticipantNumber);

		for (counter1=0;counter1<numblocks;counter1++)
		{
			if ((counter1+1) < (int)instructioninds.size())
				newblock = new TrialBlock(counter1, instructioninds[counter1]+1, instructioninds[counter1+1]-1);
			else
				newblock = new TrialBlock(counter1, instructioninds[counter1]+1, ((int)Trials.size())-1);

			newblock->MakeList(Trials);
			newblock->ShuffleList();
			blocks.push_back(newblock);
		}

		//RandomBlockPermute(blockpairs.begin(), blockpairs.size());

		for (counter1=0;counter1<(int)blocks.size();counter1++)
		{
			newstart = instructioninds[counter1]+1;

			if ((counter1+1) < (int)instructioninds.size())
				newend = instructioninds[counter1+1]-1;
			else
				newend = ((int)Trials.size()) - 1;

			blocks[counter1]->FillList(newstart, newend, Trials);
		}

		for (counter1=0;counter1<(int)blocks.size();counter1++)
		{
			delete blocks[counter1];
			blocks[counter1] = NULL;
		}

		blocks.clear();*/

		// Finished permuting blocks

		numtrials = (int) Trials.size();

		if (CurrentTrial >= numtrials)
		{
			CurrentTrial = numtrials - 1;
			if (CurrentTrial < 0)
				CurrentTrial = 0;
		}

		return true;
	}
		/* code remarked to prevent trial generation from lastrun_MAIN.txt file
		   Trial parameter generation now happens by calling methods from TrialMan() class
		   within GlutGLSL - 21/12/15
		 */
	/* else
	{
		if (state == 0)
		{
			return RestoreState(notifier, stagenotifier, 2);
		}
	} */

	return false;
}


bool AppState::FinaliseState()
{
	//remove("lastrun2.txt");
	std::string tempfilename = GenerateFilename(0);

	std::stringstream outfile;
	std::string startstr = "OctaneSinglePotatoRes_";
	std::string condstr = "";
	std::string fileext = ".txt";
	outfile << startstr << ParticipantNumber << condstr << fileext;

	if (FileExists(outfile.str()))
	{
		int counter1 = 0;
		int safetycount = 0;
		int safetylim = 99;
		bool exists = true;
		std::string tempstr = outfile.str();

		int len = tempstr.size();
		
		size_t found = tempstr.rfind(fileext);
		if (found != std::string::npos)
			tempstr.replace(found, fileext.length(),"");

		while (safetycount < safetylim)
		{
			counter1++;
			std::stringstream outfile2;
			outfile2 << tempstr << "_" << counter1 << ".txt";

			if (FileExists(outfile2.str()) == false)
			{
				rename(outfile.str().c_str(),outfile2.str().c_str());
				break;
			}

			safetycount++;
		}
	}

	//rename(tempfilename.c_str(),outfile.str().c_str());
	
	std::ifstream inputfile;
	std::ofstream outputfile;
	std::string line;
	std::string param1;
	char dateStr[9];
    char timeStr[9];
	int trialnum = 0;
	float param2 = 0.0f;
	float param3 = 0.0f;
	float param4 = 0.0f;
	int param5 = 0;
	double param6 = 0.0;

    _strdate_s(dateStr,9);
	_strtime_s(timeStr,9);

	inputfile.open(tempfilename.c_str());
	outputfile.open(outfile.str().c_str());

	if (inputfile.is_open() && outputfile.is_open())
	{
		outputfile << "OctaneSinglePotato Data File " << outfile.str() << "\n";
		outputfile << "Participant Number: " << ParticipantNumber << "\n";
		outputfile << "Date: " << dateStr << ", Time: " << timeStr << "\n";
		outputfile << "=============================\n\n";

		while (std::getline(inputfile,line))
		{
			std::stringstream sstr(line);

			if (sstr >> param1)
			{
				if (param1.compare("trial")==0)
				{
					trialnum++;
					outputfile << "\ntrial " << trialnum;
				}
				else if (param1.compare("hapticpresence")==0)
				{
					if (sstr >> param2)
						outputfile << ", " << param2;
				}
				else if (param1.compare("environment")==0)
				{
					if (sstr >> param5)
						outputfile << ", " << param5;
				}
				else if (param1.compare("glossiness")==0)
				{
					if (sstr >> param5)
						outputfile << ", " << param5;
				}
				else if (param1.compare("depth")==0)
				{
					if (sstr >> param5)
						outputfile << ", " << param5;
				}
				else if (param1.compare("conflict")==0)
				{
					if (sstr >> param5)
						outputfile << ", " << param5;
				}
				else if (param1.compare("timer1")==0)
				{
					if (sstr >> param2)
						outputfile << ", " << param2;
				}
				else if (param1.compare("timer2")==0)
				{
					if (sstr >> param2)
						outputfile << ", " << param2;
				}
				else if (param1.compare("timer3")==0)
				{
					if (sstr >> param2)
						outputfile << ", " << param2;
				}
				else if (param1.compare("glossresult")==0)
				{
					if (sstr >> param6)
						outputfile << ", " << param6;
				}
				else if (param1.compare("depthresult")==0)
				{
					if (sstr >> param6)
						outputfile << ", " << param6;
				}

			}
		}

		outputfile << "\n";
		
		inputfile.close();
		outputfile.close();
	}

	if (FileExists("oldrun.txt"))
		remove("oldrun.txt");

	rename(tempfilename.c_str(),"oldrun.txt");

	return true;
}


bool AppState::SaveExcel()
{
	// Save list of trials in excel format (comma-separated list)
	std::ofstream outputfile;

	std::string filename = GenerateFilename(1);

	outputfile.open(filename.c_str());

	if (outputfile.good())
	{
		int counter1;
		int counter2 = 0;
		float rad2deg = 180.0f / 3.14159265358979323846f;
		std::string sepchars = ", ";

		outputfile << "OctaneSinglePotato trial list\n";
		outputfile << "----------------------\n\n";
		outputfile << "Trial Number | Haptic Presence | Environment | Glossiness | Depth | Conflict | Timer 1 | Timer 2 | Timer 3 | GlossResult | DepthResult\n";
		outputfile << "----------------------\n";

		for (counter1=0;counter1<(int)Trials.size();counter1++)
		{
			if (Trials[counter1])
			{
				if (Trials[counter1]->InstructionTrial == false)
				{
					counter2++;
					outputfile << counter2 << sepchars;
					outputfile << Trials[counter1]->HapticPresence << sepchars;
					outputfile << Trials[counter1]->Environment << sepchars;
					outputfile << Trials[counter1]->Glossiness << sepchars;
					outputfile << Trials[counter1]->Depth << sepchars;
					outputfile << Trials[counter1]->Conflict << sepchars;
					
					outputfile << Trials[counter1]->Timer1 << sepchars;

					outputfile << Trials[counter1]->Block << sepchars;

					outputfile << Trials[counter1]->GlossResult << sepchars;
					outputfile << Trials[counter1]->DepthResult << "\n";
				}
			}
		}

		outputfile << "----------------------\n";
		outputfile << "\n";

		outputfile.close();
	}

	return false;
}


bool AppState::WritePosition()
{
#ifndef NO_HAPTICS
	std::ofstream outputfile;

	std::string filename = "lastcentrepos.txt";

	outputfile.open(filename.c_str());

	if (outputfile.good())
	{
		outputfile << "Last Haptic Centre Position\n";
		outputfile << "----------------------\n\n";
		outputfile << "X: " << LastPosition[0] << "\n";
		outputfile << "Y: " << LastPosition[1] << "\n";
		outputfile << "Z: " << LastPosition[2] << "\n";
		outputfile << "------------------------\n";
		outputfile << "\n";

		outputfile.close();

		return true;
	}
#endif

	return false;
}


void AppState::ClearTrials()
{
	int counter1;

	for (counter1=0;counter1<(int)Trials.size();counter1++)
	{
		if (Trials[counter1])
			delete Trials[counter1];
		
		Trials[counter1] = 0;
	}
	
	Trials.clear();

	FirstFrame = false;
	//CancelGravity = false;
	CurrentTrial = -1;
}


std::string AppState::GenerateFilename(int type)
{
	std::stringstream sstr;

	if (type == 1)
		sstr << "TrialList_p" << ParticipantNumber << ".txt";
	else if (type == 2)
		sstr << "lastrun_MAIN.txt";
	else
		sstr << "lastrun_p" << ParticipantNumber << ".txt";

	return sstr.str();
}


void AppState::TempConverter(double *vec3, char* text, int numchars)
{
	std::stringstream sstr;

	if (vec3)
	{
		sstr << "| " << vec3[0] << " " << vec3[1] << " " << vec3[2] << " |";
	}

	std::string tempstr = sstr.str();
	strcpy_s(text, numchars, tempstr.c_str());
}


bool AppState::FileExists(std::string strFilename)
{ 
  struct stat stFileInfo; 
  bool blnReturn; 
  int intStat; 

  // Attempt to get the file attributes 
  intStat = stat(strFilename.c_str(),&stFileInfo);

  if (intStat == 0)
  { 
	  // We were able to get the file attributes 
	  // so the file obviously exists. 
	  blnReturn = true; 
  }
  else
  { 
	  // We were not able to get the file attributes. 
	  // This may mean that we don't have permission to 
	  // access the folder which contains this file. If you 
	  // need to do that level of checking, lookup the 
	  // return values of stat which will give you 
	  // more details on why stat failed. 
	  blnReturn = false; 
  } 
  
  return blnReturn; 
}


// Public function to randomise order of block pairs
void AppState::RandomBlockPermute(std::vector<TrialBlock*>::iterator it, int numpairs)
{
	int counter1;
	int swapind;
	TrialBlock* tempvars;
	
	for (counter1=0;counter1<numpairs;counter1++)
	{
		// Get index of next element in list
		swapind = rand() % numpairs;

		// Swap elements
		tempvars = *(it+counter1);
		*(it+counter1) = *(it+swapind);
		*(it+swapind) = tempvars;
	}
}
