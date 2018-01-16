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


public class Land implements EventModelInterface<Influencers, Influencers>{
    //Land should be thought of as 1 square km
    //units in LBS
    
    static final int OXY_BREATHED = 130;
    static final int CO2_EXHALED = 725;  
    static final int CO2_ABSORBED = 48;
    static final int OXY_RELEASED = 260;
    static final int MAX_POP_DENSITY = 700;
    static final int MAX_TREE_POPULATION = 250000;
    
    
    private enum states{populated, forest, waste};
    private long startingAtmosphere;
    private int climateMultiplier;//<0 = cold, .5-1.5 = acceptable, 2 = hot
    private long co2;
    private long oxy;
    private int numPeople;
    private int numPlants;
    private int techLevel;
    
    public Land(){
        startingAtmosphere = 22784954430l;//per km^2
        //total starting lbs is 22,784,954,430
        //with a 500 to 1 o2/co2 ratio and o2 being 20% of air make up this means
        //
        oxy = (startingAtmosphere/5);
        co2 = oxy/500;
        numPeople = 0;
        numPlants = 250000;
    }
    
    @Override
    public Influencers OutPut() {
        if(numPeople > MAX_POP_DENSITY)
            return new Influencers("land", co2, oxy, 0, numPeople, techLevel);
        
        return new Influencers("land", co2, oxy, 0, 0, techLevel);
    }

    @Override
    public void StateTransitionExt(Influencers in, double e) throws CantPerformAction {
        if(in.getEventName().compareTo("planet") == 0){
            climateMultiplier = in.getHeat();
            co2 = in.getCO2();
            oxy = in.getOxy();
            
            if(co2 >= CO2_ABSORBED * climateMultiplier * numPlants){
                co2 -= CO2_ABSORBED * climateMultiplier * numPlants;
                oxy += OXY_RELEASED * climateMultiplier * numPlants;
            }else{
                int numGotAir = (int) (co2/(CO2_ABSORBED*climateMultiplier));
                co2 = 0;
                numPlants = numGotAir;
                oxy += OXY_RELEASED * climateMultiplier * numPlants;
            }
                
            if(oxy >= OXY_BREATHED * numPeople){
                oxy -= OXY_BREATHED * numPeople;
                co2 += CO2_EXHALED * numPeople;
            }else{
                int numGotAir = (int) (oxy/(OXY_BREATHED));
                oxy = 0;
                numPeople = numGotAir;
                co2 += CO2_EXHALED * numPeople;
            }
           
            //tree population doubles every 50 years so r = .04
            numPlants = (int) (numPlants * Math.pow(Math.E,.04));
            //human population grows at 1.68%=2/125 per year the population of next year will be:
            numPeople = (int) (numPeople * Math.pow(Math.E,.0168));
            
            if(numPlants == 0) numPlants = 10;
        }
        else if(in.getEventName().compareTo("land") == 0){
            if(in.getPeople() >= MAX_POP_DENSITY)
                numPeople+=(in.getPeople()-MAX_POP_DENSITY)/4;
            
            
            
        }
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        if(numPeople > MAX_POP_DENSITY) numPeople = MAX_POP_DENSITY;
    }

    @Override
    public void StateTransitionCon(Influencers inputItems) throws CantPerformAction {
        StateTransitionExt(inputItems, 0);
        StateTransitionInt();
    }

    @Override
    public double TimeAdvance() {
        return 1.0;
    }

    @Override
    public String GetStatus() {
        return "";
    }
    
}
