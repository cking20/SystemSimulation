/* 
 * File:   Creature.h
 * Author: chrisrk192
 *
 * Created on September 1, 2017, 3:33 PM
 * 
 * Creature holds data pertinent to a creature's preferences 
 */

#ifndef CREATURE_H
#define CREATURE_H
#include "Enums.h"
#include <iostream>
class Creature {
public:
    Creature();
    Creature(const Creature& orig);
    virtual ~Creature();
    
    int myName;
    enums::Type myType;
    enums::Cleanliness myPrefCleanliness;
    
    /**
     * Display the creature's reaction to a situation they didnt perform
     * @param isHappy does the creature like the situation
     */
    virtual void Speak(bool isHappy);
    
    /**
     * Display the creature's reaction to a situation they performed 
     * @param isHappy does the creature like the situation
     */
    virtual void SpeakAlot(bool isHappy);   
    
    /**
     * Constructor that creates an instance of creature of Type myType and 
     *      name of myName
     * @param myT the type of the creature
     * @param myN the name of the creature
     */
    Creature(enums::Type,int);
        
    /* Decides if this creature can live in a room
     * @param state the cleanliness state to compare to.
     * @return true iff the creature can live in a room with a state of state
     */
    bool CanLiveHere(enums::Cleanliness state);
    
    /* Decides if the state is the creature's preference
     * @param state the cleanliness state to compare to
     * @return true iff the state and the creatures preference are the same
     */
    bool IsIdealCleanliness(enums::Cleanliness state);
private:
};

class Animal: public Creature{
public:
    Animal(int);
    void Speak(bool isHappy);
    void SpeakAlot(bool isHappy);     
};

class NPC: public Creature{
public:
    NPC(int);
    void Speak(bool isHappy);
    void SpeakAlot(bool isHappy);
};

class Player: public Creature{
public:
    Player(int);
    void Speak(bool isHappy);
    void SpeakAlot(bool isHappy);

    
private:
};
#endif /* CREATURE_H */

