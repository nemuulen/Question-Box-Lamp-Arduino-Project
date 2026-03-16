# Question-Box-Lamp-Arduino-Project
This project is an Arduino‑based interactive desk lamp that combines ambient sensing, manual controls, and a Pomodoro‑style timer. It was developed iteratively, with several design changes and failures along the way to reach a stable final version.

## INFOSCI201 Physical Computing  
**Author:** Nemuulen Togtbaatar  

---

## 1. Project Overview
This project is an Arduino‑based interactive desk lamp that combines **environmental sensing**, **manual controls**, and a **Pomodoro‑style timer**. The lamp was developed through multiple iterations, testing phases, and redesigns to reach a stable final version.

The goal was to create a functional, responsive, and customizable desk lamp that adapts to the user’s environment while also supporting productivity through timed work sessions.

---

## 2. Project Features

### 🌡️ Sensor Mode (AUTO)
- Ambient light sensor controls **brightness**  
- Temperature sensor controls **color temperature**  
- Brighter room → brighter LED  
- Warmer temperature → cooler/bluer LED  
- Cooler temperature → warmer/orange LED  

This mode maintains comfortable, balanced lighting that adapts to the environment.

---

### 🎛️ Manual Mode
Two potentiometers allow full manual control:
- **Pot A0:** overall brightness  
- **Pot A1:** color temperature / hue  

---

### ⏱️ Pomodoro Timer
A built‑in timer supports:
- **25‑minute work sessions**
- **5‑minute breaks**
- **5‑minute quick timers**
- **Pause / resume**
- **Add 5 minutes**
- **Continuous Pomodoro loop**

The buzzer provides audio feedback for:
- Button presses  
- Timer completion  

---

### 🔘 Button Interactions

#### **Left Button (Mode Button)**
- **Single press:** toggle between SENSOR and MANUAL mode  
- **Double press:** reset timer  

#### **Right Button (Timer Button)**
- **Single short press:**  
  - If no timer running → start 5‑minute timer  
  - If timer running → pause  
  - If paused → resume  
- **Double short press:** add 5 minutes  
- **Long press:** start continuous Pomodoro mode  

---

### 📟 LCD Display
The DFRobot RGB LCD1602 shows:
- Current mode  
- Timer state  
- Sensor readings (temperature + light)  

---

## 3. System Design

### 3.1 Hardware Components
- Arduino Uno  
- RGB LED  
  - R → D9  
  - G → D10  
  - B → D11  
- Ambient light sensor → A2  
- Temperature sensor (LM35) → A3  
- Potentiometers:  
  - A0 → brightness  
  - A1 → color temperature  
- Push buttons:  
  - D12 → mode  
  - D13 → timer  
- Buzzer → D4  
- DFRobot RGB LCD1602 (SDA → A4, SCL → A5)  
- Breadboard + jumper wires  

---

### 3.2 Software and Tools
- Arduino IDE  

---

## 4. Development Process & Iterations

1. Tested whether a 5mm LED or RGB LED produced better brightness — RGB LED was significantly brighter.  
2. Built simple test code to verify RGB color and brightness control.  
3. Implemented IR remote control and successfully captured command codes.  
   - Used this tutorial:  
     - https://www.instructables.com/Arduino-Infrared-Remote-tutorial/  
     - https://youtu.be/q97VE3oEwIc  
4. Designed the lamp’s outer shell in TinkerCAD.  
5. Added ambient light and temperature sensors to create adaptive lighting.  
6. Displayed sensor values on the LCD for calibration.  
7. Encountered a major issue: IR controller stopped working (likely power or wiring).  
8. After multiple attempts and library tests, IR control was removed and replaced with **physical buttons + potentiometers**.  
9. Implemented a basic timer, then expanded it into a full Pomodoro cycle.  
10. Added multi‑press button logic (single, double, long press).  
11. Cleaned up code, comments, and finalized the system.  

---

## Iterations


---

## Final Look
![Final Lamp](https://github.com/nemuulen/Question-Box-Lamp-Arduino-Project/blob/main/Final%20look/IMG_6725.heic)

---

## Videos

