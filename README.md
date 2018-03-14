# candyKids
Project 3 for CMPT-300, a producer-consumer multithredded application.

Main
Your main() function will start and control the application. Its steps are as follows:


###### Extract Arguments
 Process the arguments passed on the command line. All arguments must be greater than 0. If any argument is 0 or less, display an error and exit the program.

######Initialize Modules
 Do any module initialization. You will have at least two modules: bounded buffer, and statistics. If no initialization is required by your implementation, you may skip this step.

######Launch factory threads
 Spawn the requested number of candy-factory threads. To each thread, pass it its factory number: 0 to (number of factories - 1).


######Launch kid threads
 Spawn the requested number of kid threads.

######Wait for requested time
 In a loop, call sleep(1). Loop as many times as the "# Seconds" command line argument. Print the number of seconds running each time, such as "Time 3s" after the 3rd sleep. This shows time ticking away as your program executes.

######Stop factory threads
 Indicate to the factory threads that they are to finish, and then call join for each factory thread. See section on candy-factory threads (below) for more.

######Wait until no more candy
 While there is still candy in the bounded buffer (check by calling a method in your bounded buffer module), print "Waiting for all candy to be consumed" and sleep for 1 second.

######Stop kid threads
For each kid thread, cancel the thread and then join the thread. For example, if a thread ID is stored in daThreadID, you would run:

 pthread_cancel(daThreadId);
 pthread_join(daThreadId, NULL);

######Print statistics
 Call the statistics module to display the statistics. See statistics section below.

######Cleanup any allocated memory
 Free any dynamically allocated memory. You may need to call cleanup functions in your statistics and bounded buffer modules if they need to free any memory.
