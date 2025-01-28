#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <cctype>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <windows.h>
using namespace std;
 
 int matchCommand(string command, int n, int i);
struct com {
    string command;
    int index;
};
vector<com> commands; //structure of storing the instruction and ins num
 
int positions[] = {25, 40, 55}; // Define positions (left, center, right)

vector<int> in; // 3 arrays for inbox outbox mid
vector<int> out;
vector<int> mid;
vector<int> inTemp; //duplicate of in to compare to out
int level; // level chosen by the player
int picked = 999; // number on the top of the robot
int errorStep = 99;
int correctLevelChosen;
int numFCommands; //command number for files
ifstream fileinput;
ifstream gamestatus;
ofstream statusupdate;
int levelsPassed;
 
//boolean showing whether the current level was completed or not
bool levelOnePassed ;
bool levelTwoPassed;
bool levelThreePassed ;
bool levelFourPassed ;
 
int levelChoose() {
    int picked = 999; // number on the top of the robot
    int errorStep = 99;
    bool levelOnePassed = false;
    bool levelTwoPassed = false;
    bool levelThreePassed = false;
    bool levelFourPassed = false;
    
    gamestatus.open("datasave.txt");
    gamestatus >> levelsPassed;
    int n = levelsPassed ;
    errorStep = 99;
    // Level selection
    cout << "                                        Welcome to Human Resource Machine                                                      "<< endl;
    cout <<"请选择以下关卡中您想要玩的关卡" << endl;
    cout<< "            LEVEL 1"; if (n< 1){ cout << "  (未通关，可选择)";}else{cout << "  (已通关,可选择)";} cout << endl;
    cout<< "            LEVEL 2"; if (n< 1){ cout << "  (上未通关，不可选择)";}else{if(n == 1)cout << "  (上关已通关，可选择)";else cout << "  (已通关,可选择)";} cout << endl;
    cout<< "            LEVEL 3"; if (n< 2){ cout << "  (上未通关，不可选择)";}else{if(n == 2)cout << "  (上关已通关，可选择)";else cout << "  (已通关,可选择)";} cout << endl;
    cout<< "            LEVEL 4"; if (n< 3){ cout << "  (上未通关，不可选择)";}else{if(n == 3)cout << "  (上关已通关，可选择)";else cout << "  (已通关,可选择)";} cout << endl;

    cin >> level;
    gamestatus.close();
    return level;
}
 


void Levelins(int n) {
    int x = 35; // Center alignment (top-middle)
    int y = 2;  // Top of the terminal
    if(n == 1){
        cout << "\033[" << y << ";" << x << "H" << "This is Level One" << endl;
        cout << "\033[" << y + 2 << ";" << x - 5 << "H" << "输入队列中有两个积木" << endl;
        cout << "\033[" << y + 3 << ";" << x - 5 << "H" << "按照顺序把积木放到输出序列" << endl;
        cout << "\033[" << y + 4 << ";" << x - 5 << "H" << "在本关卡中您的可用空地数为0" << endl;
        cout << "\033[" << y + 6 << ";" << x - 5 << "H" << "可用指令为 inbox outbox" << endl;
    }
    if(n == 2){
        cout << "\033[" << y << ";" << x << "H" << "This is Level Two"<< endl;
        cout << "\033[" << y + 2 << ";" << x - 5 << "H" << "输入队列中有8个积木" << endl;
        cout << "\033[" << y + 3 << ";" << x - 5 << "H" << "对于输入序列中的每两个东西，先把第 1 个减去第 2 个" << endl;
        cout << "\033[" << y + 4 << ";" << x - 5 << "H" << "并把结果放在输出序列中，然后把第2个减去第1个,再把结果放在输出序列中" << endl;
        cout << "\033[" << y + 5 << ";" << x - 5 << "H" << "在本关卡中您的可用空地数为3" << endl;
        cout << "\033[" << y + 7 << ";" << x - 5 << "H" << "可用指令为 inbox,outbox,copyfrom,copyto,add,sub,jump,jumpifzero" << endl;
    }
    if(n == 3){
        cout << "\033[" << y << ";" << x << "H" << "This is Level Three"<< endl;
        cout << "\033[" << y + 2 << ";" << x - 5 << "H" << "输入队列中有8个积木" << endl;
        cout << "\033[" << y + 3 << ";" << x - 5 << "H" << "从输入序列中依次取 2 个数字，如果相等则将其中一个输出，否则扔掉" << endl;
        cout << "\033[" << y + 4 << ";" << x - 5 << "H" << "在本关卡中您的可用空地数为3" << endl;
        cout << "\033[" << y + 6 << ";" << x - 5 << "H" << "可用指令为 inbox,outbox,copyfrom,copyto,add,sub,jump,jumpifzero" << endl;
    
    }
    if(n == 4){
        cout << "\033[" << y << ";" << x << "H" << "This is Level Four"<< endl;
        cout << "\033[" << y + 2 << ";" << x - 5 << "H" << "输入队列中有8个积木" << endl;
        cout << "\033[" << y + 3 << ";" << x - 5 << "H" << "从输入序列中输出每个积木的翻倍" << endl;
        cout << "\033[" << y + 4 << ";" << x - 5 << "H" << "在本关卡中您的可用空地数为3" << endl;
        cout << "\033[" << y + 6 << ";" << x - 5 << "H" << "可用指令为 inbox,outbox,copyfrom,copyto,add,sub,jump,jumpifzero" << endl;
    
    }
}


