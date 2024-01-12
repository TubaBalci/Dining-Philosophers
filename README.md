# Dining-Philosophers
Turkish version of dining philosophers by sharing resources tea, teacup and sugar.

This repository contains a modified version of the classic Dining Philosophers problem, introducing a twist by simulating a group of individuals drinking tea concurrently. The program is implemented in C, utilizing Pthreads for multithreading and Semaphores for synchronization. The scenario revolves around philosophers (participants) drinking tea with or without sugar, while ensuring proper resource management.

# Requirements
Pthreads Library
POSIX-compliant system (Linux/Unix)

The simulation adapts the Dining Philosophers problem to a tea-drinking context. Participants (philosophers) alternate between thinking, feeling thirsty, pouring tea, and drinking tea based on their preferences (with or without sugar). The implementation employs Pthreads for thread creation and Semaphores for synchronization, ensuring that the shared resources, such as cups, plates, tea pots, and sugar, are properly managed. Based on the implementation odd numbered participants use suger others don't. 
