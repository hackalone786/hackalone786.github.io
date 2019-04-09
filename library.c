int readCount;                  // init to 0; number of readers currently accessing resource

// all semaphores initialised to 1
Semaphore resourceAccess;       // controls access (read/write) to the resource
Semaphore readCountAccess;      // for syncing changes to shared variable readCount
Semaphore serviceQueue;         // FAIRNESS: preserves ordering of requests (signaling must be FIFO)

void writer()
{
    serviceQueue.P();           // wait in line to be serviced
    // <ENTER>
    resourceAccess.P();         // request exclusive access to resource
    // </ENTER>
    serviceQueue.V();           // let next in line be serviced
    
    // <WRITE>
    writeResource();            // writing is performed
    // </WRITE>
    
    // <EXIT>
    resourceAccess.V();         // release resource access for next reader/writer
    // </EXIT>
}

void reader()
{
    serviceQueue.P();           // wait in line to be serviced
    readCountAccess.P();        // request exclusive access to readCount
    // <ENTER>
    if (readCount == 0)         // if there are no readers already reading:
        resourceAccess.P();     // request resource access for readers (writers blocked)
    readCount++;                // update count of active readers
    // </ENTER>
    serviceQueue.V();           // let next in line be serviced
    readCountAccess.V();        // release access to readCount
    
    // <READ>
    readResource();             // reading is performed
    // </READ>
    
    readCountAccess.P();        // request exclusive access to readCount
    // <EXIT>
    readCount--;                // update count of active readers
    if (readCount == 0)         // if there are no readers left:
        resourceAccess.V();     // release resource access for all
    // </EXIT>
    readCountAccess.V();        // release access to readCount
}
