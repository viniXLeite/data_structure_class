#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Dictionary {
    string* word;
    int* number_of_synonyms;
    string* synonyms;
};

Dictionary get_dictionary(ifstream &input, unsigned int number_of_words) {
    Dictionary dict;
    unsigned int number_of_synonyms;

    dict.word = new string[number_of_words];
    dict.number_of_synonyms = new int[number_of_words];
    dict.synonyms = new string[number_of_words];

    string read_line;
    string synonyms_w_wspace;
    string temp_number_of_synonyms;
    unsigned int pos;
    unsigned int temp_pos;

    for(unsigned int i = 0; i < number_of_words; i++) {
        getline(input, read_line);

        // Obter a primeira palavra (antes do primeiro espaço)
        pos = read_line.find(" ");
        dict.word[i] = read_line.substr(0, pos);

        // Remover a primeira palavra e continuar
        read_line = read_line.substr(pos + 1);
        pos = read_line.find(" ");

        // Obter o número de sinônimos
        temp_number_of_synonyms = read_line.substr(0, pos);
        dict.number_of_synonyms[i] = stoi(temp_number_of_synonyms);
        cout << "--" << dict.number_of_synonyms[i] << "--" << endl;

        // Obter a sequência de sinônimos
        synonyms_w_wspace = read_line.substr(pos + 1);

        // Substituir todos os espaços por vírgulas
        for (size_t j = 0; j < synonyms_w_wspace.size(); j++) {
            if (synonyms_w_wspace[j] == ' ') {
                synonyms_w_wspace[j] = ',';
            }
        }

        // Armazenar os sinônimos no dicionário
        dict.synonyms[i] = synonyms_w_wspace;
        cout << dict.synonyms[i] << endl;
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
    ofstream output(argv[2]);

    unsigned int number_of_words;
    input >> number_of_words;
    input.ignore();
    output << number_of_words+12 << endl;

    Dictionary dict = get_dictionary(input, number_of_words);

    for(unsigned int i = 0; i < number_of_words; i++) {
        output << dict.word[i] << ", Synonyms: " << dict.synonyms[i] << endl;
    }
    
    unsigned int number_of_words_search;
    input >> number_of_words_search;
    input.ignore();

    string words_to_search[number_of_words];
    get_words_search(input, words_to_search, number_of_words_search);

    for(int i= 0; i < number_of_words_search; i++) {
        output << words_to_search[i] << endl;
    }

    return 0;
}