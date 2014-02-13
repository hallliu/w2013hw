#!/usr/bin/python
#fsm.py

import sys
from ConfigParser import *

class FSM():
    def __init__(self, filename):
        self.transitionDict = dict() #associates names to dictionaries
        #I'm using the ConfigParser module, since its format looks the most like what we got
        pars = ConfigParser() 
        pars.read(filename)
        names = pars.get('states','names').split(',')
        self.state = pars.get('states','start')
        self.end = pars.get('states','end')
        
        #loop through the names to add them to the dict
        for n in names:
            n = n.strip() 
            nDict = dict() #the dictionary that'll be associated with this name
            translist = pars.get('transitions',n).split(',') #list of transitions as "from:to"
            for t in translist:
                sToD = t.split(':')
                origin = sToD[0].strip()
                dest = sToD[1].strip()
                #add these two to the dictionary, then test if we get non-deterministic behavior
                val = nDict.setdefault(origin,dest)
                if(val != dest):
                    print 'Non-deterministic behavior detected'
                    sys.exit(1)
            self.transitionDict[n] = nDict

    def transition(self, character):
        tDict = self.transitionDict[self.state]
        self.state = tDict.get(character)
        if(self.state == None): #if not found at first, see if there's a default
            self.state = tDict.get('*')
            if(self.state == None): 
                print 'Non-exhaustive transitions detected'
                sys.exit(1)

fsm = FSM(sys.argv[1])

for c in sys.argv[2]:
    fsm.transition(c)

if fsm.state == fsm.end:
    print 'Accept'
else:
    print 'Reject. Ended in state ', fsm.state
 
