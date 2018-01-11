
# Test Script for checking elementary modes
# Dec 2017

import structural
import numpy as np

def checkThermodynamics (ls, elm):
    # Assume we're ok initially, then try to disprove
    result = True
    nCols = np.size(elm,1)
    nElms = elm.shape[0]

    for k in range (nElms):
        em = elm[k]

        for i in range (nCols):
            if (not ls.isReactionReversible(i)) and (em[i] < 0):
                result = False;

    return result


def checkElemenarity (ls, elm):
   N = ls.getStoichiometryMatrix()
   sum = 0
   for j in range (elm.shape[0]):
      em = elm[j]
      indexList = []
      nCols = np.size(em,0)
      # Find the columns of the entries that are non-zero
      for i in range (nCols):
          if em[i] != 0:
             indexList.append (i)

      # Construct submatrix of stoichiometry matrix. The columns
      # correspond to the indecies in the indexList
      subN = np.empty([N.shape[0],0])
      for i in indexList:
           subN = np.column_stack ((subN, N[:,i]))
      ns = te.nullspace (subN)
      # If the null space has dim(1) then we pass this em
      sum = sum + int (ns.shape[1])
   # If all pass then the sum should equal the number of elm
   if sum == elm.shape[0]:
       return True
   else:
       return False


def checkElm(id):
   elm = ls.getElementaryModes()
   st = ls.getStoichiometryMatrix()
   p = np.matmul (st, np.transpose(elm))
   if not np.any (p):
      if checkElemenarity (ls, elm):
          if checkThermodynamics (ls, elm):
             print("(", id, ") ------PASS-----", "Number of modes = ", elm.shape[0])
          else:
             print(id, " ------FAIL Thermo -----")
      else:
         print(id, " ------FAIL Elementarity-----")
   else:
      print(id, " ------FAIL N e = 0 -----")


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 => $X1; v;
    J3: S1 => $X2; v;
    v = 0
