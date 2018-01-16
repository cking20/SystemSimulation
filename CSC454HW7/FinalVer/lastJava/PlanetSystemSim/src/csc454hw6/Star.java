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
public class Star implements EventModelInterface<Influencers, Influencers>{
    int fuelLevel;
    int hottness;

    public Star(int fuelLevel, int hottness) {
        this.fuelLevel = fuelLevel;
        this.hottness = hottness;
    }
    
    @Override
    public Influencers OutPut() {
        return new Influencers(0, 0, hottness, 0, 0);
    }

    @Override
    public void StateTransitionExt(Influencers inputItems, double e) throws CantPerformAction {
       //does nothing
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        fuelLevel -= hottness;
    }

    @Override
    public void StateTransitionCon(Influencers inputItems) throws CantPerformAction {
        StateTransitionInt();        
    }

    @Override
    public double TimeAdvance() {
        if(fuelLevel > 0)
            return 1.0;
        else
            return Double.POSITIVE_INFINITY;
    }

    @Override
    public String GetStatus() {
        return "Fuel Left In Star: " +fuelLevel;
    }
    
}
