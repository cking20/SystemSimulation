/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw3;

import static csc454hw3.SimulationFramework.DELIM;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author chrisrk192
 */
public class CSC454HW3 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        Network xorNet = CreateXORNet();        
        Network rule = CreateRule110(32);
        
        //System.out.println(rule.GetStatus());
        
        SimulationFramework smfw;
        smfw = new SimulationFramework(false,true,true,2000);
        smfw.SetModel(xorNet);
        
        Thread theFrameWorkThread = new Thread(smfw);
        theFrameWorkThread.start();
        
        OutputPrinter op = new OutputPrinter(smfw, true);
        Thread theOutputThread = new Thread(op);
        theOutputThread.start();
        
        String inputLine = "";
        
        Scanner s = new Scanner(System.in); 
        //System.out.println("getting input");
        do{
            
            inputLine = s.nextLine();
            if(inputLine.charAt(0) != '-'){//not a system command
                String[] sa = inputLine.split(DELIM);
                smfw.AddInput(sa);
            }else{
                ProcessInput(inputLine, smfw);
            }
        } while (inputLine.compareTo("-quit") != 0);
        //System.out.println("done redaing input");
        smfw.Stop();
        op.Stop();
    }
    public static void ProcessInput(String s, SimulationFramework f){
        String[] args = s.split(" ");
        if(args[0].compareTo("-togdebug") == 0){
            f.debugMode = !f.debugMode;
        }
        if(args[0].compareTo("-setspeed") == 0){
            f.tickTime = Integer.parseInt(args[1]);
        }
        if(args[0].compareTo("-pause") == 0){
            f.paused = true;
        }
        if(args[0].compareTo("-resume") == 0){
            f.paused = false;
        }
        if(args[0].compareTo("-help") == 0){
            System.out.println("Commands\n"
                    + "-togdebug\n"
                    + "-setspeed SPEED\n"
                    + "-pause\n"
                    + "-resume\n"
                    + "-help\n"
                    + "-loadcmds FILENAME\n"
                    + "-status\n");
        }
        if(args[0].compareTo(":loadcmds") == 0){
            System.out.println(f.LoadCommandsFromFile(args[1]));
        }
        if(args[0].compareTo(":status") == 0){
            System.out.println(f.GetStatus());
        }
        
    }
    public static class OutputPrinter implements Runnable{
        SimulationFramework sim;
        public boolean keepRunning;
        public boolean hideDelimOutput;
        public void Stop(){
            keepRunning = false;
        }
        public OutputPrinter(SimulationFramework s, boolean hide) {
            sim = s;
            keepRunning = true;
            hideDelimOutput = hide;
        }
        
        @Override
        public void run() {
            
            while (keepRunning) {                
                if(sim.producedOutput){
                    ArrayList<String[]> outp = sim.FlushOutputs();
                    //System.out.println("output of size: "+outp.size());
                    if(hideDelimOutput)
                        System.out.print("Output: "+CombineOutput(outp).replace(DELIM, ""));
                    else
                        System.out.print("Output: "+CombineOutput(outp));
                } else
                    try {
                        Thread.sleep(100);
                } catch (InterruptedException ex) {
                    Logger.getLogger(CSC454HW3.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
            
        }
        
    }
    private static String CombineOutput(ArrayList<String[]> a){
        String comb = "";
        String[] temp;
        for (int i = 0; i < a.size(); i++) {
            temp = a.get(i);
            for (int j = 0; j < temp.length; j++) {
                
                if(j == temp.length-1)
                    comb += temp[j];
                else
                    comb += temp[j] + DELIM;
            }
            
            comb += "\n";
        }
        return comb;
    }
    public static Network CreateXORNet(){
        Network theNetwork = new Network();
        XOR x1 = new XOR();
        XOR x2 = new XOR();
        Mem m = new Mem();        
        String[] init = {"0","0"};
        x1.Initialize(init);
        x2.Initialize(init);
        m.Initialize(init);
        init[0] = "3";//internal to external ticks
        theNetwork.Initialize(init);
        theNetwork.Add(x1);//0
        theNetwork.Add(x2);//1
        theNetwork.Add(m);//2       
        theNetwork.Bind_II(0);        
        theNetwork.Bind_OI(0, 1);
        theNetwork.Bind_OI(1, 2);
        theNetwork.Bind_OI(2, 1);        
        theNetwork.Bind_OO(1);
        return theNetwork;
    }
    
    public static Network CreateRule110(int rule110len){
        //int rule110len = 32;
        Network rule = new Network();
        Cell temp;
        String[] initState = new String[1];
        initState[0] = "0";
        for (int i = 0; i < rule110len; i++) {
            if(i != 16){
                temp = new Cell();
                temp.Initialize(initState);
                rule.Add(temp);
            } else{
                temp = new Cell();
                initState[0] = "1";
                temp.Initialize(initState);
                initState[0] = "0";
                rule.Add(temp);
            }
        }
        //add all left couplings first
        rule.Bind_OI(0, rule110len-1);
        for (int i = 1; i < rule110len; i++) {
            rule.Bind_OI(i, i-1);
        }
        //then all right couplings
        for (int i = 0; i < rule110len -1; i++) {
            rule.Bind_OI(i, i+1);
        }
        rule.Bind_OI(rule110len-1, 0);
        //bind all to output
        for (int i = 0; i < rule110len; i++) {
            rule.Bind_OO(i);
        }
        return rule;
    }
}


