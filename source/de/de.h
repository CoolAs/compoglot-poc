#include "../settings.h"
#include "../Language.h"
#pragma once
#include <string>
#include <fstream>

class de : public Language 
{
	private:
		//Strings
		static const std::string Und;
		static const std::string Oder;
		static const std::string Articles[14][4][4];
		
		
		//Boilerplate
		bool GotoSegment(std::ifstream& is, int Segment);
    bool GotoLine(std::ifstream& is, int Line);
    std::string GetSegment(std::ifstream& File);
		void ParseVerb(int verb,int n);
		
		//Nouns
		std::string GetNounString(Noun& n);
    std::string GetNoun(Noun& n, int Case);
		int GetNounType(Noun& n);
		
		//Articles
		std::string GetArticle(Noun& n);
		
	public:
    std::string createSentence();
};
