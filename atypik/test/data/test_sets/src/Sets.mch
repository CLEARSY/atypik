/* Sets
* Author: vdemeulenaere
* Creation date: 10/05/2023
*/
MACHINE
    Sets
    
SETS 
    ENS = {Elt1, Elt2};
    ABS_ENS
    
ABSTRACT_CONSTANTS 
    cst, cst2
    
ABSTRACT_VARIABLES
    ens, ens2, ens3, ens4, ens5,
    diff, union_, inter_, union_2, inter_2,
    union_3, inter_3
    
PROPERTIES
    cst : ABS_ENS &
    cst2 : BOOL
    
INVARIANT 
    ens : POW(BOOL * INTEGER) &
    ens2 : POW1(INTEGER) &
    ens3 : FIN(ens2) &
    ens4 : FIN1(ens3) &
    ens5 : POW(POW(ABS_ENS) * POW(REAL) * POW(FLOAT)) &
    diff : POW(INTEGER) &
    union_ : POW(NAT) &
    inter_ : POW(NAT1) &
    union_2 : POW(NATURAL) &
    inter_2 : POW(NATURAL1) &
    union_3 : POW(INTEGER * ABS_ENS) &
    inter_3 : POW(INTEGER * ABS_ENS)
    
INITIALISATION
    ens5 := {ens, ens2, ens3 | ens <: ABS_ENS & ens2 <: REAL & ens3 <: FLOAT} ||
    ens := {TRUE, FALSE} * (0..10) ||
    ens2 := {1} ||
    ens3 := {0} ||
    ens4 := {4} ||
    diff := ens2 - ens3 ||
    union_ := ens2 \/ ens3 ||
    inter_ := ens2 /\ ens3 ||
    union_2 := union({ens2, ens3}) ||
    inter_2 := inter({ens2, ens3}) ||
    union_3 := UNION (cst, cst2).(cst : INT & cst2 : ABS_ENS | {cst} * {cst2}) ||
    inter_3 := INTER (cst, cst2).(cst : INT & cst2 : ABS_ENS | {cst} * {cst2})

END
