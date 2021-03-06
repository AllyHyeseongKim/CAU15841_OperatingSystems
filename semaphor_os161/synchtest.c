/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Synchronization test code.
 */

#include <types.h>
#include <lib.h>
#include <clock.h>
#include <thread.h>
#include <synch.h>
#include <test.h>


#define NSEMLOOPS     63
#define NLOCKLOOPS    120
#define NCVLOOPS      5
#define NTHREADS      32


static volatile unsigned long testval1;
static volatile unsigned long testval2;
static volatile unsigned long testval3;

/* Semaphores */
static struct semaphore *testsem;
static struct lock *testlock;
static struct cv *testcv;
static struct semaphore *donesem;
// Define semaphores for intersection points.
static struct semaphore *semNW;
static struct semaphore *semNE;
static struct semaphore *semSW;
static struct semaphore *semSE;
static struct semaphore *waitNW;
static struct semaphore *waitNE;
static struct semaphore *waitSW;
static struct semaphore *waitSE;
// Define the semaphore for printing messages.
static struct semaphore *KPRINT;
// Define the semaphore for moving cars.
static struct semaphore *POINT;
// Define the global variable for counting the number fo cars above the intersection. (ex. 0: 1 car above the intersection)
//int num_cars_intersection = -1;
//int return_controller = 32;
/* Threads */
typedef struct thread Thread;


/* Intesection points */
// Get intersection points to string.
const char* 
getCardinalPoint(CardinalPoint point)
{
	switch(point) {
		case NORTH:
			return "NORTH";
		case EAST:
			return " EAST";
		case SOUTH:
			return "SOUTH";
		case WEST:
			return " WEST";
		case NW:
			return "NW";
		case NE:
			return "NE";
		case SE:
			return "SE";
		case SW:
			return "SW";
	}
	return NULL;
}


/* Moving system */
void
pCardinalPoint(int cardinal_point)
{
	switch(cardinal_point) {
		case 4:
			P(semNW);
			break;
		case 5:
			P(semNE);
			break;
		case 6:
			P(semSE);
			break;
		case 7:
			P(semSW);
			break;

	}
}
void
vCardinalPoint(int cardinal_point)
{
        switch(cardinal_point) {
                case 4:
                        V(semNW);
                        break;
                case 5:
                        V(semNE);
                        break;
                case 6:
                        V(semSE);
                        break;
                case 7:
                        V(semSW);
                        break;

        }
}
void
pWaitPoint(int point)
{
	switch(point) {
                case 4:
                        P(waitNW);
                        break;
                case 5:
                        P(waitNE);
                        break;
                case 6:
                        P(waitSE);
                        break;
                case 7:
                        P(waitSW);
                        break;

        }
}
void
vWaitPoint(int point)
{
        switch(point) {
                case 4:
                        V(waitNW);
                        break;
                case 5:
                        V(waitNE);
                        break;
                case 6:
                        V(waitSE);
                        break;
                case 7:
                        V(waitSW);
                        break;

        }
}

