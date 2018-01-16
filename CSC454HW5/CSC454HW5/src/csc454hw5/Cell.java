/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw5;

/**
 *
 * @author chrisrk192
 */
public class Cell implements ModelInterface{
    private int state;
    @Override
    public String GetInitilizationSignature() {
        return "State(0/1)";
    }

    @Override
    public void Initialize(String[] args) {
         state = Integer.parseInt(args[0]);
    }

    @Override
    public String[] OutPut() {
        String[] s = new String[1];
        s[0] = state + "";
        return s;
    }
    /**
     * Network must pass all left couples, then all right couples
     * @param inputItems
     * @throws CantPerformAction 
     */
    @Override
    public void StateTransition(String[] inputItems) throws CantPerformAction {
        int left, right;
        left = Integer.parseInt(inputItems[0]);
        right = Integer.parseInt(inputItems[0]);
        if(left == 0 && right == 0){
            return;//state stays the same
        }
        if(left == 1 && right == 1){//state flips
            if(state == 1)
                state = 0;
            else 
                state = 1;   
            return;
        }
        if(left == 1 && right == 0)
            return;
        if(right == 1 && state == 1){
            return;
        }
        if(right == 1 && state == 0)
            state = 1;
        
        
    }

    @Override
    public String[] Tick(String[] args) throws CantPerformAction {
        String[] out = OutPut();
        StateTransition(args);
        return out;
    }

    @Override
    public String GetStatus() {
        return "state = "+state;
    }

    @Override
    public Object Clone() {
        Cell c = new Cell();
        c.state = state;
        return c;
    }
    
}
