#include "ex2.h"

#define MAX_INT 2147483647
#define MIN_INT ~MAX_INT


int swapMSB(int i) {
	if (i < 0) return i & MAX_INT;
	return i | MIN_INT;
}

int magnitudeToInt(magnitude a) {
	if (a >= 0) return a;
	if (swapMSB(a) == 0) return 0;
	return -swapMSB(a);
}

magnitude intToMagnitude(int a) {
	if (a >= 0) return a;
	return swapMSB(-a);
}

magnitude add(magnitude a, magnitude b) {
	int ia = magnitudeToInt(a), ib = magnitudeToInt(b);
	int iresult = ia + ib;
	magnitude result = intToMagnitude(iresult);
	
	if (((ia > 0) && (ib > 0) && (iresult < 0)) || ((ia < 0) && (ib < 0) && (iresult > 0))) {
		result *= -1;
	}

	return result;
}

magnitude sub(magnitude a, magnitude b) {
	return add(a, swapMSB(b));
}

magnitude multi(magnitude a, magnitude b) {
	int ia = magnitudeToInt(a), ib = magnitudeToInt(b);
	int iresult = ia * ib;
	magnitude result = intToMagnitude(iresult);
	
	if (((ia > 0) && (ib > 0) && (iresult < 0)) || 
		((ia < 0) && (ib < 0) && (iresult < 0)) || 
		((ia < 0) && (ib > 0) && (iresult > 0)) || 
		((ia > 0) && (ib < 0) && (iresult > 0))) {
		result *= -1;
	}

	return result;
}

int equal(magnitude a, magnitude b) {
	return magnitudeToInt(a) == magnitudeToInt(b);
}

int greater(magnitude a, magnitude b) {
	return magnitudeToInt(a) > magnitudeToInt(b);
}