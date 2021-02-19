public class Qiu_Garland_DivisorThreads {
	// range of numbers;
	private static final int range = 10000;
	// largest number with largest number of divisors
	private static volatile int maxDivisorNumber;
	// largest divisor counter
	private static volatile int maxDivisorCounter = 0;
	
	// result function for threads to tell us what number they got afterwards
	// if the number is bigger than the one so far, replace it.
	private synchronized static void result(int maxThreadCounter, int maxThreadNumber) {
		if(maxThreadCounter > maxDivisorCounter) {
			maxDivisorCounter = maxThreadCounter;
			maxDivisorNumber = maxThreadNumber;
		}
	}
	
	// Java has a Thread class that I will just use.
	private static class DivisorCounterThread extends Thread {
		int min, max;
		
		public DivisorCounterThread(int min, int max) {
			this.min = min;
			this.max = max;
		}
		
		// run() function is from the Thread class
		// using this to have the threads go through their range of numbers and find the number with the most divisors
		public void run() {
			long startTime = System.currentTimeMillis();
			int maxDivisors = 0;
			int largestInt = 0;
			
			for(int i = min; i < max; i++) {
				int currentDivisors = countDivisors(i);
				if(currentDivisors > maxDivisors) {
					maxDivisors = currentDivisors;
					largestInt = i;
				}
			}
			
			long timeSpent = System.currentTimeMillis() - startTime;
			System.out.println(this + " took " + timeSpent + " ms.");
			result(maxDivisors, largestInt);
		}
	}
	
	// finds the number with the most divisors, and dividing the work among threads.
	private static void divisorCounterThreads(int numberOfThreads) {
		long startTime = System.currentTimeMillis();
		//setting up threads and their ranges
		DivisorCounterThread[] worker = new DivisorCounterThread[numberOfThreads];
		int rangePerThread = range / numberOfThreads;
		int start = 1;
		int end = start + rangePerThread - 1;
		for(int i = 0; i < numberOfThreads; i++) {
			// making sure this goes all the way to the end, in this case, 10000.
			if(i == numberOfThreads - 1) {
				end = range;
			}
			worker[i] = new DivisorCounterThread(start, end);
			start = end + 1;
			end = start + rangePerThread - 1;
		}
		
		maxDivisorCounter = 0;
		for(int i = 0; i < numberOfThreads; i++) {
			// starting thread
			worker[i].start();
			// this loop waits for each worker thread to finish and that each thread has given us their results
			while(worker[i].isAlive()) {
				try {
					worker[i].join();
				}
				catch (InterruptedException e) {
				}
			}
		}
		
		// our final result
		long timeSpent = System.currentTimeMillis() - startTime;
		System.out.println("Largest number with the most divisors is " + maxDivisorNumber + " and it has " + maxDivisorCounter + " divisors.");
		System.out.println("Time Thread Took: " + timeSpent + " ms.");
	}
	
	// simple divisor counter.
	public static int countDivisors(int n) {
		int counter = 0;
		for(int i = 1; i <= n; i++) {
			if(n % i == 0) {
				counter++;
			}
		}
		return counter;
	}
	
	public static void main(String[] args) {
		// declare threads to use. (did a quick lookup and people recommended n+1 threads, n being the number of cores.)
		// i have 4 cores and 8 threads on my PC, so i'll use 5.
		int numberOfThreads = 5;
		divisorCounterThreads(numberOfThreads);
	}
}
