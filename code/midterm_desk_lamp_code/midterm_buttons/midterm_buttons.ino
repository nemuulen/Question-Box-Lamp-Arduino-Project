#include <DFRobot_RGBLCD1602.h>
DFRobot_RGBLCD1602 lcd(16, 2);

// pins
int pot_bright_pin = A3;
int pot_hue_pin = A1;
int light_sensor_pin = A2;
int temp_sensor_pin = A0;
int red_pin = 9;
int green_pin = 10;
int blue_pin = 11;
int buzzer_pin = 12;
int button1 = 6;
int button2 = 8;

// defat
bool system_on  = true;
bool auto_mode = true;
int  brightness = 200;
int  hue = 128;

// timer
bool timer_running = false;
long timer_end = 0;
long paused_remaining = 0;
long five_min = 5*60*1000;
long twentyfive = 25*60*1000;
long five_break = 5*60*1000;
bool pomodoro_mode = false;
bool in_work_phase = true;

// button actions
bool last_mode_state = HIGH;
bool last_timer_state = HIGH;
bool mode_press_active = false;
bool timer_press_active = false;
long mode_press_start = 0;
long timer_press_start = 0;
long last_mode_tap = 0;
long last_timer_tap = 0;
bool mode_pending_tap = false;
bool timer_pending_tap = false;

//screen 
long last_screen_switch = 0;
bool show_status_screen = true;

// beep helper functions
void beep_short() {
  tone(buzzer_pin, 2000, 70);
}
void beep_end() {
  for (int i=0; i<3; i++) {
    tone(buzzer_pin, 1500, 90);
    delay(140);
  }
}

// RGB
void set_rgb(int r, int g, int b) {
  analogWrite(red_pin,r);
  analogWrite(green_pin, g);
  analogWrite(blue_pin,  b);
}
void update_lamp() {
  if (!system_on) {
    set_rgb(0, 0, 0);
    return;
  }

  float t = hue/255.0;
  int r = (int)(255*(1.0-t));
  int g = 0;
  int b = (int)(255*t);
  r= (r*brightness)/ 255;
  b= (b*brightness)/ 255;

  set_rgb(r, g, b);
}

// sensors
void update_from_sensors() {
  int raw_temp= analogRead(temp_sensor_pin);
  float temp = (raw_temp * 5.0/1023.0) * 100.0;

  int raw_light = analogRead(light_sensor_pin);

  temp = constrain(temp, 15.0, 30.0);
  float t_norm = (temp - 15.0)/ 20.0;

  hue = (int)(t_norm * 255.0);

  int sensor_bright = map(raw_light, 0, 1023, 20, 255);
  brightness = constrain(sensor_bright, 20, 255);
}

void update_from_pots() {
  brightness = map(analogRead(pot_bright_pin), 0, 1023, 20, 255);
  hue = map(analogRead(pot_hue_pin), 0, 1023, 0, 255);
}

// timer
void start_timer(long duration) {
  if (duration == 0) return;
  timer_end = millis() + duration;
  // millis() - the number of milliseconds since the board powered on
  timer_running = true;
  paused_remaining = 0;
}

void toggle_pause_timer() {
  if (!timer_running && paused_remaining == 0) return;
  if (timer_running) {
    paused_remaining = timer_end- millis();
    timer_running = false;
  } else {
    timer_end = millis()+paused_remaining;
    timer_running = true;
  }
}

void reset_timer() {
  timer_running = false;
  paused_remaining = 0;
  pomodoro_mode = false;
}

void auto_adjust_from_timer() {
  if (!timer_running || !pomodoro_mode) return;
  long remaining = timer_end-millis();
  long phase_len = in_work_phase ? twentyfive : five_break;
  remaining = constrain(remaining, 0, phase_len);
  int extra=(int)((phase_len - remaining) * 60 / phase_len);
}

void timer_done() {
  if (timer_running && millis() >= timer_end) {
    timer_running = false;
    paused_remaining = 0;
    beep_end();
    if (pomodoro_mode) {
      in_work_phase = !in_work_phase;
      start_timer(in_work_phase ? twentyfive : five_break);
    }
  }
}

