/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Room.h
 * Author: chrisrk192
 *
 * Created on September 1, 2017, 3:29 PM
 */
#include "Creature.h"
#ifndef ROOM_H
#define ROOM_H

class Room {
public:
    Room();
    Room(const Room& orig);
    /**
     * Destructor. Removes all creatures in this room from memory
     */
    virtual ~Room();
    
    enums::Cleanliness state;
    int name;
    int toNorth;
    int toSouth;
    int toEast;
    int toWest;
    int maxCreatures;
    int numCreatures;
    Creature* creaturesInRoom[10];
    
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
    Room(int,enums::Cleanliness,int,int,int,int);
    
    /**
    * Tells whether the creature is in this room or not
    * @param c the creature to test if it is in this room
    * @return true iff the creature c is in this room
    */
    bool IsCreatureInRoom(Creature*);
    
    /**
    * Finds the pointer to the creature in this room with name
    * @param name the name of the creature
    * @return a pointer to the creature in this room with name, 
    *      null if the creature is not in the room
    */
    Creature* getCreatureInRoomByName(int);
    
    /**
    * Tells if a player is in this room
    * @return true iff a player is in this room
    */
    bool IsPlayerInRoom();
    
    /**
     * Get the pointer to the player in this room
     * @return the pointer to the player in this room
     */
    Creature* getPlayer();
    
    /**
    * Prints out all pertinent data about this room
    */
    void DisplayRoomInfo();
    
    /**
     * finds the name of the room in direction d from this room
     * @param d the direction to look
     * @return the name of the room in direction d, -1 on nothing in that direction
     */
    int getRoomIDInDir(enums::Direction);
    
    /**
     * Tells if this room is at capacity
     * @return true iff the room is full
     */
    bool isFull(){return numCreatures >= maxCreatures;}
    
    /**
     * Adds a creature to this room
     * @param c the creature to add to the room
     * @return true iff the creature c has been added to the rom
     */
    bool AddCreature(Creature*);
    
    /**
     * Removes a creature from this room
     * @param c the creature to remove
     * @return true iff the creature has been removed
     */
    bool RemoveCreature(Creature*);
    
    /**
    * Brings this rooms state one step closer to dirty, unless it is already dirty
    */
    void DirtyRoom();
    
    /**
    * Brings this rooms state one step closer to clean, unless it is already clean
    */
    void CleanRoom();
private:
    /**
     * Finds the index of creature c in this room's creature array
     * @param c the creature to find the index of
     * @return the index of creature c, -1 on not in room
     */
    int getIndexOfCreature(Creature*);
};

#endif /* ROOM_H */

