/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package csc454hw5;

/**
 *
 * @author chrisrk192
 */
public class TwoDTime implements Comparable<TwoDTime>{
    private double realTime;
    private int discreteTime;

    /**
     * @return the realTime
     */
    public double getRealTime() {
        return realTime;
    }

    /**
     * @return the discreteTime
     */
    public int getDiscreteTime() {
        return discreteTime;
    }

    public TwoDTime(double d, int i) {
        realTime = d;
        discreteTime = i;
    }
    public TwoDTime(double d) {
        realTime = d;
        discreteTime = 0;
    }
    
    public TwoDTime Advance(TwoDTime interval){
        if (interval.realTime > 0) {
            return new TwoDTime(realTime+interval.realTime, 0);
        } else
            return new TwoDTime(realTime, discreteTime+interval.discreteTime);       
    }  

    @Override
    public int compareTo(TwoDTime that) {
        //if(Double.isInfinite(that.realTime) && Double.isFinite(realTime)) return -1;
        //if(Double.isFinite(that.realTime) && Double.isInfinite(realTime)) return 1;
        if(that.realTime == realTime){
            if(that.discreteTime == discreteTime)
                return 0;
            if(discreteTime < that.discreteTime)
                return -1;
            return 1;
        } else if(realTime < that.realTime)
            return -1;
        //else greater    
        return 1;       
    }
}
