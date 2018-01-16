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
public class AsteroidBelt implements EventModelInterface<Influencers, Influencers>{
    private int numAsteroids;
    //a 1km meteor would cool the earth by 14.5 degrees
    private int planetCoolingFactor = -14;
    private double minT;
    private double maxT;

    public AsteroidBelt(int numAsteroids, double minTime, double maxTime) {
        this.numAsteroids = numAsteroids;
        minT = minTime;
        maxT = maxTime;
    }
    public AsteroidBelt() {
        this.numAsteroids = 879;//number of asteroids of at least .6km spotted that could hit earth
        minT = 1000;
        maxT = 1000000;
    }
    
    
    
    @Override
    public Influencers OutPut() {
        return new Influencers("asteroid",0, 0, planetCoolingFactor, 0, 0);
    }

    @Override
    public void StateTransitionExt(Influencers inputItems, double e) throws CantPerformAction {
        
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        numAsteroids--;
    }

    @Override
    public void StateTransitionCon(Influencers inputItems) throws CantPerformAction {
        StateTransitionInt();
    }

    @Override
    public double TimeAdvance() {
        if(numAsteroids > 0)
        return (Math.random()*(maxT-minT))+minT;
        //return maxT-minT;
        
        else return Double.POSITIVE_INFINITY;
    }

    @Override
    public String GetStatus() {
        return "asteroidy statusy stuff\n";
    }
    
}
