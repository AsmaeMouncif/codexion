*This project has been created as part of the 42 curriculum by <asmounci>.*

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
- `number_of_coders` — number of coders sitting in the circular hub (also equals the number of dongles on the table)
- `time_to_burnout` — max time (ms) a coder can go without starting a compile before burning out
- `time_to_compile` — time (ms) spent compiling; coder must hold 2 dongles during this phase
- `time_to_debug` — time (ms) spent debugging after each compile
- `time_to_refactor` — time (ms) spent refactoring; coder immediately attempts to compile again after this phase
- `number_of_compiles_required` — simulation stops when every coder has compiled at least this many times
- `dongle_cooldown` — time (ms) a dongle remains unavailable after being released
- `scheduler` — arbitration policy for dongle access: `fifo` (first come first served) or `edf` (earliest deadline first)

**Example:**
```bash
./codexion 5 4000 200 200 200 3 100 fifo
./codexion 3 2500 200 200 200 5 50 edf
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

- POSIX Threads documentation: https://man7.org/linux/man-pages/man7/pthreads.7.html
- The Dining Philosophers problem: Dijkstra, E.W. (1971)
- `pthread_cond_timedwait` man page: https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html
- Earliest Deadline First scheduling: Liu & Layland (1973), "Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment"
- Helgrind thread error detector: https://valgrind.org/docs/manual/hg-manual.html

**AI usage:**
