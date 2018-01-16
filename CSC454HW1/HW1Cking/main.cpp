/* 
 * File:   main.cpp
 * Author: chrisrk192
 *
 * Created on August 30, 2017, 7:00 PM
 */

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include "House.h"
#include "Room.h"
#include "Creature.h"
#include "Enums.h"
using namespace std;

/*
 * 
 */

/**
 * Print out the help information to the console
 */
void DisplayHelp(){
    cout <<"------------------Help Menu--------------------------------\n";
    cout <<"Input without a target will be assumed as a player action\n";
    cout <<"Input should be one of the following formats:\n";
    cout <<"    Target:Command\n";
    cout <<"    Command \n";
    cout <<"Valid Commands are limited to the following:\n";
    cout <<"    look\n";
    cout <<"    clean/dirty\n";
    cout <<"    north/south/east/west\n";
    cout <<"    help\n";
    cout <<"    quit\n";
    cout <<"Example Input:    0:north\n";
    cout <<"Example Input:    look\n";
    cout <<"--------------------------------------------------------------\n";
}


/**
 * Display the exit game text to the console based on how the game is exited
 * @param exitCondition the state the game is left in. 
 *      0 = fail, 1 = not complete, 2 = win
 */
void DisplayExitGame(int exitCondition){
    //exitCondition : 0 = fail, 1 = not complete, 2 = win
    if(exitCondition == 0)
        cout << "You Lost the Game. ";
    else if(exitCondition == 1)
        cout <<"You Quit the Game. ";
     else if(exitCondition == 2)
        cout <<"You Won the Game. ";
    
    cout <<"Thank you For Playing. GoodBye";
    
}

/**
 * Take in user input to set up the state and structure of the house
 * @return the built house
 */
House* BuildHouse(){
    int numRooms;
    int numCreatures;
     do{//get the number of rooms
        cout <<"Enter the number of rooms\n";
        cout << ">";
        cin >> numRooms;
    } while(numRooms < 1 || numRooms > 100);
    House* theHouse = new House();
    theHouse->numRooms = numRooms;
    //build every room
    for (int i = 0; i < numRooms; i++){
        int stateInt,north, south, east, west;
        
        //get the room info
        cout << "\nEnter the : State, North Room, South Room, East Room, West Room\n";
        cin >> stateInt >> north >>south >> east >>west;
        
        theHouse->rooms[i] = new Room(i,static_cast<enums::Cleanliness>(stateInt),north,south,east,west);
    }
    
    do{//get the number of creatures
        cout << "Enter the number of Creatures\n";
        cout << ">";
        cin >> numCreatures;
    } while(numCreatures < 1 || numCreatures > 100);
    
    //create each creature
    for (int i = 0; i < numCreatures; i++){
        int cType, roomC = -1;
        //get the room info
        cout <<"\nEnter the : Type Room\n";
        cin >> cType >> roomC;
        switch(static_cast<enums::Type>(cType)){
            case enums::animal:
                theHouse->rooms[roomC]->creaturesInRoom[theHouse->rooms[roomC]->numCreatures] = new Animal(i);
                break;
            case enums::npc:
                theHouse->rooms[roomC]->creaturesInRoom[theHouse->rooms[roomC]->numCreatures] = new NPC(i);
                break;
            case enums::player:
                theHouse->rooms[roomC]->creaturesInRoom[theHouse->rooms[roomC]->numCreatures] = new Player(i);
                break;
                
        }
        
        theHouse->rooms[roomC]->numCreatures++;
        
        
    }
    return theHouse;
}

/**
 * Parses through a line of input, determines the appropriate target and command
 * @param targetNum modified to the current target's name
 * @param command modified to the current command
 */
void ProcessInput(int &targetNum, int &command){
    targetNum = -1;
    command = -1;
    char cmd[100];
    int tempTarget = 0;
    char trg[10];
    
    do{       
        //read a line of input
        //cin.getline(cmd,100);
        cin >> cmd;
        int delimAt = -1;
        //find the delimiter
        for(int i = 0; i < 10;i++){
            if(cmd[i] == ':')
                delimAt = i;
        }
        if(delimAt>0){
            //there was a delimiter
            targetNum = 0;
            for(int j = 0; j < delimAt; j++)
                trg[j] = cmd[j];
            
            for(int j = delimAt+1; j < 100; j++)
                cmd[j-delimAt-1] = cmd[j];
        }
        int placeCounter = 1;
        char temp;
        
        for(int i = delimAt-1; i >= 0; i--){
            //get the numeric value of the character @ i
            temp = trg[i];
            targetNum += (temp-'0') * placeCounter;
            placeCounter *= 10;
        }

        //convert the cmd string into a command number
        if(strcmp(cmd, "help") == 0)
            command = 0;
        else if(strcmp(cmd, "look") == 0)
            command = 1;
        else if(strcmp(cmd, "dirty") == 0)
            command = 2;
        else if(strcmp(cmd, "clean") == 0)
            command = 3;
        else if(strcmp(cmd, "north") == 0)
            command = 4;
        else if(strcmp(cmd, "south") == 0)
            command = 5;
        else if(strcmp(cmd, "east") == 0)
            command = 6;
        else if(strcmp(cmd, "west") == 0)
            command = 7;
        else if(strcmp(cmd, "exit") == 0)
            command = 8;
        else
            command = -1;
    } while (command == -1);    
}

