#include "iostream"
#include "vector"

using namespace std;

struct Node{
    int value;
    Node* next;

    Node(int _value){
        this->value = _value;
        this->next = nullptr;
    }

    Node(){
        this->next = nullptr;
    }
};

class PersistentStack{
private:
    vector<Node*> versions;
    vector<long long> mass;

public:
    PersistentStack(){
        versions.push_back(nullptr);
        mass.push_back(0);
    }

    void push(int _version, int _mass){

      
        Node* newNode = new Node(_mass);
        newNode->next = versions[_version];
        
        versions.push_back(newNode);
        mass.push_back(mass[_version] + _mass);


    }

    void pop(int _version){

        Node* top = versions[_version];
        Node* newNode = top->next;

        versions.push_back(newNode);
        mass.push_back(mass[_version] - top->value);
    }

    long long totalMass(){
        long long totalMass = 0;
        for (int i = 0; i<mass.size(); i++){
            totalMass += mass[i];

        }

        return totalMass;
    }

    void print(){

        int n = versions.size();

        for(int i = 0; i < n; i++){

            Node* temp = versions[i];
            cout << "version " << i << ": ";

            while(temp != nullptr){
                cout << temp->value << " -> ";
                temp = temp->next;
            }

            cout << "NULL";
            cout << " | masa = " << mass[i];
            cout << endl;
        }
    }
};



 int main(){

    int n;
    cin >> n;

    PersistentStack m1;

    for(int i = 1; i <= n; i++){

        int t, m;
        cin >> t >> m;

        if(m == 0){
            m1.pop(t);
        }else{
            m1.push(t, m);
        }
    }

    cout << m1.totalMass() << endl;

    return 0;
}


