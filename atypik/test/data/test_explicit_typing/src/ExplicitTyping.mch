/* ExplicitTyping
 * Author: vdemeulenaere
 * Creation date: 05/05/2023
 */
MACHINE
    ExplicitTyping

ABSTRACT_VARIABLES 
    abs_var,
    abs_var2,
    abs_var3

ABSTRACT_CONSTANTS 
    abs_cst
    
PROPERTIES
    abs_cst : FLOAT
    
INVARIANT 
    abs_var : INT &
    abs_var2 : BOOL &
    abs_var3 : REAL
   
INITIALISATION 
    abs_var := 0 ||
    abs_var2 := FALSE ||
    abs_var3 := 3.0

END
