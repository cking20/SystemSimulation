/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Room.cpp
 * Author: chrisrk192
 * 
 * Created on September 1, 2017, 3:29 PM
 */

#include "Room.h"
#include <iostream>
#include "Enums.h"
Room::Room() {
}

Room::Room(const Room& orig) {
}
/**
 * Create a room with name na, of cleanliness c, 
 *      and other rooms in directions n,s,e,w
 * @param na the name of the room
 * @param c the initial cleanliness of the room
 * @param n the room to the north's name, -1 for no room
 * @param s the room to the south's name, -1 for no room
 * @param w the room to the west's name, -1 for no room
 * @param e the room to the east's name, -1 for no room
 */
Room::Room(int na,enums::Cleanliness c,int n,int s, int w, int e){
    
    state = c;
    name = na;
    maxCreatures = 10;
    numCreatures = 0;
    toNorth = n;
    toSouth = s;
    toEast = w;
    toWest = e;
    creaturesInRoom[maxCreatures];
    
}

/**
 * Adds a creature to this room
 * @param c the creature to add to the room
 * @return true iff the creature c has been added to the rom
 */
bool Room::AddCreature(Creature* c){
    if(this->isFull()){
        std::cout << "room "<< this->name <<"full" << std::endl;
        return false;
    }
    
    creaturesInRoom[numCreatures] = c;
    numCreatures++;
    return true;
}

/**
 * Removes a creature from this room
 * @param c the creature to remove
 * @return true iff the creature has been removed
 */
bool Room::RemoveCreature(Creature* c){
    if(this->IsCreatureInRoom(c)){
        int pos = this->getIndexOfCreature(c);
        for(int i = pos; i < maxCreatures;i++)
            creaturesInRoom[i] = creaturesInRoom[i+1];
        numCreatures--;
        return true;
    }
    return false;
}

/**
 * Finds the index of creature c in this room's creature array
 * @param c the creature to find the index of
 * @return the index of creature c, -1 on not in room
 */
int Room::getIndexOfCreature(Creature* c){
    for(int i = 0; i < numCreatures; i++){
        if(creaturesInRoom[i]->myName==c->myName)
            return i;
    }
    //creature not in Room
    return -1;
}

/**
 * finds the name of the room in direction d from this room
 * @param d the direction to look
 * @return the name of the room in direction d, -1 on nothing in that direction
 */
int Room::getRoomIDInDir(enums::Direction d){
    switch(d){
        case enums::north:
            return toNorth;
            break;
        case enums::south:
            return toSouth;
            break;
        case enums::east:
            return toEast;
            break;
        case enums::west:
            return toWest;
        default:
            std::cout << "bad dir";
            return -1;
            break;
    }
    return -1;
}

/**
 * Prints out all pertinent data about this room
 */
void Room::DisplayRoomInfo(){
    std::cout << "Currently in room :" << name << '\n';
    switch(state){
        case enums::dirty:
            std::cout << "This room is dirty" << '\n';
            break;
        case enums::halfClean:
            std::cout << "This room is half clean" << '\n';
            break;
        case enums::clean:
            std::cout << "This room is clean" << '\n';
            break;
    }
    if(toNorth != -1)
        std::cout << "There is a door to " << toNorth << " North. ";
    if(toSouth != -1)
        std::cout << "There is a door to " << toSouth << " South. ";
    if(toEast != -1)
        std::cout << "There is a door to " << toEast << " East. ";
    if(toWest != -1)
        std::cout << "There is a door to " << toWest << " West.\n";
    
    
    std::cout << "There are " << numCreatures << " creatures in this room.\n";
    for(int i = 0; i < numCreatures;i++){
        switch(creaturesInRoom[i]->myType){
            case enums::npc:
                std::cout << "  NPC    " << creaturesInRoom[i]->myName << '\n';
                break;
            case enums::animal:
                std::cout << "  Animal " << creaturesInRoom[i]->myName << '\n';
                break;
            case enums::player:
                std::cout << "  Player " << creaturesInRoom[i]->myName << '\n';
                break;
        }
    }
}

/**
 * Brings this rooms state one step closer to dirty, unless it is already dirty
 */
void Room::DirtyRoom(){
    if(state == enums::dirty)
        return;
    
    if(state == enums::halfClean){
        state = enums::dirty;
    }
    if(state == enums::clean){
        state = enums::halfClean;
    }
}

/**
 * Brings this rooms state one step closer to clean, unless it is already clean
 */
void Room::CleanRoom(){
    if(state == enums::clean)
        return;    
    if(state == enums::halfClean)
        state = enums::clean;
    
    if(state == enums::dirty)
        state = enums::halfClean;
}

/**
 * Tells whether the creature is in this room or not
 * @param c the creature to test if it is in this room
 * @return true iff the creature c is in this room
 */
bool Room::IsCreatureInRoom(Creature *c){
    for(int i = 0; i < numCreatures; i++){
        if(creaturesInRoom[i]->myName==c->myName)
            return true;
    }
    return false;
}

/**
 * Finds the pointer to the creature in this room with name
 * @param name the name of the creature
 * @return a pointer to the creature in this room with name, 
 *      null if the creature is not in the room
 */
Creature* Room::getCreatureInRoomByName(int name){
    for(int i = 0; i < numCreatures; i++){
        if(creaturesInRoom[i]->myName==name)
            return creaturesInRoom[i];
    }
    return 0;
}

/**
 * Tells if a player is in this room
 * @return true iff a player is in this room
 */
bool Room::IsPlayerInRoom(){
    for(int i = 0; i < numCreatures; i++){
        if(creaturesInRoom[i]->myType==enums::player)
            return true;
    }
    return false;
}

/**
 * Get the pointer to the player in this room
 * @return the pointer to the player in this room
 */
Creature* Room::getPlayer(){
    for(int i = 0; i < numCreatures; i++){
        if(creaturesInRoom[i]->myType==enums::player)
            return creaturesInRoom[i];
    }
    return NULL;
}

/**
 * Destructor. Removes all creatures in this room from memory
 */
Room::~Room() {
    for(int i = 0; i < numCreatures; i++){
        delete creaturesInRoom[i];        
    }
}

