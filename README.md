*This project has been created as part of the 42 curriculum by asmounci.*

# Codexion 🧵

## Description

Codexion is a concurrency simulation inspired by the Dining Philosophers problem.
Coders sit in a circular co-working hub and must acquire two USB dongles to compile
their quantum code. The simulation models real-world resource contention, requiring
careful synchronization to avoid deadlocks, starvation, and burnout.

Each coder is represented by a POSIX thread. Dongles are shared resources protected
by mutexes and condition variables. A monitor thread watches for burnout and stops
the simulation when needed.

## Instructions

**Compilation:**
```bash
make
```

**Execution:**
```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

**Arguments:**
- `number_of_coders` — Number of coders in the simulation (also equals the number of dongles on the table)

- `time_to_burnout` — Maximum time (ms) a coder can go without starting a compile before burning out. The timer starts from the beginning of their last compile or from the start of the simulation.

- `time_to_compile` — Time (ms) spent compiling. During this phase, the coder must hold two dongles simultaneously.

- `time_to_debug` — Time (ms) spent debugging after each compile session.

- `time_to_refactor` — Time (ms) spent refactoring. After this phase, the coder immediately attempts to compile again.

- `number_of_compiles_required` — Simulation stops when every coder has compiled at least this many times. If a coder burns out, the simulation stops immediately.

- `dongle_cooldown` — Time (ms) a dongle remains unavailable after being released.

- `scheduler` — Arbitration policy for dongle access: `fifo` (first come first served) or `edf` (earliest deadline first with `deadline = last_compile_start + time_to_burnout`)

**Example:**
```bash
./codexion 3 300 100 100 100 1 0 fifo 
./codexion 3 300 100 100 100 1 0 edf
```

**Output format:**

Each line in the output follows this format:
```bash
timestamp_in_ms  X  state_message
```
Where timestamp_in_ms is the time since the simulation started (in milliseconds) and X is the coder's number.

Possible state messages:
```bash
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

**Example output:**
```bash
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
100 1 is debugging
100 3 has taken a dongle
100 3 has taken a dongle
100 3 is compiling
100 2 has taken a dongle
200 2 has taken a dongle
200 2 is compiling
200 1 is refactoring
200 3 is debugging
```

**Cleanup:**
```bash
make clean   # remove object files
make fclean  # remove object files and binary
make re      # full rebuild
```

## Blocking Cases Handled

**Deadlock prevention:**
Coders always acquire their left dongle before their right dongle. The last coder
(number N) acquires in reverse order (right before left), breaking the circular
wait — one of Coffman's four necessary conditions for deadlock. This ensures the
cycle is never closed.

**Starvation prevention:**
The scheduler enforces fair access to each dongle via a priority queue (min-heap).
- `fifo`: requests are served in arrival order — no coder waits indefinitely.
- `edf`: requests are served by earliest burnout deadline, prioritizing the coder
  most at risk — prevents starvation under time pressure.

**Cooldown handling:**
After a dongle is released, it is unavailable for `dongle_cooldown` milliseconds.
The condition uses `pthread_cond_timedwait` so threads wake automatically when the
cooldown expires, even if no broadcast occurs.

**Precise burnout detection:**
A dedicated monitor thread polls every 1 ms. It compares the current time against
each coder's `last_compile_time + time_to_burnout`. When burnout is detected, the
log is printed and the simulation stops within 10 ms as required.

**Log serialization:**
All output goes through `log_state`, which holds `log_mutex` for the entire
duration of the print. This guarantees no two messages interleave on stdout.

## Thread Synchronization Mechanisms

**`pthread_mutex_t dongle.mutex`**
Protects all fields of a dongle (in_use, heap, last_release_time). Any read or
write on these fields is done under this lock.

**`pthread_cond_t dongle.cond`**
Used with `pthread_cond_timedwait` to make coders wait when a dongle is busy,
in cooldown, or not their turn in the priority queue. `pthread_cond_broadcast`
is called on release so all waiting coders re-evaluate the condition.

**`pthread_mutex_t coder.mutex`**
Protects `nb_compiles` and `last_compile_time` on each coder, which are written
by the coder thread and read by the monitor thread.

**`pthread_mutex_t sim.log_mutex`**
Serializes all output. Also used to protect `sim.stop` reads and writes to prevent
race conditions between the monitor setting stop and coders reading it.

**`pthread_mutex_t sim.stop_mutex`**
Dedicated lock for `sim.stop`, preventing data races between the monitor writing
stop=1 and coder threads reading stop in their main loop and inside dongle_acquire.

**Race condition prevention example:**
Without `coder.mutex`, the monitor could read `last_compile_time` mid-write by
the coder thread, producing a corrupt timestamp and a false burnout detection.
The mutex ensures the monitor always reads a consistent value.

## Resources

- **Codexion Visualizer** — Real-time simulation replayer: https://codexionvisualizer.dev/
  *(Interactive tool to visualize and replay simulation scenarios)*

- **Dining Philosophers** — Visual explanation of the problem (YouTube): https://youtu.be/R7tPuETDC5c?si=xgk0kWUL3g5X0sAf
  *(Used to understand the core problem before coding)*

- **Multithreading in C** — GeeksforGeeks tutorial: https://www.geeksforgeeks.org/c/multithreading-in-c/
  *(Covers pthread creation, joining, mutexes, condition variables, and common concurrency issues)*

- **Min-Heap Data Structure** — GeeksforGeeks: https://www.geeksforgeeks.org/dsa/introduction-to-min-heap-data-structure/
  *(Implementation reference for priority queue used in fifo and edf schedulers)*

**AI usage:**
- NOT used for: Core algorithm implementation, main synchronization logic, or critical sections
