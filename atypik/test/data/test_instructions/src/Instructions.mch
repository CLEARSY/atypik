/* Instructions
* Author: vdemeulenaere
* Creation date: 05/06/2023
*/
MACHINE
    Instructions
    
ABSTRACT_VARIABLES 
    var, var2, var3, var4, var5, 
    var6, var7, var8, var9, var10,
    var11, var12, var13, var14, fct
    
SETS
    ABS_SET
    
    
INVARIANT 
    var : INT &
    var2 : BOOL &   
    var3 : NAT &
    var4 : REAL &
    var5 : INTEGER & 
    var6 : INT &
    var7 : REAL &
    var8 : INTEGER &
    var9 : REAL &
    var10 : REAL &
    var11 : ABS_SET &
    var12 : ABS_SET &
    var13 : INT &
    var14 : INT &
    fct : POW(INT * BOOL * BOOL)
    
INITIALISATION
    BEGIN 
        // Assignments
        var, var2 := 0, FALSE ||
        fct(0, FALSE) := FALSE ||
        // Skip
        skip ||
        // Assertion
        ASSERT var = 1 THEN
            var3 := var
        END ||
        // Choice
        CHOICE 
            var4 := 3.14
        OR
            var4 := 4.2
        END ||
        // Conditional
        IF var4 = 3.14 THEN
            var5 := 1
        ELSE IF var4 = 4.2 THEN
                var5 := 2
            ELSE
                var5 := 3
            END
        END ||
        // Select 
        SELECT var /= var5 THEN
            var6 := var
        WHEN var = 1 THEN 
            var6 := var + 1
        WHEN var = 2 THEN
            var6 := var + 2
        ELSE 
            var6 := var - 1
        END ||
        // Case 
        CASE var6 OF
            EITHER 0 THEN
                var7 := 1.0
            OR 1 THEN
                var7 := 2.0
            ELSE
                var7 := 3.0
            END
        END ||
        // Any
        ANY x1, x2 WHERE 
            x1 : INT &
            x2 : NAT &
            x1 ** 2 + x2 ** 2 = 25
        THEN
            var8 := x1 + x2
        END ||
        // Let
        LET x1, x2 BE 
            x1 = var4 &
            x2 = var4 * 2.0
        IN 
            var9 := x1 + x2 ||
            var10 := x1 - x2
        END ||
        // Becomes in
        var11, var12 :: (ABS_SET * ABS_SET) ||
        // Becomes such that
        var13, var14 : (var13 : INT & var13 > 0 & var14 /= var13)
        
    END
    
    
END    