/**
 * Moves the creature to a random new room if it can
 * @param theHouse pointer to the house the creature is in
 * @param theCrt pointer to the creature to be moved
 * @return true iff the creature has been moved
 */
bool FindRandomNewRoom(House* theHouse, Creature* theCrt){
    Room* roomIn = theHouse->getRoomCreatureIsIn(theCrt);
    int queue[4];
    
    //get the random directions
    queue[0] = rand() % 4;//creates a random num between  0 and 3
    do{
        queue[1] = rand() % 4;
    }while(queue[0] == queue[1]);
    do{
        queue[2] = rand() % 4;
    }while(queue[2] == queue[0] || queue[2] == queue[1]);
    do{
        queue[3] = rand() % 4;
    }while(queue[3] == queue[0] || queue[3] == queue[1] || queue[3] == queue[2]);
    
    //try and move in the current random direction
    for(int i = 0; i < 4; i++){
        switch( static_cast<enums::Direction>(queue[i]) ){
            case enums::north:
                if(theHouse->CanCreatureMove(theCrt,enums::north)){//can move
                    theHouse->MoveCreature(theCrt,enums::north);
                    cout << TypeToString(theCrt->myType) << theCrt->myName 
                            << " left toward the " << DirectionToString(enums::north) << endl;
                    return true;
                }
                break;
            case enums::south:
                if(theHouse->CanCreatureMove(theCrt,enums::south)){//can move
                    theHouse->MoveCreature(theCrt,enums::south);
                    cout << TypeToString(theCrt->myType) << theCrt->myName 
                            << " left toward the " << DirectionToString(enums::south) << endl;
                    return true;
                }
                break;  
            case enums::east:
                if(theHouse->CanCreatureMove(theCrt,enums::east)){//can move
                    theHouse->MoveCreature(theCrt,enums::east);
                    cout << TypeToString(theCrt->myType) << theCrt->myName 
                            << " left toward the " << DirectionToString(enums::east) << endl;
                    return true;
                }
                break;  
            case enums::west:
                if(theHouse->CanCreatureMove(theCrt,enums::west)){//can move
                    theHouse->MoveCreature(theCrt,enums::west);
                    cout << TypeToString(theCrt->myType) << theCrt->myName 
                            << " left toward the " << DirectionToString(enums::west) << endl;
                    return true;
                }
                break;  
        }
    }
    return false;


}

/**
 * Prints out the appropriate message for the creature's feelings
 * @param happy is the creature happy
 * @param isTarget is the creature emoting due to it's actions
 * @param theCrt the creature to reference
 * @param respect the respect of the player(modified)
 */
void CretEmote(bool happy,bool isTarget,Creature* theCrt, int &respect){
    if(happy){
        if(isTarget)//happy alot
            respect += 3;
        else//happy some
            respect ++;
    } else{
        if(isTarget)//mad alot
            respect -= 3;
        else//mad some
            respect --;
    }
    
    if(isTarget){
        theCrt->SpeakAlot(happy);
    } else{
        theCrt->Speak(happy);
    }
    cout << " Respect is now " << respect << endl;    
}

/**
 * Updates the state of the house, by allowing the creature to perform it's actions
 *      based on the new cleanliness of the room the creature is in.
 * @param stateDirection the clean/dirty direction the room is heading
 * @param theCrt the creature to reference
 * @param theHouse the house the creature is in
 * @param isTarget if the creature is the one performing the action
 * @param respect the respect level of the player (modified)
 */
void CretRctToStateCng(enums::Cleanliness stateDirection, Creature* theCrt, House* theHouse, bool isTarget, int &respect){
    if(theCrt->myPrefCleanliness == enums::halfClean)
        return;//make the player react
    CretEmote(theCrt->IsIdealCleanliness(stateDirection), isTarget, theCrt, respect);
     
    //see if it should leave
    if(!(theCrt->CanLiveHere(theHouse->getRoomPlayerIn()->state))){//cant live here
        if(FindRandomNewRoom(theHouse,theCrt))
            ;//moved
        else{//had to leave
            cout << theCrt->myName << " had to leave."  << endl;
            delete theCrt;
            Room* theRoom = theHouse->getRoomCreatureIsIn(theCrt);
            theRoom->RemoveCreature(theCrt);
            for(int i = 0; i < theRoom->numCreatures;i++){
                respect--;
                theRoom->creaturesInRoom[i]->Speak(false);
                cout << " Respect is now " << respect << endl;
            }
        }        
    }
}



/**
 * Caries out the appropriate control flow for the action to be performed
 * @param target the target creature in the same room as the player
 * @param action the action to perform
 * @param theHouse the house to reference
 * @param respect the player's respect (modified)
 * @return false iff the user wants to quit
 */
