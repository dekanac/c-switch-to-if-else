%option noyywrap
%option nounput
%option noinput

%option yylineno

%{
#include <iostream>
#include <cstdlib>
#include <string> 
#include <vector>

#include "ast.hpp"
#include "parser.tab.hpp"
%}

ID  [a-zA-Z][a-zA-Z0-9_]*

%%
#include         { return include_token; }
void             { return void_token; }
int              { return int_token; }
double           { return double_token; }
char             { return char_token; }
if               { return if_token; }           
else             { return else_token; }
switch           { return switch_token; }
case             { return case_token; }
default          { return default_token; }
for              { return for_token; }
while            { return while_token; }
==               { return eq_token; }

[@<>,+/*();:=!$|'\[\]{}-]      { return *yytext; }
{ID}             { yylval.s = new std::string(yytext); return id_token; }

\<.*\>           { yylval.s = new std::string(yytext); return ppd_token; }
\".*\"           { yylval.s = new std::string(yytext); return string_token; }
[0-9]+           { yylval.i = atoi(yytext); return i_num_token; }
([0-9]+\.[0-9]+) { yylval.d = atof(yytext); return d_num_token; }


\/\/.*           { }//jednolinijski komentar
[ \t\n]          { }
.                { std::cerr << "Lex err: " << yytext << std::endl; }
%%