// Go Straight.
void
goStraight(unsigned long car_num, CardinalPoint start_point, CardinalPoint end_point)
{
	int route;
	pCardinalPoint(start_point+4);
//	P(KPRINT);
//	kprintf("// %lu gets %s\n",car_num, getCardinalPoint(start_point+4));
//	V(KPRINT);
	P(POINT);
//	P(KPRINT);
//	kprintf("// %lu gets point/ resources remain: %d\n", car_num, POINT->sem_count);
//	V(KPRINT);

	// Print the current point.
	P(KPRINT);
	kprintf("[MOVE %s]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s (GO STRAIGHT)\n", getCardinalPoint(start_point+4), car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	num_cars_intersection++;
//	kprintf("// %d cars above the intesection.\n", num_cars_intersection+1);
	route = start_point+3;
        if(route<4)
                route = 7;
	V(KPRINT);
	
	for (int i=0; i<100; i++);

	pCardinalPoint(route);
//	P(KPRINT);
//        kprintf("// %lu gets %s\n",car_num, getCardinalPoint(route));
//        V(KPRINT);
	vCardinalPoint(start_point+4);
//	P(KPRINT);
//        kprintf("// %lu returns %s\n",car_num, getCardinalPoint(start_point+4));
//       V(KPRINT);
	// Print the current point	
	P(KPRINT);
        kprintf("[MOVE %s]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s (GO STRAIGHT)\n", getCardinalPoint(route), car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	kprintf("// %d cars above the intersection.\n", num_cars_intersection+1);
	V(KPRINT);
//	for (i=0; i<100; i++);
        // Print the exiting point of cars.
        P(KPRINT);
        kprintf("-----------------------------------------------------------------------------------------\n");
        kprintf("[LEAVING]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s\n", car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	num_cars_intersection--;
//	V(POINT);
//        vCardinalPoint(route);
//	kprintf("// %d cars above the intersection.\n", num_cars_intersection+1);
        kprintf("-----------------------------------------------------------------------------------------\n");
        V(KPRINT);
	
	V(POINT);
//	P(KPRINT);
//        kprintf("// %lu returns point/ resources remain: %d\n", car_num, POINT->sem_count);
//        V(KPRINT);
	vCardinalPoint(route);
//	P(KPRINT);
//        kprintf("// %lu returns %s\n",car_num, getCardinalPoint(route));
//        V(KPRINT);
//	V(POINT);
}
// Turn Right.
void
turnRight(unsigned long car_num, CardinalPoint start_point, CardinalPoint end_point)
{
//	int i;
        pCardinalPoint(start_point+4);
//	P(KPRINT);
//        kprintf("// %lu gets %s\n",car_num, getCardinalPoint(start_point+4));
//	kprintf("// %lu gets point/ resources remain: %d\n", car_num, POINT->sem_count);
//        V(KPRINT);
//	P(POINT);

        // Print the current point.
        P(KPRINT);
        kprintf("[MOVE %s]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s (TURN RIGHT)\n", getCardinalPoint(start_point+4), car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	num_cars_intersection++;
//	kprintf("// %d cars above the instersection.\n", num_cars_intersection+1);
        V(KPRINT);
//	for (i=0; i<100; i++);
	// Print the exiting point of cars.
        P(KPRINT);
        kprintf("-----------------------------------------------------------------------------------------\n");
        kprintf("[LEAVING]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s\n", car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	num_cars_intersection--;
//	vCardinalPoint(start_point+4);
//	kprintf("// %d cars above the intersection.\n", num_cars_intersection+1);
        kprintf("-----------------------------------------------------------------------------------------\n");
	V(KPRINT);
	
//	V(POINT);
	vCardinalPoint(start_point+4);
//	P(KPRINT);
//	kprintf("// %lu returns point/ resources remain: %d\n", car_num, POINT->sem_count);
//        kprintf("// %lu returns %s\n",car_num, getCardinalPoint(start_point+4));
//        V(KPRINT);
}
// Turn Left.
void
turnLeft(unsigned long car_num, CardinalPoint start_point, CardinalPoint end_point)
{
        int route1;
	int route2;

//	P(POINT);
        pCardinalPoint(start_point+4);
//	P(KPRINT);
//        kprintf("// %lu gets %s\n",car_num, getCardinalPoint(start_point+4));
//        V(KPRINT);
        P(POINT);
//        P(KPRINT);
//        kprintf("// %lu gets point/ resources remain: %d\n", car_num, POINT->sem_count);
//        V(KPRINT);

        // Print the current point.
        P(KPRINT);
        kprintf("[MOVE %s]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s (TURN LEFT)\n", getCardinalPoint(start_point+4), car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	num_cars_intersection++;
//	kprintf("// %d cars above the intersection.\n", num_cars_intersection+1);
	route1 = start_point+3;
        if(route1<4)
                route1 = 7;
        V(KPRINT);
	
	for (int i=0; i<100; i++);

        pCardinalPoint(route1);
//	P(KPRINT);
//        kprintf("// %lu gets %s\n",car_num, getCardinalPoint(route1));
//        V(KPRINT);
	vCardinalPoint(start_point+4);
//	P(KPRINT);
//        kprintf("// %lu returns %s\n",car_num, getCardinalPoint(start_point+4));
//        V(KPRINT);
        // Print the current point      
        P(KPRINT);
//        vCardinalPoint(start_point+4);
	kprintf("[MOVE %s]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s (TURN LEFT)\n", getCardinalPoint(route1), car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));	
//	kprintf("// %d cars above the intersection.\n", num_cars_intersection+1);
	route2 = route1-1;
        if(route2<4)
                route2 = 7;
        V(KPRINT);

//	for (i=0; i<100; i++);

        pCardinalPoint(route2);
//	P(KPRINT);
//        kprintf("// %lu gets %s\n",car_num, getCardinalPoint(route2));
//        V(KPRINT);
	vCardinalPoint(route1);
//	P(KPRINT);
//        kprintf("// %lu returns %s\n",car_num, getCardinalPoint(route1));
//        V(KPRINT);
	// Print the current point      
        P(KPRINT);
        kprintf("[MOVE %s]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s (TURN LEFT)\n", getCardinalPoint(route2), car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	kprintf("// %d cars above the intersection.\n", num_cars_intersection+1);
        V(KPRINT);
//	for (i=0; i<100; i++);
        // Print the exiting point of cars.
        P(KPRINT);
        kprintf("-----------------------------------------------------------------------------------------\n");
        kprintf("[LEAVING]     CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s\n", car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
//	num_cars_intersection--;
//	V(POINT);
//        vCardinalPoint(route2);
//	kprintf("// %d cars above the intersection.\n", num_cars_intersection+1);
        kprintf("-----------------------------------------------------------------------------------------\n");
        V(KPRINT);

	V(POINT);
//        P(KPRINT);
//        kprintf("// %lu returns point/ resources remain: %d\n", car_num, POINT->sem_count);
//        V(KPRINT);
        vCardinalPoint(route2);
//        P(KPRINT);
//        kprintf("// %lu returns %s\n",car_num, getCardinalPoint(route2));
//        V(KPRINT);
//	V(POINT);
}
// Moving System
void
movingSystem(unsigned long car_num, CardinalPoint start_point, CardinalPoint end_point)
{
	int i;
	pWaitPoint(start_point+4);
	if (end_point==(start_point+2)||end_point==(start_point-2))
		goStraight(car_num, start_point, end_point);
	else if (end_point==(start_point+3)||end_point==(start_point-1))
		turnRight(car_num, start_point, end_point);
	else if (end_point==(start_point+1)||end_point==(start_point-3))
		turnLeft(car_num, start_point, end_point);
	for(i=0; i<5000; i++);
	vWaitPoint(start_point+4);
}


/*
 * Test code
 */

/* Initialize semaphores*/
static
void
inititems(void)
{
	// Initialize semaphores
	if(POINT==NULL) {
		semNW = sem_create("NW", 1);
		semNE = sem_create("NE", 1);
		semSW = sem_create("SW", 1);
		semSE = sem_create("SE", 1);
		waitNW = sem_create("WAITNW", 2);
                waitNE = sem_create("WAITNE", 2);
                waitSW = sem_create("WAITSW", 2);
                waitSE = sem_create("WAITSE", 2);
		KPRINT = sem_create("KPRINT", 1);
		POINT = sem_create("POINT", 3);
		if(POINT==NULL) {
			panic("synchtest: sem_create failed\n");
		}
	}

	if (testsem==NULL) {
		testsem = sem_create("testsem", 1);
		if (testsem == NULL) {
			panic("synchtest: sem_create failed\n");
		}
	}
	if (testlock==NULL) {
		testlock = lock_create("testlock");
		if (testlock == NULL) {
			panic("synchtest: lock_create failed\n");
		}
	}
	if (testcv==NULL) {
		testcv = cv_create("testlock");
		if (testcv == NULL) {
			panic("synchtest: cv_create failed\n");
		}
	}
	if (donesem==NULL) {
		donesem = sem_create("donesem", 0);
		if (donesem == NULL) {
			panic("synchtest: sem_create failed\n");
		}
	}
}

static
void
semtestthread(void *cars, unsigned long car_num)
{
	P(donesem);
	Thread *car = (Thread*)cars;

	// Define start and end points of cars.
	CardinalPoint start_point;
	CardinalPoint end_point;
	// Generate two different random numbers in 0~3.
	start_point = random()%4;
	end_point = random()%4;
	while (start_point==end_point) {
		end_point = random()%4;
	}

	// Print the approaching point of cars.
	P(KPRINT);
	kprintf("-----------------------------------------------------------------------------------------\n");
	kprintf("[APPROACHING] CAR NUMBER: %2lu| APPROACHING POINT: %s, TARGET POINT: %s\n", car_num, getCardinalPoint(start_point), getCardinalPoint(end_point));
	kprintf("-----------------------------------------------------------------------------------------\n");
	V(KPRINT);

	for (int i=0; i<50; i++);

	// Move the car with the moving system.
	movingSystem(car_num, start_point, end_point);
	car[car_num].t_state = S_ZOMBIE;
	
//	return_controller--;
//	P(KPRINT);
//        kprintf("***DONE Thread: %lu\n", car_num);
//	V(KPRINT);
//        V(KPRINT);
	V(donesem);
}

int
semtest(int nargs, char **args)
{
	int i, result;

	(void)nargs;
	(void)args;

	inititems();
	kprintf("Starting semaphore test...\n");
	kprintf("If this hangs, it's broken: ");
	for (i=0; i<NTHREADS; i++) {
		V(donesem);
	}
	kprintf("ok\n");
	kprintf("-----------------------------------------------------------------------------------------\n");
	kprintf("-----------------------------------------------------------------------------------------\n");


	// Generate car threads
	Thread *cars = NULL;
	cars = (Thread*)kmalloc(sizeof(Thread)*NTHREADS);
	for (i=0; i<NTHREADS; i++) {
		result = thread_fork("semtest", NULL, semtestthread, (void*)cars, i);
		if (result) {
			panic("semtest: thread_fork failed: %s\n",
			      strerror(result));
		}
	}

	for (i=0; i<NTHREADS; i++) {
		V(donesem);			
	}
	
	for (i=0; i<NTHREADS*100000; i++);

	for (i=0; i<NTHREADS*2; i++) {
                P(donesem);             
        }

//	while(return_controller>0);

	// Free threads
	if (cars!=NULL) {
		kfree(cars);
		cars = NULL;
	}
	P(KPRINT);
	kprintf("Free Threads.\n");
	V(KPRINT);
	
	/* so we can run it again */
//	V(testsem);
//	V(testsem);
	kprintf("-----------------------------------------------------------------------------------------\n");
        kprintf("-----------------------------------------------------------------------------------------\n");
	kprintf("Semaphore test done.\n");
	return 0;
}

static
void
fail(unsigned long num, const char *msg)
{
	kprintf("thread %lu: Mismatch on %s\n", num, msg);
	kprintf("Test failed\n");

	lock_release(testlock);

	V(donesem);
	thread_exit();
}

static
void
locktestthread(void *junk, unsigned long num)
{
	int i;
	(void)junk;

	for (i=0; i<NLOCKLOOPS; i++) {
		lock_acquire(testlock);
		testval1 = num;
		testval2 = num*num;
		testval3 = num%3;

		if (testval2 != testval1*testval1) {
			fail(num, "testval2/testval1");
		}

		if (testval2%3 != (testval3*testval3)%3) {
			fail(num, "testval2/testval3");
		}

		if (testval3 != testval1%3) {
			fail(num, "testval3/testval1");
		}

		if (testval1 != num) {
			fail(num, "testval1/num");
		}

		if (testval2 != num*num) {
			fail(num, "testval2/num");
		}

		if (testval3 != num%3) {
			fail(num, "testval3/num");
		}

		lock_release(testlock);
	}
	V(donesem);
}


int
locktest(int nargs, char **args)
{
	int i, result;

	(void)nargs;
	(void)args;

	inititems();
	kprintf("Starting lock test...\n");

	for (i=0; i<NTHREADS; i++) {
		result = thread_fork("synchtest", NULL, locktestthread,
				     NULL, i);
		if (result) {
			panic("locktest: thread_fork failed: %s\n",
			      strerror(result));
		}
	}
	for (i=0; i<NTHREADS; i++) {
		P(donesem);
	}

	kprintf("Lock test done.\n");

	return 0;
}

static
void
cvtestthread(void *junk, unsigned long num)
{
	int i;
	volatile int j;
	struct timespec ts1, ts2;

	(void)junk;

	for (i=0; i<NCVLOOPS; i++) {
		lock_acquire(testlock);
		while (testval1 != num) {
			gettime(&ts1);
			cv_wait(testcv, testlock);
			gettime(&ts2);

			/* ts2 -= ts1 */
			timespec_sub(&ts2, &ts1, &ts2);

			/* Require at least 2000 cpu cycles (we're 25mhz) */
			if (ts2.tv_sec == 0 && ts2.tv_nsec < 40*2000) {
				kprintf("cv_wait took only %u ns\n",
					ts2.tv_nsec);
				kprintf("That's too fast... you must be "
					"busy-looping\n");
				V(donesem);
				thread_exit();
			}

		}
		kprintf("Thread %lu\n", num);
		testval1 = (testval1 + NTHREADS - 1)%NTHREADS;

		/*
		 * loop a little while to make sure we can measure the
		 * time waiting on the cv.
		 */
		for (j=0; j<3000; j++);

		cv_broadcast(testcv, testlock);
		lock_release(testlock);
	}
	V(donesem);
}

int
cvtest(int nargs, char **args)
{

	int i, result;

	(void)nargs;
	(void)args;

	inititems();
	kprintf("Starting CV test...\n");
	kprintf("Threads should print out in reverse order.\n");

	testval1 = NTHREADS-1;

	for (i=0; i<NTHREADS; i++) {
		result = thread_fork("synchtest", NULL, cvtestthread, NULL, i);
		if (result) {
			panic("cvtest: thread_fork failed: %s\n",
			      strerror(result));
		}
	}
	for (i=0; i<NTHREADS; i++) {
		P(donesem);
	}

	kprintf("CV test done\n");

	return 0;
}

////////////////////////////////////////////////////////////

/*
 * Try to find out if going to sleep is really atomic.
 *
 * What we'll do is rotate through NCVS lock/CV pairs, with one thread
 * sleeping and the other waking it up. If we miss a wakeup, the sleep
 * thread won't go around enough times.
 */

#define NCVS 250
#define NLOOPS 40
static struct cv *testcvs[NCVS];
static struct lock *testlocks[NCVS];
static struct semaphore *gatesem;
static struct semaphore *exitsem;

static
void
sleepthread(void *junk1, unsigned long junk2)
{
	unsigned i, j;

	(void)junk1;
	(void)junk2;

	for (j=0; j<NLOOPS; j++) {
		for (i=0; i<NCVS; i++) {
			lock_acquire(testlocks[i]);
			V(gatesem);
			cv_wait(testcvs[i], testlocks[i]);
			lock_release(testlocks[i]);
		}
		kprintf("sleepthread: %u\n", j);
	}
	V(exitsem);
}

static
void
wakethread(void *junk1, unsigned long junk2)
{
	unsigned i, j;

	(void)junk1;
	(void)junk2;

	for (j=0; j<NLOOPS; j++) {
		for (i=0; i<NCVS; i++) {
			P(gatesem);
			lock_acquire(testlocks[i]);
			cv_signal(testcvs[i], testlocks[i]);
			lock_release(testlocks[i]);
		}
		kprintf("wakethread: %u\n", j);
	}
	V(exitsem);
}

int
cvtest2(int nargs, char **args)
{
	unsigned i;
	int result;

	(void)nargs;
	(void)args;

	for (i=0; i<NCVS; i++) {
		testlocks[i] = lock_create("cvtest2 lock");
		testcvs[i] = cv_create("cvtest2 cv");
	}
	gatesem = sem_create("gatesem", 0);
	exitsem = sem_create("exitsem", 0);

	kprintf("cvtest2...\n");

	result = thread_fork("cvtest2", NULL, sleepthread, NULL, 0);
	if (result) {
		panic("cvtest2: thread_fork failed\n");
	}
	result = thread_fork("cvtest2", NULL, wakethread, NULL, 0);
	if (result) {
		panic("cvtest2: thread_fork failed\n");
	}

	P(exitsem);
	P(exitsem);

	sem_destroy(exitsem);
	sem_destroy(gatesem);
	exitsem = gatesem = NULL;
	for (i=0; i<NCVS; i++) {
		lock_destroy(testlocks[i]);
		cv_destroy(testcvs[i]);
		testlocks[i] = NULL;
		testcvs[i] = NULL;
	}

	kprintf("cvtest2 done\n");
	return 0;
}