bool performUserAction(int target, int action, House* theHouse, int &respect){ 
    int keepPlaying = true; //continue by default
    int firstRandDir, secondRandDir, thirdRandDir, fourthRandDir;
    Room* activeRoom = theHouse->getRoomPlayerIn();
    Creature* targetCreature = NULL;// = theHouse->findCreatureByID(target);
    Creature * curCret = NULL;
    if(target >= 0)
        targetCreature = theHouse->findCreatureByID(target);
    
    switch(action){
        //help
        case 0: 
            DisplayHelp();
            break;
        //look
        case 1: 
            activeRoom->DisplayRoomInfo();
            break;
        //dirty (2)
        case 2:
            activeRoom->DirtyRoom();
            
            if(target != -1){
                cout << target << " dirties the room. State is now " << CleanlinessToString(activeRoom->state) << endl;
                for(int i = 0; i < activeRoom->numCreatures;i++){
                    curCret = activeRoom->creaturesInRoom[i];
                    //player wont react because his "pref is half"
                    CretRctToStateCng(enums::dirty, curCret, theHouse, (targetCreature->myName == curCret->myName), respect);

                }
            } else {
                cout << "You dirty the room. State is now " << CleanlinessToString(activeRoom->state) << endl;
                for(int i = 0; i < activeRoom->numCreatures;i++){
                    curCret = activeRoom->creaturesInRoom[i];
                    //player wont react because his "pref is half"
                    CretRctToStateCng(enums::dirty, curCret, theHouse, false, respect);

                }
            }
                               
            break;
        //clean (0)
        case 3:
            activeRoom->CleanRoom();
            
            if(target != -1){
                cout << target << " cleans the room. State is now " << CleanlinessToString(activeRoom->state) << endl;
                for(int i = 0; i < activeRoom->numCreatures;i++){
                    curCret = activeRoom->creaturesInRoom[i];
                    //player wont react because his "pref is half"
                    CretRctToStateCng(enums::clean, curCret, theHouse, (targetCreature->myName == curCret->myName), respect);

                }
            } else {
                cout << "You clean the room. State is now " << CleanlinessToString(activeRoom->state) << endl;
                for(int i = 0; i < activeRoom->numCreatures;i++){
                    curCret = activeRoom->creaturesInRoom[i];
                    //player wont react because his "pref is half"
                    CretRctToStateCng(enums::clean, curCret, theHouse, false, respect);

                }
            }
            break;
        //north
        case 4:
            if(target != -1)
                if(theHouse->CanCreatureMove(targetCreature,enums::north))
                    theHouse->MoveCreature(targetCreature,enums::north);
                else{
                    CretEmote(false,false,targetCreature,respect);
                }
            else
                theHouse->MoveCreature(activeRoom->getPlayer(),enums::north);
            break;
        //south
        case 5:
            if(target != -1)
                if(theHouse->CanCreatureMove(targetCreature,enums::south))
                    theHouse->MoveCreature(targetCreature,enums::south);
                else{
                    CretEmote(false,false,targetCreature,respect);
                }
            else
                theHouse->MoveCreature(activeRoom->getPlayer(),enums::south);
            break;
        //east
        case 6: 
            if(target != -1)
                if(theHouse->CanCreatureMove(targetCreature,enums::east))
                    theHouse->MoveCreature(targetCreature,enums::east);
                else{
                    CretEmote(false,false,targetCreature,respect);
                }
            else
                theHouse->MoveCreature(activeRoom->getPlayer(),enums::east);
            break;
        //west
        case 7: 
            if(target != -1)
                if(theHouse->CanCreatureMove(targetCreature,enums::west))
                    theHouse->MoveCreature(targetCreature,enums::west);
                else{
                    CretEmote(false,false,targetCreature,respect);
                }
            else
                theHouse->MoveCreature(activeRoom->getPlayer(),enums::west);
            break;
        //exit
        case 8: 
            DisplayExitGame(1);
            keepPlaying = false;
            break;
        default: break;
        
    }
    return keepPlaying;
}

/**
 * Checks to see if the player has won/lost
 * @param theHouse the house to reference
 * @param respect the player's respect
 * @param runAgain if the player wants to run it keep playing
 * @return whether the game should keep going
 */
bool Update(House* theHouse,int &respect, bool runAgain){
    if(runAgain == false){
        //cout << "Goodbye.";
        return false;
    } 
    if(respect <= 0){
        cout << "You Lose.";
        return false;
    }
    if(respect >= 80){
        cout << "You Win!";
        return false;
    } 
}

/**
 * Plays a game about managing creatures in a house, in an attempt to get the
 *      most possible respect
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) {
    int respect = 40;
    int maxCret = 10;
    int maxRoom = 100;
    bool runAgain = true;
    //get startup input
    House* theHouse; 
    theHouse = BuildHouse();
   
    //game loop
    while(runAgain){
        int target; int command;
        ProcessInput(target,command);
        runAgain = performUserAction(target, command,theHouse, respect);
        runAgain = Update(theHouse, respect, runAgain);
    }
   
    //clear memory
    delete theHouse;
    return 0;
}

