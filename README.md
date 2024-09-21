# WES237b Final Project Connors Jackson and Ricardo Lizarraga
This is the first part of the FFT Radix-2 project.
The second git repo for our project can be found here: https://github.com/RicardoUCSD/final/tree/master (Make sure you select the correct branch.)

# How to Use:
Download the repo. Then rename the fft file you want to use to "fft.c". For example, if you want to run using the naive fft, rename the file "fft_naive.c" to "fft.c". Then type $make in terminal to run the fft.

We originally ran these on a Google Fold phone.

# Description of Repo
This is the original code for the fft algorithm. We started by modifying the fft code here in this repo before merging with the other repo that has more and larger datasets created using Python. We used this repo for initial benchmarking and to verify that the fft would work with a single dataset of length N=4,096. We ran the fft one time to verify the results were correct, then another 20,000 times to be able to compare the execution time of each fft.

We began by using the naive fft and adjusting the source code to use a single case for N=4096. We then created a new fft for loop unrolling. We then added Neon functions to the inner-most for loop of the fft compute. Note that there are 2 cases that we tested to see which would perform better. We then added loop unrolling and the Neon intrinsic functions together (again with 2 cases).

# Description of fft files
1. fft_naive - this is the naive fft we found online.
2. fft_unroll - this is an fft after adding loop unrolling factor of 2, though larger (even) numbers could be used.
3. fft_Neon (Neon Add) - uses a naive fft and Neon intrinsic functions. Uses the addional add (vadd_f32) to calculate the real and imaginary products rather than doing the add on the CPU.
4. fft_Neon (No Neon Add) - the same as the above, but does the extra add for real/imaginary product on the CPU.
5. fft_Both (Neon Add) - Uses both Neon Intrinsic Functions and Loop Unrolling. Uses the neon intrinsic add.
6. fft_Both (No Neon Add) - Uses both Neon Intrinsic Functions and Loop Unrolling. Does not Use the neon intrinsic add.

# Initial Performance Benchmarking Results
We ran a $time make seven times on each of the above fft variances and yielded the following results:
1. Naive                =   7.253   seconds
2. Loop Unrolled        =   6.855   seconds
3. Neon (Neon  Add)     =   28.571  seconds
4. Neon (No Neon Add)   =   25.492  seconds
5. Both (Neon  Add)     =   14.719  seconds
6. Both (No Neon  Add)  =   14.619  seconds

The Loop unrolled code performed the best, likely because more efficient caching allowed the program to execute a bit faster than the naive implementation. 

The Neon Intrinsic functions did not speed up the process, likely because the amount of time saved by more efficient computation was negated by time the CPU spent in memomry declaring variables to prepare the Neon to run the arithmetic. The reason Neon functions were not very helpful in this Radix-2 FFT is because, unlike matrix multiplication, FFTs are not very sequential. Even though it is possible to add Neon intrinsic functions to the 


# Original FFt Code Citation
Much of this code was adapted from Lloyd Rochester: https://github.com/lloydroc/arduino_fft

Further documentation on Neon Intrinsic Functions can be found here: https://developer.arm.com/architectures/instruction-sets/intrinsics/#f:@navigationhierarchiessimdisa=[%5BNeon%5D]&f:@navigationhierarchiesreturnbasetype=[%5Bfloat%5D]&f:@navigationhierarchieselementbitsize=[%5B32%5D]
