/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw5;

import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 *
 * @author chrisrk192
 */
public class SimulationFramework<I, O> implements Runnable{
    //static String DELIM = ", ";
    boolean consoleMode = false;
    boolean debugMode = false;
    private boolean eventMode = true;
    boolean paused = false;
    boolean tickOnEmptyInput = true;
    public boolean producedOutput = false;
    public long tickTime = 1000;
    private double currentTime = 0.0;
    private ModelInterface theModel;
    private EventModelInterface<I,O> theEventModel;
    private EventNetwork<I,O> theEventNetwork;
    private final ArrayList<I> theCmds; 
    private final ArrayList<TwoDTime> theCmdsTime;
    private final ArrayList<O> theOutputs;
    private final ArrayList<TwoDTime> theOutputsTime;
    
    //private int numModels;
    private volatile boolean keepSimulating = true;
    //private ModelInterface instance;
    
    SimulationFramework(boolean inEventMode, boolean inDebugMode,boolean tickOnEmpty, boolean startPaused,long deltaTime){
        //instance = m;   
        eventMode = inEventMode;
        tickTime = deltaTime; 
        debugMode = inDebugMode;
        tickOnEmptyInput = tickOnEmpty;
        paused = startPaused;
        //theModels = new ArrayList<>();
        theCmds = new ArrayList<>();
        theOutputs = new ArrayList<>();
        theCmdsTime = new ArrayList<>();
        theOutputsTime = new ArrayList<>();
    }
    
    public void SetDiscreteTimeModel(ModelInterface m){
        //theModels.add(m);
        theModel = m;
    }
    public void SetDiscreteEventModel(EventModelInterface m){
        //theModels.add(m);
        theEventModel = m;
    }
    public void SetDiscreteEventNetwork(EventNetwork m){
        //theModels.add(m);
        theEventNetwork = m;
    }
    public String GetDiscreteTimeModelStatus(){
        return theModel.GetStatus();
    }
    public void AddInput(I s){
        synchronized(theCmds){
        theCmds.add(s);
        }
    }
    public void AddInput(TwoDTime t, I s){
        synchronized(theCmds){
        theCmds.add(s);
        }
        synchronized(theCmdsTime){
        theCmdsTime.add(t);
        }
    }
    public ArrayList<OutputTimePairs<O>> FlushOutputs(){
        ArrayList<OutputTimePairs<O>> ret = new ArrayList<>();
        synchronized(theOutputs){
            synchronized(theOutputsTime){
                for (int i = 0; i < theOutputs.size() && i < theOutputsTime.size(); i++) {
                   ret.add(new OutputTimePairs<>(theOutputsTime.get(i),theOutputs.get(i)));
                }
            
            theOutputs.clear();
            theOutputsTime.clear();
            producedOutput = false;
            }
        }      
        return ret;
    }
    public void Stop(){
        keepSimulating = false;
    }
    
