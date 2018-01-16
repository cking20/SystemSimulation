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
public class DrillPressModel implements EventModelInterface<Integer, Integer>{
    private int p;//number of parts to process
    private double s;//time left to process the first of those parts 
    private double t;//time to process part
    public DrillPressModel(int p, double t) {
        this.p = p;
        this.t = t;
        s = t;
    }
    
    
    
    @Override
    public Integer OutPut() {
        return 1;
    }

    @Override
    public void StateTransitionExt(Integer inputItems, double e) throws CantPerformAction {
        if(p > 0){
            p += inputItems;
            s -= e;
        } else{
            p+= inputItems;
            s = t;
        }
            
    }

    @Override
    public void StateTransitionInt() throws CantPerformAction {
        p--;
        s = t;
    }

    @Override
    public void StateTransitionCon(Integer inputItems) throws CantPerformAction {
        p += inputItems - 1;
        s = t;
    }

    @Override
    public double TimeAdvance() {
        if(p > 0)
            return s;
        else 
            return Double.POSITIVE_INFINITY;
    }

    @Override
    public String GetStatus() {
        return "numParts="+p+" timeToFinish1stPart="+s;
    }
    
}
