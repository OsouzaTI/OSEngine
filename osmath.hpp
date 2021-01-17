#ifndef OSMATH_H
#define OSMATH_H
	
//---------- MATH
#define PI              3.14159265359 // PI
#define HALF_PI         PI/2
#define TWO_PI          6.28318530718 // TWO PI
#define TWO_PI_GRAUS    360
#define RAD(x) (x*PI)/180

namespace Math {


	inline double map(float v, float a, float b, float x, float y) {
		return x + ((y - x) / (b - a)) * (v - a);
	}

	inline void swap(int* a, int* b) {
		int temp = *a;
		*a = *b;
		*b = temp;
	}

}

#endif // !OSMATH_H