int checkLevel(int level) {
// checking for valid input
    while (true) {
        if (cin.fail() || level < 1 || level > levelsPassed+1) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "无效的输入，请从新输入想玩的关卡" << endl;
            cin >> level; // You need to get the input again after clearing the error
        } else {
            return level;
        }
    }
}

void commandprint(int n){ //running commands printout
    int x = 80; // Center alignment (top-middle)
    int y = 2;  // Top of the terminal
    cout << "\033[" << y<< ";" << x << "H" << "        --------CODES---------"<< endl;
    for(int p = 0; p <= n; p++){
        cout << "\033[" << y+p+1 << ";" << x << "H" << "                        "<<commands[p].command<< endl;
    }
}

void printRobotAtPosition(int position) {
    // Clear the screen
    cout << "\033[2J\033[H";

    // Draw the robot at the specified position
    cout << "\033[9;" << position << "H+---+";           // Top box
    cout << "\033[11;" << position << "H| "<<(picked == 999 ? "X" : to_string(picked))<<" |";
    cout << "\033[13;" << position << "H+---+";

    cout << "\033[14;" << position << "H  @ @";          // Head
    cout << "\033[15;" << position << "H @   @";         // Eyes
    cout << "\033[16;" << position << "H   +";          // Body
    cout << "\033[17;" << position << "H  / \\";
    cout << "\033[18;" << position << "H /   \\";
}

