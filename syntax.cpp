#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <stack>
#include <utility>
struct node {
    int val;
};
int state = 0;
std::vector<std::string> state_stack;

std::vector<std::pair<std::string, int> > production_rule;

std::unordered_map<std::string, int> stringToNumberMap;

std::string state_table[16][11] = {
    {"S5", "", "", "", "", "S4", "", "", "1","2","3"},
    {"","S6","S7","","","","","Acc","","",""},
    {"","R3","R3","S8","S9","","R3","R3","","",""},
    {"","R6","R6","R6","R6","","R6","R6","","",""},
    {"S5","","","","","S4","","","10","2","3"},
    {"","R8","R8","R8","R8","","R8","R8","","",""},
    {"S5","","","","","S4","","","","11","3"},
    {"S5","","","","","S4","","","","12","3"},
    {"S5","","","","","S4","","","","","13"},
    {"S5","","","","","S4","","","","","14"},
    {"","S6","S7","","","","S15","","","",""},
    {"","R1","R1","S8","S9","","R1","R1","","",""},
    {"","R2","R2","S8","S9","","R2","R2","","",""},
    {"","R4","R4","R4","R4","","R4","R4","","",""},
    {"","R5","R5","R5","R5","","R5","R5","","",""},
    {"","R7","R7","R7","R7","","R7","R7","","",""}

};


std::string locate(int state_locate, std::string symbol) {
    
    return state_table[state_locate][stringToNumberMap[symbol]];
}

node* make_tree() {
    node* myNodePtr = new node();
    return myNodePtr;
}

node* make_leaf() {
    node* myNodePtr = new node();
    myNodePtr->val = 0;
    return myNodePtr;
}

void shift(std::string located, std::vector<std::string>* input, int index){
    if(located.size() < 3) {
        state = located[1] - '0';
    } else {
        std::string table_located = "";
        table_located += located[1];
        table_located += located[2];
        state = std::stoi(table_located);
    }
    
    state_stack.push_back((*input)[index]);
    state_stack.push_back(std::to_string(state));
}

void reduce(){

}

void initializeMap() {
    stringToNumberMap["id"] = 0;
    stringToNumberMap["+"] = 1;
    stringToNumberMap["-"] = 2;
    stringToNumberMap["*"] = 3;
    stringToNumberMap["/"] = 4;
    stringToNumberMap["("] = 5;
    stringToNumberMap[")"] = 6;
    stringToNumberMap["$"] = 7;
    stringToNumberMap["E"] = 8;
    stringToNumberMap["T"] = 9;
    stringToNumberMap["F"] = 10;

    production_rule.push_back(std::make_pair("E", 3));
    production_rule.push_back(std::make_pair("E", 3));
    production_rule.push_back(std::make_pair("E", 1));
    production_rule.push_back(std::make_pair("T", 3));
    production_rule.push_back(std::make_pair("T", 3));
    production_rule.push_back(std::make_pair("T", 1));
    production_rule.push_back(std::make_pair("F", 3));
    production_rule.push_back(std::make_pair("F", 1));
}


int main (int argc, char* argv[]) {
    std::vector<std::string> input;
    std::vector<std::string>* vectorPtr = &input;

    for (int i = 1; i < argc; ++i){
        input.push_back(argv[i]);

    }

    initializeMap();

    int index = 0;
    state_stack.push_back("0");

    while(index < input.size()){

        std::cout << "stack: ";
        for(int i = 0; i < state_stack.size(); ++i) {
            std::cout << state_stack[i];
        }
        std::cout << std::endl;

        std::cout << "index: " << index << std::endl;
        std::cout << "input: " << input[index] << std::endl;
        for(int i = 0; i < input.size(); ++i) {
            std::cout << input[i];
        }
        std::cout << std::endl;
        std::cout << "state: " << state_stack.back() << std::endl;

        std::string located = locate(std::stoi(state_stack.back()), input[index]);
        std::cout << "located: " << located << std::endl;

       
        if(located[0] == 'S'){
            std::cout << "<<<<<<<<<<<Shift>>>>>>>>>>>" << std::endl;
            std::cout << located[1];
            std::cout << input[index];
            std::cout << "size: " << located.size();
            shift(located, vectorPtr, index);
            /*
            if(located.size() < 3) {
                state = located[1] - '0';
            } else {
                std::string table_located = "";
                table_located += located[1];
                table_located += located[2];
                state = std::stoi(table_located);
            }
            
            state_stack.push_back(input[index]);
            state_stack.push_back(std::to_string(state));*/
            index++;


        } else if(located[0] == 'R') {
            std::cout << "<<<<<<<<<<<Reduce>>>>>>>>>>>" << std::endl;
            int reduce_state = located[1] - '0';
            std::string reduced_value = (production_rule[reduce_state-1]).first;
            int production_length = (production_rule[reduce_state-1]).second;
            std::cout << "rule first " << reduced_value << std::endl;
            std::cout << "rule second " << production_length << std::endl;
            
            for(int i = 0; i < production_length; ++i) {
                std::cout << "popped: " << state_stack.back() << std::endl;
                state_stack.pop_back();
                std::cout << "popped: " << state_stack.back() << std::endl;
                state_stack.pop_back();
            }
            input.insert(input.begin() + index , reduced_value);

            /*std::cout << "popped: " << state_stack.back() << std::endl;
            state_stack.pop_back();
            std::cout << "popped: " << state_stack.back() << std::endl;
            state_stack.pop_back();
            input.insert(input.begin()+ index + 1, "F");
            */

        } else if(located[0] == 'A') {
            std::cout << "Accept";
            break;
        } else if(located.empty()) {
            std::cout << "Error";
            index++;

        } else {
            std::cout << "<<<<<<<<<<<<<num shift>>>>>>>>>>>>";
            state_stack.push_back(input[index]);
            state_stack.push_back(located);
            index++;

        }

        std::cout << std::endl;
        std::cout << std::endl;

    }

    //std::cout << locate(11, '$');
    return 0;
}
