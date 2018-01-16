/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw4;

import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 *
 * @author chrisrk192
 */
public class SimulationFramework implements Runnable{
    static String DELIM = ", ";
    boolean consoleMode = false;
    boolean debugMode = false;
    private boolean eventMode = true;
    boolean paused = false;
    boolean tickOnEmptyInput = true;
    public boolean producedOutput = false;
    public long tickTime = 1000;
    private double currentTime = 0.0;
    private ModelInterface theModel;
    private EventModelInterface theEventModel;
    private final ArrayList<String[]> theCmds; 
    private final ArrayList<String[]> theOutputs;
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
    }
    
    public void SetDiscreteTimeModel(ModelInterface m){
        //theModels.add(m);
        theModel = m;
    }
    public void SetDiscreteEventModel(EventModelInterface m){
        //theModels.add(m);
        theEventModel = m;
    }
    public String GetStatus(){
        return theModel.GetStatus();
    }
    public void AddInput(String[] s){
        synchronized(theCmds){
        theCmds.add(s);
        }
    }
    
    public ArrayList<String[]> FlushOutputs(){
        ArrayList<String[]> ret;
        synchronized(theOutputs){
            ret = (ArrayList<String[]>)theOutputs.clone();
            theOutputs.clear();
            producedOutput = false;
        }
        
        return ret;
    }
    public void Stop(){
        keepSimulating = false;
    }
    @Override
    public void run(){ 
        if(eventMode)// discrete event simulation
            while (keepSimulating){//!Thread.interrupted())
                //System.out.println("simulating thread");
                try{
                    if (!theCmds.isEmpty() && !paused) {
                        String[] cmd;
                        synchronized (theCmds) {
                            cmd = theCmds.remove(0);
                        }
                        double nextCMDtime;
                        try{
                            nextCMDtime = Double.parseDouble(cmd[0]);
                        } catch(NumberFormatException n){
                            nextCMDtime = 0;
                            System.err.println("Input Malformed: 1st param not a double");
                        }
                        String[] out = new String[0];
                        double timeto = currentTime + theEventModel.TimeAdvance();
                        if(nextCMDtime < timeto){
                            currentTime = nextCMDtime;
                            theEventModel.StateTransitionExt(RemoveFirst(cmd));

                        }else if(nextCMDtime > timeto){
                            synchronized (theCmds) {
                                theCmds.add(0, cmd);
                            }
                            currentTime = timeto;
                            out = theEventModel.OutPut();
                            theEventModel.StateTransitionInt();

                        } else {
                            currentTime = timeto;
                            out = theEventModel.OutPut();
                            theEventModel.StateTransitionCon(RemoveFirst(cmd));

                        }
                        synchronized (theOutputs){
                            String[] timeStamp = new String[1];
                            timeStamp[0] = "Time "+currentTime+": ";
                            theOutputs.add(timeStamp);
                            theOutputs.add(out);
                            producedOutput = true;

                            if(debugMode){
                                String[] debug = new String[1];
                                debug[0] = "Time "+currentTime+": "+theEventModel.GetStatus();
                                    theOutputs.add(debug);
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
        else
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
                            theOutputs.add(debug);//System.out.println(theModel.GetStatus());
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
                            theOutputs.add(debug);//System.out.println(theModel.GetStatus());
                    }
                }
                try {
                    Thread.sleep(tickTime);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

            }           
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
        } catch(IOException e){
            ret+= "Warning No cmds Loaded. IOException error 001\n";
        }
        return ret;
        
       
    }
    
   private String[] RemoveFirst(String[] s){
       String[] newS = new String[s.length-1];
       for(int i = 0; i < newS.length; i++){
           newS[i] = s[i+1];
       }
       return newS;
   } 
}