void printSides24(){
    int x = 10; // Fixed x position for the structure
    int y = 0;  // Fixed y position for the structure
    

    cout << "\033[" << y << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 2 << ";" << x << "H" << "   |  "<<(in[0] == 999 ? "X" : to_string(in[0]))<<"  |                                                  |  "<<(out[0] == 999 ? "X" : to_string(out[0]))<<"  |" << endl;
    cout << "\033[" << y + 4 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 6 << ";" << x << "H" << "   |  "<<(in[1] == 999 ? "X" : to_string(in[1]))<<"  |                                                  |  "<<(out[1] == 999 ? "X" : to_string(out[1]))<<"  |" << endl;
    cout << "\033[" << y + 8 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 10 << ";" << x << "H" << "   |  "<<(in[2] == 999 ? "X" : to_string(in[2]))<<"  |                                                  |  "<<(out[2] == 999 ? "X" : to_string(out[2]))<<"  |" << endl;
    cout << "\033[" << y + 12<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 14 << ";" << x << "H" << "   |  "<<(in[3] == 999 ? "X" : to_string(in[3]))<<" |                                                  |  "<<(out[3] == 999 ? "X" : to_string(out[3]))<<"  |" << endl;
    cout << "\033[" << y + 16<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 18 << ";" << x << "H" << "   | "<<(in[4] == 999 ? "X" : to_string(in[4]))<<"   |                                                  |  "<<(out[4] == 999 ? "X" : to_string(out[4]))<<"  |" << endl;
    cout << "\033[" << y + 20<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 22<< ";" << x << "H" << "   |  "<<(in[5] == 999 ? "X" : to_string(in[5]))<<" |                  +----+----+----+                |  "<<(out[5] == 999 ? "X" : to_string(out[5]))<<"  |" << endl;
    cout << "\033[" << y + 24<< ";" << x << "H" << "   +-----+                  | "<<(mid[0] == 999 ? "X" : to_string(mid[0]))<<"  | "<<(mid[1] == 999 ? "X" : to_string(mid[1]))<<"  | "<<(mid[2] == 999 ? "X" : to_string(mid[2]))<<" |                +-----+" << endl;
    cout << "\033[" << y + 26<< ";" << x << "H" << "   |  "<<(in[6] == 999 ? "X" : to_string(in[6]))<<"  |                  +---+---+-----+                 |  "<<(out[6] == 999 ? "X" : to_string(out[6]))<<"  |" << endl;
    cout << "\033[" << y + 28 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 30<< ";" << x << "H" << "   |  "<<(in[7] == 999 ? "X" : to_string(in[7]))<<"  |                                                  |  "<<(out[7] == 999 ? "X" : to_string(out[7]))<<"  |" << endl;
    cout << "\033[" << y + 32<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
}
void printSides1(){
    int x = 10; // Fixed x position for the structure
    int y = 0;  // Fixed y position for the structure

    cout << "\033[" << y << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 2 << ";" << x << "H" << "   |  "<<(in[0] == 999 ? "X" : to_string(in[0]))<<"  |                                                  |  "<<(out[0] == 999 ? "X" : to_string(out[0]))<<"  |" << endl;
    cout << "\033[" << y + 4 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 6 << ";" << x << "H" << "   |  "<<(in[1] == 999 ? "X" : to_string(in[1]))<<"  |                                                  |  "<<(out[1] == 999 ? "X" : to_string(out[1]))<<"  |" << endl;
    cout << "\033[" << y + 8 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 10 << ";" << x << "H" << "   |  "<<'X'<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 12<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 14 << ";" << x << "H" << "   |  "<<'X'<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 16<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 18 << ";" << x << "H" << "   |  "<<'X'<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 20<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 22<< ";" << x << "H" << "   |  "<<'X'<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 24<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 26<< ";" << x << "H" << "   |  "<<'X'<<"  |                                                   |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 28 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 30<< ";" << x << "H" << "   |  "<<'X'<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 32<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
}
 
 void printSides3(){
 int x = 10; // Fixed x position for the structure
    int y = 0;  // Fixed y position for the structure
    

    cout << "\033[" << y << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 2 << ";" << x << "H" << "   |  "<<(in[0] == 999 ? "X" : to_string(in[0]))<<"  |                                                  |  "<<(out[0] == 999 ? "X" : to_string(out[0]))<<"  |" << endl;
    cout << "\033[" << y + 4 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 6 << ";" << x << "H" << "   |  "<<(in[1] == 999 ? "X" : to_string(in[1]))<<"  |                                                  |  "<<(out[1] == 999 ? "X" : to_string(out[1]))<<"  |" << endl;
    cout << "\033[" << y + 8 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 10 << ";" << x << "H" << "   |  "<<(in[2] == 999 ? "X" : to_string(in[2]))<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 12<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 14 << ";" << x << "H" << "   |  "<<(in[3] == 999 ? "X" : to_string(in[3]))<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 16<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 18 << ";" << x << "H" << "   |  "<<(in[4] == 999 ? "X" : to_string(in[4]))<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 20<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 22<< ";" << x << "H" << "   |  "<<(in[5] == 999 ? "X" : to_string(in[5]))<<"  |                  +----+----+----+                 |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 24<< ";" << x << "H" << "   +-----+                  | "<<(mid[0] == 999 ? "X" : to_string(mid[0]))<<"  | "<<(mid[1] == 999 ? "X" : to_string(mid[1]))<<"  | "<<(mid[2] == 999 ? "X" : to_string(mid[2]))<<" |                 +-----+" << endl;
    cout << "\033[" << y + 26<< ";" << x << "H" << "   |  "<<(in[6] == 999 ? "X" : to_string(in[6]))<<"  |                  +---+---+---+-+                 |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 28 << ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
    cout << "\033[" << y + 30<< ";" << x << "H" << "   |  "<<(in[7] == 999 ? "X" : to_string(in[7]))<<"  |                                                  |  "<<'X'<<"  |" << endl;
    cout << "\033[" << y + 32<< ";" << x << "H" << "   +-----+                                                  +-----+" << endl;
}
 
