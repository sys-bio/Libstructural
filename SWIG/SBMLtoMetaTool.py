import tellurium as te
import structural
import numpy as np
import pkg_resources

#r =te.loada("""
#   $X -> S1; v;   
#    v = 0       
#"""
#)
#%%
model_path = pkg_resources.resource_filename('structural','test')
##
model_path = model_path + '\\testModel'+ "28.xml"
ls = structural.LibStructural()
ls.loadSBMLFromFile(model_path)
ls.getElementaryModesDouble()

def test(ls):
    from structural.test import testLibStructuralSBML
    testLibStructuralSBML.run()
test(ls)

#%%

##ls.loadSBMLFromString(r.getSBML())
#
#
#print ls.getElementaryModesDouble()
#print ('\n')
#print ls.getElementaryModesInteger()

for i in range(1,25):
    
    model_path = pkg_resources.resource_filename('structural','test/testModel'+str(i)+".xml")
    ls = structural.LibStructural()
    ls.loadSBMLFromFile(model_path)
    print ("Test model #" + str(i)+'\n')
#    print (ls.getElementaryModesDouble())
    if type(ls.getElementaryModesDouble()) == np.ndarray:       
        print np.sum(ls.getElementaryModesDouble(), axis=1) == np.sum(ls.getElementaryModesInteger(),axis=1) 

#%%

def converSBMLtoMetaToolString(SBML_str):
    def isAllPositive(v):
        AllPositive = True
    
        for i in v:
            if i < 0:
                AllPositive = False
                break
        return AllPositive

    def isAllNegative(v):
        AllNegative = True
        for i in v:
            if i > 0:
                AllNegative = False
                break
        return AllNegative

    mStr = ''
    
    ls = structural.LibStructural()
    ls.loadSBMLFromString(SBML_str)
    
    rxn_ids = ls.getReactionIds()
    flt_ids = ls.getFloatingSpeciesIds()
    bnd_ids = ls.getBoundarySpeciesIds()
    if bnd_ids == ():
        return None
    matx = ls.getStoichiometryMatrix()
          
    mStr += "-ENZREV" + "\n"

    for i in range(len(rxn_ids)):
        if ls.isReactionReversible(i):
            mStr += rxn_ids[i] + " "
        
    mStr += "\n\n"+"-ENZIRREV" + "\n"
    
    for i in range(len(rxn_ids)):
        if not ls.isReactionReversible(i):
            mStr += rxn_ids[i] + " "
    
    mStr += "\n\n"+"-METINT"+"\n"
    
    for  i in flt_ids:
        mStr += str (i) + " "
    
    mStr += "\n\n"+"-METEXT"+"\n"
    
    mStr += bnd_ids[0] + " "
        
    mStr += "\n\n"+"-CAT"+"\n"  
    
    for i in range(len(rxn_ids)):
        react_list = []
        col = matx[:,i]
        mStr += rxn_ids[i] + " : "
        if isAllPositive(col):
            mStr += bnd_ids[0]
        else:
            for j in range(len(flt_ids)):
                if matx[j,i] < 0:
                    stStr = ''
                    if abs(matx[j,i]) > 1:
                        stStr = str(abs(matx[j,i])) + ' ' 
                    react_list.append(stStr + flt_ids[j])
            mStr += react_list[0]
            for k in range(1,len(react_list)):
                mStr += " + " + react_list[k]
        mStr += " = "
        
        react_list = []
        if isAllNegative(col):
            mStr += bnd_ids[0]
        else:
            for j in range(len(flt_ids)):
                if matx[j,i] > 0:
                    react_list.append(flt_ids[j])
            mStr += react_list[0]
            for k in range(1,len(react_list)):
                mStr += " + " + react_list[k]
        mStr += " .\n"
    
    return mStr


mStr = converSBMLtoMetaToolString(r.getSBML())

f = tempfile.TemporaryFile (delete=False)
d = tempfile.gettempdir()

resultFile = d+"\\MetaToolResult.txt"
metatoolFile = f.name
f.write (mStr)
f.close()
te.saveToFile ('c:\\tmp\\m.txt', mStr)

pathToMetatool = site.getsitepackages()[1] + '\\structural\\' + 'metaToolDouble.exe'

exit_code = subprocess.call ([pathToMetatool, metatoolFile, resultFile])

