# Test Script for checking elementary modes
# Dec 2017

import structural
import numpy as np
import pkg_resources
import os
import tempfile


def nullspace(A, atol=1e-13, rtol=0):
    A = np.atleast_2d(A)
    u, s, vh = np.linalg.svd(A)
    tol = max(atol, rtol * s[0])
    nnz = (s >= tol).sum()
    ns = vh[nnz:].conj().T
    return ns


def checkThermodynamics(ls, elm, rxnIdIndex):
    # Assume we're ok initially, then try to disprove
    result = True
    nElms = elm.shape[0]

    for k in range(nElms):
        em = elm[k]

        for i in (rxnIdIndex):
            if (not ls.isReactionReversible(i)) and (em[rxnIdIndex.index(i)] < 0):
                result = False;

    return result


def checkElementarity(ls, elm, emType):
    if emType == 'Integer':
        N = ls.getStoichiometryMatrix()
    else:
        d = tempfile.gettempdir()
        resultFile = os.path.join(d,"MetaToolResult.txt")

        if os.path.isfile(resultFile):
            line_array = []
            with open(resultFile) as f:
                for lines in f:
                    line_array.append(lines)

            f.close()

            start_pt = line_array.index('STOICHIOMETRIC MATRIX\n')

            row_num = int(line_array[4].split()[2])
        else:
            print_stmnt = "(" + str(id) + ") Metatool: File Not Found"
            print(print_stmnt)
            return

        N = []

        for i in range(row_num):
            N.append(line_array[start_pt + 3 + i].split())

        N = np.array(N, dtype=float)

    sum = 0
    for j in range(elm.shape[0]):
        em = elm[j]
        indexList = []
        nCols = np.size(em, 0)
        # Find the columns of the entries that are non-zero
        for i in range(nCols):
            if em[i] != 0:
                indexList.append(i)

        # Construct submatrix of stoichiometry matrix. The columns
        # correspond to the indecies in the indexList
        subN = np.empty([N.shape[0], 0])
        for i in indexList:
            subN = np.column_stack((subN, N[:, i]))
        ns = nullspace(subN)
        # If the null space has dim(1) then we pass this em
        sum = sum + int(ns.shape[1])
    # If all pass then the sum should equal the number of elm
    if sum == elm.shape[0]:
        return True
    else:
        return False


def checkElmInteger(id, ls):
    elm = ls.getElementaryModesInteger()
    st = ls.getStoichiometryMatrix()
    p = np.matmul(st, np.transpose(elm))
    rxnIdIndex = range(len(ls.getReactionIds()))

    if not np.any(p):
        if checkElementarity(ls, elm, 'Integer'):
            if checkThermodynamics(ls, elm, rxnIdIndex):
                print_stmnt = "(" + str(id) + ") ------PASS----- Number of modes = " + str(elm.shape[0])
                print(print_stmnt)
            else:
                print_stmnt = "(" + str(id) + ") ------FAIL Thermo-----"
                print(print_stmnt)
        else:
            print_stmnt = "(" + str(id) + ") ------FAIL Elementarity-----"
            print(print_stmnt)
    else:
        print_stmnt = "(" + str(id) + ") ------FAIL N e = 0-----"
        print(print_stmnt)

def checkgEFM(id, ls):
    elm = ls.getgElementaryModes()
    elm = np.around(elm, decimals=2)
    st = ls.getStoichiometryMatrix()
    p = np.matmul(st, np.transpose(elm))
    rxnIdIndex = range(len(ls.getReactionIds()))

    if not np.any(p):
        if checkElementarity(ls, elm, 'Integer'):
            if checkThermodynamics(ls, elm, rxnIdIndex):
                print_stmnt = "(" + str(id) + ") ------PASS----- Number of modes = " + str(elm.shape[0])
                print(print_stmnt)
            else:
                print_stmnt = "(" + str(id) + ") ------FAIL Thermo-----"
                print(print_stmnt)
        else:
            print_stmnt = "(" + str(id) + ") ------FAIL Elementarity-----"
            print(print_stmnt)
    else:
        print_stmnt = "(" + str(id) + ") ------FAIL N e = 0-----"
        print(print_stmnt)

