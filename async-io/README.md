# Async IO

Simplified producer consumer

Create 2 applications:

### Serial Requirements:
* Run loop executing task N times

### Async Requirements:
* Main thread creates tasks
* Consumer thread, accepts tasks without blocking
* Run loop creating and feeding tasks N times
* First in First out

### Shared:
* N is a command-line specified number
* Build and run on Linux


## Testing
* Program will be timed externally
* Test will be done with varying values for N

---

## Task Details
* Print the numbers 1 to N to stdout