#print te.readFromFile(resultFile)
def getElementaryModesDouble(resultFile):
    line_array = []
    with open(resultFile) as f:
        for lines in f:
            line_array.append(lines)
            
    start_pt = line_array.index("ELEMENTARY MODES\n")
    
    if line_array[start_pt+1] == ' \n':
        row_num = int(list(line_array[start_pt+2].split()[2])[1])
#        col_num = int(list(line_array[start_pt+2].split()[4])[1])
        
        elementaryModeMatrix = []
        for i in range(row_num):
            elementaryModeMatrix.append(line_array[start_pt+3+i].split())
        elementaryModeMatrix = np.array(elementaryModeMatrix, dtype=float)
    f.close()
    return elementaryModeMatrix

print getElementaryModesDouble(resultFile)

#%%

def getElementaryModesDouble(self):
    def isAllPositive(v):
        AllPositive = True
    
        for i in v:
            if i < 0:
                AllPositive = False
                break
        return AllPositive

    def isAllNegative(v):
        AllNegative = True
        for i in v:
            if i > 0:
                AllNegative = False
                break
        return AllNegative

    mStr = ''
    
    rxn_ids = self.getReactionIds()
    flt_ids = self.getFloatingSpeciesIds()
    bnd_ids = self.getBoundarySpeciesIds()
    if bnd_ids == ():
        return None
    matx = self.getStoichiometryMatrix()
          
    mStr += "-ENZREV" + "\n"

    for i in range(len(rxn_ids)):
        if self.isReactionReversible(i):
            mStr += rxn_ids[i] + " "
        
    mStr += "\n\n"+"-ENZIRREV" + "\n"
    
    for i in range(len(rxn_ids)):
        if not self.isReactionReversible(i):
            mStr += rxn_ids[i] + " "
    
    mStr += "\n\n"+"-METINT"+"\n"
    
    for  i in flt_ids:
        mStr += str (i) + " "
    
    mStr += "\n\n"+"-METEXT"+"\n"
    
    mStr += bnd_ids[0] + " "
        
    mStr += "\n\n"+"-CAT"+"\n"  
    
    for i in range(len(rxn_ids)):
        react_list = []
        col = matx[:,i]
        mStr += rxn_ids[i] + " : "
        if isAllPositive(col):
            mStr += bnd_ids[0]
        else:
            for j in range(len(flt_ids)):
                if matx[j,i] < 0:
                    stStr = ''
                    if abs(matx[j,i]) > 1:
                        stStr = str(abs(matx[j,i])) + ' ' 
                    react_list.append(stStr + flt_ids[j])
            mStr += react_list[0]
            for k in range(1,len(react_list)):
                mStr += " + " + react_list[k]
        mStr += " = "
        
        react_list = []
        if isAllNegative(col):
            mStr += bnd_ids[0]
        else:
            for j in range(len(flt_ids)):
                if matx[j,i] > 0:
                    react_list.append(flt_ids[j])
            mStr += react_list[0]
            for k in range(1,len(react_list)):
                mStr += " + " + react_list[k]
        mStr += " .\n"


    f = tempfile.TemporaryFile (delete=False)
    d = tempfile.gettempdir()
    
    resultFile = d+"\\MetaToolResult.txt"
    metatoolFile = f.name
    f.write (mStr)
    f.close()
    te.saveToFile ('c:\\tmp\\m.txt', mStr)
    
    pathToMetatool = site.getsitepackages()[1] + '\\structural\\' + 'metaToolDouble.exe'
    
    exit_code = subprocess.call ([pathToMetatool, metatoolFile, resultFile])
    
    if exit_code == 0:
        line_array = []
        with open(resultFile) as f:
            for lines in f:
                line_array.append(lines)
                
        start_pt = line_array.index("ELEMENTARY MODES\n")
        
        if line_array[start_pt+1] == ' \n':
            row_num = int(list(line_array[start_pt+2].split()[2])[1])
    #        col_num = int(list(line_array[start_pt+2].split()[4])[1])
            
            elementaryModeMatrix = []
            for i in range(row_num):
                elementaryModeMatrix.append(line_array[start_pt+3+i].split())
            elementaryModeMatrix = np.array(elementaryModeMatrix, dtype=float)
        f.close()
        return elementaryModeMatrix
    else:
        return exit_code

