// This is the only file that you may modify for PC Lab.

// There are two function defined in this file.  You, as a C S 429H
// student, must write C code to initialize the array "mem" so the
// program in "mem-chase.c" runs both as fast as possible and as
// slow as possible.

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#define PAGE_SIZE 4096
void init_data_fast(unsigned long int mem[], unsigned long int size) {
  // Simple, in-order accesses
  unsigned long int i;

  for (i = 0; i < size; i++)
    mem[i] = (i + 1);

  // And now, we overwrite the final entry
  mem[size - 1] = 0;
  //check_hits_all(mem, size);
  //check_for_duplicates(mem, size);
}

void check_for_duplicates(unsigned long int mem[], unsigned long int size) {
  for (unsigned long int i =0; i < size; i++) {
    for (unsigned long int j = i+1; j <size; j++) {
      if (mem[i] ==mem[j]) {
        printf("%08ld\n", i);
        printf("%08ld\n", j);
        printf("%08ld\n", mem[i]);

        assert(0);
      }
    }
  }
}
void check_hits_all(unsigned long int mem[], unsigned long int size) {
  unsigned long int at = mem[0];
  unsigned long int total = 1;
  printf("chechking hits all\n");
  while (at !=0) {
    
    at = mem[at];
    total++;
    if (total > size) 
      assert(0);//circular loop
  }
  printf("%08ld\n", total);
  printf("%08ld\n", size);

}
void init_data_slow(unsigned long int mem[], unsigned long int size) {
  // with 16 * 512/sizeof unsigned int have 3.9e10
  //with 8 * is 3.96e10
  //with 6 is 3.5
  //with 4 * is 4.06e10
  //with 3 * is 3.13
  //with 2 * is 3.91
  
  int log2 = 0;
  while (size >> log2) {
    log2++;
  }
  
  int changeBy = 4*512/sizeof(unsigned long int);//i think 64 should work too: its 128 (set bytes from )
  //getconf -a | grep CACHE command -> size / linesize is 512 cache lines -> 512/8 (assoc) = 64 for set  so 6 set bits
  //-> 6 set bits = 64

  //increment by block size, experimentally worse than following program

  bool lastOne = 0;
  //printf("Changeby is %08ld\n", changeBy);
  //getting 3.1812e10 on 22,1 
  printf("initialize\n");
  for (int startingIdx = 0; startingIdx < changeBy; startingIdx++) {
    for (int i = startingIdx; i < size; i+=changeBy) {
      if (lastOne)
        assert(0);

      unsigned long int val = i + changeBy;
      if (val < size)  {
        mem[i] = val;
        assert(val !=0);
      }
        
      else
        {
          mem[i] = startingIdx + 1;//changeBy - startingIdx-1;
          if (mem[i] == 0)
            lastOne = 1;
          if (mem[i] >=changeBy) {
            assert(mem[i] == changeBy);
            mem[i] = 0;
          }
        }
        
        
    }
  }
  printf("swapsies\n");

  for (unsigned long int i = changeBy; i < (size-changeBy-1)/2; i++) {
    unsigned long int swap1 = i;//mem[i];
    if (i % (2048*2*2*32*64*128) == 0)
      printf("%f percent done\n", (2*100*(float)i)/(size-1-changeBy));
    unsigned long int swap2 = size-1-changeBy-i;//swap1 + ((size-1-i-swap1)/changeBy)*changeBy;
    swap2 -= ((swap2-i) % changeBy);
    while ((swap2-i) %changeBy)
      swap2--;
    while (swap2 >=size-1-i) {
      swap2 -= changeBy;
    }
    unsigned long int p1 = i-changeBy;//swap1 - changeBy;
    unsigned long int p2 = swap2 - changeBy;
    unsigned long int n1 = mem[p1];
    unsigned long int a1 = mem[n1];

    unsigned long int n2 = mem[p2];
    unsigned long int a2 = mem[n2];
    if (!p1 || !p2 || !n2 || !n1 || !a1 || !a2 || n1==n2 || n1 == p2 || n1 == a2 || a1 == p2 || a2 == p1)
      continue;
    
    mem[p1] = n2;
    mem[n2] = a1;
    mem[p2] = n1;
    mem[n1] = a2;

  }

  
  printf("swapsies 2\n");
  
  for (unsigned long int i = 1; i < size; i++) {
    if ((i)%5==0)
      i++;//stop us from dragging anything too far
      
    unsigned long int p1 = i;
    unsigned long int n1 = mem[i];
    unsigned long int a1 = mem[n1];

    unsigned long int a11 = mem[a1];
    
    
    
    if (a11==0)
      continue;

    unsigned long int p2 = mem[a11];
    unsigned long int n2 = mem[p2];
    unsigned long int a2 = mem[n2];
    if (!p1 || !p2 || !n2 || !n1 || !a1 || !a2)
      continue;
    
    mem[p1] = n2;
    mem[n2] = a1;
    mem[p2] = n1;
    mem[n1] = a2;
  }

  
  
  //check_for_duplicates(mem,size);
  printf("swapsies 3\n");
  
  for (unsigned long int i = 0; i < size; i+=2) {
    //swap this one and the next one
    unsigned long int p1 =i;
    unsigned long int n1 = mem[p1];
    unsigned long n2 = mem[n1];
    unsigned long a = mem[n2];
    
    if (!p1 || !n1 || !n2 || !a)
      continue;
    mem[p1] = n2;
    mem[n2] = n1;
    mem[n1] = a;
  }
  

  //check_hits_all(mem, size);
  printf("exitting slow\n");
  
}