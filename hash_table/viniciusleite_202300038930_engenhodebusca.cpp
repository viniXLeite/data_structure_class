#include <iostream>
#include <string>
#include <fstream>

using namespace std;

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
    string *available_indexs;
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

// allocates the servers informations, which are the first three lines of the input file, to a Server_info variable
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

    for(unsigned int i = 0; i <= number_of_requests-1; i++) {
        getline(input, str);
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

unsigned int double_hash(unsigned int checksum, unsigned int number_of_servers, unsigned int index) {
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
    cout << double_hash(96, 4000, 5);

    // Creates an array of server and initializes it with the proper sizes
    Server* servers = new Server[servers_info.number_of_servers];
    for(unsigned int i = 0; i <= servers_info.number_of_servers-1; i++) {
        servers[i].available_indexs = new string[servers_info.max_capacity];
    }

    unsigned int double_hash_index;
    unsigned int current_server;
    unsigned int checksum;
    unsigned int first_server;

    for(unsigned int current_request = 0; current_request < servers_info.number_of_requests; current_request++) {

        double_hash_index = 0;
        checksum = check_sum(requests[current_request].checksum_str);
        first_server = double_hash(checksum, servers_info.number_of_servers, 0);
        
        while (true) {
            current_server = double_hash(checksum, servers_info.number_of_servers, double_hash_index);

            // verifies if the server has available internal spaces
            if (servers[current_server].number_of_occupied_indexs < 32) {
                bool allocated = false;

                // Tries to allocate the current request to some available current_server position
                for (unsigned int i = 0; i < servers_info.max_capacity; i++) {

                    if (servers[current_server].available_indexs[i].empty()) {  // verifies if the current server index is empty

                        if (first_server != current_server) output << "S"<< first_server << "->" << "S" << current_server << endl;

                        servers[current_server].available_indexs[i] = requests[current_request].str_to_print;
                        output << "[S" << current_server << "]" << " "; 
                        
                        // Print out all the requests allocated on each server
                        for(unsigned int log_index = 0; log_index <= servers_info.max_capacity-1; log_index++) {

                            if (servers[current_server].available_indexs[log_index].empty() == false) {
                                if (log_index != servers_info.max_capacity-1 && servers[current_server].available_indexs[log_index+1].empty())
                                    output << servers[current_server].available_indexs[log_index];
                                else
                                    output << servers[current_server].available_indexs[log_index] << ", ";
                            }
                        }

                        output << endl;
                        servers[current_server].number_of_occupied_indexs++;
                        allocated = true;
                        break;
                    }

                }

                // if successfully allocated breaks while(true) loop and passes to the next request
                if (allocated) break;
            }

            double_hash_index++;

            if (double_hash_index >= servers_info.number_of_servers) {
                break;
            }
        }
    }

    return 0;
} 