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
public class Planet implements EventModelInterface<Influencers, Influencers>{
   
    private int techLevel = 0;
    private String name;
    private long totalPlanetCO2;
    private long totalPlanetOxy;
    private int climateMultiplier;
    private final int idealTemperature;
    private int currentTemperature;
    private int numLands;
    private double timeTillnextYear = 1.0;

    public Planet(String name, int numLands, long CO2, long Oxy, int heat) {
        this.name = name;
        this.totalPlanetCO2 = CO2;
        this.totalPlanetOxy = Oxy;
        this.idealTemperature = heat;
        this.currentTemperature = heat;
        this.numLands = numLands;
        
    }

    @Override
    public Influencers OutPut() {
        return new Influencers("planet", totalPlanetCO2/numLands, totalPlanetOxy/numLands, climateMultiplier, 0, 0);
    }

    @Override
    public void StateTransitionExt(Influencers in, double e) throws CantPerformAction {        
        totalPlanetCO2+=in.getCO2();
        totalPlanetOxy+=in.getOxy();
        timeTillnextYear -= e;
        //NEED ADD HEAT BACK IN BUT BETTTER
        
        
        
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        totalPlanetCO2 = 0;
        totalPlanetOxy = 0;
        climateMultiplier = 0;
        timeTillnextYear = 1.0;
    }

    @Override
    public void StateTransitionCon(Influencers inputItems) throws CantPerformAction {
        StateTransitionInt();
        StateTransitionExt(inputItems, 0);
    }

    @Override
    public double TimeAdvance() {
        return timeTillnextYear;
    }

    @Override
    public String GetStatus() {
        return name+": "+
                "total oxy : "+totalPlanetOxy+"\n"+
                "total co2 : "+totalPlanetCO2+"\n"+
                "total oxy : "+totalPlanetOxy+"\n"+
                "ideal temp : "+idealTemperature+"\n"
                
                ;
    }
    
    //
    private double CalculateIdealClimateMultiplier(){
        
        return 1.0;
    }
}
