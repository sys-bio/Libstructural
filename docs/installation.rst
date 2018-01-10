Installation on Python
======================

Currently, we support Python 2.7+ and 3.6+ for both Windows (64 bit and 32-bit) and MacOS.
Follow the steps below to install libStructural.

------------
Dependencies
------------

LibStructural depends on numpy. If you don't have numpy installed you'll need to install it. To check if you have numpy installed
type the following at the Python console:

.. code:: python
   import numpy
.. end

If there are no errors then you have numpy installed. If not follow the instructions below:


-----------
Using Pip
-----------

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
