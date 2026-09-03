#include "iostream"
#include "vector"
#include "string"

using namespace std;

class PersisentArray{
private:
    vector<int*> versions;
    int n;

public:

    PersisentArray(int* array,int _n){
        
        this->versions.push_back(array);
        this->n = _n;
        
   
    }

    

    void create(int i, int j, int x){
        int* temp = new int[n];
        
        // copia
        for (int k = 0; k < n;k++){
            temp[k] = versions[i-1][k];
        }

        temp[j-1] = x;

        versions.push_back(temp);
    }

    int get(int i,int j){
        int* temp = versions[i-1];
        return temp[j-1];
    }



};

int main(){

    int n;
    int instruction;

    cin >> n;

    int* array = new int[n];

    for(int i = 0; i < n; i++){
        cin >> array[i];
    }

    PersisentArray p1(array, n);

    cin >> instruction;

    for(int i = 0; i < instruction; i++){

        string inst;
        int version;
        int key;
        int value;

        cin >> inst;

        if(inst == "create"){
            cin >> version >> key >> value;

            p1.create(version, key, value);
        }
        else if(inst == "get"){
            cin >> version >> key;

            cout << p1.get(version, key) << endl;
        }
    }

    return 0;
}