# Concurrency problems
In this repo I tried to solve 4 classical concurrency programming problems using Linux system calls and basic synchronization structures like mutexes and semaphores in C.
All programs have adjustable parameters like number of producers or producers in the Producer-consumer problem, or maximum time of philosopher eating in Dining philosophers problem.

Below are formulations of each problems:

## Producer-consumer problem
- The producer generates data items and puts them into
the buffer with a limited capacity.
- The consumer takes the items, thereby removes them
from the buffer, leaving free space.
- At the producer side, the synchronisation problem
appears when the buffer is full, so there is no free space
for a new (already produced) item.
- At the consumer side, the synchronisation problem
appears when the buffer is empty, so there is no item
to be taken

## Reader-writer problem
- Two kinds of users (readers and writers) use a
shared resource (reading room or library).
- Readers can use the resource in the shared mode, i.e.
multiple readers can occupy the reading room at the
same time.
- Writers can use the resource in the exclusive mode, i.e.
a writer in the reading room excludes other users
(both readers and other writers).
- The problem of synchronisation is to prevent the user
from entering the reading room in an incompatible
mode.

## Dining philosophers problem
Five philosophers sit at a round table with a bowl of
spaghetti.
- Each philosopher alternately thinks and eats.
- To eat spaghetti the philosopher needs two forks.
- Forks are placed between each pair of adjacent
philosophers, so each of the five forks is shared by two
philosophers.
- Each fork is used by at most one philosopher at a time,
therefore at most one of two neighbours can eat.
- The problem is to synchronise the philosopher so that each
of them can eventually eat something, avoiding
unnecessary waiting and following the rules of access to
forks.

## Sleeing barbers problem - TODO
There is a barbershop with p chairs in the waiting room
and n barber chairs used by barbers to serve their
customers.
- When there is a customer in the shop, they wake up a
barber, then the barber finds a free chair to serve the
customer.
- If no free barber chair is available, the customer takes a seat
in the waiting room.
- If there is no free chair in the waiting room, the customer
leaves the barbershop.
- The issue here is to synchronise the barbers and the
customers, i.e. to lead to the simultaneous state of a barber
serving a client and the client being served by the barber.