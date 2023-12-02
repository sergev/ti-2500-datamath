A goal of this project is to simulate and study the firmware
of calculators TI-2500 Datamath and Sinclair Scientific.

Based on sources from projects:

 * [TICalculatorJSSimulator](https://github.com/shirriff/TICalculatorJSSimulator)
 * [ArduinoNanoSinclairScientificCalculator](https://github.com/arduinoenigma/ArduinoNanoSinclairScientificCalculator)

Build:

    make

Run tests:

    make test

Invoke calculator in REPL mode:

    $ build/src/ti2500
    Try calculations on TI-2500.
    Don't forget '=' at the end!
    Use q to quit.

            0.
    > 3+5=
            8.
    > c
            0.
    > -7/2=
            -3.5
    > q
