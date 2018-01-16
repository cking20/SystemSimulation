/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw3;

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
    boolean paused = false;
    boolean tickOnEmptyInput = true;
    public boolean producedOutput = false;
    public long tickTime = 1000;
    private ModelInterface theModel;
    private final ArrayList<String[]> theCmds; 
    private final ArrayList<String[]> theOutputs;
    //private int numModels;
    private volatile boolean keepSimulating = true;
    //private ModelInterface instance;
    
    SimulationFramework(boolean inDebugMode,boolean tickOnEmpty, boolean startPaused,long deltaTime){
        //instance = m;      
        tickTime = deltaTime; 
        debugMode = inDebugMode;
        tickOnEmptyInput = tickOnEmpty;
        paused = startPaused;
        //theModels = new ArrayList<>();
        theCmds = new ArrayList<>();
        theOutputs = new ArrayList<>();
    }
    
    public void SetModel(ModelInterface m){
        //theModels.add(m);
        theModel = m;
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
        while (keepSimulating){//!Thread.interrupted())
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
            //String out = "OutPut: ";
            String[] outputs = m.Tick(input);
            //for(int i = 0; i < outputs.length;i++){
              //  out += outputs[i];
            //}
            //System.out.println(out);//need to pass in an array of strings to be processesed this tick
            
            return outputs;
            //m.StateTransition(input);
        }catch(CantPerformAction e){
            System.err.println(e);
        }
        return new String[0];
    }
        
        
    public String LoadCommandsFromFile(String fileName){
        //ArrayList<String[]> cmds = new ArrayList<>();
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
    
   

    
    /*
    private class Simulator implements Runnable{
        
        public boolean keepRunning;

        public Simulator() {
            //deltaTime = d;
            keepRunning = true;
        }

        @Override
        public void run() {
            while(keepRunning){  
                System.out.println("simulating");
                 
                    
                try {
                    Thread.sleep(tickTime);
                } catch (InterruptedException ex) {
                    Logger.getLogger(SimulationFramework.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }*/
    
}