bool passedOrNo(){
    for (int i = 0 ; i < in.size(); i++){
        if (in[i] != 999) return false;
    }
    //if all the commands not processed return false
    return true;
}
 
 
void shiftAndSetXin( int size) {
    // Check if the array is empty or has only one element
    if (size <= 0) {
        return; // No operation needed
    }
 
    // Shift elements to the left
    for (int i = 0; i < size - 1; ++i) {
        in[i] = in[i + 1];
    }
    in[size - 1] = 999;
}
 
void shiftAndSetXout( int size) {
    if (size <= 0) {
        return; // No operation needed
    }
 
    // Shift elements to the right
    for (int i = size - 1; i > 0; --i) {
        out[i] = out[i - 1];
    }
}
 
 
bool checkInEmpty(int insnum){ // true if it is empty and come out from the program
    int count = 0;
    int outcount = 0;
    for(int i = 0; i < in.size(); i++){
        if (in[i] != 999) count++;
    }
    /*
    for(int i = 0; i < out.size(); i++){
        if (out[i] == 999) outcount++;
    }
    */
    if ((count == 0|| insnum == commands.size())&& picked == 999  ) {
        return true;
    }
    else{
        return false;
    }
}
 
 
void inbox(){ // end when inboxing and the array is empty
    picked = in[0];
    shiftAndSetXin(in.size());
    //outputrobot(); // then need to circulate the things in in array
 
}
 
 
void outbox(int i){ // x is the instruction number
    if (picked != 999){
        shiftAndSetXout( out.size());
        out[0] = picked;
        picked = 999;
        //outputrobot();
    }
    else {
        errorStep = min(errorStep,i);
        //cout << "Error on instruction " << i << endl;
    }
}
 
 
void add(int n, int i) {
    if (picked != 999 && mid[n]!=999 && n<mid.size()-1 && n >= 0){
        picked+=mid[n];      
        //outputrobot();
    }
    else {
        errorStep = min(errorStep, i);
        //cout << "Error on instruction " << step << endl;
    } 
}
 
void sub(int n, int i) {
    if (picked != 999 && mid[n] != 999 && n < mid.size()-1 && n >= 0){
        picked-=mid[n];   
        //outputrobot();   
    }
    else {
        errorStep= min(errorStep, i);
        //cout << "Error on instruction " << step << endl;
    } 
}
 
 
void copyTo(int n, int i) {
    if (picked != 999 && n < mid.size()-1 && n >=0){
        mid[n] = picked; 
        //outputrobot();
    }
    else {
        errorStep = min(errorStep, i);
        //cout << "Error on instruction " << step << endl;
    } 
}
 
void copyFrom(int n,  int i) {
    if (  mid[n]!=999 && n < mid.size() && n >= 0){
        picked = mid[n];
        //outputrobot();
    }
    else {
        errorStep = min(errorStep, i);
        //cout << "Error on instruction " << step << endl;
    } 
}
 
