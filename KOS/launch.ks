function SafeStage {
    wait until stage:ready.
    stage.
}

function Launch {
    set throttle to 1.
    until ship:verticalspeed > 0 {
        safestage.
        wait 1.
    }

    print("Vessel launched").
}

function AtmoBreach {
    lock TargetPitch to 88.963 - 1.03287 * alt:radar^0.409511.
    lock steering to heading(90,TargetPitch).
    local OldThrust is ship:availablethrust.
    until alt:apoapsis > 80000 {
        if OldThrust > ship:availablethrust + 10 {
            SafeStage.
            wait until stage:ready.
            set OldThrust to ship:availablethrust.
        }
    }
    print("Atmospheric escape achieved").
}

function Circularise {
    wait until alt:radar > ship:orbit:body:atm:height.
    local i is -1000.
    local Besti is -1000.
    set Circle to node(time:seconds + eta:apoapsis, 0, 0, i).
    add Circle.
    set BestE to Circle:orbit:eccentricity.
    remove Circle.
    from {set i to 0.} until i > 1000 step {set i to i + 1000.} do {
        set Circle to node(time:seconds + eta:apoapsis, 0, 0, i). 
        add Circle.
        set NewE to Circle:orbit:eccentricity.
        remove Circle.
        if NewE < BestE {
            set BestE to NewE.
            set Besti to i.
        }
    }

    from {set i to Besti - 500.} until i > Besti + 500 step {set i to i +100.} do {
        set Circle to node(time:seconds + eta:apoapsis, 0, 0, i). 
        add Circle.
        set NewE to Circle:orbit:eccentricity.
        remove Circle.
        if NewE < BestE {
            set BestE to NewE.
            set Besti to i.
        }
    }

    from {set i to Besti - 250.} until i > Besti + 250 step {set i to i +50.} do {
        set Circle to node(time:seconds + eta:apoapsis, 0, 0, i). 
        add Circle.
        set NewE to Circle:orbit:eccentricity.
        remove Circle.
        if NewE < BestE {
            set BestE to NewE.
            set Besti to i.
        }
    }

    from {set i to Besti - 125.} until i > Besti + 125 step {set i to i +5.} do {
        set Circle to node(time:seconds + eta:apoapsis, 0, 0, i). 
        add Circle.
        set NewE to Circle:orbit:eccentricity.
        remove Circle.
        if NewE < BestE {
            set BestE to NewE.
            set Besti to i.
        }
    }

    RunManuevre(time:seconds + eta:apoapsis, 0, 0, Besti).

    print("Circularisation complete").
}

function RunManuevre {

    parameter utime, radial, normal, prograde.

    set mnv to node(utime, radial, normal, prograde).
    add mnv.
    set totalisp to 0.
    set totalfuelflow to 0.

    list engines in allengines.
    for eng in allengines {
        if eng:ignition {
            set totalisp to (totalisp + eng:vacuumisp).
            set totalfuelflow to (totalfuelflow + (eng:availablethrust / eng:vacuumisp)).
        }
    }

    set avadeltaV to (totalisp * 9.80665 * ln(ship:mass/ship:drymass)).
    set mnvdeltaV to mnv:deltav:mag. 

    if avadeltaV < mnvdeltaV {
        print ("Not enough deltaV").
    } else {
        set mf to ship:mass / constant:e^(mnvdeltaV / (totalisp * 9.80665)).
        set burntime to ((ship:mass - mf) / totalfuelflow) * 10.
        set spark to (mnv:eta + time:seconds - burntime/2).
        set cutoff to (mnv:eta + time:seconds + 0.5*burntime).

        print burntime.
        print spark.
        print cutoff.

        wait until time:seconds > (mnv:eta - 10).
        lock steering to mnv:deltav.

        wait until time:seconds > spark.
        lock throttle to 1.
        wait until time:seconds > cutoff.
        lock throttle to 0.
        unlock steering.
        unlock throttle.
        remove mnv.
    }

    print("Manuever executed").

}

if alt:radar < 1000 {

    Launch.
    AtmoBreach.
    set throttle to 0.
    Circularise.
}