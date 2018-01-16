/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

import java.util.ArrayList;

/**
 *
 * @author chrisrk192
 */
public class EventScheduler<Input> {
    private ArrayList<Event<Input>> theQueue;

    public EventScheduler() {
        theQueue = new ArrayList<>();
    }
       
    public boolean isEmpty(){
        return theQueue.isEmpty();
    }
    
    public void Add(Event<Input> e){
        FilterIn(e);
    }
    public Event<Input> Peek(){
        MergeIntExt();
        return theQueue.get(0);
    }
    public Event<Input> Pop(){
        MergeIntExt();
        return theQueue.remove(0);
    }
    
    //further times are at the end
    private void FilterIn(Event<Input> e){
        if(theQueue.isEmpty()){
            theQueue.add(e);
            return;
        }
        for(int i = 0; i < theQueue.size();i++){
            if(theQueue.get(i).myTime.compareTo(e.myTime) > 0){
                theQueue.add(i,e);
                return;
            }
        }   
        theQueue.add(theQueue.size(), e);
    }
    
    //should be called on Add
    public void MergeIntExt(){
        //for each real time merge a model's 
        //internal and external event to a confluent
            //System.out.println("--------------------Before Merge----------------------------");
            //Print();
        
            int j,initialIndex;
            j = 0;
            initialIndex = 0;
            boolean gotInternal = false;
            boolean gotExternal = false;
            Event<Input> iEvent = theQueue.get(initialIndex);
            if(iEvent.typeOfEvent == Event.type.internal){
                gotInternal = true;
            }else if(iEvent.typeOfEvent == Event.type.external){
                gotExternal = true; 
            }
            TwoDTime iTime = iEvent.myTime;
            
            while(j < theQueue.size() && theQueue.get(j).myTime.compareTo(iTime) == 0){
                //if the model is the same, and have internal and external events
                if(iEvent.model == theQueue.get(j).model){
                    if(theQueue.get(j).typeOfEvent == Event.type.internal)
                        gotInternal = true;
                    else if(theQueue.get(j).typeOfEvent == Event.type.external)
                        gotExternal = true; 
                    if(gotExternal && gotInternal){
                        System.out.println("SCHEDULE FOUND CONFLUENT");
                        //System.out.println("initialIndex = "+initialIndex+" j="+j);
                        Event<Input> conEvent = new Event();
                        conEvent.typeOfEvent = Event.type.confluent;
                        conEvent.model = theQueue.get(j).model;
                        conEvent.myTime = theQueue.get(j).myTime;
                        if(theQueue.get(j).typeOfEvent == Event.type.internal)
                            conEvent.timeQueued = theQueue.get(j).timeQueued;
                        else
                            conEvent.input = theQueue.get(j).input;
                        
                        if(theQueue.get(initialIndex).typeOfEvent == Event.type.internal)
                            conEvent.timeQueued = theQueue.get(initialIndex).timeQueued;
                        else
                            conEvent.input = theQueue.get(initialIndex).input;
                            
                        if(initialIndex > j){
                            theQueue.remove(j);
                            theQueue.remove(initialIndex-1);
                        } else{   
                            theQueue.remove(initialIndex);
                            theQueue.remove(j-1);
                        }
                        System.out.println("Filtering In");
                        
                        theQueue.add(0,conEvent);
                        gotExternal = false;
                        gotInternal = false;
                    }
                }  
                j++;
            }
            //System.out.println("--------------------After Merge----------------------------");
            //Print();
    }
    //for deleting old internal events
    public Event DeleteOldInternals(EventModelInterface e){
        Event ret = null;
         for(int i = 0; i < theQueue.size();i++){
             Event<Input> ev = theQueue.get(i);
             if(ev.model == e && ev.typeOfEvent == Event.type.internal){
                ret = theQueue.remove(i);
             }            
         }    
         return ret;
    }
    
    public void Print(){
        for(int i = 0;i < theQueue.size();i++){
            System.out.println(theQueue.get(i).Print());
        }
    
    }
    
}