/*
Change jump and jumpifzero methods when end > start
 
*/
 
 
int jump(int end, int index) { //end = user input, index = index of jump
    int i = end;
 
    if(end>=commands.size() || end < 0) {
        errorStep = min(index, errorStep);
        //cout << "Error on instruction " << end << endl;
        return -1;
    }
    if(end==index) {
        return 99;
    }
    //cout << "hi: " << ((end-index)*100)/100;
    return (end-index)*100;
    /*
    else {
        if(end < index){ //if jump goes up
            while(true) {
                int x = matchCommand(commands[i].command, commands[i].index, i); // calling out each and every previous calls
                if(x==-1) {
                    break;
                }
                if(i == index) {
                    i = end;
                }
                i++;
            }
            return 0;
        }else{ //if jump goes down
            return index-end;
        }
    }
    */
 
}
 
 
int  jumpifzero(int end, int index){ //end = user input, index = index of jump
    int i = end;
 
    if(end>=commands.size() || end < 0 || picked == 999) {
        errorStep = min(index, errorStep);
        //cout << "Error on instruction " << end << endl;
        return index;
    }
    if(picked==0) {
        if(end==index) {
            return 99;
        }
        //cout << "hi: " << ((end-index)*100)/100;
        return (end-index)*100;
    }
    return 99;

    /*
    else {
        if(picked == 0){
            if(end < index){
                while(true) {
                    int x = matchCommand(commands[i].command, commands[i].index, i); // calling out each and every previous calls
 
                    if(x==-1) {
                        break;
                    }
                    if(i == index) {
                        i = end;
                    }
                    i++;
                }
                return -1;
            }else{
                return end-1;
            }
        }
        else{
            return index +1;
        }
    }
    */
}
 
 
 
void levelOne() {
    mid.resize(0);
    reverse(out.begin(), out.end());
    int check[] = {1, 2};
    for(int i = 0; i < out.size(); i++){
        if (out[i] != check[i]){
            levelOnePassed = false;
            return;
        }
    }
    if(levelsPassed < 1)
        levelsPassed = 1;
    levelOnePassed =  true;
    return;
}
 
void levelTwo() {
    mid.resize(3);
    /*for (int i = 0; i < inTemp.size()-1; i+=2) {
        if (out[i] != inTemp[i]-inTemp[i+1]) {
            levelTwoPassed = false;
            return;
        }
        else if(out[i+1] != inTemp[i+1]-inTemp[i]){ 
            levelTwoPassed =  false;
            return;
        }
    }*/
    reverse(out.begin(), out.end());
    int check[] = {-6, 6, 4, -4, 0, 0, 18, -18};
    for(int i = 0; i < out.size(); i++){
        if (out[i] != check[i]){
            levelTwoPassed = false;
            return;
        }
    }
    if(levelsPassed < 2)
        levelsPassed = 2;
    levelTwoPassed = true;
    
}
 
void levelThree() {
    mid.resize(3);
    reverse(out.begin(), out.end());
    int check[] = {7, -3};
    for(int i = 0; i < out.size(); i++){
        if (out[i] != check[i]){
            levelThreePassed = false;
            return;
        }
    }
    
    if(levelsPassed < 3)
        levelsPassed = 3;
    levelThreePassed= true;
    return;
}
 
