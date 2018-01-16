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
public class Mem implements ModelInterface {
    int qHead;
    int qTail;
    
    @Override
    public String GetInitilizationSignature() {
        return "initialState1, initialState2";
    }

    @Override
    public void Initialize(String[] args) {
        if(args.length >= 2){
            qHead = Integer.parseInt(args[0]);
            qTail = Integer.parseInt(args[1]);
        }
        else
            qHead = 0; qTail = 0;
    }
    
    @Override
    public String[] Tick(String[] args) throws CantPerformAction{
        String[] out = OutPut();
        StateTransition(args);
        return out;
    }
    
    @Override
    public String[] OutPut() {
        
        String[] s = new String[1];
        s[0] = qHead + "";
        return s;
    }

    @Override
    public void StateTransition(String[] inputItems) throws CantPerformAction {
        
        qHead = qTail;
        qTail = Integer.parseInt(inputItems[0]);
    }

    @Override
    public String GetStatus() {
        return "Queue: " + qHead + ", " + qTail;
    }

    @Override
    public Object Clone() {
        Mem m = new Mem();
        m.qHead = this.qHead;
        m.qTail = this.qTail;
        return m;
    }
    
}
