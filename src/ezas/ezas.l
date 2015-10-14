%{
#include "ezas.tab.cc.h"
%}

%option yylineno

integer [0-9]+
string \".\"
symbol [a-zA-Z_][a-zA-Z_0-9]*
newline [\n\r]
%%

"call" return CALL;
".entry" return ENTRY;
".proc" return PROC;
{string} return STRING;
{integer} return INTEGER;
{symbol} return SYMBOL;
{newline} return NEWLINE;
%%
