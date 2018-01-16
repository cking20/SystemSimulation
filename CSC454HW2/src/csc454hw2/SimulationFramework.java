/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw2;

import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.TimerTask;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 *
 * @author chrisrk192
 * @param <Model>
 */
public class SimulationFramework<Model extends ModelInterface>{
    static String DELIM = ", ";
    boolean consoleMode = false;
    boolean debugMode = false;
    public double tickTime = 1.0;
    private ArrayList<Model> theModels;
    private ArrayList<String[]> theCmds;
    private int numModels;
    private Model instance;
    
    SimulationFramework(Model m, boolean inConsoleMode, boolean inDebugMode, double deltaTime){
        instance = m;
        consoleMode = inConsoleMode;
        tickTime = deltaTime;
        debugMode = inDebugMode;
    }
    
    private void BuildSimulation(){
        Scanner s = new Scanner(System.in);
        
        if(!consoleMode){
            System.out.println("Enter The Model File Name.\n");
            String modFileName = s.nextLine();
            theModels = LoadModelsFromFile(instance, modFileName);
            
            System.out.println("Enter The Input File Name.\n");
            String inpFileName = s.nextLine();
            theCmds = LoadCommandsFromFile(inpFileName);
        } else {
            theModels = new ArrayList<>();
            System.out.println("Enter the Number Of Models.\n");
            numModels = s.nextInt();
            s.nextLine();
            String tempLine;
            Model tempMod;
            for(int i = 0; i < numModels; i++){
                System.out.println(instance.GetInitilizationSignature());
                
                tempLine = s.nextLine();
                tempMod = (Model)instance.Clone();
                String[] sa = tempLine.split(DELIM);
                
                tempMod.Initialize(sa);
                theModels.add(tempMod);
            }
            theCmds = new ArrayList<>();
        }        
    }
    
    public void RunSimulation(){
        String inputLine = "";
        Scanner s = new Scanner(System.in);
        
        BuildSimulation();
        while(!theCmds.isEmpty()){
            for(Model m : theModels){
                if(!consoleMode){
                    String cmdList[] = theCmds.get(0);
                    for(int i = 0; i < cmdList.length; i++){
                        if(i < cmdList.length-1)
                            System.out.print(cmdList[i]+", ");
                        else
                            System.out.println(cmdList[i]+"\n");
                    }
                }
                Tick(m, theCmds.get(0));
                if(debugMode)
                    System.out.println(m.GetStatus());
            }
            theCmds.remove(0);
        }        
        do {            
            inputLine = s.nextLine();
            String[] sa = inputLine.split(DELIM);
            theCmds.add(sa);
            
            while(!theCmds.isEmpty()){
            for(Model m : theModels){
                Tick(m, theCmds.get(0));
                if(debugMode)
                    System.out.println(m.GetStatus());
            }
            theCmds.remove(0);
        }
        } while (inputLine.compareTo("quit") != 0);
        
        //simulate until theCmds is empty
        //then ask for input
    }
    
    private void Tick(Model m, String[] input){
        try{
            System.out.println("OutPut: "+m.OutPut());//need to pass in an array of strings to be processesed this tick
            m.StateTransition(input);
        }catch(CantPerformAction e){
            System.out.println(e);
        }
    }
        
    public ArrayList<String[]> LoadCommandsFromFile(String fileName){
        ArrayList<String[]> cmds = new ArrayList<>();
        File cmdsFile = new File(fileName);
        try (Scanner inCmdsFileScanner = new Scanner(cmdsFile)) {
            int cnt;
            
            
            cnt = 0;
            while (inCmdsFileScanner.hasNext()) {
                String line = inCmdsFileScanner.nextLine();
                String[] cmdsInLine = line.split(DELIM);
                cmds.add(cmdsInLine);
                cnt++;
            }
            
            //System.out.println("Done loading, " + cnt + " commands loaded. ");
            if (cnt == 0) {
                System.err.println("Warning No cmds Loaded. ");
            }
        } catch(IOException e){
            System.err.println("Warning No cmds Loaded. IOException error 001");
        }
        return cmds;
       
    }
    public ArrayList<Model> LoadModelsFromFile(Model instance,String fileName){
        ArrayList<Model> mods = new ArrayList<>();
        File modsFile = new File(fileName);
        try (Scanner inCmdsFileScanner = new Scanner(modsFile)) {
            int cnt;
            
            Model tempMod = (Model) instance.Clone();
            
            String tempLine;
            
            cnt = 0;
            while (inCmdsFileScanner.hasNext()) {
                tempLine = inCmdsFileScanner.nextLine();
                tempMod = (Model)tempMod.Clone();
                tempMod.Initialize(tempLine.split(DELIM));
                mods.add(tempMod);
                cnt++;
            }
            
            //System.out.println("Done loading, " + cnt + " models loaded. ");
            if (cnt == 0) {
                System.err.println("Warning No Contacts Loaded. ");
            }
        } catch(IOException e){
            System.err.println("Warning No Contacts Loaded. IOException error 002");
            e.printStackTrace();
        }
        return mods;
       
    }
    public static void SaveLog(ArrayList<String> outList, String logFileName) 
            throws IOException{
        File logFile = new File(logFileName);
        FileWriter outFileWriter = new FileWriter(logFile, false);
        
        PrintWriter outPrintWriter = new PrintWriter(outFileWriter, true);
        for(int cnt = 0; cnt < outList.size(); cnt++){
            outPrintWriter.println(outList.get(cnt));
        }
        outPrintWriter.close();
        
    }
    
    
}