    //assume input already added, model added
    public ArrayList<OutputTimePairs<O>> RunEventSimulation(){
        ArrayList<OutputTimePairs<O>> theoutput = new ArrayList<>();
        EventScheduler<I> schedule = new EventScheduler<>();
        TwoDTime current2Dtime = new TwoDTime(0, 0);
        
        //for each model queue internal event
        for(int i = 0; i < theEventNetwork.atomics.size(); i++){
            Event<I> e = new Event<>();
            e.model = theEventNetwork.atomics.get(i);
            e.typeOfEvent = Event.type.internal;
            e.myTime = new TwoDTime(e.model.TimeAdvance()+current2Dtime.getRealTime());
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
                //System.out.println("queued input @ "+e.myTime.getRealTime());
                e.typeOfEvent = Event.type.external;
                e.timeQueued = current2Dtime;
                schedule.Add(e);
            }
            
        }
        theCmds.clear();
        theCmdsTime.clear();
        theOutputs.clear();
        theOutputsTime.clear();
        System.out.println(schedule.Peek().myTime.getRealTime());
        //double e;
        while(!schedule.isEmpty() && Double.isFinite(schedule.Peek().myTime.getRealTime())){
            
            //pop event
            O o = null;
            Event even = schedule.Pop();
            System.out.println("-------------------------------Currently working with event"+even.Print());
            schedule.Print();
            //switch type
            try{
                //e = even.myTime.getRealTime() - current2Dtime.getRealTime();
            switch(even.typeOfEvent){
                //ext
                case external:
                    
                    current2Dtime = even.myTime;
                    //System.out.println("ext evnt @ current Time: "+current2Dtime.getRealTime()+" e = "+(even.myTime.getRealTime()-even.timeQueued.getRealTime()));
                    //remove internals for that model
                    //get the time the old int event was queued then sub and thats e
                    Event oldInternalEvent = schedule.DeleteOldInternals(even.model);
                    double ee = 0;
                    if(oldInternalEvent != null){
                        ee = current2Dtime.getRealTime() - oldInternalEvent.timeQueued.getRealTime();
                    }
                    //run model ext
                    even.model.StateTransitionExt(even.input,ee);
                    System.out.println("EXT Debug: "+even.model+" "+even.model.GetStatus());
                    //queue new internal event
                    Event<I> newEven = new Event<>();
                    newEven.model = even.model;
                    newEven.myTime = current2Dtime.Advance(new TwoDTime(even.model.TimeAdvance(), 0));
                    //System.out.println("queue newEven.time = "+newEven.myTime.getRealTime());
                    newEven.typeOfEvent = Event.type.internal;
                    newEven.timeQueued = current2Dtime;
                    schedule.Add(newEven);
                    break;
                //int
                case internal:
                    current2Dtime = even.myTime;
                    //System.out.println("int evnt @ current Time: "+current2Dtime.getRealTime());
                    //remove internals for that model(dont think i need to)
                    //schedule.DeleteOldInternals(even.model);
                    //produce output
                    o = (O)even.model.OutPut();
                    //send this output to the appropriate other models
                    if(theEventNetwork.isExternOutputModel(even.model)){
                        //System.out.println("ADDING OUTPUT ----------------------------------------------------------");
                        theOutputs.add(o);
                        theOutputsTime.add(current2Dtime);
                        theoutput.add(new OutputTimePairs<>(current2Dtime,o));                        
                    }// else {//not an external output, add ext events for others bound to
                        ArrayList<EventModelInterface> boundTo = theEventNetwork.getModelsOutputBoundTo(even.model);
                        for (int i = 0; i < boundTo.size(); i++) {
                            Event<I> newE = new Event<>();
                            newE.model = boundTo.get(i);
                            newE.myTime = current2Dtime.Advance(new TwoDTime(0, 0));
                            //System.out.println("queue newEven.time = "+newE.myTime.getRealTime());
                            newE.typeOfEvent = Event.type.external;
                            newE.input = (I)o;
                            newE.timeQueued = current2Dtime;
                            schedule.Add(newE);
                        }
                    //}
                    //run int
                    even.model.StateTransitionInt();
                    System.out.println("INT Debug: "+even.model+" "+even.model.GetStatus());
                    //queue new int
                    even.myTime = current2Dtime.Advance(new TwoDTime(even.model.TimeAdvance(), 0));
                    even.timeQueued = current2Dtime;
                    schedule.Add(even);
                    break;
                //con
                case confluent:
                    current2Dtime = even.myTime;
                    //System.out.println("con evnt @ current Time: "+current2Dtime.getRealTime());
                    //remove internals for that model(dont think i need to) 
                    //schedule.DeleteOldInternals(even.model);
                    //produce output
                    o = (O)even.model.OutPut();
                    //send this output to the appropriate other models
                    //external output
                    if(theEventNetwork.isExternOutputModel(even.model)){ 
                        theOutputs.add(o);
                        theOutputsTime.add(current2Dtime);
                        theoutput.add(new OutputTimePairs<>(current2Dtime,o));
                    }// else {//not external output
                        ArrayList<EventModelInterface> boundTo2 = theEventNetwork.getModelsOutputBoundTo(even.model);
                        for (int i = 0; i < boundTo2.size(); i++) {
                            Event<I> newE = new Event<>();
                            newE.model = boundTo2.get(i);
                            newE.myTime = current2Dtime.Advance(new TwoDTime(0, 0));
                            //System.out.println("queue newEven.time = "+newE.myTime.getRealTime());
                            newE.typeOfEvent = Event.type.external;
                            newE.input = (I)o;
                            newE.timeQueued = current2Dtime;
                            schedule.Add(newE);
                        }
                    //}
                    //run int
                    even.model.StateTransitionCon(even.input);
                    System.out.println("CON Debug: "+even.model+" "+even.model.GetStatus());
                    //queue new int
                    even.typeOfEvent = Event.type.internal;
                    even.myTime = current2Dtime.Advance(new TwoDTime(even.model.TimeAdvance(), 0));
                    even.timeQueued = current2Dtime;
                    schedule.Add(even);
                    break;
            }
            }catch(CantPerformAction e){
                System.err.println(e.getMessage());
            }
            
            System.out.println("Done with this event\n");
        }
        return theoutput;
    }
    
    @Override
    public void run(){ 
        if(eventMode){// discrete event simulation
            while (keepSimulating){//!Thread.interrupted())
                //System.out.println("simulating thread");
                try{
                    if (!theCmds.isEmpty() && !paused) {
                        I cmd;
                        TwoDTime nextCmdTime;
                        synchronized (theCmds) {
                            cmd = theCmds.remove(0);
                        }
                        synchronized (theCmdsTime) {
                            nextCmdTime = theCmdsTime.remove(0);
                        }

                        O out = null;//must be 
                        //get time of next auto model event
                        double timeto = currentTime + theEventModel.TimeAdvance();
                        if(nextCmdTime.getRealTime() < timeto){//external
                            double e = nextCmdTime.getRealTime() - currentTime;
                            currentTime = nextCmdTime.getRealTime();                            
                            theEventModel.StateTransitionExt(cmd, e);
                            
                        }else if(nextCmdTime.getRealTime() > timeto){//internal
                            synchronized (theCmds) {
                                theCmds.add(0, cmd);
                                synchronized (theCmdsTime) {
                                    theCmdsTime.add(0,nextCmdTime);
                                }
                            }
                            currentTime = timeto;
                            out = theEventModel.OutPut();
                            theEventModel.StateTransitionInt();

                        } else {//confluent
                            currentTime = timeto;
                            out = theEventModel.OutPut();
                            theEventModel.StateTransitionCon(cmd);

                        }
                        synchronized (theOutputs){
                            //if(out != null){
                                synchronized (theOutputsTime){
                                    TwoDTime outT = new TwoDTime(currentTime);
                                    theOutputsTime.add(outT);
                                }
                                theOutputs.add(out);
                                producedOutput = true;
                            //}
                            if(debugMode){
                                synchronized (theOutputsTime){
                                    TwoDTime outT = new TwoDTime(currentTime);
                                    theOutputsTime.add(outT);
                                }
                                O debug;// = new String[1];
                                //debug = theEventModel.GetStatus();
                                //theOutputs.add(debug);
                            }
                            
                        }               
                    } else if(theCmds.isEmpty() && !paused){
                        keepSimulating = false;
                        O out;
                        currentTime = currentTime + theEventModel.TimeAdvance();
                        out = theEventModel.OutPut();
                        theEventModel.StateTransitionInt();
                        synchronized (theOutputs){
                            synchronized (theOutputsTime){
                                TwoDTime outT = new TwoDTime(currentTime);
                                theOutputsTime.add(outT);
                            }
                            theOutputs.add(out);
                            producedOutput = true;

                            if(debugMode){
                                synchronized (theOutputsTime){
                                    TwoDTime outT2 = new TwoDTime(currentTime);
                                    theOutputsTime.add(outT2);
                                }
                                O debug;// = new String[1];
                                //debug = theEventModel.GetStatus();
                                //theOutputs.add(debug);
                            }
                        }
                    }
                } catch(CantPerformAction n){System.err.println(n);} 
                try {
                    Thread.sleep(0);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            
        }/*else
            
            while (keepSimulating){//discrete time sim
                //System.out.println("simulating thread");
                if (!theCmds.isEmpty() && !paused) {
                    String[] cmd;
                    synchronized (theCmds) {
                        cmd = theCmds.remove(0);
                    }
                    String[] out = Tick(theModel, cmd);
                    synchronized (theOutputs){
                        theOutputs.add(out);
                        producedOutput = true;
                    }
                    if(debugMode){
                        String[] debug = new String[1];
                        debug[0] = theModel.GetStatus();
                            theOutputs.add(debug);//System.out.println(theModel.GetDiscreteTimeModelStatus());
                    }
                }else if(tickOnEmptyInput && !paused){//dont tick if there isnt any input
                    String[] out = Tick(theModel, new String[0]);
                    synchronized (theOutputs){
                        theOutputs.add(out);
                        producedOutput = true;
                    }
                    if(debugMode){
                        String[] debug = new String[1];
                        debug[0] = theModel.GetStatus();
                            theOutputs.add(debug);//System.out.println(theModel.GetDiscreteTimeModelStatus());
                    }
                }
                try {
                    Thread.sleep(tickTime);
                } catch (InterruptedException even) {
                    even.printStackTrace();
                }

            } 
            */
    }
    
    private String[] Tick(ModelInterface m, String[] input){
        try{
            String[] outputs = m.Tick(input);
            return outputs;
        }catch(CantPerformAction e){
            System.err.println(e);
        }
        return new String[0];
    }
    /*       
    public String LoadCommandsFromFile(String fileName){
        String ret = "";
        File cmdsFile = new File(fileName);
        try (Scanner inCmdsFileScanner = new Scanner(cmdsFile)) {
            int cnt;
            cnt = 0;
            while (inCmdsFileScanner.hasNext()) {
                String line = inCmdsFileScanner.nextLine();
                String[] cmdsInLine = line.split(DELIM);
                theCmds.add(cmdsInLine);
                cnt++;
            }
            //System.out.println("Done loading, " + cnt + " commands loaded. ");
            if (cnt == 0) {
                ret += "Warning No cmds Loaded.\n";
            } else
                ret += cnt + " command lines loaded.\n";
            inCmdsFileScanner.close();
        } catch(IOException even){
            ret+= "Warning No cmds Loaded. IOException error 001\n";
        }
        return ret;
    }
    */
   
}
