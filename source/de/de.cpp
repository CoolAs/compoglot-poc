#include "../settings.h"
#include "../parser/Parser.h"
#include "de.h"
#include <string>
#include <iostream>
#include <fstream>

const std::string de::Und = "und";
const std::string de::Oder = "oder";
const std::string de::QuestionMarkers[6] = {"wer","was","wann","wo","warum","wie"};

std::string de::GetSegment(std::ifstream& File)
{
	std::string Segment = "";
	if (File.good())
		std::getline(File,Segment);
	size_t pos;
	pos = Segment.find("_");
	if (pos != std::string::npos)
		Segment = Segment.substr(0,pos);

	return Segment;
}

bool de::GotoSegment(std::ifstream& File, int Segment)
{
	int ID = 0;
	int C = 0;
	while (ID < Segment)
	{
		if (File.good())
		{
			C = File.get();
			if (C=='_') ++ID;
			else if (C=='\n')
			{
				return true;
			}
		}
		else return true;
	}
	return false;
}

bool de::GotoLine(std::ifstream& File, int Line)
{
	std::string Temp;
	for (int LineNum = 1; LineNum < Line; ++LineNum)
	{
		if (File.good())
			std::getline(File,Temp);
		else return true;
	}
	return false;
}

void de::ParseVerb(int verb,int n)
{
	std::ifstream is(DICTIONARY DE_FOLDER "extverbs.txt");
	if (GotoLine(is,n)) return;
	std::string Line;
	std::getline(is,Line);
	is.close();
	Parser p;
	p.parseExt(Line,this,verb);
 
}

Noun * de::getIt(int id)
{
	Noun * n = new Noun();
	n->ID = id;
	int nType = GetNounType(*n);
	if (nType == 'm')
		n->ID = 3;
	else if (nType == 'f')
		n->ID = 4;
	else if (nType == 'n')
		n->ID = 5;
	return n;
}

std::string de::createSentence()
{
	std::string SentenceString = "";
	
	bool VerbFirst = Data&0x01;
	if (Question > 0) VerbFirst = true;
	if (Question > 1 && Question < 8)
	{
		SentenceString += QuestionMarkers[Question - 2] + " ";
	}
	StartVerb = "";
	EndVerbs = "";
	//First of all, parse the ExtVerb files
	if(ExtVerb1!=0){
		ParseVerb(0,ExtVerb1);
		ExtVerb1=0;
	}
	if (ExtVerb2!=0){
		ParseVerb(1,ExtVerb2);
		ExtVerb2=0;
	}
	std::string VerbString;
	VerbString = GetVerbPresentSimple(0, Verb3);
	if (VerbString.compare("")!=0)
		EndVerbs += VerbString + " ";
	VerbString = GetVerbPresentSimple(0, Verb2);
	if (VerbString.compare("")!=0)
		EndVerbs += VerbString + " ";
	FillVerbs(Subjects[0],Verb1);
	
	std::string SubjectString = "";
	for (int i = 0; i <= NumFilledSubjects; ++i)
	{
		
		if (i!=0 && i < NumFilledSubjects) SubjectString +=", ";
		if (i==NumFilledSubjects && NumFilledSubjects > 0 && SubConjunctionType == 0) SubjectString+= " " + Und + " ";
		else if (i==NumFilledSubjects && NumFilledSubjects > 0 && SubConjunctionType == 1) SubjectString += " " + Oder + " ";
		SubjectString += GetNounString(Subjects[i]);
	}
	
	if (VerbFirst)
		SentenceString += StartVerb + " ";
	
	if (SubjectString.compare("")!=0)
		SentenceString += SubjectString + " ";
	
	if (!IsClause && !VerbFirst && st != BUTTON_TEXT)
		SentenceString += StartVerb + " ";
	
	std::string ObjectsString = "";
	
	for (int objid = 0; objid < 8; ++objid)
	{
		for (int i = 0; i <= NumFilledObjects[objid]; ++i)
		{
				if (i != 0 && i < NumFilledObjects[objid]) ObjectsString += ", ";
				if (i == NumFilledObjects[objid] && NumFilledObjects[objid] > 0 && ObjConjunctionType[objid] == 0) ObjectsString += " " + Und + " ";
				else if (i == NumFilledObjects[objid] && NumFilledObjects[objid] > 0 && ObjConjunctionType[objid] == 1) ObjectsString += " " + Oder + " ";
				ObjectsString += GetNounString(Objects[objid][i]);
		}		
		if (NumFilledObjects[objid] >= 0) ObjectsString += " ";
	}
	
	SentenceString += ObjectsString;
	
	if (st == BUTTON_TEXT)
		SentenceString += StartVerb + " ";
	
	if (Negate)
		SentenceString += "nicht ";
	
	SentenceString += EndVerbs;
	if (IsClause && !VerbFirst)
		SentenceString += " " + StartVerb;
	
	while (SentenceString[SentenceString.length()-1]==' ')
		SentenceString.pop_back();
	
	if (Conjunction && SubClause)
	{
		SentenceString += GetSubClause();
	}
	
	while (SentenceString[SentenceString.length()-1]==' ')
		SentenceString.pop_back();
	
	if	(Punctuation && !IsClause)
	{
		if (Question==0)
			SentenceString+=".";
		else
			SentenceString+="?";
	}
	
	if (Capital && !IsClause)
	{
		char FirstLetter = SentenceString.at(0);
		
		if (FirstLetter >= 'a' && FirstLetter <= 'z')
		{
			FirstLetter -= ('a' - 'A');
			SentenceString[0] = FirstLetter;
		}
		else if (FirstLetter == 0xc3)
		{
			if (SentenceString.at(1)==0xbc) // ü
				SentenceString[1] = 0x9c; // Ü
			else if (SentenceString.at(1) == 0xa4) // ä
				SentenceString[1] = 0x84; // Ä
			else if (SentenceString.at(1) == 0xb6)
				SentenceString[1] = 0x96; // Ö
		}
	}
	
	return SentenceString;

}
