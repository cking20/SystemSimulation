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
public class StringtoIntModel implements EventModelInterface<String, Integer>{
    String val = null;
    @Override
    public Integer OutPut() {
        return Integer.parseInt(val);
    }

    @Override
    public void StateTransitionExt(String inputItems, double e) throws CantPerformAction {
        val = inputItems;
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        val = null;
    }

    @Override
    public void StateTransitionCon(String inputItems) throws CantPerformAction {
       val = inputItems;
    }

    @Override
    public double TimeAdvance() {
        if(val != null) return 1.0;
        return Double.POSITIVE_INFINITY;
    }

    @Override
    public String GetStatus() {
        if(val == null) return "null val";
        return val;
    }
    
}
