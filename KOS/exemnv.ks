function exemnv {

parameter utime, radial, normal, prograde.

set mnv to node(utime, radial, normal, prograde).
set totalisp to 0.
set totalfuelflow to 0.

list engines in allengines.
for eng in allengines {
    if eng:active {
        set totalisp to (totalisp + eng:isp).
        set totalfuelflow to (totalfuelflow + end:fuelflow).
    }
}

set avadeltaV to (totalisp * 9.80665 * ln(ship:mass/ship:drymass)).
set mnvdeltaV to mnv:deltav:mag. 

if avadeltaV < mnvdeltaV {
    print ("Not enough deltaV").
} else {
    set mf to ship:mass / e^(mnvdeltaV / (totalisp * 9.80665)).
    set burntime to ((ship:mass - mf) / totalfuelflow).
    set spark to (time:seconds + burntime/2).
    set cutoff to (time:seconds + 1.5*burntime).

    print burntime, launchtime, cutoff.

    add mnv.
    wait until time:seconds is (mnv:eta - 10).
    lock steering to mnv:deltav.

    wait until time:seconds is spark.
    lock throttle to 1.
    wait until time:seconds is cutoff.
    lock throttle to 0.
    unlock steering.
    remove mnv.
    }

}