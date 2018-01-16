/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

/**
 *
 * @author chrisrk192
 */
public class Event<I> {
    public enum type{internal,external,confluent};
    public TwoDTime myTime;
    public EventModelInterface model;
    public type typeOfEvent;
    public I input;
    public TwoDTime timeQueued;
    
    public String Print(){
        return "Time: "+myTime.getRealTime()+","+myTime.getDiscreteTime()+": "+model+", "+typeOfEvent+" "+input;
    }
}