void levelFour() {
    mid.resize(3);
    reverse(out.begin(), out.end());
    int check[] = {-6, -4, -2, 0, 2, 4, 6, 8};
    for(int i = 0; i < out.size(); i++){
        if (out[i] != check[i]){
            levelFourPassed = false;
            return;
        }
    }
    
    if(levelsPassed < 4)
        levelsPassed = 4;
    levelFourPassed = true;
    return;
}
 
 
int matchCommand(string str, int n, int i) { //str = commands[i].command, n = commands[i].index, i = instruction number
    if (correctLevelChosen == 2 || correctLevelChosen == 4){
        if(str == "inbox") {
            if (in[0] != 999) {
                inbox();
                printRobotAtPosition(positions[0]);
                printSides24();
                commandprint(i);
                Sleep(800);
                system("cls");
                return i;
            }
            else{
                return -1;
            }
        }
        if(str == "outbox") {
            outbox(i);
            printRobotAtPosition(positions[2]);
            printSides24();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "add") {
            add(n, i);
            printRobotAtPosition(positions[1]);
            printSides24();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "sub") {
            sub(n, i);
            printRobotAtPosition(positions[1]);
            printSides24();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "copyto") {
            copyTo(n, i);
            printRobotAtPosition(positions[1]);
            printSides24();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "copyfrom") {
            copyFrom(n, i);
            printRobotAtPosition(positions[1]);
            printSides24();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "jump") {
            printSides24();
            commandprint(i);
            Sleep(200);
            system("cls");
            return jump(n-1, i);
        }
        if(str == "jumpifzero") {
            printSides24();
            commandprint(i);
            Sleep(200);
            system("cls");
            return jumpifzero(n-1, i);
        }
        errorStep = i;
        return -1;
    }
    else{
        if(correctLevelChosen == 1){
            if(str == "inbox") {
                if (in[0] != 999) {
                    inbox();
                    printRobotAtPosition(positions[0]);
                    printSides1();
                    commandprint(i);
                    Sleep(800);
                    system("cls");
                    return i;
                }
                else{
                    return -1;
                }
            }
            if(str == "outbox") {
                outbox(i);
                printRobotAtPosition(positions[2]);
                printSides1();
                commandprint(i);
                Sleep(800);
                system("cls");
                return i;
            }
            errorStep = i;
            return -1;
        }
        else{
            if(str == "inbox") {
            if (in[0] != 999) {
                inbox();
                printRobotAtPosition(positions[0]);
                printSides3();
                commandprint(i);
                Sleep(800);
                system("cls");
                return i;
            }
            else{
                return -1;
            }
        }
        if(str == "outbox") {
            outbox(i);
            printRobotAtPosition(positions[2]);
            printSides3();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "add") {
            add(n, i);
            printRobotAtPosition(positions[1]);
            printSides3();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "sub") {
            sub(n, i);
            printRobotAtPosition(positions[1]);
            printSides3();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "copyto") {
            copyTo(n, i);
            printRobotAtPosition(positions[1]);
            printSides3();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "copyfrom") {
            copyFrom(n, i);
            printRobotAtPosition(positions[1]);
            printSides3();
            commandprint(i);
            Sleep(800);
            system("cls");
            return i;
        }
        if(str == "jump") {
            printSides3();
            commandprint(i);
            Sleep(200);
            system("cls");
            return jump(n-1, i);
        }
        if(str == "jumpifzero") {
            printSides3();
            commandprint(i);
            Sleep(200);
            system("cls");
            return jumpifzero(n-1, i);
        }
        errorStep = i;
        return -1;
        }
    }
    errorStep = i;
    return -1;

    
}
 
void inputCommandsManual(int numCommands) {
    commands.resize(numCommands);
    for (int i = 0; i < numCommands; i++){
        string input;
        cin >> input;
        commands[i].command = input;
        int ind; //used to change coms.index
        if(input == "inbox" || input =="outbox") { //for seperating the ones followed by a num and not followed
            ind = -1;
        }
        else {
            cin >> ind;
        }
        commands[i].index = ind;
    }
} 

void inputCommandsFile(int numFCommands) {

    commands.resize(numFCommands);
    for (int i = 0; i < numFCommands; i++){
        string input;
        fileinput >> input;
        commands[i].command = input;
        int ind; //used to change coms.index
        if(input == "inbox" || input =="outbox") { //for seperating the ones followed by a num and not followed
            ind = -1;
        }
        else {
            fileinput >> ind;
        }
        commands[i].index = ind;
    }
}

