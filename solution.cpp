#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <thread>

using namespace std;
class solution {

private:

bool DEBUG;
int clck;
vector<string> vect_lines;
vector<int>* t_vars;
vector<int> regVal;

public :

solution(ifstream &file_in,int clck_in = 10 ,bool DEBUG_in = false);
void dbg(const string &msg);
vector<int>* alu();
int mips_clock();

};


vector<int> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<int> tokens;
    while (getline(ss, item, delim)) {
    	tokens.push_back(stoi(item));
    }
    return tokens;
}

vector<string> splitInstValues(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

vector<int> add(string instVal,char delimiter,vector<int> regVal){

    vector<string> addVect = splitInstValues(instVal, delimiter);
    regVal[stoi(addVect[0].substr(1, 1))] =
            regVal[stoi(addVect[1].substr(1, 1))] + regVal[stoi(addVect[2].substr(1, 1))];
    return regVal;
}

vector<int> sub(string instVal,char delimiter,vector<int> regVal){

    vector<string> addVect = splitInstValues(instVal, delimiter);
    regVal[stoi(addVect[0].substr(1, 1))] =
            regVal[stoi(addVect[1].substr(1, 1))] - regVal[stoi(addVect[2].substr(1, 1))];
    return regVal;
}

vector<int> addi(string instVal,char delimiter,vector<int> regVal){

    vector<string> addVect = splitInstValues(instVal, delimiter);
    regVal[stoi(addVect[0].substr(1, 1))] = regVal[stoi(addVect[1].substr(1, 1))] + stoi(addVect[2]);
    return regVal;
}

vector<int> mul(string instVal,char delimiter,vector<int> regVal){

    vector<string> addVect = splitInstValues(instVal, ',');

    regVal[stoi(addVect[0].substr(1, 1))] =
            regVal[stoi(addVect[1].substr(1, 1))] * regVal[stoi(addVect[2].substr(1, 1))];

    return regVal;
}

vector<int> div(string instVal,char delimiter,vector<int> regVal){

    vector<string> addVect = splitInstValues(instVal, ',');

    regVal[stoi(addVect[0].substr(1, 1))] =
            regVal[stoi(addVect[1].substr(1, 1))] / regVal[stoi(addVect[2].substr(1, 1))];
    return regVal;

}

int mapVal(vector<string> vect_lines,int j,unordered_map<string, int> labelName){
    return labelName.at(vect_lines[j].substr(vect_lines[j].find(" ")+1))-4;
}

void printValues(vector<int> regVal){
    for(int k=0;k<regVal.size();k++){
        cout<<regVal[k]<<" ";
    }
    cout<<"\n";
}

int getInstructionNumber(unordered_map<string, int> labelName,vector<string> addVect){
  return labelName.at(addVect[2])-4;
}

string getInstructionType(vector<string> vect_lines,int j){
    return vect_lines[j].substr(0, vect_lines[j].find(" "));
}

string getInstructionValue(vector<string> vect_lines,int j){
    return vect_lines[j].substr(vect_lines[j].find(" ") + 1);
}

solution::solution(ifstream &file, int clk, bool DEBUG_in) {

    clck = clk;

    unordered_map<string, int> labelName;
    string operation;
    string line;
    getline(file, line);
    regVal = split(line, ',');
    cout << line << endl;
    int i = 2;

    if (file.is_open()) {

        while (getline(file, line)) {
            i++;
            vect_lines.push_back(line);
            operation = line.substr(0, line.find(" "));

            if (!operation.substr(0, 5).compare("label")) {

                labelName.insert(pair<string, int>(operation, i));
            }

        }

        file.close();
    }
    cout << "after init" << endl;
    int counter=0;
        for (int j = 0; j < vect_lines.size()-1;) {

            //call clock after every instruction
            if(mips_clock()==1){
                cout << counter << endl;
                counter++;
                string instVal;
                cout << vect_lines[j] << endl;

                string instType=getInstructionType(vect_lines,j);

                if (instType.substr(0, 5).compare("label") != 0) {
                    instVal = getInstructionValue(vect_lines,j);

                } else {
                    string temp = vect_lines[j].substr(vect_lines[j].find(" ") + 1);
                    instType = temp.substr(0, temp.find(" "));
                    instVal = temp.substr(temp.find(" ") + 1);

                }

                vector<string> addVect = splitInstValues(instVal, ',');

                if (instType=="add") {

                    regVal= add(instVal,',',regVal);

                } else if (instType=="sub") {

                    regVal= sub(instVal,',',regVal);

                } else if (instType=="addi") {

                    regVal= addi(instVal,',',regVal);

                } else if (instType=="mul") {

                    regVal= mul(instVal,',',regVal);

                } else if (instType=="div") {

                    regVal= div(instVal,',',regVal);

                }else if (instType=="b") {

                    j=mapVal(vect_lines,j,labelName);

                }else if (instType=="beq") {

                    vector<string> addVect = splitInstValues(instVal, ',');
                    if(regVal[stoi(addVect[0].substr(1, 1))] == regVal[stoi(addVect[1].substr(1, 1))]){
                        j=getInstructionNumber(labelName,addVect);

                    }

                }else if (instType=="bnq") {

                    vector<string> addVect = splitInstValues(instVal, ',');
                    if(regVal[stoi(addVect[0].substr(1, 1))] != regVal[stoi(addVect[1].substr(1, 1))]){
                        j=getInstructionNumber(labelName,addVect);

                    }

                }
                printValues(regVal);
                j++;
            }

        }

    }

vector<int>* solution::alu(){
    vector<int>* val=new vector<int>;

    for(int i=0;i<regVal.size();i++) {
        val->push_back(regVal[i]);
    }
	return val;
}

int solution::mips_clock() {
chrono::milliseconds timespan(clck); 

this_thread::sleep_for(timespan);
static int cycle = 0;
if (cycle == 0 )
	cycle = 1;
else 
	cycle = 0;
return cycle;
}

