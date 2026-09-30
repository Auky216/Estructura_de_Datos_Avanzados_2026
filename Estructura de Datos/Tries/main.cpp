#include <iostream>

using namespace std;

struct Node{
    Node* children[26];
    bool endWord;

    Node(){
        this->endWord = 0;

        for(int i = 0; i < 26; i++){
            children[i] = nullptr;
        }

    }

};


class Tries{
private: 
        Node* root;

public:

    
};

int main(){

}