/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

/**
 *
 * @author chrisrk192
 * Interface all models used in the simulation framework or eventNetwork
 */
public interface EventModelInterface<I, O>{
    O OutPut();    
    void StateTransitionExt(I inputItems, double e) throws CantPerformAction;
    void StateTransitionInt() throws CantPerformAction;
    void StateTransitionCon(I inputItems) throws CantPerformAction;
    double TimeAdvance();
    public String GetStatus();
}
class CantPerformAction extends Exception {
    public CantPerformAction(String message){
        super(message);
    }
}