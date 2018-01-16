/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
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
        if(sink == -1) throw new RuntimeException("Attempting to bind to nonexisting model");
        bindings.add(new Binding(-1,sink));
    }
    //binds output from srouce as input to sink
    public void Bind_OI(EventModelInterface e,EventModelInterface k){
        int source = atomics.indexOf(e);
        if(source == -1) throw new RuntimeException("Attempting to bind from nonexisting model");
        int sink = atomics.indexOf(k);
        if(sink == -1) throw new RuntimeException("Attempting to bind to nonexisting model");
        bindings.add(new Binding(source,sink));
    }
    //binds output from source to the output of this network
    public void Bind_OO(EventModelInterface k){
        int source = atomics.indexOf(k);
        if(source == -1) throw new RuntimeException("Attempting to bind from nonexisting model");
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

    public boolean IsNetworkInvalid(){
        for(int i = 0; i < bindings.size(); i++){
            if(bindings.get(i).from == -1){
                //runtime exception already thrown
            }else if(bindings.get(i).to == -1){
                //runtime exception already thrown
            }else{
                EventModelInterface from = atomics.get(bindings.get(i).from);
                EventModelInterface to  = atomics.get(bindings.get(i).to);
                Type[] f = null;
                Type[] t = null;
                Type[] genericInterfaces = from.getClass().getGenericInterfaces();
                for (Type genericInterface : genericInterfaces) {
                    if (genericInterface instanceof ParameterizedType) {
                        f = ((ParameterizedType) genericInterface).getActualTypeArguments();   
                    }
                } 
                Type[] genericInterfaces2 = to.getClass().getGenericInterfaces();
                for (Type genericInterface : genericInterfaces2) {
                    if (genericInterface instanceof ParameterizedType) {
                        t = ((ParameterizedType) genericInterface).getActualTypeArguments();         
                    }
                }
                if(f[1] != t[0])
                    return true;   
            }
        }
        return false;
    }
    
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
