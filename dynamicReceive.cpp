#include <cstdlib> //malloc() free() realloc()
#include <cstddef> //NULL
#include <sys/socket.h> //recv()

int dynamicReceive(int sockfd, int maxBytes, char *destinationBuffer)
{
    const int
        PAGE_SIZE = 4096; //this can be changed based on implementation
    char
        *bufferStart, *temporaryPointer;
    int
        memoryAllocationCount = 0, totalCharsReceived = 0, temporaryCharsReceivedCount;

    //allocate an initial buffer
    bufferStart = (char*)malloc(PAGE_SIZE);
    if(bufferStart == NULL)
    {
        return -1;
    }

    memoryAllocationCount++;

    while(totalCharsReceived < maxBytes)
    {
        //recv into buffer
        temporaryCharsReceivedCount = recv(sockfd, bufferStart + totalCharsReceived, PAGE_SIZE, 0);
        
        //check for recv() errors
        if(temporaryCharsReceivedCount < 1)
        {
            free(bufferStart);
            
            return -1;
        }
        
        totalCharsReceived += temporaryCharsReceivedCount;
        temporaryCharsReceivedCount = 0;
        
        //check if buffer needs to be resized
        if(totalCharsReceived == PAGE_SIZE * memoryAllocationCount)
        {
            memoryAllocationCount++;
            
            //resize buffer to a larger size to read more chars
            temporaryPointer = (char*)realloc(bufferStart, PAGE_SIZE * memoryAllocationCount);
            if(temporaryPointer == NULL)
            {
                free(bufferStart);
                
                return -1;
            }
            
            bufferStart = temporaryPointer;
            temporaryPointer = NULL;
        }
        else
        {   //resize buffer to a smaller size
            temporaryPointer = (char*)realloc(bufferStart, totalCharsReceived);
            if(temporaryPointer == NULL)
            {
                free(bufferStart);
                
                return -1;
            }
            
            //successfully exit
            destinationBuffer = temporaryPointer;
            //bufferStart = temporaryPointer;
            //temporaryPointer = NULL;
            break;
        }
    }

    return totalCharsReceived;
}
