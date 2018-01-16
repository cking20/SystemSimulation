/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

import java.math.BigDecimal;
import java.util.ArrayList;

/**
 *
 * @author chrisrk192
 */
public class SimulationFramework<I, O>{
    boolean consoleMode = false;
    boolean debugMode = false;
    private EventModelInterface<I,O> theEventModel;
    private EventNetwork<I,O> theEventNetwork;
    private final ArrayList<I> theCmds; 
    private final ArrayList<TwoDTime> theCmdsTime;
        
    SimulationFramework(){
        debugMode = false;
        theCmds = new ArrayList<>();
        theCmdsTime = new ArrayList<>();
    }
    
    SimulationFramework(boolean inDebugMode){
        debugMode = inDebugMode;
        theCmds = new ArrayList<>();
        theCmdsTime = new ArrayList<>();
    }
    
    public void SetDiscreteEventModel(EventModelInterface m){
        theEventModel = m;
    }
    
    public void SetDiscreteEventNetwork(EventNetwork m){
        theEventNetwork = m;
    }
    
    public void AddInput(I s){
        synchronized(theCmds){
        theCmds.add(s);
        TwoDTime t = new TwoDTime(0, 0);
        theCmdsTime.add(t);
        }
    }
    
    public void AddInput(double realTime, I s){
        synchronized(theCmds){
        theCmds.add(s);
        }
        synchronized(theCmdsTime){
        TwoDTime t = new TwoDTime(realTime);
        theCmdsTime.add(t);
        }
    }
    
    public void AddInput(double realTime, int discreteTime, I s){
        synchronized(theCmds){
        theCmds.add(s);
        }
        synchronized(theCmdsTime){
        TwoDTime t = new TwoDTime(realTime, discreteTime);
        theCmdsTime.add(t);
        }
    }
    
    public String GetStatus(){
        return theEventNetwork.GetStatus();
    }
    public void ClearInput(){
        theCmds.clear();
        theCmdsTime.clear();
    }
       