// buttons
void handle_buttons() {
  long now = millis();
  bool mode_state  = digitalRead(button1);
  bool timer_state = digitalRead(button2);

  // button1
  if (!mode_press_active && mode_state == LOW) {
    mode_press_start = now;
    mode_press_active = true;
  }
  if (mode_press_active && mode_state == HIGH) {
    long dur = now - mode_press_start;
    mode_press_active = false;
    if (dur>20) {
      if (mode_pending_tap && (now - last_mode_tap) < 500) {
        mode_pending_tap = false;
        reset_timer();
        beep_short();
      } else {
        mode_pending_tap = true;
        last_mode_tap = now;
      }
    }
  }
  if (mode_pending_tap && (now - last_mode_tap) >= 500) {
    mode_pending_tap = false;
    auto_mode = !auto_mode;
    beep_short();
  }

  // button2
  if (!timer_press_active && timer_state == LOW) {
    timer_press_start = now;
    timer_press_active = true;
  }

  if (timer_press_active && timer_state == HIGH) {
    long dur = now - timer_press_start;
    timer_press_active = false;
    if (dur>1200) {
      pomodoro_mode = true;
      in_work_phase = true;
      start_timer(twentyfive);
      timer_pending_tap = false;
      beep_short();
    } else if (dur > 20) {
      if (timer_pending_tap && (now - last_timer_tap) < 500) {
        timer_pending_tap = false;
        pomodoro_mode = false;
        long remaining = timer_running ? (timer_end - now) : paused_remaining;
        remaining += five_min;
        start_timer(remaining);
        beep_short();
      } else {
        timer_pending_tap = true;
        last_timer_tap = now;
      }
    }
  }

  if (timer_pending_tap && (now - last_timer_tap) >= 500) {
    timer_pending_tap = false;
    pomodoro_mode = false;
    if (!timer_running && paused_remaining == 0) {
      start_timer(five_min);
    } else {
      toggle_pause_timer();
    }
    beep_short();
  }
}

// LCD screen helper
void draw_status_screen() {
  lcd.setCursor(0, 0);
  lcd.print(system_on ? "on " : "off");
  lcd.print("mode:");
  lcd.print(auto_mode ? "sensor " : "manual ");

  lcd.setCursor(0, 1); //next row
  if (!timer_running && paused_remaining == 0 && !pomodoro_mode) {
    lcd.print("timer: stopped ");
    return;
  }

  long remaining = timer_running ? timer_end - millis() : paused_remaining;
  int m=remaining/ 60000;
  int s=(remaining% 60000)/ 1000;
  lcd.print(pomodoro_mode ? (in_work_phase ? "work " : "break ") : "timer ");
  lcd.print(m);
  lcd.print("m ");
  if (s<10) lcd.print("0");
  lcd.print(s);
  lcd.print("s ");
}

void draw_sensor_screen() {
  float temp = (analogRead(temp_sensor_pin) * 5.0 / 1023.0) * 100.0;
  int raw_light = analogRead(light_sensor_pin);
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C  l:");
  lcd.print(raw_light);
}

// setup
void setup() {
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(button1, INPUT_PLUP);
  pinMode(button2, INPUT_PLUP);
  lcd.init();
  lcd.setRGB(0, 120, 255);
  lcd.print("Desk lamp ready!");
  delay(1200);
  lcd.clear();
  update_from_sensors();
  update_lamp();
}

void loop() {
  handle_buttons();
  if (system_on) {
    auto_mode ? update_from_sensors() : update_from_pots();
    auto_adjust_from_timer();
    update_lamp();
  }
  timer_done();
  long now= millis();
  if (now-last_screen_switch>4000) {
    show_status_screen = !show_status_screen;
    last_screen_switch = now;
    lcd.clear();
  }
  show_status_screen ? draw_status_screen() : draw_sensor_screen();
  delay(40);
}


// Temperature & light sensor threshold:
// 1) Temperature sensor:
//    - Raw output is converted to °C.
//    - We constrain the usable range to 15°C–30°C because this is the
//      realistic indoor temperature range.
//    - The constrained temperature is normalized to 0.0–1.0 and mapped
//      directly to hue (0–255), creating a smooth warm to cool color gradient:
//          15°C -> hue = 0 (warm/red)
//          25°C -> hue ≈128 (neutral/purple)
//          30°C -> hue = 255 (cool/blue)
//
// 2) Light sensor:
//    - Raw light value (0–1023) is mapped to a brightness range of 20–255.
//    - This prevents the LED from turning completely off and can show visible results when testing.

