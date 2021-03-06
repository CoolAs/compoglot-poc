#pragma once
#include <string>
#include "../Language.h"
#include "ParseElement.h"
#include "ParseGroup.h"

class ParseList : public ParseElement
{
protected:
	ParseList(std::string name) : ParseElement(name+" (List)") {}

	virtual ParseGroup* getGroup(Language& lang);

public:
	ParseList() : ParseElement("List") {}

	virtual bool match(std::string name);
	virtual void parse(ParserIO& io, Language& lang);
	
};
