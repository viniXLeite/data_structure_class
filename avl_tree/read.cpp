#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Dictionary {
    string* word;
    string* number_of_synonyms;
    string* synonyms;
};

Dictionary get_dictionary(ifstream &input, unsigned int number_of_words) {
    Dictionary dict;
    unsigned int number_of_synonyms;

    dict.word = new string[number_of_words];
    dict.number_of_synonyms = new string[number_of_words];
    dict.synonyms = new string[number_of_words];

    string read_line;
    unsigned int pos;

    for(unsigned int i = 0; i < number_of_words; i++) {
        getline(input, read_line);

        pos = read_line.find(" ");
        dict.word[i] = read_line.substr(0, pos);

        read_line = read_line.substr(pos+1);
        pos = read_line.find(" ");
        
        dict.number_of_synonyms[i] = read_line.substr(0, pos);
        dict.synonyms[i] = read_line.substr(pos+1);
    }

    return dict;
}

void get_words_search(ifstream &input, string* words_to_search, unsigned int number_of_words_search) {
    for(int i = 0; i < number_of_words_search; i++) {
        getline(input, words_to_search[i]);
    }
}

int main(int argc, char* argv[]) {
    ifstream input(argv[1]);
    ofstream ouput(argv[2]);

    unsigned int number_of_words;
    input >> number_of_words;
    input.ignore();

    Dictionary dict = get_dictionary(input, number_of_words);

    for(unsigned int i = 0; i < number_of_words; i++) {
        ouput << dict.word[i] << ", Synonyms: " << dict.synonyms[i] << endl;
    }
    
    unsigned int number_of_words_search;
    input >> number_of_words_search;
    input.ignore();

    string words_to_search[number_of_words];
    get_words_search(input, words_to_search, number_of_words_search);

    for(int i= 0; i < number_of_words_search; i++) {
        ouput << words_to_search[i] << endl;
    }

    return 0;
}