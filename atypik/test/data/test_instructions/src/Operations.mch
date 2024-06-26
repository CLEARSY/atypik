/* Operations
 * Author: vdemeulenaere
 * Creation date: 13/06/2023
 */
MACHINE
    Operations

OPERATIONS 
    out1, out2 <-- pair(in1, in2) =
    PRE
        in1 : INT & in2 : INT
    THEN
        BEGIN
            out1 := in1 ||
            out2 := in2
        END
    END

END
