/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw2;

/**
 *
 * @author chrisrk192
 */
public class CSC454HW2 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        VendingMachine inst = new VendingMachine();
        SimulationFramework<VendingMachine> smfw;
        smfw = new SimulationFramework<>(inst,true,true,1.0);
        smfw.RunSimulation();
    }
    
}
