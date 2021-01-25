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

	template<typename T>
	inline void swap(T* a, T* b) {
		T temp = *a;
		*a = *b;
		*b = temp;
	}

	template<typename T>
	inline int sign(T a) {
		if (a < 0) {
			return -1;
		}
		else if (a > 0) {
			return 1;
		}
		return 0;
	}

	template<typename T>
	inline int max(T a, T b) {
		if (a > b) return a;
		else return b;
		return a;
	}

	template<typename T>
	inline int min(T a, T b) {
		if (a < b) return a;
		else return b;
		return a;
	}

}

#endif // !OSMATH_H
