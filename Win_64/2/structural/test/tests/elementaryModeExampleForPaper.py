# -*- coding: utf-8 -*-
"""
Created on Thu Dec 21 14:20:40 2017

@author: hsauro
"""

import tellurium as te
import roadrunner
import structural

r = te.loada("""
     J1: $Xo => A; v;
     J2: A => B; v;
     J3: A => C; v;
     J4: B + E => 2 D; v;
     J5: $X1 => E; v;
     J6: B => C + F; v;
     J7: C => D; v;
     J8: D => $X2; v;
     J9: F => $X3; v;
     v = 0;
""")

r.exportToSBML('testModel1.xml') 
ls = structural.LibStructural()
ls.loadSBMLFromString(r.getSBML())

print ls.getElementaryModes()

