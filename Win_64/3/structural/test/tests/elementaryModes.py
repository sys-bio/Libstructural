
# Test Script for checking elementary modes
# Dec 2017

import structural
import numpy as np

def nullspace(A, atol=1e-13, rtol=0):

    A = np.atleast_2d(A)
    u, s, vh = np.linalg.svd(A)
    tol = max(atol, rtol * s[0])
    nnz = (s >= tol).sum()
    ns = vh[nnz:].conj().T
    return ns

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
      ns = nullspace (subN)
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



def checkStructure(ls):
    if 'Fail' in ls.validateStructuralMatrices():
        print("Structural Matrix Validation ----- FAIL-----")
    else:
        print("Structural Matrix Validation ----- PASS -----")
        print('\n')



# r = te.loada('''
#     J1: $Xo -> S1; v;
#     J2: S1 => $X1; v;
#     J3: S1 => $X2; v;
#     v = 0
# ''')
# r.exportToSBML('testModel1.xml')
# ls = structural.LibStructural()
# ls.loadSBMLFromString(r.getSBML())
# checkElm(1)

ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel1.xml')
checkElm(1)
checkStructure(ls)

ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel2.xml')
checkElm(2)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel3.xml')
checkElm(3)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel4.xml')
checkElm(4)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel5.xml')
checkElm(5)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel6.xml')
checkElm(6)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel7.xml')
checkElm(7)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel8.xml')
checkElm(8)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel9.xml')
checkElm(9)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel10.xml')
checkElm(10)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel11.xml')
checkElm(11)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel12.xml')
checkElm(12)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel13.xml')
checkElm(13)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel14.xml')
checkElm(14)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel15.xml')
checkElm(15)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel16.xml')
checkElm(16)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel17.xml')
checkElm(17)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel18.xml')
checkElm(18)
checkStructure(ls)


ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel19.xml')
checkElm(19)
checkStructure(ls)

# 22
ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel20.xml')
checkElm(20)
checkStructure(ls)
eml1 = ls.getElementaryModes()


# 21
ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel21.xml')
checkElm(21)
checkStructure(ls)
eml2 = ls.getElementaryModes()


# 22
ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel22.xml')
checkElm(22)
checkStructure(ls)


# 23
ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel23.xml')
checkElm(23)
checkStructure(ls)


# 24
ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel24.xml')
checkElm(24)
checkStructure(ls)
