Installation on Python
======================

Currently we support Python 2.7+ and 3.6+ for both Windows (64 bit and 32-bit) and MacOS.
Follow the steps below to install libStructural.

By far the easiest way to get LibStructural working is to install Tellurium (tellurium.analogmachine.org) and type at the Telluirum python console the instruation:


.. code:: python

   import tellurium as te
   te.installPackage ('structural')
   
.. end

This should work for Windows and Mas OSX.

If you want to install LibStructural on the standard python distribution found at python.org then follow the instructions below:

------------
Dependencies
------------

LibStructural depends on numpy. If you don't have numpy installed you'll need to install it. To check if you have numpy installed
type the following at the Python console:

.. code:: python

   import numpy
   
.. end

If there are no errors then you have numpy installed. If there are errors follow these instructions. 


Windows 7/8/10
--------------

Download the numpy binaries from:

https://www.lfd.uci.edu/~gohlke/pythonlibs/#numpy

If you are using a 32-bit python download: numpy-1.14.0+mkl-cp37-cp37m-win32.whl

If you are using a 64-bit python download: numpy-1.14.0+mkl-cp37-cp37m-win_amd64.whl

Copy these files to c:\pythonXX\scripts where XX is your python version eg 27. 

At the windows command line, cd to c:\pythonXX\scripts and type: pip install numpy-1.14..etc

This should install numpy. 

Max OSX
-------

For the mac installation pleae see: https://scipy.org/install.html

-----------
Using Pip
-----------

Once you have numpy installed you can use a standard pip installation to get libstructural. See below is you are using Tellurium.

libStructural is available via PyPI, which is the simplest method to obtain libStructural.
To install libStructural using pip, run following line on console or terminal:

``pip install structural``


--------------------
From Python Console
--------------------

If you are using Tellurium (`tellurium.analogmachine.org <http://tellurium.analogmachine.org/>`_) then you can install structural directly from the Python console:

.. code:: python

    import tellurium as te
    te.installPackage ('structural')

.. end

----------------------
From Python Terminal
----------------------

1 - Use `git <https://git-scm.com/>`_ and type the following command in the console or terminal which will clone the source code under Libstructural folder.

``git clone https://github.com/yosefmaru/Libstructural.git``

2 - Open your console or terminal. Go to package's root directory and Run the installer script by using the following command line. This will install **structural** to the Python release associated with the console or terminal you are using.

``python setup.py install``

3 - Test the Libstructural by importing it in Python.

``import structural``

**If you have trouble importing the module with the setup script, check to see if there are multiple Python installations on your machine and also check the output of the setup script to see which version of Python is the install target.**
