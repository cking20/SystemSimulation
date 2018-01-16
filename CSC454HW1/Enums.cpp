/*Enums is a collection of enumerators and functions on them that aid in the 
 *      clearity of the rest of the program. 
 */
#include "Enums.h"
/**
 * Returns the string form of Type enumerator
 * @param t The type to get the string value of
 * @return the string version of t
 */
std::string TypeToString(enums::Type t){
    switch(t){
        case enums::animal:
            return "animal";
        case enums::npc:
            return "npc";
        case enums::player:
            return "player";
    }
};
/**
 * Returns the string form of Cleanliness enumerator
 * @param t The type to get the string value of
 * @return the string version of t
 */
std::string CleanlinessToString(enums::Cleanliness t){
    switch(t){
        case enums::clean:
            return "clean";
        case enums::halfClean:
            return "half clean";
        case enums::dirty:
            return "dirty";
    }
};
/**
 * Returns the string form of Direction enumerator
 * @param t The type to get the string value of
 * @return the string version of t
 */
std::string DirectionToString(enums::Direction t){
    switch(t){
        case enums::north:
            return "north";
        case enums::south:
            return "south";
        case enums::east:
            return "east";
        case enums::west:
            return "west";
    }
};
