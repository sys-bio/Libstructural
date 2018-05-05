Example Models
===============

The models contained in the package directory, **.../test/** , are shown below with their respective Antimony string representation. The J's indicate reaction id and species with "$" sign in front of them represent boundary species which are not shown in the figures. The "v" represents reaction rate.


| **Test Model 1:**
| J1: $Xo -> S1; v;
| J2: S1 => $X1; v;
| J3: S1 => $X2; v;
| v = 0

.. figure:: ./testmodels/testModel1.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 2:**
| J1: $Xo -> S1; v;
| J2: S1 -> $X1; v;
| J3: S1 -> $X2; v;
| v = 0

.. figure:: /testmodels/testModel2.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 3:**
| J1: $Xo -> S1; v;
| J2: S1 -> S2; v;
| J3: S2 => $X1; v;
| J4: S2 => $X2; v;
| J5: S1 => $X3; v;
| v = 0

.. figure:: /testmodels/testModel3.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 4:**
| J1: $Xo -> S1; v;
| J2: S1 -> S2; v;
| J3: S2 -> S3; v;
| J4: S1 -> S3; v;
| J5: S3 -> $X1; v;
| v = 0

.. figure:: /testmodels/testModel4.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 5:**
| J1: $Xo => S1; v;
| J2: S1 => S2; v;
| J3: S2 => S3; v;
| J4: S1 => S3; v;
| J5: S2 => S4; v;
| J6: S3 => S4; v;
| J7: S4 => $X1; v;
| v = 0

.. figure:: /testmodels/testModel5.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 6:**
| J1: $Xo -> S1; v;
| J2: S1 -> S2; v;
| J3: S2 -> S3; v;
| J4: S1 -> S3; v;
| J5: S3 -> $X1; v;
| J6: S2 -> S4; v;
| J7: S2 -> S5; v;
| J8: S4 -> S5; v;
| J9: S5 ->; v;
| v = 0

.. figure:: /testmodels/testModel6.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 7**
| J1: $Xo -> S1; v;
| J2: S1 -> S2; v;
| J3: S2 -> S3; v;
| J4: S1 -> S3; v;
| J5: S3 -> $X1; v;
| J6: S3 -> $X2; v;
| v = 0


.. figure:: /testmodels/testModel7.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 8**
| J1: S1 -> S2; v;
| J2: S2 -> S1; v;
| J3: S2 -> S3; v;
| J4: S3 -> S2; v;
| v = 0

.. figure:: /testmodels/testModel8.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 9**
| J1: $Xo -> S1; v;
| J2: S1 -> $X2; v;
| J3: S1 -> S2; v;
| J4: S2 -> $X3; v;
| J5: S2 -> $X4; v;
| J6: $X1 -> S3; v;
| J7: S3 -> S2; v;
| J8: S3 -> $X5; v;
| v = 0

.. figure:: /testmodels/testModel9.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 10:**
| J1: $Xo -> S1; v;
| J2: S1 -> S3; v;
| J3: S1 -> S2; v;
| J4: S2 -> S4; v;
| J5: S4 -> S3; v;
| J6: S3 -> S5; v;
| J7: S4 -> S6; v;
| J8: S6 -> $X1; v;
| J9: S6 -> S5; v;
| v = 0

.. figure:: /testmodels/testModel10.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 11:**
| J1: $Xo -> S1; v;
| J2: $X1 -> S1; v;
| J3: S1 -> $X2; v;
| v = 0

.. figure:: /testmodels/testModel11.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 12:**
| J1: $Xo -> S1; v;
| J2: S1 -> S2 + S4; v;
| J3: S2 -> S3 + S4 ; v;
| J4: S3 -> S4; v;
| J5: S4 -> $X1; v;
| v = 0

.. figure:: /testmodels/testModel12.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 13:**
| J1: $Xo -> S1; v;
| J2: S1 -> S2; v;
| J3: S2 -> S3; v;
| J4: S1 -> S3; v;
| J5: S3 -> S4; v;
| J6: S3 -> S5; v;
| J7: S4 -> S5; v;
| J8: S5 -> $X1; v;
| v = 0

.. figure:: /testmodels/testModel13.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 14:**
| J1: $Xo -> S1; v;
| J2: S1 -> S2; v;
| J3: S2 -> S3; v;
| J4: S1 -> S3; v;
| J5: S3 -> S4; v;
| J6: S3 -> S5; v;
| J7: S4 -> S5; v;
| J8: S5 -> S6; v;
| J9: S5 -> S7; v;
| J10: S6 -> S7; v;
| J11: S7 -> $X1; v;
| v = 0

.. figure:: /testmodels/testModel14.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 15:**
| J1: $Xo -> S1 + S2; v;
| J2: S1 -> S3; v;
| J3: S2 + S3 -> $X1; v;
| v = 0

.. figure:: /testmodels/testModel15.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 16:**
| J1: $Xo -> S1; v;
| J2: S1 + $X1 -> S2; v;
| J3: S2 -> S1 + $X2; v;
| v = 0

.. figure:: /testmodels/testModel16.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 17:**
| J1: $Xo => S1; v;
| J2: S1 + $X1 => S2; v;
| J3: S2 => S1 + $X2; v;
| v = 0

.. figure:: /testmodels/testModel17.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 18**
| J1: $Xo -> S1; v;
| J2: S1 + S2 -> S3 + $X1; v;
| J3: S1 + S3 -> S2 + $X2; v;
| v = 0

.. figure:: /testmodels/testModel18.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 19:**
| J1: $Xo + S2 -> S1; v;
| J2: S1 -> S2 + S5; v;
| J3: S5 + S4 -> S3; v;
| J4: S3 -> S4 + $X1; v;
| v = 0

.. figure:: /testmodels/testModel19.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 20:**
| J1: $Xo => S1; v;
| J2: S1 => $X2; v;
| J3: S1 => S2; v;
| J4: S2 => $X3; v;
| J5: S2 => $X4; v;
| J6: $X1 => S3; v;
| J7: S3 => S2; v;
| J8: S3 => $X5; v;
| v = 0

.. figure:: /testmodels/testModel20.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 21**
| J1: $Xo => S1; v;
| J2: S1 => $X2; v;
| J3: S1 -> S2; v;
| J4: S2 => $X3; v;
| J5: S2 => $X4; v;
| J6: $X1 => S3; v;
| J7: S3 => S2; v;
| J8: S3 => $X5; v;
| v = 0

.. figure:: /testmodels/testModel21.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 22:**
| J1: $Xo -> S1; v;
| J2: S1 => $X2; v;
| J3: S1 -> S2; v;
| J4: S2 => $X3; v;
| J5: S2 => $X4; v;
| J6: $X1 => S3; v;
| J7: S3 => S2; v;
| J8: S3 => $X5; v;
| v = 0

.. figure:: /testmodels/testModel22.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 23:**
| J1: $Xo -> S1; v;
| J2: S1 => $X2; v;
| J3: S1 -> S2; v;
| J4: S2 => $X3; v;
| J5: S2 => $X4; v;
| J6: $X1 => S3; v;
| J7: S3 -> S2; v;
| J8: S3 => $X5; v;
| v = 0

.. figure:: /testmodels/testModel23.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 24:**
| J1: $Xo -> S1; v;
| J2: S1 => $X2; v;
| J3: S1 -> S2; v;
| J4: S2 => $X3; v;
| J5: S2 => $X4; v;
| J6: $X1 -> S3; v;
| J7: S3 -> S2; v;
| J8: S3 => $X5; v;
| v = 0

.. figure:: /testmodels/testModel24.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 25:**
| $X -> S1; v;
| v = 0

.. figure:: /testmodels/testModel25.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 26:**
| J1: $Xo -> S1; v;
| J2: $X1 -> S2; v;
| J3: S2 -> S3; v;
| J4: S3 -> S1 + S4; v;
| J5: S4 -> S3 + S5; v;
| J6: S5 -> $X2; v;
| v = 0

.. figure:: /testmodels/testModel26.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 27:**
| J1: $Xo -> S1 + S2; v;
| J2: S1 -> S2; v;
| J3: S2 -> $X1; v;
| v = 0

.. figure:: /testmodels/testModel27.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 28:**
| J1: $Xo + S1 => S2; v;
| J2: S2 => S3 + S4; v;
| J3: S1 => S5 + S3; v;
| J4: S4 + S5 => S6; v;
| J5: S6 => $X1; v;
| v = 0

.. figure:: /testmodels/testModel28.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 29:**
| J1: $Xo -> S1; v
| J2: S1 -> $X1; v;
| v = 0

.. figure:: /testmodels/testModel29.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 30:**
| J1: $Xo => S1; v
| J2: S1 => S3; v;
| J3: S1 => S2; v;
| J4: S2 => S5; v;
| J5: S2 => S4; v;
| J6: S3 => S6; v;
| J7: S3 => S7; v;
| J8: S7 => S6; v;
| J9: S7 => S4;v;
| J10: S5 => S4; v;
| J11: S6 => $X1; v;
| J12: S5 => $X2; v;
| v = 0

.. figure:: /testmodels/testModel30.png
   :figclass: align-center
   :scale: 18 %

| **Test Model 31:**
| J1: $Xo => S1; v;
| J2: S1 => S2 + S3; v;
| J3: S2 + S3 => S4 + S5; v;
| J4: S5 => $X1; v;
| J5: S4 => S2; v;
| J6: S5 => S1; v;
| v = 0

.. figure:: /testmodels/testModel31.png
   :figclass: align-center
   :scale: 18 %
