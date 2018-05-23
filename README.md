# Stepper-Motor-Prototype-Tester

This controller is made as a prototype tool to test mechanics from a stepper motor. On the controller, there is a button which is used to change the state of the motor. The controller can have 2 states set, one while the button is pressed ad one while the button is not pressed. The controller can be set using a serial user interface. There are 4 different states that can be set; stop, move to a position, move amount and continuously moving. Stop is stop to stop the motor. Move to a position is to move to a set position. Move amount is to move a set amount no matter what the other set state is. Continuously moving make the motor continuously move at a set speed in a direction.
2 different or the same state can be set, for example; state 1 (button = 0) is continuously moving and state 2 (button = 1) is Move to a position. This will make the motor continuously run but when the button is pressed, the motor will go to the set position. This means that the motor will return to the same spot even tho it has moved away at a random distance.
Another example would be setting the same state but with different speed setting or position settings.
To access the menu, plug the controller into the USB and the menu will appear in the serial console. This menu allows you to set how the controller reacts to the button press.
Welcome v1.0 Stepper motor controller
Commads:
    P – Program (ON Type, ON Distance, ON Speed, OFF Type, OFF Distance, OFF Speed)
    A – Move amount (Distance)
    T – Move To positon (Distance)
    S – Set setting (m1, m2, m3)
    D – Display details
    H – Help
Serial.println();
Type:
    0 – Stop
    1 – Move to position
    2 – Move amount
    3 – Continuously moving

The Arduino program and PCB is available from https://github.com/tinkersprojects/Stepper-Motor-Prototype-Tester
The PCB is available from https://tinkersprojects.com/product/stepper-motor-prototype-tester/
