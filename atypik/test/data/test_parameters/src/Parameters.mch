/* Parameters
 * Author: vdemeulenaere
 * Creation date: 28/06/2023
 */
MACHINE
    Parameters(p1, p2, p3, SET_ONE, SET_TWO)

CONSTRAINTS
    p1 : INT &
    p2 : BOOL &
    p3 : SET_ONE &
    card(SET_ONE) = 10
    
END
