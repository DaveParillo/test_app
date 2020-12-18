# Tesing builds
This is a very slightly modified version of the 
Asynchronous Publisher example from
the RTI community-examples GitHub repo.

Modified to:
 - build with DDS 5.3.1
 - added a 'echo' where the publisher publishes count.
   The echo function is in a separate library.
 - moved the pubblisher and subscriber shudown functions
   to a separate 'util' library