void matchingcommands(int numCommands){
    int i = 0;
    while (i < numCommands) {
        int n = matchCommand(commands[i].command, commands[i].index, i);
        if(n==-1) {
            break;
        }
        else if(n>=100 || n<=-100) { //how many indeces to jump
            //cout << i << " " << n << endl;
            i+=(n/100);
        }
        else if(n==99) { //if end==index
            i++;
        }
        else {
            i++;
        }
        
    }
    /*
    for(int i = 0 ; i < numCommands; i ++){
        i= matchCommand(commands[i].command, commands[i].index, i);
        if (i == -1) break;
    }
    */
}
 
 
void playLevelManual(int n, int numCommands) {

    statusupdate.open("datasave.txt"); // to put in the passed levels if success
    if(n==1) {
        in.resize(2);
        inTemp.resize(2);
        out.resize(2);
        in = {1, 2};
 
        mid.resize(2);
 
        inTemp = in;
        out = {999, 999};
            
        inputCommandsManual(numCommands);
        matchingcommands(numCommands);
        levelOne();
 
        if(levelOnePassed && errorStep == 99) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if (levelsPassed < 1) levelsPassed = 1;
            statusupdate << levelsPassed;
        }
        else if (errorStep != 99) {
            system("cls");
            cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
    }
    if(n==2) {
        in.resize(8);
        inTemp.resize(8);
        out.resize(8);
        in = {3, 9, 5, 1, -2, -2, 9, -9};
        inTemp = in;
        out = {999,999,999,999,999,999,999,999};
 
        mid.resize(3);
        mid = {999, 999, 999};
            
        inputCommandsManual(numCommands);
        matchingcommands(numCommands);
        levelTwo();
 
        
        
            
        if(levelTwoPassed && errorStep == 99) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if (levelsPassed < 2) levelsPassed = 2;
            statusupdate << levelsPassed;
        }
        else if (errorStep != 99) {
            cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
        //levelChoose();
    }
    if(n==3) {
        in.resize(8);
        inTemp.resize(8);
        out.resize(2);
        in = {6,2, 7, 7, -9, 3, -3, -3};
        inTemp = in;
        out = {999,999};
 
        mid.resize(3);
        mid = {999, 999, 999};
            
        inputCommandsManual(numCommands);
        matchingcommands(numCommands);
        levelThree();
        /*
        for(int i = 0; i < out.size(); i ++) {
            cout << out[i] << " ";
        }
        */
        if(levelThreePassed && errorStep == 99) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if (levelsPassed < 3) levelsPassed = 3;
            statusupdate << levelsPassed;
        }
        else if (errorStep != 99) {
            cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
        //levelChoose();
    }
    if(n==4) {
        in.resize(8);
        inTemp.resize(8);
        out.resize(8);
        in = {-3,-2,-1,0,1,2,3,4};
        inTemp = in;
        out = {999,999,999,999,999,999,999,999};
 
        mid.resize(3);
        mid = {999, 999, 999};
            
        inputCommandsManual(numCommands);
        matchingcommands(numCommands);
        levelFour();
        /*
        for(int i = 0; i < out.size(); i ++) {
            cout << out[i] << " ";
        }
        */
        if(levelFourPassed && errorStep == 99) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if (levelsPassed < 4) levelsPassed = 4;
            statusupdate << levelsPassed;
        }
        else if (errorStep != 99) {
            cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
        //levelChoose();
    }
    statusupdate.close();
}
 

void playLevelFile(int n, int numCommands) {
        statusupdate.open("datasave.txt"); // to put in the passed levels if success
    if(n==1) {
        in.resize(2);
        inTemp.resize(2);
        out.resize(2);
        in = {1, 2};

        mid.resize(2);

        inTemp = in;
        out = {999, 999};
            
        inputCommandsFile(numFCommands);
        matchingcommands(numCommands);
        levelOne();
        if(levelOnePassed) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if(levelsPassed < 1) levelsPassed = 1;
            statusupdate << levelsPassed;
        }
        else if (errorStep != 99) {
            system("cls");
            cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
    }
    if(n==2) {
        in.resize(8);
        inTemp.resize(8);
        out.resize(8);
        in = {3, 9, 5, 1, -2, -2, 9, -9};
        inTemp = in;
        out = {999,999,999,999,999,999,999,999};

        mid.resize(3);
        mid = {999, 999, 999};
            
        inputCommandsFile(numFCommands);
        matchingcommands(numCommands);
        levelTwo();
            
        if(levelTwoPassed) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if(levelsPassed < 2) levelsPassed = 2;
            statusupdate << levelsPassed;;
        }
        else if (errorStep != 99) {
        cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
        //levelChoose();
    }
    if(n==3) {
        in.resize(8);
        inTemp.resize(8);
        out.resize(2);
        in = {6,2, 7, 7, -9, 3, -3, -3};
        inTemp = in;
        out = {999,999};

        mid.resize(3);
        mid = {999, 999, 999};
            
        inputCommandsFile(numFCommands);
        matchingcommands(numCommands);
        levelThree();

        /*
        for(int i = 0; i < out.size(); i ++) {
            cout << out[i] << " ";
        }*/

        if(levelThreePassed) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if(levelsPassed < 3) levelsPassed = 3;
            statusupdate << levelsPassed;
        }
        else if (errorStep != 99) { 
        cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
        //levelChoose();
    }
    if(n==4) {
        in.resize(8);
        inTemp.resize(8);
        out.resize(8);
        in = {-3,-2,-1,0,1,2,3,4};
        inTemp = in;
        out = {999,999,999,999,999,999,999,999};
 
        mid.resize(3);
        mid = {999, 999, 999};
            
        inputCommandsFile(numFCommands);
        matchingcommands(numCommands);
        levelFour();

        /*
        for(int i = 0; i < out.size(); i ++) {
            cout << out[i] << " ";
        }*/

        if(levelFourPassed) {
            cout << "                                            Success,您通关了！" << endl<<endl;
            if(levelsPassed < 4) levelsPassed = 4;
            statusupdate << levelsPassed;
        }
        else if (errorStep != 99) { 
        cout << "                                                输入有误!, Error on instruction " << errorStep+1 <<endl <<endl;
        }
        else {
            cout << "                                               Fail, 您失败了!" << endl;
        }
        //levelChoose();
    }
    statusupdate.close();
}
 
