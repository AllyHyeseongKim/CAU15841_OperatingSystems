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

#ifndef _TEST_H_
#define _TEST_H_

/*
 * Declarations for test code and other miscellaneous high-level
 * functions.
 */

/*
 * Car system.
 */

/* intersection Points*/
// Define intersection points.
typedef enum cardinalPoint {
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3,
        NW = 4,
        NE = 5,
        SE = 6,
        SW = 7
} CardinalPoint;
const char* getCardinalPoint(CardinalPoint);

/* moving system */
void pCardinalPoint(int);
void vCardinalPoint(int);
void pWaitPoint(int);
void vWaitPoint(int);
void goStraight(unsigned long, CardinalPoint, CardinalPoint);
void turnRight(unsigned long, CardinalPoint, CardinalPoint);
void turnLeft(unsigned long, CardinalPoint, CardinalPoint);
void movingSystem(unsigned long, CardinalPoint, CardinalPoint);

/*
 * Test code.
 */

/* data structure tests */
int arraytest(int, char **);
int bitmaptest(int, char **);
int threadlisttest(int, char **);

/* thread tests */
int threadtest(int, char **);
int threadtest2(int, char **);
int threadtest3(int, char **);
int semtest(int, char **);
int locktest(int, char **);
int cvtest(int, char **);
int cvtest2(int, char **);

/* filesystem tests */
int fstest(int, char **);
int readstress(int, char **);
int writestress(int, char **);
int writestress2(int, char **);
int longstress(int, char **);
int createstress(int, char **);
int printfile(int, char **);

/* other tests */
int malloctest(int, char **);
int mallocstress(int, char **);
int malloctest3(int, char **);
int malloctest4(int, char **);
int nettest(int, char **);

/* Routine for running a user-level program. */
int runprogram(char *progname);

/* Kernel menu system. */
void menu(char *argstr);

/* The main function, called from start.S. */
void kmain(char *bootstring);


#endif /* _TEST_H_ */
