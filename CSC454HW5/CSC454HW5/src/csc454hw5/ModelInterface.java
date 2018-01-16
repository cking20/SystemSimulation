/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw5;

/**
 *
 * @author chrisrk192
 * 
 */
public interface ModelInterface {    
    //void Input(String inputItem);
    /**
     * Gets the order that args in Initilize are accepted
     * @return signature
     */
    String GetInitilizationSignature();
    /**
     * Set all internal values based on args
     * @param args 
     */
    void Initialize(String[] args);
    /**
     * 
     * @return output based on current state
     */
    String[] OutPut();    
    void StateTransition(String[] inputItems) throws CantPerformAction;
    public String[] Tick(String[] args) throws CantPerformAction;
    public String GetStatus();
    
    public Object Clone();
    
}
class CantPerformAction extends Exception {
    public CantPerformAction(String message){
        super(message);
    }
}
