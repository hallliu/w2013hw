#!/usr/bin/python2
import math
a=[[213,128,37,18,3,1,0,0,0,0,0,0,0],
    [103,143,98,42,8,4,2,0,0,0,0,0,0],
    [75,103,121,54,30,13,2,1,0,1,0,0,0],
    [0,20,43,53,86,70,54,37,18,10,5,2,2]]

x=[0,0,0,0]

for i in range(4):
    s=0
    for j in range(len(a[i])):
        x[i]=x[i]+a[i][j]*j
        s=s+a[i][j]
    xbar=x[i]/float(s)
    print "param: {:.4}".format(xbar)
    print math.pow(-0.98/math.sqrt(s)+math.sqrt(xbar),2),",",math.pow(0.98/math.sqrt(s)+math.sqrt(xbar),2)
    x[i]=x[i]/float(s)

for i in range(13):
    print i,
    for j in range(4):
        print "& {},{}".format(a[j][i],int(round(400*math.exp(-x[j])*math.pow(x[j],i)/math.factorial(i)))),
    
    print "\\\\"
