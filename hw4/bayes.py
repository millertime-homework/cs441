#!/usr/bin/python

# Copyright 2011 Russell Miller
# See included LICENSE file

# bayes.py
# Naive Bayesian learner for heart anomalies

def main():
    """open a learning file, run learner, open
    the tester, test it"""
    learn_files = ["spect-orig.train.csv",
                   "spect-itg.train.csv",
                   "spect-resplit.train.csv" ]

    test_files = ["spect-orig.test.csv",
                  "spect-itg.test.csv",
                  "spect-resplit.test.csv" ]

    for i in range(3):
        f = open(learn_files[i])
        data_set = []
        for line in f:
            # massage the csv into a list of integers
            data_set.append(map(int, line[:-1].split(',')))            
        nbayes = NBayes(data_set)

#end main

class NBayes:
    """ADT to hold the learner data"""
    def __init__(self, d):
        self.nfeatures = len(d[0]) - 1
        self.ninstances = len(d)
        self.data = d
        self.learn()
    
    def learn(self):
        """Scan the data and train yer brain on it"""
        print("Number of instances: %d" % self.ninstances)
        print("Number of features: %d" % self.nfeatures)
        class_0 = 0
        class_1 = 0
        for instance in self.data:
            c = instance[0]
            if c == 0:
                class_0 += 1
            elif c == 1:
                class_1 += 1
            else:
                #ohshit
                print("Unexpected class %d found!" % c)
                exit()
        # print("Class 0: %d Class 1: %d" % (class_0,class_1))

if __name__=="__main__":
    main()
