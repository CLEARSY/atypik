/* Arithmetic
 * Author: vdemeulenaere
 * Creation date: 23/05/2023
 */
MACHINE
    Arithmetic

ABSTRACT_VARIABLES
    add, sub, mul, div, pow, var1, var2,
    add_r, sub_r, mul_r, div_r, max_i, min_i,
    max_r, min_r, var3, var4, sum_i, mul_i, sum_r,
    mul_r2
    
INVARIANT
    add : INTEGER &
    sub : INTEGER &
    mul : INTEGER &
    div : INTEGER &
    pow : INTEGER &
    var1 : INTEGER &
    var2 : INTEGER &
    add_r : REAL &
    mul_r : REAL &
    sub_r : REAL &
    div_r : REAL &
    max_i : INT &
    min_i : INT &
    max_r : REAL &
    min_r : REAL &
    var3 : BOOL &
    var4 : INT &
    sum_i : INT &
    mul_i : INT &
    sum_r : REAL &
    mul_r2 : REAL
    
INITIALISATION
    var1 := pred(MININT) ||
    var2 := succ(MAXINT) ||
    add := -var1 + var2 ||
    sub := floor(add_r) - ceiling(sub_r) ||
    mul := var1 * var2 ||
    div := var1 / var2 ||
    pow := var1 ** var2 ||
    add_r := real(var1) + real(var2) ||
    sub_r := 3.14 - real(var1) ||
    mul_r := real(var1) * real(var1) ||
    div_r := real(var1) / real(var2) ||
    var3 := bool(var1 = var2 or FALSE = TRUE) ||
    max_i := max({1,2,3}) ||
    min_i := min({1,2,3}) ||
    max_r := max({1.0,2.0,3.0}) ||
    min_r := min({1.0,2.0,3.0}) ||
    var4 := card({1, 2}) ||
    sum_i := SIGMA xx.(xx : {1, 2, 3} | xx + 1) ||
    mul_i := PI xx.(xx : NAT1 & xx <= 3 | xx) ||
    sum_r := SIGMA xx.(xx : {1.0, 1.1} | xx) ||
    mul_r2 := PI xx.(xx : {1.1, 1.2, 1.3} | xx) 

END