def checkElmDouble(id, ls):
    elm = ls.getElementaryModesDouble()
    rxnIds = ls.getReactionIds()
    emRxnID = ls.getElementaryModesMetaToolRxnIds()

    rxnIdIndex = []
    for i in emRxnID:
        rxnIdIndex.append(rxnIds.index(i))

    d = tempfile.gettempdir()
    resultFile = os.path.join(d,"MetaToolResult.txt")

    if os.path.isfile(resultFile):
        line_array = []
        with open(resultFile) as f:
            for lines in f:
                line_array.append(lines)

        f.close()

        start_pt = line_array.index('STOICHIOMETRIC MATRIX\n')

        row_num = int(line_array[4].split()[2])
    else:
        print_stmnt = "(" + str(id) + ") Metatool: File Not Found"
        print(print_stmnt)
        return

    st = []

    for i in range(row_num):
        st.append(line_array[start_pt + 3 + i].split())

    st = np.array(st, dtype=float)
    p = np.matmul(st, np.transpose(elm))
    if not np.any(p):
        if checkElementarity(ls, elm, 'Double'):
            if checkThermodynamics(ls, elm, rxnIdIndex):
                print_stmnt = "(" + str(id) + ") ------PASS----- Number of modes = " + str(elm.shape[0])
                print(print_stmnt)
            else:
                print_stmnt = "(" + str(id) + ") ------FAIL Thermo-----"
                print(print_stmnt)
        else:
            print_stmnt = "(" + str(id) + ") ------FAIL Elementarity-----"
            print(print_stmnt)
    else:
        print_stmnt = "(" + str(id) + ") ------FAIL N e = 0-----"
        print(print_stmnt)




def run():
    print("\n\n============================================")
    print("Running Integer Elementary mode tests...")
    print("============================================\n")

    for i in range(31):
        ls = structural.LibStructural()
        model_path = pkg_resources.resource_filename('structural', '/test/testModel' + str(i + 1) + '.xml')
        ls.loadSBMLFromFile(model_path)

        if i + 1 in [25,28,29]:
            el = ls.getElementaryModesInteger()
            if len(el) == 0:
                print_stmnt = "(" + str(i + 1) + ") ------PASS----- Number of modes = 0"
                print(print_stmnt)
            else:
                print_stmnt = "(" + str(i + 1) + ") ------FAIL-----"
                print(print_stmnt)
        else:
            checkElmInteger(i + 1, ls)
            
    print("\n\n============================================")
    print("Running gEFM tests...")
    print("============================================\n")

    for i in range(31):
        ls = structural.LibStructural()
        model_path = pkg_resources.resource_filename('structural', '/test/testModel' + str(i + 1) + '.xml')
        ls.loadSBMLFromFile(model_path)

        if i + 1 in [25,28,29]:
            el = ls.getgElementaryModes()
            if len(el) == 0:
                print_stmnt = "(" + str(i + 1) + ") ------PASS----- Number of modes = 0"
                print(print_stmnt)
            else:
                print_stmnt = "(" + str(i + 1) + ") ------FAIL-----"
                print(print_stmnt)
        else:
            checkgEFM(i + 1, ls)

    print("\n\n============================================")
    print("Running Double Elementary mode tests...")
    print("============================================\n")

    for i in range(31):
        ls = structural.LibStructural()
        model_path = pkg_resources.resource_filename('structural', '/test/testModel' + str(i + 1) + '.xml')
        ls.loadSBMLFromFile(model_path)

        if i + 1 in [25,28,29]:
            el = ls.getElementaryModesDouble()
            if len(el) == 0:
                print_stmnt = "(" + str(i + 1) + ") ------PASS----- Number of modes = 0"
                print(print_stmnt)
            else:
                print_stmnt = "(" + str(i + 1) + ") ------FAIL-----"
                print(print_stmnt)
        else:
            checkElmDouble(i + 1, ls)

            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel1.xml')
            # checkElm(1)


            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel2.xml')
            # checkElm(2)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel3.xml')
            # checkElm(3)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel4.xml')
            # checkElm(4)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel5.xml')
            # checkElm(5)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel6.xml')
            # checkElm(6)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel7.xml')
            # checkElm(7)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel8.xml')
            # checkElm(8)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel9.xml')
            # checkElm(9)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel10.xml')
            # checkElm(10)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel11.xml')
            # checkElm(11)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel12.xml')
            # checkElm(12)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel13.xml')
            # checkElm(13)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel14.xml')
            # checkElm(14)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel15.xml')
            # checkElm(15)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel16.xml')
            # checkElm(16)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel17.xml')
            # checkElm(17)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel18.xml')
            # checkElm(18)



            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel19.xml')
            # checkElm(19)


            # # 22
            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel20.xml')
            # checkElm(20)



            # # 21
            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel21.xml')
            # checkElm(21)



            # # 22
            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel22.xml')
            # checkElm(22)



            # # 23
            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel23.xml')
            # checkElm(23)



            # # 24
            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel24.xml')
            # checkElm(24)



            # 25
            # ls = structural.LibStructural()
            # ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/testModel25.xml')
            # el = ls.getElementaryModesInteger()
            # if len(el) == 0:
            # print("(", 25, ") ------PASS-----")
            # else:
            # print("(", 25, ") ------FAIL-----"
