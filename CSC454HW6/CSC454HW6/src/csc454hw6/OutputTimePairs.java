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
public class OutputTimePairs<Data>{
       private Data data;
       private TwoDTime time;

        public OutputTimePairs(TwoDTime t,Data d) {
            data = d;
            time = t;
        }

        /**
         * @return the data
         */
        public Data getData() {
            return data;
        }

        /**
         * @return the time
         */
        public TwoDTime getTime() {
            return time;
        }
   }
