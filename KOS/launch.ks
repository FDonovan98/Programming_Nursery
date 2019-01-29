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
}

Launch.
AtmoBreach.
wait until false.