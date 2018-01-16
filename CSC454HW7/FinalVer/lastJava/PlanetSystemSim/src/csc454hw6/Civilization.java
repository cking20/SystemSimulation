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
public class Civilization implements EventModelInterface<Influencers, Influencers>{
    //in lbs per person per capita
    static final int LEVEL_1_CO2_EMISSION = 1000;
    static final int LEVEL_2_CO2_EMISSION = 2000;
    static final int LEVEL_3_CO2_EMISSION = 6600;
    static final int LEVEL_4_CO2_EMISSION = 11000;
    private String name;
    private int numPeople;
    private int techLevel;
    private int techLevelUpCounter;

    public Civilization(String name, int numPeople, int techLevel) {
        this.name = name;
        this.numPeople = numPeople;
        this.techLevel = techLevel;
        techLevelUpCounter = 0;
    }
    public Civilization() {
        this.name = "x";
        this.numPeople = 0;
        this.techLevel = 0;
        techLevelUpCounter = 0;
    }
    
    
    @Override
    public Influencers OutPut() {
        //if surplus of people and max tech, send people to rocket and co2 to planet
        if(numPeople > Land.MAX_POP_DENSITY && techLevel > 4)
            return new Influencers("civilization", numPeople*LEVEL_4_CO2_EMISSION, 0, 0, numPeople-Land.MAX_POP_DENSITY, techLevel);
        switch(techLevel){
            case 0:
                return new Influencers("civilization", 0, 0, 0, 0, techLevel);       
            case 1:
                return new Influencers("civilization", numPeople*LEVEL_1_CO2_EMISSION, 0, 0, 0, techLevel);                
            case 2:
                return new Influencers("civilization", numPeople*LEVEL_2_CO2_EMISSION, 0, 0, 0, techLevel);               
            case 3: 
                return new Influencers("civilization", numPeople*LEVEL_3_CO2_EMISSION, 0, 0, 0, techLevel);                
            case 4:
                return new Influencers("civilization", numPeople*LEVEL_4_CO2_EMISSION, 0, 0, 0, techLevel);     
        }
        return new Influencers("civilization", 0, 0, 0, 0, 0);
    }

    @Override
    public void StateTransitionExt(Influencers inputItems, double e) throws CantPerformAction {
        //if the land has a surplus population for every 1000 years, increases the tech level
        numPeople = inputItems.getPeople();
        if(numPeople > 0){
            techLevelUpCounter++;
            if (techLevelUpCounter >= 1000) {
                techLevel++;
                techLevelUpCounter = 0;
            }
        }
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        //
    }

    @Override
    public void StateTransitionCon(Influencers inputItems) throws CantPerformAction {
        StateTransitionExt(inputItems, 0);
    }

    @Override
    public double TimeAdvance() {
        return 1.0;
    }

    @Override
    public String GetStatus() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
