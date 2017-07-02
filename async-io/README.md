# Async IO

Simplified producer consumer

Create 2 applications:

### Serial Requirements:
* Run loop executing task N times

### Async Requirements:
* Main thread creates tasks
* Consumer thread, accepts tasks without blocking
* Run loop creating and feeding tasks N times

### Shared:
* N is commandline specified number
* Task is unknown closure
* Build and run on Linux


## Testing
* Program will be timed externally
* Closure details will be revealed 5 minutes  
before the coding time ends
* Test will be done with different values for N
* Each task has to be executed

---

## Closure Details
* Print `Hello World` to stdout
