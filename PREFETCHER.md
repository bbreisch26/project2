1. Describe how your prefetcher works.

The custom prefetcher implemented in the code is a strider prefetcher. When no known pattern has been seen, the prefetcher will be conservative and fetch the next cache line. If the last two prefetches had the same jump between memory addresses, the prefetcher will fetch the next line with the same jump from the current address. This strategy is more flexible than an adjacent prefetcher in that it can adapt to a variety of memory access patterns.

2. Explain how you chose that prefetch strategy.

I chose this prefetch strategy after reviewing the suggested papers and realizing that an efficient but convoluted prefetching strategy is not realistic to be implemented in hardware and has little value. I wanted to implement a simplistic prefetcher that provided a comparable hit rate to the other two prefetching strategies for sequential memory accesses but fares far better with non-sequential memory access patterns. This strategy achieves that goal with low overhead. This prefetcher design that applies well to objects like linked lists that do not necessarily benefit from direct spatial locality. For example, on trace #3 the sequential prefetcher has a 0% hit rate because none of the memory accesses are directly adjacent, but my prefetcher design achieves a 31.67% hit rate. 

3. Discuss the pros and cons of your prefetch strategy.
This prefetch strategy has relatively low overhead compared to the sequential prefetcher and a comparable overhead to the adjacent prefetcher. it will only prefetch one line at most, reducing the amount of memory bandwidth used up for prefetching.  It does require more hardware to store the last two memory jumps when compared to the adjacent prefetcher, two registers, a computational unit to calculate the memory jump, and some logic gates to implement the decision logic. 

4. Demonstrate that the prefetcher could be implemented in hardware (this can be
   as simple as pointing to an existing hardware prefetcher using the strategy
   or a paper describing a hypothetical hardware prefetcher which implements
   your strategy).
   This prefetcher strategy has been proposed by many academic papers and used in industry. Intel uses a strided prefetcher for the Instruction Pointer (basically the PC). [2] cited below details the hardware design for a strided prefetcher. 


5. Cite any additional sources that you used to develop your prefetcher.
[1] Assignment 2: Understanding Data Cache Prefetching
https://www.inf.ed.ac.uk/teaching/courses/car/Pracs/2017-18/Assignment2.pdf
[2] Analysis of Hardware Prefetching Across Virtual Page Boundaries
http://tnm.engin.umich.edu/wp-content/uploads/sites/353/2017/12/2007.07.Analysis_of_hardware_prefetching.pdf