    //assume input already added
    public ArrayList<OutputTimePairs<O>> RunEventSimulation(){
        if(theEventModel == null && theEventNetwork == null) {
            throw new RuntimeException("No Network nor Model Added");
        }        
        if(theEventNetwork == null){
            theEventNetwork = new EventNetwork<>();
            theEventNetwork.Add(theEventModel);
            theEventNetwork.Bind_II(theEventModel);
            theEventNetwork.Bind_OO(theEventModel);
        } 
        if(theEventNetwork.bindings.isEmpty()){
            throw new RuntimeException("Network not binded");
        }
        if(theEventNetwork.getExternInputModels().isEmpty()){
            throw new RuntimeException("Network Input not binded");
        }
        if(theEventNetwork.getExternOutputModels().isEmpty()){
            throw new RuntimeException("Network Output not binded");
        }
        if (theEventNetwork.IsNetworkInvalid()) {
            throw new RuntimeException("Network bingings are of invalid types");
        }
       
        ArrayList<OutputTimePairs<O>> theoutput = new ArrayList<>();
        EventScheduler<I> schedule = new EventScheduler<>();
        TwoDTime current2Dtime = new TwoDTime(0, 0);       
        //for each model queue internal event
        for(int i = 0; i < theEventNetwork.atomics.size(); i++){
            Event<I> e = new Event<>();
            e.model = theEventNetwork.atomics.get(i);
            e.typeOfEvent = Event.type.internal;          
            if(Double.isFinite(e.model.TimeAdvance()))
                e.myTime = new TwoDTime(current2Dtime.getRealTime().add(
                        BigDecimal.valueOf(e.model.TimeAdvance())));
            else
                e.myTime = TwoDTime.INFINITE;
            e.timeQueued = current2Dtime;
            schedule.Add(e);
        }
        //queue event for each input
        for (int i = 0; i < theCmds.size(); i++) {         
            //send input to each thing hooked up to I of network
            ArrayList<EventModelInterface> ins = theEventNetwork.getExternInputModels();
            for (int j = 0; j < ins.size(); j++) {
                Event<I> e = new Event<>();
                e.input = theCmds.get(i);
                e.model = ins.get(j);
                e.myTime = theCmdsTime.get(i);            
                e.typeOfEvent = Event.type.external;
                e.timeQueued = current2Dtime;
                schedule.Add(e);
            }          
        }
        if(debugMode) System.out.println(schedule.Peek().myTime.getRealTime());
        while(!schedule.isEmpty() && schedule.Peek().myTime != TwoDTime.INFINITE){
            //O o = null;
            Event even = schedule.Pop();
            if(debugMode) System.out.println("-------------------------------Currently working with event"+even.Print());
            schedule.Print();
            try{
                switch(even.typeOfEvent){
                    //ext
                    case external:                    
                        current2Dtime = even.myTime;                   
                        //remove internals for that model
                        //get the time the old int event was queued then sub and thats e
                        Event oldInternalEvent = schedule.DeleteOldInternals(even.model);
                        double ee = 0;
                        if(oldInternalEvent != null){
                            ee = current2Dtime.getRealTime().doubleValue() - oldInternalEvent.timeQueued.getRealTime().doubleValue();
                        }
                        //run model ext
                        even.model.StateTransitionExt(even.input,ee);
                        if(debugMode) System.out.println("EXT Debug: "+even.model+" "+even.model.GetStatus());
                        //queue new internal event
                        Event<I> newEven = new Event<>();
                        newEven.model = even.model;
                        if(Double.isFinite(newEven.model.TimeAdvance()))
                            newEven.myTime = new TwoDTime(current2Dtime.getRealTime().add(
                                    BigDecimal.valueOf(newEven.model.TimeAdvance())));
                        else
                            newEven.myTime = TwoDTime.INFINITE;
                        newEven.typeOfEvent = Event.type.internal;
                        newEven.timeQueued = current2Dtime;
                        schedule.Add(newEven);
                        break;
                    //int
                    case internal:
                        current2Dtime = even.myTime;                   
                        //remove internals for that model(dont think i need to) 
                        //produce output
                        /////////////////////////////////////////////////o = (O)even.model.OutPut();
                        //send this output to the appropriate other models
                        if(theEventNetwork.isExternOutputModel(even.model)){
                            theoutput.add(new OutputTimePairs<>(current2Dtime,(O)even.model.OutPut()));                        
                        }//not an external output, add ext events for others bound to
                            ArrayList<EventModelInterface> boundTo = theEventNetwork.getModelsOutputBoundTo(even.model);
                            for (int i = 0; i < boundTo.size(); i++) {
                                Event<I> newE = new Event<>();
                                newE.model = boundTo.get(i);
                                newE.myTime = current2Dtime.Advance(new TwoDTime(0, 0));

                                newE.typeOfEvent = Event.type.external;
                                /////////////////////////newE.input = (I)o;
                                newE.input = (I)even.model.OutPut();
                                newE.timeQueued = current2Dtime;
                                schedule.Add(newE);
                            }
                        //run int
                        even.model.StateTransitionInt();
                        if(debugMode) System.out.println("INT Debug: "+even.model+" "+even.model.GetStatus());
                        //queue new int
                        if(Double.isFinite(even.model.TimeAdvance()))
                            even.myTime = new TwoDTime(current2Dtime.getRealTime().add(
                                    BigDecimal.valueOf(even.model.TimeAdvance())));
                        else
                            even.myTime = TwoDTime.INFINITE;
                        even.timeQueued = current2Dtime;
                        schedule.Add(even);
                        break;
                    //con
                    case confluent:
                        current2Dtime = even.myTime;                  
                        //remove internals for that model(dont think i need to) 
                        //produce output
                        //o = (O)even.model.OutPut();
                        //send this output to the appropriate other models
                        //external output
                        if(theEventNetwork.isExternOutputModel(even.model)){ 
                            theoutput.add(new OutputTimePairs<>(current2Dtime,(O)even.model.OutPut()));
                        }// else {//not external output
                            ArrayList<EventModelInterface> boundTo2 = theEventNetwork.getModelsOutputBoundTo(even.model);
                            for (int i = 0; i < boundTo2.size(); i++) {
                                Event<I> newE = new Event<>();
                                newE.model = boundTo2.get(i);
                                newE.myTime = current2Dtime.Advance(new TwoDTime(0, 0));                           
                                newE.typeOfEvent = Event.type.external;
                                newE.input = (I)even.model.OutPut();
                                newE.timeQueued = current2Dtime;
                                schedule.Add(newE);
                            }
                        //}
                        //run int
                        even.model.StateTransitionCon(even.input);
                        if(debugMode) System.out.println("CON Debug: "+even.model+" "+even.model.GetStatus());
                        //queue new int
                        even.typeOfEvent = Event.type.internal;
                        if(Double.isFinite(even.model.TimeAdvance()))
                            even.myTime = new TwoDTime(current2Dtime.getRealTime().add(
                                    BigDecimal.valueOf(even.model.TimeAdvance())));
                        else
                            even.myTime = TwoDTime.INFINITE;
                        even.timeQueued = current2Dtime;
                        schedule.Add(even);
                        break;
                }
            }catch(CantPerformAction e){
                System.err.println(e.getMessage());
            }            
            if(debugMode) System.out.println("Done with this event\n");
        }
        return theoutput;
    }
}
