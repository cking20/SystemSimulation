/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw4;

/**
 *
 * @author chrisrk192
 */
public interface EventModelInterface {
    String[] OutPut();    
    void StateTransitionExt(String[] inputItems) throws CantPerformAction;
    void StateTransitionInt() throws CantPerformAction;
    void StateTransitionCon(String[] inputItems) throws CantPerformAction;
    double TimeAdvance();
    public String GetStatus();
    
}
