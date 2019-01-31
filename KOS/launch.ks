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
    parameter H.
    lock TargetPitch to 88.963 - 1.03287 * alt:radar^0.409511.
    lock steering to heading(90,TargetPitch).
    local OldThrust is ship:availablethrust.
    until alt:apoapsis > H {
        if OldThrust > ship:availablethrust + 10 {
            SafeStage.
            wait until stage:ready.
            set OldThrust to ship:availablethrust.
        }
    }

    print("Atmospheric escape achieved").

    until alt:radar > ship:orbit:body:atm:height {
        if alt:apoapsis > H {
            set throttle to 0.
        } else if alt:apoapsis < H {
            set throttle to 1.
        }
    }

    print("Atmospheric escape maintained").
}

function Circularise {
    wait until alt:radar > ship:orbit:body:atm:height.
    local i is 0.
    local Besti is 0.
    set Circle to node(time:seconds + eta:apoapsis, 0, 0, i).
    add Circle.
    set BestE to Circle:orbit:eccentricity.
    remove Circle.
    set StepSize to 2560.

    until StepSize < 5 {
        from {set i to Besti - StepSize.} until i > Besti + StepSize step {set i to i + StepSize.} do {
            set Circle to node(time:seconds + eta:apoapsis, 0, 0, i). 
            add Circle.
            set NewE to Circle:orbit:eccentricity.
            remove Circle.
            if NewE < BestE {
                set BestE to NewE.
                set Besti to i.
            }
        }
        set StepSize to StepSize * 0.5.
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

        wait until time:seconds > spark - 0.5.
        lock throttle to 1.
        wait until time:seconds > cutoff - 0.5.
        lock throttle to 0.
        unlock steering.
        unlock throttle.
        remove mnv.
    }

    print("Manuever executed").

}

function GroundToOrbit {
    parameter H is 0.
    if H = 0 {
        set H to ship:orbit:body:atm:height + 10000.
    }
    print("Entering circular orbit of " + H + " meters").
    Launch.
    AtmoBreach(H).
    set throttle to 0.
    Circularise.
}

function OrbitToOrbit {
    parameter Target.

    Intercept(Target).
    Circularise.
}

function Intercept {
    parameter Target.

    set i to 0.
    set Besti to 0.
    set t to 0.
    set Bestt to 0.
    set Besth to 100000000.
    set StepSize to 256.
    set InterceptMNV to node(TIME:SECONDS + t, 0, 0, i).
    add InterceptMNV.

    local mini to 0.
    local midt to 0.
    local decreasing to true.

    set InterceptMNV to node(TIME:SECONDS + eta:apoapsis, 0, 0, i).
    add InterceptMNV.

    until interceptmnv:orbit:apoapsis > target:orbit:periapsis - Target:soiradius {
        remove interceptmnv.
        set i to i + 10.
        set InterceptMNV to node(TIME:SECONDS + t, 0, 0, i).
        add interceptmnv.
    }

    set Mini to i.

    until InterceptMNV:orbit:transition = "ENCOUNTER" and interceptmnv:orbit:nextpatch:body = target {
        remove interceptmnv.
        set t to t + 10.
        set InterceptMNV to node(TIME:SECONDS + t, 0, 0, mini).
        add interceptmnv.
    }
    set midt to t.
    remove interceptmnv.

    from {set i to mini.} until decreasing = false step {set i to i + 5.} do {
        set decreasing to false.
        from {set t to midt - 0.125 * ship:orbit:period.} until t > midt + 0.125 * ship:orbit:period step {set t to t + 1.} do {
            set InterceptMNV to node(TIME:SECONDS + t, 0, 0, i).
            add InterceptMNV.
            if InterceptMNV:orbit:transition = "ENCOUNTER" and interceptmnv:orbit:nextpatch:body = target {
                if InterceptMNV:orbit:nextpatch:periapsis < Besth and InterceptMNV:orbit:nextpatch:periapsis > Target:atm:height + 10000 {
                    set decreasing to true.
                    set Besth to InterceptMNV:orbit:nextpatch:periapsis.
                    set Besti to i.
                    set Bestt to t.
                }
            } 
            remove InterceptMNV.
        } 
    }

    if Besti = 0 and Bestt = 0 {
        print("No encounter possible").
    } else {
        set InterceptMNV to node(TIME:SECONDS + Bestt, 0, 0, Besti).
        add InterceptMNV.
    }

}


//GroundToOrbit().
Intercept(Mun).
//OrbitToOrbit(Mun).