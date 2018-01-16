/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw5;

import java.util.ArrayList;

/**
 *
 * @author chrisrk192
 */
public class EventNetwork<I,O>{
    public ArrayList<EventModelInterface<I,O>> atomics;
    public ArrayList<Binding> bindings;
    
    public EventNetwork() {
        atomics = new ArrayList<>();  
        
        bindings = new ArrayList<>();
    }
    
    
    public void Add(EventModelInterface model){
        atomics.add(model);
    }
       //binds input to network as input to this network
    public void Bind_II(EventModelInterface e){
        
        int sink = atomics.indexOf(e);
        bindings.add(new Binding(-1,sink));
    }
    //binds output from srouce as input to sink
    public void Bind_OI(EventModelInterface e,EventModelInterface k){
        int source = atomics.indexOf(e);
        int sink = atomics.indexOf(k);
        bindings.add(new Binding(source,sink));
    }
    //binds output from source to the output of this network
    public void Bind_OO(EventModelInterface k){
        int source = atomics.indexOf(k);
        bindings.add(new Binding(source,-1));
    }

    public ArrayList<EventModelInterface> getExternInputModels(){
        ArrayList<EventModelInterface> a = new ArrayList<>();
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.get(i).from == -1)
                a.add(atomics.get(bindings.get(i).to));
        }
        return a;
    }
    
    public ArrayList<EventModelInterface> getExternOutputModels(){
        ArrayList<EventModelInterface> a = new ArrayList<>();
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.get(i).to == -1)
                a.add(atomics.get(bindings.get(i).from));
        }
        return a;
    }
    public boolean isExternOutputModel(EventModelInterface e){
        //ArrayList<EventModelInterface> a = new ArrayList<>();
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.get(i).to == -1)
                if(atomics.get(bindings.get(i).from) == e)
                    return true;
                
        }
        return false;
    }
    public ArrayList<EventModelInterface> getModelsOutputBoundTo(EventModelInterface e){
        ArrayList<EventModelInterface> a = new ArrayList<>();
        int myIndex = atomics.indexOf(e);
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.get(i).from == myIndex && bindings.get(i).to != -1)
                a.add(atomics.get(bindings.get(i).to));
        }
        return a;
    }
    
    
    
    
/*
    public void Couplings(String[] inputToNetwork){
        //then copy outputs to inputs based on mappings
        Binding temp;
        String[] cur;
        String[] trueOutput = new String[0];
        for (int i = 0; i < bindings.size();i++) {
            temp = bindings.get(i);
            if(temp.to == -1){
                if(temp.from == -1)
                    ;
                else 
                    trueOutput = ConcatinateString(trueOutput,outputs.get(temp.from));
            }
            else if(temp.from == -1){
                cur = inputs.get(temp.to);
                inputs.set(temp.to, ConcatinateString(cur,inputToNetwork));
            }
            else{
                //System.out.println("coupling "+temp.from +" to "+ temp.to);
                cur = inputs.get(temp.to);
                inputs.set(temp.to, ConcatinateString(cur,outputs.get(temp.from)));
            }
                
                
        }
        //return trueOutput;
    }
   
*/    
   
    public String GetStatus() {
        String output = "Begin Status of Network\n";
        output += bindings.size() + " Bindings\n";
        
        for(int j = 0; j < bindings.size(); j++){
            output += "     Binding of " + bindings.get(j).from + " to " + bindings.get(j).to+ '\n';
        }
        output += atomics.size() + " SubParts\n";
        for(int i = 0; i < atomics.size(); i++){
            output += "     " + atomics.get(i).GetStatus().replace("\n", "\n     ")+"\n";
        }
        output += "End Status of Network\n";
        return output;
    }

    
    /**
     * -1 should signify IO to/from the network
     */
    private class Binding {
        public int from;
        public int to;

        public Binding(int from, int to) {
            this.from = from;
            this.to = to;
        }
        
        
    }
}
