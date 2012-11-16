%{
#include <iostream>
using namespace std;
%}
%%
NaN   			{cout << 999999999999; } 
. 			{ cout << yytext ; };
%%
int main( int argc, char *argv[] ) {
	
	// open a file handle to a particular file:

	if(argc != 2)
	{		
		cout << "IL FAUT Un FICHIER" <<endl;
		return 1;
	}

	FILE *myfile = fopen(argv[1], "r");
	// make sure it's valid:
	if (!myfile) {
		cout << "I can't open a.snazzle.file!" << endl;
		return -1;
	}
	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// lex through the input:
	yylex();
}
