import tellurium as te
import structural
import numpy as np
import SBMLtoMetaTool
import tempfile
import subprocess
import site


r =te.loada("""
   2A -> B; v;
   A + B -> C; v;
   A + C -> D + B; v;
   $x -> A; v;
   B -> $x0; v;
    v = 0       
"""
)

mStr = SBMLtoMetaTool.converSBMLtoMetaToolString(r.getSBML())

f = tempfile.TemporaryFile (delete=False)
metatoolFile = f.name
f.write (mStr)
f.close()
te.saveToFile ('c:\\tmp\\m.txt', mStr)

pathToMetatool = site.getsitepackages()[1] + '\\structural\\' + 'metaToolDouble.exe'

exit_code = subprocess.call ([pathToMetatool, metatoolFile, "result.txt"])

