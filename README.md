# ESP32-Line-Tracking-Robot

My second robotics projects, where I built a Line Tracking Robot controlled by an ESP32. 
The main challenge: designing and assembling the chassis using only reused or improvised materials, instead of relying on a 3D-printed model or a pre-designed kit.

This project is still under development. The robot can already move forward and detect lines with the IR sensors, but I am working on improving the logic for smoother turns.

---

## Components 
 
- ESP32-WROOM-32E
- L293D
- IR Obstacle Avoidance Module (x2)
- Line Tracking Module
- DC Motor with Reducer (x2)
- ESP32 Camera Extension Board
- 4xAA Battery Holder
- Jumper Wires
- Breadboard

---

## How It Works

The robot follows a black line using its IR sensors. Depending on which sensor detecs the line, the ESP32 sends PWM signals through the L293D to control the motors. 

A Finiste State Machine (FSM) defines the possible states (Forward, Left Turn, Right Turn, Search) and transitions based on sensor input. 



#### States
- Forward → when only the center sensor detects the line (010), the robot moves straight ahead.
- Slight Left → when the left sensor is active (100) or both left and center are active (110).
- Slight Right → when the right sensor is active (001) or both right and center are active (011).
- Search (Lost Line) → when no sensor detects the line (000). The robot turns in the last known direction (lastTurn) until it finds the line again.

#### Transitions
- Input: binary combination of sensors (L, C, R).
- Transition: FSM changes state according to the detected pattern.

*This FSM design makes the robot responsive and robust, since it reacts directly to sensor input and can recover when it loses track of the line.*
---

# Wiring

[Wiring Diagram](LTM_WD.png)

---

# Challenges and Learnings

- DC Motors: 
The first attempts used regular DC motors without reducers. No matter what, the robot lacked the torque to move properly. I then learned about *gear-reduced motors*, switched to them, and the improvement was immediate. 

- Robot Chassis: 
Iterated with Legos, plastic toys, food containers, cardboard, and even went back to Legos before finally finding a workable solution. In the end, I repurposed parts from a metal toy car, which gave me enough structural pieces to create a stable chassis that could hold everything together securely. 

- Problem Solving: 
Learned to adapt, improvise and iterate when components did not work as expected. To seek solutions and to try and see everything through a different point of view when things did not work out as expected.

---

# Why I Built This

As my second robotics project, I wanted to see how far I could go compared to my first one. 

This Time, I aimed to: 

- Push myself beyond my first robotics build [Smart Traffic Light](https://github.com/pinedaangel20/smart-traffic-light-esp32-lcd-button)
- Explore new components and real-world constraints.
- Improve problem-solving skills by improvising mechanical solutions. 

It was very rewarding to see all the pieces come together after multiple iterations. 

---



