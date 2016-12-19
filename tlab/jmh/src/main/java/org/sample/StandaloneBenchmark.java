package org.sample;
import java.lang.*;
import java.util.*;

public class StandaloneBenchmark {

    // Entry point for running this benchmark outside of jmh
    public static void main(String a[]){
        System.out.println("hello");
        testMethod(100000, 1000000);
    }

    // Benchmark starts here
    public static void testMethod(int loop_cnt, int alloc_cnt) {
        int i;

        for(i=0; i<loop_cnt; i++){
            allocate_and_forget(alloc_cnt);
        }
    }

    // Allocate some object instances
    // These instances should go out of scope when the function returns
    private static void allocate_and_forget(int alloc_cnt) { 
        Integer temp;
        int i;
        for(i=0; i<alloc_cnt; i++){
            temp = new Integer(i);
        }
        return;
    }
}
