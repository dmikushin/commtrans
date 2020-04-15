CXXFLAGS = -O2 -g -Wall

all: bin/commtrans

comments_translator.o: comments_translator.cpp config.h translator.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

translator.o: translator.cpp config.h translator.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/commtrans: comments_translator.o translator.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -liconv

clean:
	rm -f comments_translator.o translator.o bin/commtrans

