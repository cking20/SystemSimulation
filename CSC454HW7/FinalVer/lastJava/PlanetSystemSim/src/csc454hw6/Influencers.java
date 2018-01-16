/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

/**
 *
 * @author chrisrk192
 */
public class Influencers {
    //private final boolean influenceOtherPlanet;
    private final long co2;
    private final long oxy;
    private final int heat;
    private final int people;
    private final int techLevel;
    private final String eventName;

    public Influencers(long CO2, long Oxy, int heat, int people, int technLevel) {
        this.co2 = CO2;
        this.oxy = Oxy;
        this.heat = heat;
        this.people = people;
        //this.influenceOtherPlanet = influenceAnotherPlaent;
        this.techLevel = technLevel;
        eventName = "";
    }
    
     public Influencers(String eventName, long CO2, long Oxy, int heat, int people, int technLevel) {
        this.co2 = CO2;
        this.oxy = Oxy;
        this.heat = heat;
        this.people = people;
        //this.influenceOtherPlanet = influenceAnotherPlaent;
        this.techLevel = technLevel;
        this.eventName = eventName;
    }
    
     public String getEventName(){
         return eventName;
     }
    /**
     * @return the CO2
     */
    public long getCO2() {
        return co2;
    }
    
    /**
     * @return the people
     */
    public int getPeople() {
        return people;
    }
    
    /**
     * @return the techLevel
     */
    public int getTechLevel() {
        return techLevel;
    }

    /**
     * @return the Oxy
     */
    public long getOxy() {
        return oxy;
    }

    /**
     * @return the heat
     */
    public int getHeat() {
        return heat;
    }
    
    
    
}
