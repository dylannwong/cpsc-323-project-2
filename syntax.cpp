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
    node* self;
    node* left;
    node* right;

    node(int s, std::string sym, node* lef, node* righ) : state(s), symbol(sym), self(this), left(lef), right(righ) {}
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
    
    //node_new = make_tree(rhs[1], rhs[0],rhs[2])
    
    node* root = new node(state, symbol, left, right);
    tree_nodes.push_back(root);
    return (root);
    
}

node* make_leaf(int state, std::string token, node* address) {
    node* myNodePtr = new node(state, token, address, nullptr);
    tree_nodes.push_back(myNodePtr);

    return myNodePtr;
}

void print_tree(node* root, int depth = 0) {
    if (tree.size() <= depth) {
        tree.resize(depth + 1);
    }

    if(root == nullptr) {
        node* temp = new node(0, "NULL", nullptr, nullptr);
        tree[depth].push_back(temp);
        return;
    }
    
    
    tree[depth].push_back(root);

    int current_depth = depth;

    print_tree(root->left, depth = depth + 1);
    

    depth = current_depth;
    print_tree(root->right, depth = depth + 1);
    

}

void output_tree(node* root) {
    
    print_tree(root);

    int dep = 150;
    for (int i = 0; i < tree.size(); ++i) {

        int spacing = dep / std::pow(2, i + 1);

        std::cout << std::setw(spacing) << "";

        for (int j = 0; j < tree[i].size(); ++j) {
            std::string symb = tree[i][j]->symbol;
            std::string held_symb = tree[i][j]->held_symbol;

            if(symb != "NULL") {
                if(!held_symb.empty()) {
                    if(held_symb == "()") {
                        std::cout << "( " << symb << " )";
                    }else {
                        std::cout << symb << held_symb;

                    }
                } else {
                    std::cout << symb;

                }
            }


            if (j < tree[i].size() - 1) {
                std::cout << std::setw(spacing * 2) << "";
            }
        }

            std::cout << std::endl;
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
    std::cout << newNode << std::endl;
    state_stack.push_back(newNode);
    //state_stack.push_back(std::to_string(state));
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
    /*
    node right_right(0, "right.right", nullptr, nullptr);
    node right_left(0, "right.left", nullptr, nullptr);
    node left_left_left(0, "left.left.left", nullptr, nullptr);
    node left_left(0,"left.left", &left_left_left, nullptr);

    node left(0,"left", &left_left, nullptr);
    node right(0, "right", &right_right, &right_left);

    node root(0, "root", &left, &right);

    output_tree(&root);
    */


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

            //std::cout << std::endl;
            //std::cout << "BEFORE: " << std::endl;

            int reduce_state = located[1] - '0';

            std::vector<node*> rhs;
            std::string reduced_value = (production_rule[reduce_state-1]).first;
            std::string production = ((production_rule[reduce_state-1]).second);
            
            /*std::cout << "reduced state:" << reduce_state << std::endl;
            std::cout << "value " << reduced_value << std::endl;
            std::cout << "production " << production << std::endl;
            */

            int production_length = production.size();

            std::cout << reduced_value << "->" << production;
            
            for(int i = 0; i < production_length; ++i) {
                rhs.insert(rhs.begin(), state_stack.back());
                state_stack.pop_back();
            }
            //std::cout << "NEW STATE" << (&state_stack.back())->state << std::endl;

            std::string located = locate(state_stack.back()->state, reduced_value);
            /*std::cout << std::endl;
            std::cout << "state: " << state << std::endl;
            std::cout << "reduced value : " <<  reduced_value << std::endl;

            std::cout << "LOCATED: " << located << std::endl;
            */
            node* node_new = nullptr;
            if(rhs[0]->symbol == "(" && rhs[2]->symbol == ")" ) {
                std::cout << "PARENTHE";
                node_new = make_leaf(stoi(located), reduced_value, rhs[1]);
                node_new->held_symbol = "()";
                std::cout << std::endl;
                std::cout << "RHS1: " << rhs[0]->symbol <<std::endl;
                std::cout << "RHS2: " << rhs[1]->left <<std::endl;
                std::cout << "RHS3: " << rhs[2]->symbol <<std::endl;


            } else if(production_length > 2) {
                //std::cout << "MADE IT" << std::flush;
                //node_new = make_tree(rhs[1]->symbol, rhs[0], rhs[2]);
                //output_tree(node_new);
                //node root(reduced_value, '+' , rhs[1], rhs[2])

                //node_new = new node(stoi(located), reduced_value, nullptr, nullptr);

                node_new = make_tree(stoi(located), reduced_value, rhs[0], rhs[2]);
                node_new->held_symbol = rhs[1]->symbol;
            } else {
                node_new = make_leaf(stoi(located), reduced_value, rhs[0]);
                //node_new = new node(stoi(located), reduced_value, nullptr, nullptr);
                //tree_nodes.push_back(node_new);

            }

            state_stack.push_back(node_new);
            
            //input.insert(input.begin() + index , reduced_value);

        } else if(located[0] == 'A') {
            std::cout << "TREE NODES: " << std::endl;
            for(int i = 0; i < tree_nodes.size(); ++i) {
                
                std::cout << tree_nodes[i] << std::endl;
                if(!tree_nodes[i]->held_symbol.empty()) {
                    std::cout << tree_nodes[i]->held_symbol << std::endl;
                }
                std::cout << tree_nodes[i]->symbol << std::endl;
                std::cout << tree_nodes[i]->left << std::endl;
                std::cout << tree_nodes[i]->right << std::endl;
            }

            output_tree(tree_nodes[tree_nodes.size() - 1]);
            std::cout << "Accept";
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
