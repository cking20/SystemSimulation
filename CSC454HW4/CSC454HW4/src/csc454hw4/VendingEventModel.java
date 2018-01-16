/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw4;

import java.util.ArrayList;

/**
 *
 * @author chrisrk192
 */
public class VendingEventModel implements EventModelInterface{

    static enum inputSet{n, d, q}
    static enum outputSet{n, d, q, coffee} 
    double e = 0.0;
    int qNum,nNum,dNum,vNum;
    public VendingEventModel(){
        qNum = 0;
        nNum = 0; 
        dNum = 0;
        vNum = 0;   
        
    }
    public VendingEventModel(int nq, int nd, int nn, int nv){
        qNum = nq;
        dNum = nd;
        nNum = nn;
        vNum = nv;
    }
    
    @Override
    public String[] OutPut() {
        String output = "";
        //if(vNum >= 100) {    
            // give out coffee
            for(int j = 0; j < vNum / 100; j++){
                if(j == (vNum / 100)-1){
                    output+= outputSet.coffee.name();
                } else{
                    output+= outputSet.coffee.name()+", ";
                }
            }
            //give out change
            ArrayList<outputSet> l = CalcChange(vNum % 100, qNum, dNum, nNum);
            for(int i = 0; i < l.size();i++){
                if(i == l.size()-1){
                    output+= l.get(i).name();
                } else{
                    output+= l.get(i).name()+", ";
                }
            }
            
                
        //}
        String[] so = output.split(", ");
        return so; 
    }

    @Override
    public void StateTransitionExt(String[] inputItems) throws CantPerformAction {
        e = 0.0;
        inputSet s;
        try{
            s = inputSet.valueOf(inputItems[0]);
        } catch(IllegalArgumentException ex){
            throw new CantPerformAction("Bad InputItems");
        }
        switch(s){
            case q:
                qNum++;
                vNum+=25;
                break;
            case n:
                nNum++;
                vNum+=5;
                break;
            case d:
                dNum++;
                vNum+=10;
                break; 
        }
    }
    
    @Override //give back change and or give coffee
    public void StateTransitionInt(){
        e = 0.0;
        //if(vNum >= 100) {
            vNum = vNum % 100;//give out coffee
            ArrayList<outputSet> l = CalcChange(vNum, qNum, dNum, nNum);
            //give out chnage
            for(int i = 0; i < l.size();i++){
               switch(l.get(i)){
                   case q:
                       qNum--;
                       break;
                   case d:
                       dNum--;
                       break;
                   case n:
                       nNum--;
                       break;  
               } 
            }
            vNum = 0;             
        //}        
    }
    @Override
    public void StateTransitionCon(String[] inputItems)throws CantPerformAction{
        StateTransitionInt();
        StateTransitionExt(inputItems);
    }
    
    @Override
    public double TimeAdvance(){
        if(vNum > 0)
            return 2.0;
        return Double.POSITIVE_INFINITY;
    }

    @Override
    public String GetStatus() {
        return    "number of quarters = " +qNum+"\n"
                + "number of dimes    = " +dNum+"\n"
                + "number of nickels  = " +nNum+"\n"
                + "value in machine   = " +vNum+"\n";
               
    }

    
    private ArrayList<outputSet> CalcChange(int value, int numQ, int numD, int numN) {
        ArrayList<outputSet> l = new ArrayList<>();
        boolean changeMadeFlag = false;
        while(value > 0){
            if(value >= 25 && numQ > 0){
                l.add(outputSet.q);
                value -= 25;
                numQ--;
                changeMadeFlag = true;
            }
            if(value >= 10 && numD > 0){
                l.add(outputSet.d);
                value -= 10;
                numD--;
                changeMadeFlag = true;
            }
            if(value >= 5 && numN > 0){
                l.add(outputSet.n);
                value -= 5;
                numN--;
                changeMadeFlag = true;
            }   
            if(numD <= 0 && numQ <= 0 && numN <= 0 || changeMadeFlag == false){
                value = 0;              
            }
            changeMadeFlag = false;               
        }
        return l;
    }
}
