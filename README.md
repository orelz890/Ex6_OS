# The assignment:


## Write:
+ Thread safe queue, must implement:
    * createQ
    * destroyQ
    * enQ
    * deQ
+ Active_object, must implement:
    * newAO(queue*,func1*,func2*)
    * destroyAO(AO*)
+ Pipeline of active_object, must implement:
    * Server main thread encharge of reading the client input
    * First AO will operate kisar cifer(+1 shift) on the clients input
    * Second AO will turn all letters to upercase 
    * Third AO will send the output to the proper client
+ Guard:
    * Class/struct making a thread safe environment(mutex)
+ Singleton, must implement:
    * Instance - returns a pointer to a singleton
    * Destroy
+ Reactor, must implement:
    * void* newReactor()
    * InstallHandler(Reactor*,func*,fd) - func will handle the file descriptor input
    * RemoveHandler(Reactor*) - will free the fd handler
    * Creat client & server for testing


# How to run:


## part 1,2,3:

### Run client & server:
+ Step 1: Open terminal & enter - make server
+ Step 2: Open another terminal & enter - make client

### Run test:
+ Step 1: Open terminal & enter - make server
+ Step 2: Open another terminal & enter - make test1


## part 4(Guard):
+ Step 1: Open terminal & enter - make guard


## part 5(Singleton):
+ Step 1: Open terminal & enter - make singleton


## part 6(Reactor):
+ Step 1: Open terminal & enter - make pollserver
+ Step 2: Open another terminal & enter - make pollclient



All done with the preperation, enjoy :)
