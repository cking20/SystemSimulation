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
public class VendingMachine implements ModelInterface{
    static String initSig = "\"number of quarters(0-100)\", \"number of dimes(0-100)\","
            + " \"number of nickles(0-100)\", \"initial value in machine(0-10000)\","
            + " \"cancel pushed(t/f)\"";
    
    static enum inputSet{nickel, dime, quarter, cancel, wait}
    static enum outputSet{nickel, dime, quarter, coffee, nothing} 
    int numQuarters = 0, numNickels = 0, numDimes = 0, totalVal = 0;
    boolean cancelPushed = false;
    
    @Override
    public String GetInitilizationSignature() {
        return initSig;
    }  
    @Override
    public void Initialize(String[] args) {
        
        numQuarters = Integer.parseInt(args[0]);
        numDimes = Integer.parseInt(args[1]);
        numNickels = Integer.parseInt(args[2]);
        totalVal = Integer.parseInt(args[3]);
        cancelPushed = args[4].compareTo("t") == 0;
    }
    @Override
    public String[] OutPut(){
        String output = "";
        if(cancelPushed){            
                ArrayList<outputSet> l = CalcChange(totalVal, numQuarters, numDimes, numNickels);
                for(int i = 0; i < l.size();i++){
                    if(i == l.size()-1){
                        output+= l.get(i).name();
                    } else{
                        output+= l.get(i).name()+", ";
                    }
                }
            
        }else if(totalVal >= 100) {
            for(int j = 0; j < totalVal / 100; j++){
                if(j == (totalVal / 100)-1){
                    output+= outputSet.coffee.name();
                } else{
                    output+= outputSet.coffee.name()+", ";
                }
            }
                
        }
        if(output.compareTo("") ==0){
            output+=outputSet.nothing.name();
        }
        String[] so = output.split(", ");
        return so;       
    }
    
    @Override
    public void StateTransition(String[] inputItems) throws CantPerformAction{ 
        if(cancelPushed){
           try{
                ArrayList<outputSet> l = CalcChange(totalVal, numQuarters, numDimes, numNickels); 
                for(int i = 0; i < l.size();i++){
                    switch(l.get(i)){
                        case quarter:
                            numQuarters--;
                            totalVal-=25;
                            break;
                        case nickel:
                            numNickels--;
                            totalVal-=5;
                            break;
                        case dime:
                            numDimes--;
                            totalVal-=10;
                            break;                        
                    }
                }
                if(totalVal > 0)
                    throw new CantPerformAction("Cant make all change.");
            }catch(CantPerformAction e){
               cancelPushed = false;
               throw new CantPerformAction(e.getMessage());
            }
            cancelPushed = false;
        }else if(totalVal >= 100) {
            int initVal = totalVal / 100;
            for(int j = 0; j < initVal; j++){               
                totalVal -= 100;
            }
        }
    for(int n = 0; n < inputItems.length; n++){
            inputSet s;
            try{
                s = inputSet.valueOf(inputItems[n]);
            } catch(IllegalArgumentException e){
                s = inputSet.wait;
            }
            switch(s){
                case quarter:
                    numQuarters++;
                    totalVal+=25;
                    break;
                case nickel:
                    numNickels++;
                    totalVal+=5;
                    break;
                case dime:
                    numDimes++;
                    totalVal+=10;
                    break; 
                case cancel:
                    cancelPushed = true;
                    break;
                case wait:
                    break;
            }
        }
    }
    
    @Override
    public String[] Tick(String[] args) throws CantPerformAction{
        String[] out = OutPut();
        StateTransition(args);
        return out;
    }
    
    @Override
    public String GetStatus(){
        return    "number of quarters = " +numQuarters+"\n"
                + "number of dimes    = " +numDimes+"\n"
                + "number of nickels  = " +numNickels+"\n"
                + "value in machine   = " +totalVal+"\n"
                + "cancel pushed      = " +cancelPushed+"\n";
    }
    
    @Override
    public Object Clone() {
        VendingMachine v = new VendingMachine();
        v.numDimes = numDimes;
        v.numNickels = numNickels;
        v.numQuarters = numQuarters;
        v.totalVal = totalVal;
        v.cancelPushed = cancelPushed;
        return v;
    }
    private ArrayList<outputSet> CalcChange(int value, int numQ, int numD, int numN) {
        ArrayList<outputSet> l = new ArrayList<>();
        boolean changeMadeFlag = false;
        while(value > 0){
            if(value >= 25 && numQ > 0){
                l.add(outputSet.quarter);
                value -= 25;
                numQ--;
                changeMadeFlag = true;
            }
            if(value >= 10 && numD > 0){
                l.add(outputSet.dime);
                value -= 10;
                numD--;
                changeMadeFlag = true;
            }
            if(value >= 5 && numN > 0){
                l.add(outputSet.nickel);
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
