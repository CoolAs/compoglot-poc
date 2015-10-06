#pragma once
#include <string>



#ifdef SUPER_DEBUG
#define DEBUG
#endif

#define DICTIONARY "../lang/"
#define EN_FOLDER "en/"
#define EN_EN_FOLDER "en/"
#define DE_FOLDER "de/"
#define EO_FOLDER "eo/"

#define BUFFER_SIZE 256
#define NUM_BUF 256
#define NUM_ADVERBS 5
#define EN_NEGATIVE "negative"
#define EN_EN_DONOT "don't "
#define EN_EN_DOESNOT "doesn't "
#define EN_EN_ANDNOUN "and "
#define EN_EN_ORNOUN "or "
#define EN_EN_OF "of "
#define EN_EN_WHO "who "
#define EN_EN_THAT "that "
#define EN_EN_WHICH ", which "
#define EN_EN_FUTURE "will "
#define DE_UNDNOMEN "und "
#define DE_ODERNOMEN "oder "
#define DE_NEGATIVE "minus "
#define DE_AUTOCHANGESIMPLE 1

void warn(const char * message, const char * fallback);
int nDigits(int i);
#include "Language.h"
Language* getLanguageFromStringId(char* Id);
Language* getLanguageFromStringID(std::string ID);
void printNoun(std::string name, Noun* n);


#define DE_CONTRACTPREPOSITIONS 1
