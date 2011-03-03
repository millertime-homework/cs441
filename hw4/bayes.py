#!/usr/bin/python

# Copyright 2011 Russell Miller
# See included LICENSE file

# bayes.py
# Naive Bayesian learner for heart anomalies

import math

def main():
    """open a learning file, run learner, open
    the tester, test it"""
    shorthands = ["orig","itg","resplit"]
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
        f.close()
        nbayes = NBayes(data_set)
        f = open(test_files[i])
        test_set = []
        for line in f:
            # massage the csv into a list of integers
            test_set.append(map(int, line[:-1].split(',')))
        f.close()
        a,b,t,n,m = nbayes.test(test_set)
        print("%s %d/%d %d/%d %d/%d" % (shorthands[i],a+b,t,a,n,b,m))

class NBayes:
    """ADT to hold the learner data"""
    def __init__(self, d):
        self.nfeatures = len(d[0]) - 1
        self.ninstances = len(d)
        # initialize table of features
        self.features = [[],[]]
        for f in range(self.nfeatures):
            self.features[0].append(0)
        for f in range(self.nfeatures):
            self.features[1].append(0)
        # initialize class counters
        self.nclass = []
        self.nclass.append(0)
        self.nclass.append(0)
        self.data = d
        self.learn()
    
    def learn(self):
        """Scan the data and train yer brain on it"""
        #print("Number of instances: %d" % self.ninstances)
        #print("Number of features: %d" % self.nfeatures)
        for instance in self.data:
            c = instance[0]
            try:
                self.nclass[c] += 1
            except IndexError:
                print "Non-binary class value"
                exit()
            i = 0
            for f in instance[1:]:
                if f == 1:
                    self.features[c][i] += 1
                i += 1

    def test(self, test_instances):
        """Using the features table from the train data,
        calculate how correct the test data is"""
        ntests = len(test_instances)
        correct_ones = 0
        correct_zeros = 0
        ones = 0
        zeros = 0
        for instance in test_instances:
            c = instance[0]
            L = []
            L.append(0)
            L.append(0)
            for i in range(2):
                # likelihook of just the class
                L[i] = math.log(self.nclass[i] + .5) 
                L[i] -= math.log(self.nclass[1] + self.nclass[0] + .5)
            for i in range(2):
                for j in range(self.nfeatures):
                    if instance[j] == 1:
                        L[i] += math.log(self.features[i][instance[j]] + .5)
                        L[i] -= math.log(self.nclass[i] + .5)
                    elif instance[j] == 0:
                        L[i] += math.log(self.nclass[i] - self.features[i][instance[j]] + .5)
                        L[i] -= math.log(self.nclass[i] + .5)
                    else:
                        print("Non-binary feature value in test")
                        exit()
            if c == 1:
                ones += 1
                if L[1] > L[0]:
                    correct_ones += 1
            elif c == 0:
                zeros += 1
                if L[1] <= L[0]:
                    correct_zeros += 1
            else:
                print("Non-binary class value in test")
                exit()
        return correct_ones,correct_zeros,ntests,ones,zeros

if __name__=="__main__":
    main()
