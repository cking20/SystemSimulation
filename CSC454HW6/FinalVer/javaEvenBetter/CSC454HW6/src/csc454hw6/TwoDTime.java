/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw6;

import java.math.BigDecimal;

/**
 *
 * @author chrisrk192
 */
public class TwoDTime implements Comparable<TwoDTime>{
    //private double realTime;
    public static BigDecimal INFINITY = BigDecimal.valueOf(Double.MAX_VALUE);
    public static TwoDTime INFINITE = new TwoDTime(0,0);
    private BigDecimal realTime;
    private int discreteTime;

    public BigDecimal getRealTime() {
        if(realTime == null)
            return null;
        else return realTime;
    }
    
    public int getDiscreteTime() {
        return discreteTime;
    }

    public TwoDTime(double d, int i) {
        realTime = BigDecimal.valueOf(d);
        discreteTime = i;
    }
    public TwoDTime(double d) {
        realTime = BigDecimal.valueOf(d);
        discreteTime = 0;
    }
    public TwoDTime(BigDecimal d, int i) {
        realTime = d;
        
        discreteTime = i;
    }
    public TwoDTime(BigDecimal d) {
        realTime = d;
        discreteTime = 0;
    }
    
    public TwoDTime Advance(TwoDTime interval){
        //if (interval.realTime > 0) {
        if (interval.realTime.compareTo(BigDecimal.ONE) > 0) {
            //return new TwoDTime(realTime+interval.realTime, 0);
            return new TwoDTime(realTime.add(interval.realTime), 0);
        } else
            return new TwoDTime(realTime, discreteTime+interval.discreteTime);       
    }  

    @Override
    public int compareTo(TwoDTime that) {
        if(that == TwoDTime.INFINITE && this == TwoDTime.INFINITE)
            return 0;
        if(that == TwoDTime.INFINITE)
            return -1;
        if(this == TwoDTime.INFINITE)
            return 1;   
        int equality = realTime.compareTo(that.realTime);           
        if(equality == 0){
            if(that.discreteTime == discreteTime)
                return 0;
            if(discreteTime < that.discreteTime)
                return -1;
            return 1;
        }
        return equality;
    }
}
