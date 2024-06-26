/* Pairs
 * Author: vdemeulenaere
 * Creation date: 23/05/2023
 */
MACHINE
    Pairs

ABSTRACT_VARIABLES 
    var, var2
    
INVARIANT
    var : REAL * INTEGER &
    var2 : POW(INTEGER * BOOL)
    
INITIALISATION
    var := 1.0 |-> 1 ||
    var2 := {(0 |-> FALSE), (1, TRUE)}

END
