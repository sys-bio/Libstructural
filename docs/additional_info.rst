Uploading Wheels (pip)
======================
**Before generating and uploading wheels, read this information carefully:**

Versioning in PyPI is very strict. You need to follow a `specific guideline <https://www.python.org/dev/peps/pep-0440/>`_ when bumping the version number.

There is absolutely NO do-overs. Once you upload a version, that namespace is occupied forever. Therefore, we will follow post-release version scheme for minor bug fixes. When you wish to upload a new version without changes in version number, add .post<post-release-version-number> at the end. It is not at all unusual to upload several posts for a given release in this manner. Just increment the post number in the setup.py script.

LibStructural supports Microsoft Windows and Apple Mac OS X. Check each section for detailed instructions.

-------
Windows
-------

To generate wheels and upload it to PyPI, you need Python, `twine <https://pypi.org/project/twine/>`_, and PyPI account with the right authentication. Make sure that the version number in setup.py is correct.

To build a Python wheel run:

``python setup.py bdist_wheel --python-tag=cp27 --plat-name=win_amd64``

in command console. Make sure you are in a correct python version corresponding with the python tag. The above line generates a Python wheel for 64-bit Python 2.7. Change flags if you want to generate wheels for different Python versions (cp35, cp36, etc.) or different architecture (win32). Once finished, a wheel will be generated under dist folder within sub-folder (Win_64/2, etc.) with name such as:

``structural-2.3.0.post1-cp27-none-win_amd64.whl``

Make sure to rename ABI tag 'none' to 'cp27m' (or 'cp35m', 'cp36m', etc.), e.g.:

``structural-2.3.0.post1-cp27-cp27m-win_amd64.whl``

To upload the wheels to PyPI, run:

``twine upload <file-location> -u <username> -p <password>``

It is also possible to configure a .pypirc file so that you don't have to type in your username and password every time when you upload. See `this page <https://packaging.python.org/tutorials/distributing-packages/#create-an-account>`_ for more info.

---------
Mac OS X
---------

The process for building PyPI distributions on Mac parallels the process for Windows. The following command lines are useful:

``python setup.py bdist_wheel --python-tag=cp27 --plat-name=macosx_10_11_x86_64``

``python3 setup.py bdist_wheel --python-tag=cp36 --plat-name=macosx_10_11_x86_64``

The wheel will be put in the dist directory. If one of the fields in the wheel directory is "none", change to cp27m (replace 27 with whatever Python version you used).

To upload the package, use twine :

``twine upload structural-2.1.0-cp27-cp27m-macosx_10_11_x86_64.whl``
