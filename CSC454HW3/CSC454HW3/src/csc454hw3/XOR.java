/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw3;

/**
 *
 * @author chrisrk192
 */
public class XOR implements ModelInterface{
    private int state;
    
    
    
    @Override
    public String GetInitilizationSignature() {
        return "initialState";
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

    @Override
    public void StateTransition(String[] inputItems) throws CantPerformAction {
        int one, two;
        if(inputItems.length < 2)
            throw new CantPerformAction("not enough inputs");
        try{
            one = Integer.parseInt(inputItems[0]);
        } catch(NumberFormatException e){
            throw new CantPerformAction("Input invalid: \""+inputItems[0]+"\"");
        }
        try{
            two = Integer.parseInt(inputItems[1]);
        } catch(NumberFormatException e){
            throw new CantPerformAction("Input invalid: \""+inputItems[1]+"\"");
        }
        state =  one ^ two;
    }

    @Override
    public String GetStatus() {
        return "state = "+state;
    }

    @Override
    public Object Clone() {
        XOR x = new XOR();
        x.state = this.state;
        return x;
    }

    @Override
    public String[] Tick(String[] args) throws CantPerformAction{
        String[] out = OutPut();
        StateTransition(args);
        return out;
    }
    
}
