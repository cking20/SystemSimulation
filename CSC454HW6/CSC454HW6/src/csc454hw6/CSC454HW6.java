/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

//import static csc454hw4.SimulationFramework.DELIM;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author chrisrk192
 */
public class CSC454HW6 {

    /**
     * @param args the command line arguments
     */
    static String DELIM = ", ";
    public static void main(String[] args) {
        
        //Network xorNet = CreateXORNet();        
        //Network rule = CreateRule110(32);    
        
        SimulationFramework<Integer,Integer> smfw;
        smfw = new SimulationFramework(true,true,true,true,2000);
        //smfw.SetDiscreteTimeModel(xorNet);
        //smfw.SetDiscreteEventModel(new VendingEventModel());
        smfw.SetDiscreteEventNetwork(CreateDrillPressEventNetwork());
        //Thread theFrameWorkThread = new Thread(smfw);
        //theFrameWorkThread.start();
        
        //OutputPrinter op = new OutputPrinter(smfw, true);
        //Thread theOutputThread = new Thread(op);
        //theOutputThread.start();
        
        String inputLine = "";
        
        Scanner s = new Scanner(System.in); 
        //System.out.println("getting input");
        do{          
            inputLine = s.nextLine();
            if(inputLine.charAt(0) != '-'){//not a system command
                String[] sa = inputLine.split(DELIM);
                //event mode specific
                TwoDTime t = new TwoDTime(Double.parseDouble(sa[0]), Integer.parseInt(sa[1]));
                sa = RemoveTimeData(sa);
                //only acccpets one int per line
                smfw.AddInput(t, Integer.parseInt(sa[0]));
            }else{
                ProcessInput(inputLine, smfw);
            }
        } while (inputLine.compareTo("-quit") != 0);
        //System.out.println("done redaing input");
        //smfw.Stop();
        //op.Stop();
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
        if(args[0].compareTo("-run") == 0){
            String o = CombineOutput(f.RunEventSimulation());
            System.out.println(o);
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
        if(args[0].compareTo("-loadcmds") == 0){
            LoadCommandsFromFile(f,args[1]);
        }
        if(args[0].compareTo("-status") == 0){
            System.out.println(f.GetDiscreteTimeModelStatus());
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
            /*
            while (keepRunning) {                
                if(sim.producedOutput){
                    ArrayList<OutputTimePairs<String[]>> outp = sim.FlushOutputs();
                    if(outp == null)
                        System.err.println("outp is null");
                    //System.out.println("output of size: "+outp.size());
                    if(hideDelimOutput)
                        System.out.print("Output: "+CombineOutput(outp).replace(DELIM, ""));
                    else
                        System.out.print("Output: "+CombineOutput(outp));
                } else
                    try {
                        Thread.sleep(100);
                } catch (InterruptedException ex) {
                    Logger.getLogger(CSC454HW6.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
          */  
        }
        
    }
    private static void LoadCommandsFromFile(SimulationFramework f, String filepath){
        File source = new File(filepath);
        int count = 0;
        try{
            Scanner inFileScanner = new Scanner(source);
            while(inFileScanner.hasNext()){
                String[] sa = inFileScanner.nextLine().split(DELIM);
                TwoDTime t = new TwoDTime(Double.parseDouble(sa[0]), Integer.parseInt(sa[1]));
                sa = RemoveTimeData(sa);
                //only acccpets one int per line
                f.AddInput(t, Integer.parseInt(sa[0]));
                count++;
            }
            inFileScanner.close();
            System.out.println("File Loaded "+count+" commands.");
        }catch(FileNotFoundException e){
            System.out.println("File Not Found");
        }
    }
    private static String CombineOutput(ArrayList<OutputTimePairs<Integer>> a){
        String comb = "";
        OutputTimePairs<Integer> temp;
        for (int i = 0; i < a.size(); i++) {
            temp = a.get(i);
            //String[] d = temp.getData();
            Integer d = temp.getData();
            if (d != null) {
                comb+= "(Time: "+temp.getTime().getRealTime()+","+temp.getTime().getDiscreteTime()+")\n";
                comb+= "Output: "+d;
                /*for (int j = 0; j < d.length; j++) {

                    if(j == d.length-1)
                        comb += d[j];
                    else
                        comb += d[j] + DELIM;
                }*/

                comb += "\n";
            }
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
    private static EventNetwork<Integer, Integer> CreateDrillPressEventNetwork(){
        EventNetwork<Integer,Integer> theNetwork = new EventNetwork<>();
        DrillPressModel drill = new DrillPressModel(0, 2);
        DrillPressModel press = new DrillPressModel(0, 1);
        theNetwork.Add(press);
        theNetwork.Add(drill);
        
        theNetwork.Bind_II(press);
        theNetwork.Bind_OI(press, drill);
        theNetwork.Bind_OO(drill);
        
        return theNetwork;
    
    }
    private static String[] RemoveTimeData(String[] s){
       String[] newS = new String[s.length-2];
       for(int i = 0; i < newS.length; i++){
           newS[i] = s[i+2];
       }
       return newS;
   } 
}


