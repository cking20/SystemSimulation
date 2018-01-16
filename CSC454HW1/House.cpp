/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   House.cpp
 * Author: chrisrk192
 * 
 * Created on September 1, 2017, 3:24 PM
 */

#include "House.h"



House::House(const House& orig) {
}
/**
 * Constructor, instanciates a house with 100 rooms
 */
House::House(){
    maxRooms = 100;
    numRooms = 0;
    
}

/**
 * Get the pointer to a room with name roomName
 * @param roomName
 * @return the pointer of room with name roomName
 */
Room* House::getRoom(int roomName){
    return rooms[roomName];
}

/**
 * Get the room pointer that a creature c is in
 * @param c the creature to reference
 * @return the pointer to the room
 */
Room* House::getRoomCreatureIsIn(Creature*c){
    for(int i = 0; i < numRooms;i++){
        if(rooms[i]->IsCreatureInRoom(c))
            return rooms[i];
    }
    return 0;
}

/**
 * Searches through all the rooms for a creature with name 
 * @param name the name of the creature to find
 * @return the pointer to the creature with name if the creature exists, 
 *      null otherwise
 */
Creature* House::findCreatureByID(int name){
    Creature* temp;
    for(int i = 0; i < numRooms;i++){
        temp = rooms[i]->getCreatureInRoomByName(name);
        if(temp != 0)
            return temp;
    }
    return 0;
}

/**
 * Searches for the room that the player is in 
 * @return the pointer to the room player is in, 
 *      null if the player cant be found
 */
Room* House::getRoomPlayerIn(){
    for(int i = 0; i < numRooms;i++){
        if(rooms[i]->IsPlayerInRoom())
            return rooms[i];
    }
    return 0;
}

/**
 * Tells whether a creature can move in the direction d. Creatures can only 
 *      move if the room they are in has a valid room in direction d and that 
 *      room is not full
 * @param c the creature to reference
 * @param d the direction to move to
 * @return true iff the creature is able to move in direction d
 */
bool House::CanCreatureMove(Creature *c, enums::Direction d){
    bool canGO = false;
    //Room targetRoom;
    Room *target = this->getRoomCreatureIsIn(c);
    switch(d){
        case enums::north:
            if(target->toNorth == -1){
                std::cout << "cant move north target is -1"<<std::endl;
                return false;
            }
            if(this->getRoom(target->toNorth)->isFull()){
                std::cout << "cant move north target is full"<<std::endl;
                return false;
            }
            break;
        case enums::south:
            if(target->toSouth == -1){
                std::cout << "cant move south target is -1"<<std::endl;
                return false;
            }
            if(this->getRoom(target->toSouth)->isFull()){
                std::cout << "cant move south target is full"<<std::endl;
                return false;
            }
            break;
        case enums::east:
            if(target->toEast == -1){
                std::cout << "cant move east target is -1"<<std::endl;
                return false;
            }
            if(this->getRoom(target->toEast)->isFull()){
                std::cout << "cant move east target is full"<<std::endl;
                return false;
            }
            break;
        case enums::west:
            if(target->toWest == -1){
                std::cout << "cant move west target is -1"<<std::endl;
                return false;
            }
            if(this->getRoom(target->toWest)->isFull()){
                std::cout << "cant move west target is full"<<std::endl;
                return false;
            }
            break;
        default:
            std::cout << "default called in CanCreatureMove"<<std::endl;
            return false;
            break;
    }
    return true;
}

/**
 * Move a creature c in direction d if the creature can move in that direction
 * @param c the creature to reference
 * @param d the direction to go
 * @return true iff the creature has been moved
 */
bool House::MoveCreature(Creature *c, enums::Direction d){
    if(!(this->CanCreatureMove(c,d))){
        std::cout << c->myName << "cant move(MoveCreature)"<<std::endl;
        return false;    
    }
    Room* currentRoom = this->getRoomCreatureIsIn(c);
    //std::cout << "current room :" << currentRoom->name << std::endl;
    Room* targetRoom =  this->getRoom(currentRoom->getRoomIDInDir(d));
    //std::cout << "target room :" << targetRoom->name << std::endl;
    if(targetRoom->AddCreature(c)){
        currentRoom->RemoveCreature(c);
        
        if(!(c->CanLiveHere(targetRoom->state))){
            targetRoom->state = enums::halfClean;
        }
    } else{
        std::cout<<"add creature was false"<<std::endl;
    }
   
    
}

/**
 * Destructor, deletes all rooms in the house
 */
House::~House() {
    for(int i = 0; i << numRooms;i++){
        delete rooms[i];
            
    }
}

