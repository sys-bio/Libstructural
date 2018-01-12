Installation on Python
======================

Currently we support Python 2.7+ and 3.6+ for both Windows (64 bit and 32-bit) and MacOS.
Follow the steps below to install LibStructural.

By far the easiest way to get LibStructural working is to install Tellurium (tellurium.analogmachine.org) and type at the Tellurium Python console the instruction:


.. code:: python

   import tellurium as te
   te.installPackage ('structural')

.. end

This should work for Windows and Mas OSX version of Tellurium. That completes the installation.

If you want to install LibStructural on the standard python distribution found at python.org then follow the instructions below:

-------------
Dependencies
-------------

For those who want to install LibStructural on other distributions including the standard distibution at python.org, you'll need the following instructions.

LibStructural depends on numpy and scipy. If you don't have numpy and scipy installed you'll need to install it. To check if you have numpy and scipy installed type the following at the Python console:

.. code:: python

   import numpy
   import scipy

.. end

If there are no errors then you have numpy and scipy installed. If there are errors follow the instructions below:

**NOTE:** To install the dependencies you will need to have pip available with your python distribution. Pip is already installed if you're using Python 2 >=2.7.9 or Python 3 >=3.4 binaries downloaded from python.org (by default, in **../PythonXX/Scripts** directory). If you don't have pip installed, go to the link below to install pip.

https://pip.pypa.io/en/stable/installing/

Numpy and scipy on Windows 7/8/10
-----------------------

**Option 1:** You can simply run **pip install numpy** from your command line (make sure to cd to your python directory).


**Option 2:** If option 1 causes errors, follow the following work around:

Download the numpy binaries from:

https://www.lfd.uci.edu/~gohlke/pythonlibs/#numpy

If you are using a 32-bit Python download: numpy-1.14.0+mkl-cp37-cp37m-win32.whl

If you are using a 64-bit Python download: numpy-1.14.0+mkl-cp37-cp37m-win_amd64.whl

Copy which ever file is appropriate to c:\\PythonXX\\Scripts where XX is your python version eg 27.

At the windows command line, cd to c:\\PythonXX\\Scripts and type: pip install numpy-1.14..etc

This should install numpy, if it doesn't find someone who can help you.

Numpy on Max OSX
----------------

For the Mac installation please see: https://scipy.org/install.html

----------------------------------
Using Pip to Install LibStructural
----------------------------------

Once you have numpy installed you can use a standard pip installation to install Libstructural. See below if you are using Tellurium.

LibStructural is available via PyPI. To install LibStructural using pip, run following line on at your terminal window. If you are doing this on Windows
you will need to make sure you are in c:\\PythonXX\\Scripts directory, otherwise it is likely it won't find pip.

``pip install structural``


---------------------------------
From Python the Tellurium Console
---------------------------------

If you are using Tellurium (`tellurium.analogmachine.org <http://tellurium.analogmachine.org/>`_) then you can install structural directly from the Python console:

.. code:: python

    import tellurium as te
    te.installPackage ('structural')

.. end

------------------------------
From Python Terminal (Experts)
------------------------------

If you are familar with obtaining your code from the GitHub repo you can follow these instructions.

1 - Use `git <https://git-scm.com/>`_ and type the following command in the console or terminal which will clone the source code under Libstructural folder.

``git clone https://github.com/yosefmaru/Libstructural.git``

2 - Open your console or terminal. Go to package's root directory and Run the installer script by using the following command line. This will install **structural** to the Python release associated with the console or terminal you are using.

``python setup.py install``

3 - Test the Libstructural by importing it in Python.

``import structural``

**If you have trouble importing the module with the setup script, check to see if there are multiple Python installations on your machine and also check the output of the setup script to see which version of Python is the install target.**