string exitgame(){
    string endorno;
    cout << "                                        游戏已经结束，请选择您想执行的操作                                                      "<< endl<< endl;
    cout << "                 输入'play'进入选关卡界面" << endl <<endl;
    cout << "                 输入'quit'退出并保存游戏" <<endl<< endl;

    cout << std::flush; // Ensure all output is displayed

    // Clear any previous error state and ignore leftover input
    cin.clear(); // Clear any error state
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover input

    cin >> endorno;
    return endorno;
}

int main() {
    string endorno ;
    int levelch;
    levelch =levelChoose();
    correctLevelChosen = checkLevel(levelch);
    int numcommands;

    int type;
    string filename;
    cout << "如果想要文件输入请输入 1, 如果想要键盘输入请输入 2"<< endl;
    cin >> type;
    if(type == 1) { //if not integer
        system("cls");
        Levelins(correctLevelChosen);
        cin.get();
        cout << "  请输入文件名:";
    
        cin >> filename;


        fileinput.open(filename); //import file
        fileinput >>numFCommands;
        
        system("cls");
        playLevelFile(correctLevelChosen, numFCommands);    
        fileinput.close();
        //cout << out[0] << out[1];
    } else {
        system("cls");
        Levelins(correctLevelChosen);

        cout << endl;
        cout << "                      请依次输入指令数量和指令"<< endl;
        cin >> numcommands;

        playLevelManual(correctLevelChosen, numcommands);

    }
    endorno = exitgame();


    while (endorno == "play"){
        system("cls");
        levelch =levelChoose();
        correctLevelChosen = checkLevel(levelch);
        cout << "如果想要文件输入请输入1, 如果想要键盘输入请输入2"<< endl;
        cin >> type;

        if(type == 1) { //if not integer
            system("cls");
            Levelins(correctLevelChosen);
            cout << "请输入文件名";

            cin >> filename;

            fileinput.open(filename); //import file 
            fileinput.clear(); //import file
            fileinput.seekg(0);
            fileinput >>numFCommands;
            system("cls");
            errorStep = 99;
            playLevelFile(correctLevelChosen, numFCommands);  
            fileinput.close();  
            //cout << out[0] << out[1];
        } else {
            system("cls");
            Levelins(correctLevelChosen);
            cin.get();
        
            cout << endl;
            cout << "                      请依次输入指令数量和指令"<< endl;
            cin >> numcommands;
            errorStep = 99;
            playLevelManual(correctLevelChosen, numcommands);

        }
        endorno = exitgame();

    }


    return 0;


}
 
 
//the problem about error output and the success fail jjudgement
 
 
//game ends when in is empty