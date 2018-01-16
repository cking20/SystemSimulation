/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw4;

import java.util.ArrayList;

/**
 *
 * @author chrisrk192
 * @param <M>
 */
public class Network implements ModelInterface {
    public int numInternalTicksToExternal;
    private ArrayList<ModelInterface> atomics;
    private ArrayList<String[]> inputs;
    private ArrayList<String[]> outputs;
    private ArrayList<Binding> bindings;
    public Network() {
        numInternalTicksToExternal = 1;
        atomics = new ArrayList<>();  
        inputs = new ArrayList<>();
        outputs = new ArrayList<>();
        bindings = new ArrayList<>();
    }
    
    
    public void Add(ModelInterface model){
        atomics.add(model);
        inputs.add(new String[0]);
        outputs.add(new String[0]);
    }
       //binds input to network as input to this network
    /**
     * binds input for network to input of source
     * @param sink 
     */
    public void Bind_II(int sink){
        bindings.add(new Binding(-1,sink));
    }
    //binds output from srouce as input to sink
    public void Bind_OI(int source, int sink){
        bindings.add(new Binding(source,sink));
    }
    //binds output from source to the output of this network
    public void Bind_OO(int source){
        bindings.add(new Binding(source,-1));
    }

    
    @Override
    public String GetInitilizationSignature() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public void Initialize(String[] args) {
        numInternalTicksToExternal = Integer.parseInt(args[0]);
    }
     
    @Override
    public String[] Tick(String[] args) throws CantPerformAction{
        String[] out = new String[0];
        for (int i = 0; i < numInternalTicksToExternal; i++) {
            out = OutPut();
            StateTransition(args);
        }
        return out;
    }
    
    @Override
    public String[] OutPut() {
        //run output function on every atomic inside and store output to the outputs
        for(int i = 0; i < atomics.size(); i++){
            String[] temp = atomics.get(i).OutPut();
            outputs.set(i, temp);
        }
        //need to output from this network...
        String[] trueOutput = new String[0];
        for (int i = 0; i < bindings.size();i++) {
            Binding temp = bindings.get(i);
            if(temp.to == -1){
                if(temp.from == -1)
                    ;
                else 
                    trueOutput = ConcatinateString(trueOutput,outputs.get(temp.from));
            }
        }
        return trueOutput;
    }

    public void Couplings(String[] inputToNetwork){
        //then copy outputs to inputs based on mappings
        Binding temp;
        String[] cur;
        String[] trueOutput = new String[0];
        for (int i = 0; i < bindings.size();i++) {
            temp = bindings.get(i);
            if(temp.to == -1){
                //System.out.println("temp to == -1 skippin as should be done in output");
                //taken care of in output
                //trueOutput = ConcatinateString(trueOutput,outputs.get(temp.from));
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
    public void FlushInputs(){
        String[] nul = new String[0];
        for (int i = 0; i < inputs.size(); i++) {
            inputs.set(i, nul);
        }
    }
    
    @Override
    public void StateTransition(String[] inputItems) throws CantPerformAction {
        FlushInputs();
        Couplings(inputItems);
        for(int i = 0; i < atomics.size(); i++){     
            atomics.get(i).StateTransition(inputs.get(i));
            //outputs.set(i, temp);
        }
        
    }

    @Override
    public String GetStatus() {
        String output = "Begin Status of Network\n";
        output += bindings.size() + " Bindings\n";
        output += numInternalTicksToExternal + " Internal Ticks to External\n";
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

    @Override
    public Object Clone() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }


    private String[] ConcatinateString(String[] a, String[] b) {
        int aLen = a.length;
        int bLen = b.length;
        String[] c= new String[aLen+bLen];
        System.arraycopy(a, 0, c, 0, aLen);
        System.arraycopy(b, 0, c, aLen, bLen);
        return c;
    }
    /**
     * -1 should signify IO to/from the network
     */
    private class Binding{
        public int from;
        public int to;

        public Binding(int from, int to) {
            this.from = from;
            this.to = to;
        }
        
        
    }
}