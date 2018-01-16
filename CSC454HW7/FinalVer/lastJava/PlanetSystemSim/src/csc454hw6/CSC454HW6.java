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
        SimulationFramework<Integer,Integer> smfw;
        smfw = new SimulationFramework(true);
        //smfw.SetDiscreteEventNetwork(CreateDrillPressEventNetwork());
        //smfw.SetDiscreteEventNetwork(CreateTestEventNetwork());
        String inputLine = "";
        Scanner s = new Scanner(System.in); 
        do{          
            inputLine = s.nextLine();
            if(inputLine.charAt(0) != '-'){//not a system command
                String[] sa = inputLine.split(DELIM);
                double r = Double.parseDouble(sa[0]);
                int d = Integer.parseInt(sa[1]);
                sa = RemoveTimeData(sa);
                //only uses one int per line
                smfw.AddInput(r, d, Integer.parseInt(sa[0]));
            }else{
                ProcessInput(inputLine, smfw);
            }
        } while (inputLine.compareTo("-quit") != 0);
        
    }
    
    public static void ProcessInput(String s, SimulationFramework f){
        String[] args = s.split(" ");
        if(args[0].compareTo("-togdebug") == 0){
            f.debugMode = !f.debugMode;
        }
        if(args[0].compareTo("-run") == 0){
            String o = CombineOutput(f.RunEventSimulation(100000000.0));
            System.out.println(o);
        }
        if(args[0].compareTo("-help") == 0){
            System.out.println("Commands\n"
                    + "-togdebug\n"
                    + "-help\n"
                    + "-run"
                    + "-loadcmds FILENAME\n"
                    + "-status\n");
        }
        if(args[0].compareTo("-loadcmds") == 0){
            LoadCommandsFromFile(f,args[1]);
        }
        if(args[0].compareTo("-status") == 0){
            System.out.println(f.GetStatus());
        }        
    }
 
    private static void LoadCommandsFromFile(SimulationFramework f, String filepath){
        File source = new File(filepath);
        int count = 0;
        try{
            Scanner inFileScanner = new Scanner(source);
            while(inFileScanner.hasNext()){
                String[] sa = inFileScanner.nextLine().split(DELIM);
                double r = Double.parseDouble(sa[0]);
                int d = Integer.parseInt(sa[1]);
                sa = RemoveTimeData(sa);
                //only uses one int per line
                f.AddInput(r, d, Integer.parseInt(sa[0]));
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
    
    private static EventNetwork<Influencers, Influencers> BuildSpaceEventNetwork(){
        EventNetwork<Influencers,Influencers> space = new EventNetwork<>();
        
        
        return space;
    }
    
    
    
    /*
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
    private static EventNetwork<Integer, Integer> CreateTestEventNetwork(){
        EventNetwork<Integer,Integer> theNetwork = new EventNetwork<>();
        IntToStingModel in = new IntToStingModel();
        StringtoIntModel out = new StringtoIntModel();
        theNetwork.Add(in);
        theNetwork.Add(out);
        
        theNetwork.Bind_II(in);
        theNetwork.Bind_OI(in, out);
        theNetwork.Bind_OO(out);
        
        return theNetwork;  
    }
    */
    
    private static String[] RemoveTimeData(String[] s){
       String[] newS = new String[s.length-2];
       for(int i = 0; i < newS.length; i++){
           newS[i] = s[i+2];
       }
       return newS;
   } 
}


