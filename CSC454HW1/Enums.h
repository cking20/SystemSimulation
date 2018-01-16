
/* 
 * File:   Enums.h
 * Author: chrisrk192
 *
 * Created on September 1, 2017, 3:45 PM
 * 
 * Enums is a collection of enumerators and functions on them that aid in the 
 *      clearity of the rest of the program. 
 */
#include <string>

#ifndef ENUMS_H
#define ENUMS_H


namespace enums{
    //all the possible types
    enum Type {player,animal,npc};
    //all the possible cleanliness states
    enum Cleanliness{clean,halfClean,dirty};
    //add the possible directions
    enum Direction{north,south,east,west};
    
    
}
/**
 * Returns the string form of Type enumerator
 * @param t The type to get the string value of
 * @return the string version of t
 */
std::string TypeToString(enums::Type);
/**
 * Returns the string form of Cleanliness enumerator
 * @param t The type to get the string value of
 * @return the string version of t
 */
std::string CleanlinessToString(enums::Cleanliness );
/**
 * Returns the string form of Direction enumerator
 * @param t The type to get the string value of
 * @return the string version of t
 */
std::string DirectionToString(enums::Direction);
#endif /* ENUMS_H */

