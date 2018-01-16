/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Creature.cpp
 * Author: chrisrk192
 * 
 * Created on September 1, 2017, 3:33 PM
 * 
 * Creature holds data pertinent to a creature's preferences 
 */

#include "Creature.h"
#include "Enums.h"
#include <iostream>
Creature::Creature() {
}

Creature::Creature(const Creature& orig) {
}

/**
     * Constructor that creates an instance of creature of Type myType and 
     *      name of myName
     * @param myT the type of the creature
     * @param myN the name of the creature
     */
Creature::Creature(enums::Type myT, int myN){
    myType = myT;
    myName = myN;
    //Set the preferred cleanliness based on the type
    if(myType == enums::animal)
        myPrefCleanliness = enums::clean;
    else if(myType == enums::npc)
        myPrefCleanliness = enums::dirty;
    else if(myType == enums::player)
        myPrefCleanliness = enums::halfClean;
    

}

/* Decides if this creature can live in a room
     * @param state the cleanliness state to compare to.
     * @return true iff the creature can live in a room with a state of state
     */
bool Creature::CanLiveHere(enums::Cleanliness state){
            if( state == enums::halfClean)
                return true;
            if( state == myPrefCleanliness)
                return true;
            else return false;
}

/* Decides if the state is the creature's preference
     * @param state the cleanliness state to compare to
     * @return true iff the state and the creatures preference are the same
     */
bool Creature::IsIdealCleanliness(enums::Cleanliness state){
    if( state == myPrefCleanliness)
        return true;
    else return false;
}

void Creature::Speak(bool isHappy){
    if(isHappy)
        std::cout << "Creature " << myName << " happy.";
    else
        std::cout << "Creature " << myName << " mad.";
}

void Creature::SpeakAlot(bool isHappy){
    if(isHappy)
        std::cout << "Creature " << myName << " happy alot.";
    else
        std::cout << "Creature " << myName << " mad alot.";
}
/**
 * Destructor, nothing needs to be deleted within creature.
 */
Creature::~Creature() {
}

Animal::Animal(int name){
    myPrefCleanliness = enums::clean;
    myType = enums::animal;
    myName = name;
}

void Animal::Speak(bool isHappy){
    if(isHappy)
        std::cout << "Animal " << myName << " licks your face.";
    else
        std::cout << "Animal " << myName << " growls at you.";
}

void Animal::SpeakAlot(bool isHappy){
    if(isHappy)
        std::cout << "Animal " << myName << " licks your face a lot.";
    else
        std::cout << "Animal " << myName << " growls alot at you.";
}

NPC::NPC(int name){
    myPrefCleanliness = enums::dirty;
    myType = enums::npc;
    myName = name;
}

void NPC::Speak(bool isHappy){
    if(isHappy)
        std::cout << "NPC " << myName << " smiles at you.";
    else
        std::cout << "NPC " << myName << " grumbles at you.";
}

void NPC::SpeakAlot(bool isHappy){
    if(isHappy)
        std::cout << "NPC " << myName << " smiles a lot.";
    else
        std::cout << "NPC " << myName << " grumbles alot at you.";
}

Player::Player(int name){
    myPrefCleanliness = enums::halfClean;
    myType = enums::player;
    myName = name;
}

void Player::Speak(bool isHappy){
    if(isHappy)
        std::cout << "Player " << "speak called h1.";
    else
        std::cout << "Player " << "speak called m1.";
}

void Player::SpeakAlot(bool isHappy){
    if(isHappy)
        std::cout << "Player " << "speak called h3.";
    else
        std::cout << "Player " << "speak called m3.";
}
