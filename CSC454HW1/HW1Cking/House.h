/* 
 * File:   House.h
 * Author: chrisrk192
 *
 * Created on September 1, 2017, 3:24 PM
 * 
 * House hold all pertinent 
 */
#include "Room.h"
#include "Creature.h"
#ifndef HOUSE_H
#define HOUSE_H

class House {
public:
    /**
    * Constructor, instanciates a house with 100 rooms
    */
    House();
    House(const House& orig);
    
    /**
    * Destructor, deletes all rooms in the house
    */
    virtual ~House();
    int maxRooms;
    int numRooms;
    Room* rooms[100];

    House(int);
    /**
    * Get the pointer to a room with name roomName
    * @param roomName
    * @return the pointer of room with name roomName
    */
    Room* getRoom(int);
    
    /**
    * Get the room pointer that a creature c is in
    * @param c the creature to reference
    * @return the pointer to the room
    */
    Room* getRoomCreatureIsIn(Creature*);
    
    /**
    * Tells whether a creature can move in the direction d. Creatures can only 
    *      move if the room they are in has a valid room in direction d and that 
    *      room is not full
    * @param c the creature to reference
    * @param d the direction to move to
    * @return true iff the creature is able to move in direction d
    */
    bool CanCreatureMove(Creature*, enums::Direction);
    
    /**
    * Move a creature c in direction d if the creature can move in that direction
    * @param c the creature to reference
    * @param d the direction to go
    * @return true iff the creature has been moved
    */
    bool MoveCreature(Creature*, enums::Direction);
    
    /**
    * Searches for the room that the player is in 
    * @return the pointer to the room player is in, 
    *      null if the player cant be found
    */
    Room* getRoomPlayerIn();
    
    /**
    * Searches through all the rooms for a creature with name 
    * @param name the name of the creature to find
    * @return the pointer to the creature with name if the creature exists, 
    *      null otherwise
    */
    Creature* findCreatureByID(int);
private:

};

#endif /* HOUSE_H */

