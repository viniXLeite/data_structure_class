#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//struct Stack

struct Servers_info {
    unsigned int number_of_servers;
    unsigned int max_capacity;
    unsigned int number_of_requests;
};

struct Request {
    string str_to_print;
    string checksum_str;
};

struct Server {
    unsigned int number_of_occupied_indexs;
    string available_indexs[32];
};

// formats each string to get the checksum and the print out versions
Request format_string(string str) {
    Request Request;
    unsigned int pos_white_space = str.find(" ");
    str = str.substr(pos_white_space+1);
    Request.str_to_print = str;

    string checksum_str;

    for(char c : str) {
        if( c != ' ') {
            checksum_str += c;
        }
    }

    Request.checksum_str = checksum_str;
    return(Request);
}

// allocates the servers informations, which are the first three lines of the input file, in a Server_info variable
Servers_info get_servers_info(ifstream &input) {
    Servers_info servers_info;
    string first_line;
    getline(input, first_line);

    size_t pos = first_line.find(" ");
    string string_number_of_servers = first_line.substr(0, pos);
    servers_info.number_of_servers = stoi(string_number_of_servers);

    string string_max_capacity = first_line.substr(pos+1);
    servers_info.max_capacity = stoi(string_max_capacity);

    string third_line;
    getline(input, third_line);
    servers_info.number_of_requests = stoi(third_line);

    return(servers_info);
}

// Distribute the read reuests in a Request type vector
void store_resquests_on_vector(ifstream &input, unsigned int number_of_requests, Request* Requests) {

    string str;
    Request rqt;

    for(int i = 0; i <= number_of_requests-1; i++) {
        getline(input, str);
        unsigned int pos = str.find(" ");
        Requests[i] = format_string(str);
    }
}

// function that returns the XOR of each pair of chars in a concatenated string
unsigned int check_sum(string str) {
    unsigned int checksum = 0;
    for( char ch : str) {
        checksum ^= static_cast<unsigned int>(ch);
    }
    return checksum;
}

unsigned int server_index(unsigned int checksum, unsigned int number_of_servers, unsigned int index) {
    return ((7919 * checksum) + index * (104729 * checksum + 123)) % number_of_servers;
}

int main(int argc, char* argv[]) {
    ifstream input(argv[1]);
    ofstream output(argv[2]);

    Servers_info servers_info = get_servers_info(input);
    cout << "Ok server_info" << endl;

    // Creates a Request vector to store 
    Request requests[servers_info.number_of_requests];
    store_resquests_on_vector(input, servers_info.number_of_requests, requests);
    cout << "ok read_file" << endl;

    cout << check_sum("ufs") << endl; 
    cout << server_index(96, 4000, 5);


    Server* servers = new Server[servers_info.number_of_servers];

    /*
    for(int requests_index = 0; requests_index <= number_requests-1; i++)
            int double_hash_index = 0
            while(1) int server_index_var = server_index(checksum, number_of_servers, double_hash_index) if ok servers[server_index_var].available_indexs[index] => allocate esle index++ try again
                if  servers[servers_index].available_indexs[index] full => server_index_var = server_index(checksum, number_of_servers, double_hash_index+1)

    */

    return 0;
}