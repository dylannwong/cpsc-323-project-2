#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <stack>
#include <utility>
#include <iomanip>
#include <math.h>

struct node {
    int state;
    std::string symbol;
    std::string held_symbol;
    node* left;
    node* right;

    node(int s, std::string sym, node* lef, node* righ) : state(s), symbol(sym), left(lef), right(righ) {}
};
std::vector<std::vector<node*> > tree;

std::vector<node*> tree_nodes; 
int state = 0;
std::vector<node*> state_stack;

std::vector<std::pair<std::string, std::string> > production_rule;

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

node* make_tree(int state, std::string symbol, node* left, node* right) {
        
    node* root = new node(state, symbol, left, right);
    tree_nodes.push_back(root);
    return (root);
    
}

node* make_leaf(int state, std::string token, node* address) {
    node* myNodePtr = new node(state, token, nullptr, address);
    tree_nodes.push_back(myNodePtr);

    return myNodePtr;
}

void print_tree(node* root, std::string route = "", bool left = false) {
    if (root == nullptr) return;
    
    std::cout << route;
    
    if(left) {
        std::cout << "├── ";
    } else {
        std::cout << "└── ";
    }
    
    if(!root->held_symbol.empty()) {
        std::cout << root->symbol << "->" << root->held_symbol << std::endl;
    } else {
        std::cout << root->symbol << std::endl;

    }
     std::string route2;

    if(left) {
        route2 = route + "│   ";
    } else {
        route2 = route + "    ";
    }
    
    if (root->left != nullptr || root->right != nullptr) {
        if (root->left != nullptr) {
            print_tree(root->left, route2, root->right != nullptr);
        }
        
        if (root->right != nullptr) {
            print_tree(root->right, route2, false);
        }
    }
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
    node* newNode = new node(state, ((*input)[index]), nullptr, nullptr);
    state_stack.push_back(newNode);
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

    production_rule.push_back(std::make_pair("E", "E+T"));
    production_rule.push_back(std::make_pair("E", "E-T"));
    production_rule.push_back(std::make_pair("E", "T"));
    production_rule.push_back(std::make_pair("T", "T*F"));
    production_rule.push_back(std::make_pair("T", "T/F"));
    production_rule.push_back(std::make_pair("T", "F"));
    production_rule.push_back(std::make_pair("F", "(E)"));
    production_rule.push_back(std::make_pair("F", "i"));
}


int main (int argc, char* argv[]) {

    std::vector<std::string> input;
    std::vector<std::string>* vectorPtr = &input;

    for (int i = 1; i < argc; ++i){
        input.push_back(argv[i]);
    }

    initializeMap();

    int index = 0;
    node intial_node(0, "$", nullptr, nullptr);
    state_stack.push_back(&intial_node);

    std::cout << std::setw(20) << std::left << "stack"
              << std::setw(20) << std::left << "input"
              << std::setw(10) << std::left << "entry"
              << std::setw(20) << std::left << "production/action"
              << std::endl;

    while(index < input.size()){

        std::string temp_stack = "";
        std::string temp_input = "";
        for(int i = 0; i < state_stack.size(); ++i) {
            temp_stack += (state_stack[i])->symbol;
        }
        for(int i = index; i < input.size(); ++i) {
            temp_input += input[i];
        }

        std::cout << std::setw(20) << std::left << temp_stack
                  << std::setw(20) << std::left << temp_input;

        std::string located = locate((state_stack.back()->state), input[index]);

        if(located[0] == 'S'){
            std::cout << std::setw(10) << std::left << located
                      << std::setw(20) << std::left << "Shift";

            
            shift(located, vectorPtr, index);
            
            index++;


        } else if(located[0] == 'R') {
            std::cout << std::setw(10) << std::left << located
                      << std::setw(2) << std::left << "Reduce, ";

            int reduce_state = located[1] - '0';

            std::vector<node*> rhs;
            std::string reduced_value = (production_rule[reduce_state-1]).first;
            std::string production = ((production_rule[reduce_state-1]).second);

            int production_length = production.size();

            std::cout << reduced_value << "->" << production;
            
            for(int i = 0; i < production_length; ++i) {
                rhs.insert(rhs.begin(), state_stack.back());
                state_stack.pop_back();
            }

            std::string located = locate(state_stack.back()->state, reduced_value);

            node* node_new = nullptr;
            if(rhs[0]->symbol == "(" && rhs[2]->symbol == ")" ) {
                node_new = make_leaf(stoi(located), reduced_value, rhs[1]);
                node_new->held_symbol = "()";

            } else if(production_length > 2) {

                node_new = make_tree(stoi(located), reduced_value, rhs[0], rhs[2]);
                node_new->held_symbol = rhs[1]->symbol;
            } else {
                node_new = make_leaf(stoi(located), reduced_value, rhs[0]);
             
            }

            state_stack.push_back(node_new);
            

        } else if(located[0] == 'A') {


            std::cout << "Accept";
            std::cout << std::endl;
            std::cout << std::endl;

            std::cout << "TREE NODES: " << std::endl;
            print_tree(tree_nodes[tree_nodes.size() - 1]);
            break;
        } else if(located.empty()) {
            std::cout << "Error";
            break;

        } else {
            std::cout << std::setw(10) << std::left << located
                      << std::setw(20) << std::left << "Shift";

            node numNode(stoi(located), ((input)[index]), nullptr, nullptr);
            state_stack.push_back(&numNode);
            index++;

        }

        std::cout << std::endl;
        std::cout << std::endl;

    }

    return 0;
}
