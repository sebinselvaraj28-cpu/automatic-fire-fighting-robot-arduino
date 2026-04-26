# automatic-fire-fighting-robot-arduino

An autonomous robot that detects fire, moves toward the flame source, and extinguishes it automatically using a water pump and rotating servo nozzle.

## 📌 Project Overview

Fire accidents can cause serious damage to life and property. This project is a low-cost autonomous fire fighting robot designed using Arduino Uno, flame sensors, DC motors, motor drivers, water pump, and servo motor.

The robot continuously scans the surroundings. When a flame is detected, it moves toward the source and activates the water pump to extinguish it.

---

## 🚀 Features

- Detects fire using 3 flame sensors
- Left / Center / Right directional tracking
- Autonomous movement toward fire
- Water pump activation near fire source
- Servo motor sweeps nozzle left-right
- Low-cost and compact design
- Suitable for academic mini project

---

## 🛠 Components Used

- Arduino Uno
- 3x Flame Sensors
- 2x L298N Motor Driver Modules
- 2x N20 DC Geared Motors
- Mini Water Pump
- SG90 Servo Motor
- 11V / 12V Battery
- 7805 Regulated 5V Supply
- Chassis + Wheels
- Jumper Wires

---

## ⚙ Working Principle

1. Robot powers ON.
2. Flame sensors scan surroundings.
3. If no fire found → robot rotates in place.
4. If left flame sensor detects fire → turn left.
5. If right flame sensor detects fire → turn right.
6. If center detects fire → move forward.
7. On reaching near flame:
   - stop movement
   - start pump
   - servo sweeps water nozzle

---

## 🔌 Pin Connections

### Flame Sensors
| Sensor | Arduino |
|--------|---------|
| Left   | A0 |
| Center | A1 |
| Right  | A2 |

### Drive Motors (L298N #1)
| Pin | Arduino |
|----|---------|
| ENA | D5 |
| ENB | D6 |
| IN1 | D7 |
| IN2 | D8 |
| IN3 | D9 |
| IN4 | D10 |

### Pump (L298N #2)
| Pin | Arduino |
|----|---------|
| ENA | D3 |
| IN1 | D12 |
| IN2 | D13 |

### Servo
| Servo Signal | Arduino |
|-------------|---------|
| PWM | D11 |

---

## 🎯 Applications

- Home fire safety
- Labs
- Warehouses
- Small industries
- Educational robotics

---

## 🔮 Future Scope

- IoT alerts
- Camera monitoring
- Obstacle avoidance
- Smoke sensor integration
- Bigger water tank
