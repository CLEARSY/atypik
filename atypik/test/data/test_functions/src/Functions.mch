/* Functions
* Author: vdemeulenaere
* Creation date: 25/05/2023
*/
MACHINE
    Functions
    
ABSTRACT_VARIABLES
    var, var2, var3, var4, var5, fun
    
INVARIANT 
    var : POW(REAL * INTEGER) &
    var2 : POW(REAL * INTEGER) &
    var3 : BOOL &
    var4 : POW(REAL * POW(INTEGER)) &
    var5 : POW(REAL * INTEGER) &
    fun : POW(INT * REAL * BOOL) &
    var : {0.0, 1.0} --> {0, 1} &
    var : {0.0, 1.0, 2.0} +-> {0, 1} &
    var : {0.0, 1.0, 2.0} >+> {0, 1, 2} &
    var : {0.0, 1.0} >-> {0, 1, 2} &
    var2 : {0.0, 1.0, 2.0, 3.0} +->> {1, 2} &
    var2 : {0.0, 1.0, 2.0} -->> {1, 2} &
    var : {0.0, 1.0} >->> {0, 1}
    
INITIALISATION
    var := {(0.0 |-> 0), (1.0 |-> 1)} ||
    var2 := {(0.0 |-> 1), (1.0 |-> 2), (2.0 |-> 2)} ||
    fun := %(var, var2).(var : INT & var2 : REAL | bool(real(var) = var2)) ||
    var3 := fun(0, 0.0) ||
    var4 := fnc(var) ||
    var5 := rel(var4)
END
