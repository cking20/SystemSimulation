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
public class Rocket implements EventModelInterface<Influencers, Influencers>{
    private double rocketTime;
    private int numPeople = 0;
    
    public Rocket(){
        rocketTime = 84000.0;
    }
    
    public Rocket(double travelTime){
        rocketTime = travelTime;    
    }
    
    @Override
    public Influencers OutPut() {
        return new Influencers("rocket", 0, 0, 0, numPeople, 5);
    }

    @Override
    public void StateTransitionExt(Influencers inputItems, double e) throws CantPerformAction {
        //blow up the ship
        if (inputItems.getEventName().compareTo("asteroid") == 0) {
            numPeople = 0;
        }else{
            numPeople+=inputItems.getPeople();
        }
        
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        numPeople = 0;
    }

    @Override
    public void StateTransitionCon(Influencers inputItems) throws CantPerformAction {
        StateTransitionExt(inputItems, 0);
    }

    @Override
    public double TimeAdvance() {
        if (numPeople > 0) {
            return rocketTime;            
        }
        return Double.POSITIVE_INFINITY;
    }

    @Override
    public String GetStatus() {
        if (numPeople > 0) {
            return "Traveling with "+numPeople+" people.";
        }
        return "";
    }
    
    
}