''')
r.exportToSBML('testModel1.xml')
ls = structural.LibStructural()
ls.loadSBMLFromString(r.getSBML())
checkElm(1)

r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> $X1; v;
    J3: S1 -> $X2; v;
    v = 0
''')
r.exportToSBML('testModel2.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(2)


r = te.loada('''

    J1: $Xo -> S1; v;
    J2: S1 -> S2; v;
    J3: S2 => $X1; v;
    J4: S2 => $X2; v;
    J5: S1 => $X3; v;
    v = 0
''')
r.exportToSBML('testModel3.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(3)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> S2; v;
    J3: S2 -> S3; v;
    J4: S1 -> S3; v;
    J5: S3 -> $X1; v;
    v = 0
''')
r.exportToSBML('testModel4.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(4)


r = te.loada('''
    J1: $Xo => S1; v;
    J2: S1 => S2; v;
    J3: S2 => S3; v;
    J4: S1 => S3; v;
    J5: S2 => S4; v;
    J6: S3 => S4; v;
    J7: S4 => $X1; v;
    v = 0
''')
r.exportToSBML('testModel5.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(5)



r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> S2; v;
    J3: S2 -> S3; v;
    J4: S1 -> S3; v;
    J5: S3 -> $X1; v;
    J6: S2 -> S4; v;
    J7: S2 -> S5; v;
    J8: S4 -> S5; v;
    J9: S5 ->; v;
    v = 0
''')
r.exportToSBML('testModel6.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(6)



r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> S2; v;
    J3: S2 -> S3; v;
    J4: S1 -> S3; v;
    J5: S3 -> $X1; v;
    J6: S3 -> $X2; v;
    v = 0
''')
r.exportToSBML('testModel7.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(7)


r = te.loada('''
    J1: S1 -> S2; v;
    J2: S2 -> S1; v;
    J3: S2 -> S3; v;
    J4: S3 -> S2; v;
    v = 0
''')
r.exportToSBML('testModel8.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(8)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> $X2; v;
    J3: S1 -> S2; v;
    J4: S2 -> $X3; v;
    J5: S2 -> $X4; v;
    J6: $X1 -> S3; v;
    J7: S3 -> S2; v;
    J8: S3 -> $X5; v;
    v = 0
''')
r.exportToSBML('testModel9.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(9)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> S3; v;
    J3: S1 -> S2; v;
    J4: S2 -> S4; v;
    J5: S4 -> S3; v;
    J6: S3 -> S5; v;
    J7: S4 -> S6; v;
    J8: S6 -> $X1; v;
    J9: S6 -> S5; v;
    v = 0
''')
r.exportToSBML('testModel10.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(10)

r = te.loada('''
    J1: $Xo -> S1; v;
    J2: $X1 -> S1; v;
    J3: S1 -> $X2; v;
    v = 0
''')
r.exportToSBML('testModel11.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(11)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> S2 + S4; v;
    J3: S2 -> S3 + S4 ; v;
    J4: S3 -> S4; v;
    J5: S4 -> $X1; v;
    v = 0
''')
r.exportToSBML('testModel12.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(12)



r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> S2; v;
    J3: S2 -> S3; v;
    J4: S1 -> S3; v;
    J5: S3 -> S4; v;
    J6: S3 -> S5; v;
    J7: S4 -> S5; v;
    J8: S5 -> $X1; v;
    v = 0
''')
r.exportToSBML('testModel13.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(13)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 -> S2; v;
    J3: S2 -> S3; v;
    J4: S1 -> S3; v;
    J5: S3 -> S4; v;
    J6: S3 -> S5; v;
    J7: S4 -> S5; v;
    J8: S5 -> S6; v;
    J9: S5 -> S7; v;
    J10: S6 -> S7; v;
    J11: S7 -> $X1; v;
    v = 0
''')
r.exportToSBML('testModel14.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(14)


r = te.loada('''
    J1: $Xo -> S1 + S2; v;
    J2: S1 -> S3; v;
    J3: S2 + S3 -> $X1; v;
    v = 0
''')
r.exportToSBML('testModel15.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(15)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 + $X1 -> S2; v;
    J3: S2 -> S1 + $X2; v;
    v = 0
''')
r.exportToSBML('testModel16.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(16)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 + $X1 -> S2; v;
    J3: S2 -> S1 + $X2; v;
    v = 0
''')
r.exportToSBML('testModel17.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(17)


r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 + S2 -> S3 + $X1; v;
    J3: S1 + S3 -> S2 + $X2; v;
    v = 0
''')
r.exportToSBML('testModel18.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(18)



r = te.loada('''
    J1: $Xo + S2 -> S1; v;
    J2: S1 -> S2 + S5; v;
    J3: S5 + S4 -> S3; v;
    J4: S3 -> S4 + $X1; v;
    v = 0
''')
r.exportToSBML('testModel19.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(19)


# 20
r = te.loada('''
    J1: $Xo => S1; v;
    J2: S1 => $X2; v;
    J3: S1 => S2; v;
    J4: S2 => $X3; v;
    J5: S2 => $X4; v;
    J6: $X1 => S3; v;
    J7: S3 => S2; v;
    J8: S3 => $X5; v;
    v = 0
''')
r.exportToSBML('testModel20.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(20)
eml1 = ls.getElementaryModes()


# 21
r = te.loada('''
    J1: $Xo => S1; v;
    J2: S1 => $X2; v;
    J3: S1 -> S2; v;
    J4: S2 => $X3; v;
    J5: S2 => $X4; v;
    J6: $X1 => S3; v;
    J7: S3 => S2; v;
    J8: S3 => $X5; v;
    v = 0
''')
r.exportToSBML('testModel21.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(21)
eml2 = ls.getElementaryModes()


# 22
r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 => $X2; v;
    J3: S1 -> S2; v;
    J4: S2 => $X3; v;
    J5: S2 => $X4; v;
    J6: $X1 => S3; v;
    J7: S3 => S2; v;
    J8: S3 => $X5; v;
    v = 0
''')
r.exportToSBML('testModel22.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(22)


# 23
r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 => $X2; v;
    J3: S1 -> S2; v;
    J4: S2 => $X3; v;
    J5: S2 => $X4; v;
    J6: $X1 => S3; v;
    J7: S3 -> S2; v;
    J8: S3 => $X5; v;
    v = 0
''')
r.exportToSBML('testModel23.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(23)


# 24
r = te.loada('''
    J1: $Xo -> S1; v;
    J2: S1 => $X2; v;
    J3: S1 -> S2; v;
    J4: S2 => $X3; v;
    J5: S2 => $X4; v;
    J6: $X1 -> S3; v;
    J7: S3 -> S2; v;
    J8: S3 => $X5; v;
    v = 0
''')
r.exportToSBML('testModel24.xml')
ls.loadSBMLFromString(r.getSBML())
checkElm(24)
