FLAGS = -g -ansi -pedantic -Wall -Wconversion -m32

PAR_PATH = ./Parser/
TOK_PATH = ./Tokenizer/
ANA_PATH = ./Analyzer/

objects = Analyzer.o Tokenizer.o Parser.o TestParserQQ.o


TestParserQQ: $(objects) 
	g++ $(FLAGS) -o TestParserQQ $(objects) 


TestParserQQ.o: TestParserQQ.cpp $(PAR_PATH)Parser.h $(TOK_PATH)Tokenizer.h 
	g++ $(FLAGS) -c  TestParserQQ.cpp
	
	
Parser.o: $(PAR_PATH)Parser.cpp $(PAR_PATH)Parser.h $(TOK_PATH)Tokenizer.h 
	g++ $(FLAGS) -c  $(PAR_PATH)Parser.cpp


Tokenizer.o: $(TOK_PATH)Tokenizer.cpp $(TOK_PATH)Tokenizer.h 
	g++ $(FLAGS) -c  $(TOK_PATH)Tokenizer.cpp
	
	
Analyzer.o: $(ANA_PATH)Analyzer.cpp $(ANA_PATH)Analyzer.h 
	g++ $(FLAGS) -c  $(ANA_PATH)Analyzer.cpp
	
	
clean:
	rm TestParserQQ $(objects)	
 







