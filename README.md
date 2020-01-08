# Queueing-Theory-Simulator
A simulator of a queue with a poisson arrival rate of lambda, a poisson service rate of mu. As the size of the queue grows, the probability of another element that arrives to enter the queue lowers.\
Simulator input:
* T - Maximum arrival time
* lambda - poisson rate for arrival
* mu - poisson rate for service
* P0, P1, ..., Pn - Where Pi is the probability that an element that arrived will enter the queue, given the are i elements in the queue
