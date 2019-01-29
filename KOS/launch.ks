core:part:getmodule("kOSProcessor"):doevent("Open Terminal").

function SafeStage {
    wait until stage:ready.
    stage.
}

function Launch {
    set throttle to 1.
    until vessel:verticalspeed > 0 {
        safestage.
        wait 1.
    }
}

function AtmoBreach {
    lock TargetPitch to 88.963 - 1.03287 * alt:radar^0.409511.
    lock steering to heading(90,TargetPitch).
    global OldThrust is vessel:availablethrust.
    until alt:apoapsis > 80000 {
        AutoStage(OldThrust).
        wait 1.
    }
}

function AutoStage {
    parameter OldThrust.
    if OldThrust > vessel:availablethrust + 10 {
        SafeStage.
        wait 1.
        set OldThrust to vessel:availablethrust.
    }

}

Launch.
AtmoBreach.
print("Breach").
wait until false.