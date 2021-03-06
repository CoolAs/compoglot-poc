#include <stdexcept>
#include <iostream>
#include <cctype>
#include "ParseException.h"
#include "ParserIO.h"


std::string ParserIO::getInput() const
{
	return std::string(input);
}

int ParserIO::getCurPos() const
{
	return curPos;
}

void ParserIO::expect(char character,std::string state)
{
	if (peek(state)!=character)
	{
		throw ParseException(std::string("Unexpected character '") + peek(state) + "', expected '" + character + "', while parsing " + state, input,curPos);
	}
	else
		get(state);
}

char ParserIO::peek(std::string state)
{
	try
	{
		return input.at(curPos);
	}
	catch (const std::out_of_range& oor)
	{
		throw ParseException("Unexpected end of input, while parsing " + state,input,curPos);
	}
}

char ParserIO::get(std::string state)
{
	try
	{
		char c = input.at(curPos);
		curPos += 1;
		return c;
	}
	catch(const std::out_of_range& oor)
	{
		throw ParseException("Unexpected end of input, while parsing " + state,input,curPos);
	}
}

void ParserIO::skipWhitespace(std::string state)
{
	while(std::isspace(peek(state)))
	{
		curPos += 1;
	}
}

std::string ParserIO::getName(std::string state)
{
	skipWhitespace(state);

	std::string name = "";
	while (std::isalnum(peek(state)))
		name += get(state);

	return name;
}

std::string ParserIO::getString(std::string state)
{
	char strchar = peek(state);
	if (peek(state)=='"')
		expect('"',state);
	else
		expect('\'',state);

	std::string retstr = "";
	while(peek(state) != strchar)
		retstr += get(state);

	expect(strchar,state);

	return retstr;
}

int ParserIO::getInt(std::string state)
{
	skipWhitespace(state);

	bool neg = false;
	if (peek(state)=='-')
	{
		neg = true;
		get(state);
	}	

	std::string number = "";
	while (std::isdigit(peek(state)))
		number += get(state);

	if (number=="")
		throw ParseException("Expected integer, while parsing " + state,input,curPos);

	int num = 0;
	for (unsigned int i = 0; i < number.length(); ++i)
	{
		num *= 10;
		num += number.at(i) - '0';
	}

	if (neg) num *= -1;
	return num;
	
}

bool ParserIO::getBool(std::string state)
{
	skipWhitespace(state);

	std::string name = getName(state);

	if (name == "true" || name == "T") return true;
	else if (name == "false" || name == "F") return false;

	throw ParseException(std::string("Expected boolean, got \"") + name + "\", while parsing " + state,input,curPos);
}
