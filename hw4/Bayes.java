/*
 * Copyright 2011 Russell Miller
 * See included LICENSE file
 *
 * Bayes.java
 * Naive Bayesian learner for heart anomalies
 */

class Bayes
{
    public final static void main(String [] args)
    {
        String [] shorthands = {"orig","itg","resplit"};
        String [] trainfiles = {"spect-orig.train.csv",
                                "spect-itg.train.csv",
                                "spect-resplit.train.csv"};
        String [] testfiles  = {"spect-orig.test.csv",
                                "spect-itg.test.csv",
                                "spect-resplit.test.csv"};
        for(int i = 0; i < 3; i++) {
            Bayes bay = new Bayes(trainfiles[i]);
            //open test file..
            //classify..
            //print results
        }
    }

    private int [] F;
    private int [] N;

    public Bayes(String filename)
    {
        //open trainer file..
        //read the file into a table..
        // something like for each String.charAt(i)
    }
}
