/* Relation
 * Author: vdemeulenaere
 * Creation date: 23/05/2023
 */
MACHINE
    Relation

ABSTRACT_VARIABLES
    var, var2, var3, inv, proj, proj2,
    direct_prod, paral_prod, iter, rfl_closure,
    closure_var, var4, var5, var6, var7, var8
    
INVARIANT
    var : POW(POW(INTEGER * BOOL)) &
    var2 : POW(INTEGER * INTEGER) &
    var3 : POW(BOOL * INTEGER) &
    inv : POW(BOOL * INTEGER) &
    proj : POW(INTEGER * BOOL * INTEGER) &
    proj2 : POW(INTEGER * BOOL * BOOL) &
    direct_prod : POW(BOOL * (INTEGER * REAL)) &
    paral_prod : POW((BOOL * REAL) * (INTEGER * BOOL)) &
    iter : POW(INTEGER * INTEGER) &
    rfl_closure : POW(INTEGER * INTEGER) &
    closure_var : POW(INTEGER * INTEGER) &
    dom(var3) = {TRUE, FALSE} &
    ran(var3) = {1,2,3} &
    inv[{TRUE, FALSE}] = {1, 2} &
    var4 : POW(BOOL * INTEGER) &
    var5 : POW(BOOL * INTEGER) &
    var6 : POW(BOOL * INTEGER) &
    var7 : POW(BOOL * INTEGER) &
    var7 : POW(BOOL * INTEGER) &
    var8 : POW(BOOL * INTEGER)
    

INITIALISATION
    var := 0..5 <-> BOOL ||
    var2 := id({1,2,3}) ||
    inv := {(0 |-> FALSE), (1 |-> TRUE)}~ ||
    proj := prj1({0,1}, {FALSE, TRUE}) || 
    proj2 := prj2({0,1}, {FALSE, TRUE}) ||
    var3 := (inv ; var2) ||
    direct_prod := var3 >< {(FALSE |-> 0.0), (TRUE |-> 1.0)} ||
    paral_prod := (var3 || {(0.0 |-> FALSE), (1.0 |-> TRUE)}) ||
    iter := iterate(var2, 2) ||
    rfl_closure := closure(var2) ||
    closure_var := closure1(var2) ||
    var4 := BOOL <<| inv ||
    var5 := {FALSE, TRUE} <| inv ||
    var6 := var5 |> INTEGER ||
    var7 := var6 |>> {1,2} ||
    var8 := var6 <+ var7

END
