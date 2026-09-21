# FreeRTOS ATmega328P - Deferred Interrupt Processing

This project illustrates the implementation of the **Deferred Interrupt Processing** pattern under **FreeRTOS** on an **ATmega328P** microcontroller (Arduino Uno), simulated in **Proteus VSM**.

---

## 🎯 Why This Architecture in the Real World? (Industrial Use Case)

In critical embedded systems (Aerospace, Defense, Automotive):
- **Absolute rule:** An interrupt service routine (**ISR**) must be as short as possible (a few microseconds) so it doesn't block the system or miss other higher-priority hardware interrupts.
- **Problem:** Processing an event (e.g., reading a sensor via I2C/SPI, formatting a network packet, writing to UART) requires too many CPU cycles to be done directly inside the ISR.

### 🚁 Concrete Industrial Applications:
1. **Flight Control Systems (Aerospace):** An ISR detects a pulse from a position/acceleration sensor, gives a binary semaphore, then immediately returns control. The computation task adjusts control surfaces in the background.
2. **Industrial Sensors / Radar:** Reception of a hardware alert signal. The ISR validates the event, and a dedicated task runs the heavy filtering algorithm.
3. **Buttons & Human-Machine Interfaces (HMI):** Instant detection of a button press in the ISR without blocking the rest of the real-time tasks.

---

## ⚙️ Architecture & Synchronization Mechanism

```
[Hardware Button (PD2/INT0)]
             │
             ▼ (Falling Edge)
+----------------------------+
|      ISR (INT0_vect)       |  <-- Executes in a few µs
| xSemaphoreGiveFromISR()    |
| taskYIELD()                |
+----------------------------+
             │ (Releases xButtonSemaphore)
             ▼
+----------------------------+
|  vButtonHandlerTask (RTOS) |  <-- Immediately unblocked (High priority)
|  xSemaphoreTake() == TRUE  |
|  Processes the event (UART)|
+----------------------------+
```

## 🛠️ Technical Stack

- **Microcontroller:** ATmega328P @ 16 MHz
- **RTOS:** FreeRTOS v9.0.0 (AVR/GCC port)
- **Toolchain:** avr-gcc, avr-libc, Make
- **Simulation:** Proteus VSM (Virtual Terminal)

---

## 📸 Simulation Results

![Terminal simulation result](docs/terminal_result.png)

The Virtual Terminal confirms the hardware interrupt trigger and its immediate handling within the FreeRTOS task context.

---

## 🚀 Build & Run

### 1. Build via Makefile
```bash
make clean
make
```

### 2. Simulation in Proteus
1. Load the generated `.elf` or `.hex` file onto the ATmega328P.
2. Check the wiring of the virtual terminal (crossed TXD/RXD) and the button on PD2/INT0.
3. Run the simulation and press the button to observe real-time processing.
