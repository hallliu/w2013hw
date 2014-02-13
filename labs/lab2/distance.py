#!/usr/bin/python
# distance.py
# calculates the distance between two texts
# Note: This script works on version 2.7.2

import sys
import re
import math

def getWords(wlist): #function to obtain the normalized word-frequencies
    freq = dict()
    wlist = filter(lambda x: x != '', wlist) #filter out the empty strings
    for w in wlist: #initial loop to add up the absolute freqs
        if (w in freq): freq[w] += 1 # if it's already in the list
        else: freq[w] = 1 # if not, put it in and set count to 1

    sFreqSq = 0
    for w in freq: #second loop to get the sum of squares 
        sFreqSq += math.pow(freq[w],2)
    
    rootSq = math.sqrt(sFreqSq) 
    for w in freq: #third loop to normalize
        freq[w] /= rootSq

    return freq

if (len(sys.argv) != 3):
    print "Usage: distance.py file1 file2" #print usage message if number of args is incorrect

#open file handles for the specified files
f1 = open(sys.argv[1],"r")
f2 = open(sys.argv[2],"r")

#get the words in each by sending to lowercase and splitting along non-alphanumeric or _ characters
l1 = re.split('\W+', f1.read().lower())
l2 = re.split('\W+', f2.read().lower())

#calculate (normalized) word-frequency associations
f1dict = getWords(l1)
f2dict = getWords(l2)

sNormSq = 0 #the running value of the sum of squares of diffs of norms
for x in set(f1dict.keys()) | set(f2dict.keys()): #converts the list of keys into a set so we can take the union easily
    sNormSq += math.pow(f1dict.get(x,0)-f2dict.get(x,0), 2) #the get function returns 0 if x is not in the dict, as per the assignment

print "The distance between the two texts is ", math.sqrt(sNormSq), "\n"

