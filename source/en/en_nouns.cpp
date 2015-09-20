#include "../settings.h"
#include "en.h"
#include <string>
#include <iostream>
#include <fstream>

int en::GetNounType(int NounNum)
{
	if (NounNum < 0) return 0;
#ifdef DEBUG
	std::cout << "[EN] GetNounType(int NounNum = " << NounNum << " )" << std::endl;
#endif
	std::ifstream is(DICTIONARY EN_EN_FOLDER "nouns");
	GotoLine(is,NounNum);
	int Data = is.get();
	is.close();
	return Data;
}

std::string en::GetNounString(noun* Noun, bool ObjCase)
{
	if (Noun->id==0) return "";
#ifdef DEBUG
	std::cout << "[EN] GetNounString(noun* Noun)" << std::endl;
#endif
	//Create the final string that will be returned. (Empty for now).
	std::string NounString = "";

	//Create the string that will be inserted before the articles (so that we can correctly turn 'a' into 'an' if this begins with a vowel)
	std::string NounResult;
	
	//Prepend all the adjectives to the NounResult, if the adjectives exist.
	for (int i = 0; i < 16; ++i)
	{
		std::string Adjective = GetAdjective(Noun->adj[i]);
		if (Adjective.compare("")!=0)
			NounResult += Adjective + " ";
	}

	//Prepend the Noun to NounResult.
	NounResult += GetNoun(Noun,ObjCase);

	//Get the string of the article, telling it to turn 'a' into 'an' if NounResult begins with a vowel.
	std::string Article = GetArticle(Noun,IsVowel(NounResult[0]));

	//Get the numberal
	std::string Numeral = GetNumeral(Noun,false);

	//Get the preposition
	std::string Prepos = GetPrepos(Noun->prepos);

	//Start the NounString with the preposition if necessary
	if (Prepos.compare("")!=0)
		NounString+=Prepos + " ";

	//Add the article
	if (Article.compare("")!=0)
		NounString+=Article + " ";

	//Add the Numeral. This is not currently added to noun result, and shouldn't matter as you shouldn't be adding a numeral when you use a/an
	if (Numeral.compare("")!=0)
		NounString+=Numeral + " ";

	//Add the noun result to the noun string (adjectives, and noun)
	NounString+=NounResult;

	//If there is a genitive object, add it.
	if (Noun->usegenitive)
	{
		NounString += " " + GenitiveMarker + " ";
		NounString += GetNounString(Noun->genitivenoun,true);
	}

	//If there is a relative clause, add it.
	if (Noun->useRClause)
	{
		Noun->rClause->clause = true;
		
		//If the relative clause is about a persion, use "who"
		int NounType = GetNounType(Noun->id);
		if (NounType == 'm' || NounType == 'f' || NounType == 'p' || NounType == 'd')
		{
			NounString += " " + RClausePersonalMarker;	
		}
		//Otherwise if the relative clause is essential use "that"
		else if (Noun->rClauseEssential) NounString += " " + RClauseEssentialMarker;
		//Otherwise (non essential) use "which"
		else NounString += " " + RClauseNonEssentialMarker;
		
		//Append the clause
		NounString += " " + Noun->rClause->createSentence();
	}

	//Return the result.
	return NounString;
}

std::string en::GetNoun(noun* Noun,bool ObjCase)
{
#ifdef DEBUG
	std::cout << "[EN] GetNoun(noun* Noun)" <<std::endl;
#endif
	std::ifstream is(DICTIONARY EN_FOLDER "nouns");
	if (GotoLine(is,Noun->id)) return "";
	int Typ = is.get();
	if (Typ == 'd')
	{
		int GetID = 1;
		if (ObjCase && Noun->reflex) GetID = 4;
		else if (ObjCase) GetID = 2;
		if (GotoSegment(is,GetID)) return "";
		return GetSegment(is);
	}
	else if (Typ == 'n' || Typ == 'm' || Typ == 'f')
	{
		if (GotoSegment(is,1)) return "";
		std::string Base = GetSegment(is);
		if (Noun->plural) Base += "s";
		return Base;
	}
	else if (Typ == 'o')
	{
		int GetID = 1;
		if (Noun->plural)
			GetID = 2;
		if (GotoSegment(is,GetID)) return "";
		return GetSegment(is);
	}
	return "";
}

