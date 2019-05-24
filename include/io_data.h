/*
 * Copyright 2019 Vrije Universiteit Amsterdam, The Netherlands
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef IO_DATA_H
#define IO_DATA_H



#include <iostream>
#include <fstream>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <cstdlib>
#include <cstring>



#include "io_data_constants.h"



using namespace std;


extern float allowed_error;
extern float allowed_float_error;
extern bool doPrintOutput;


template<typename T> 
class Output {
    public:    
	static T *cmpOutput; 

    private:    
	Output() {}
	Output(const Output& rhs) {}
	void operator=(const Output& rhs) {}
};
template<typename T> T *Output<T>::cmpOutput = NULL;



template<typename T>
inline T* aligned_new(size_t size, size_t alignment) {
    assert(alignment > 0);
    assert((alignment & (alignment - 1)) == 0);
    assert(alignment >= sizeof(void*));
    assert(size >= sizeof(void*));
    // assert(size/sizeof(void*)*sizeof(void*) == size);

    // alignment in terms of T
    // size of a pointer in terms of T
    size_t alignmentInT = alignment / sizeof(T);
    size_t sizePointerInT = sizeof(void*)/sizeof(T);

    // size elements, alignment and the pointer (in terms of T)
    T* orig = new T[size + alignmentInT + sizePointerInT];
    T* aligned =
        (T*) ((size_t)orig + (
            (((size_t)orig + alignment + sizeof(void*)) & ~(alignment - 1)) -
            (size_t)orig)
        );
    *((T**)aligned - 1) = orig;
    return aligned;
}


template<typename T>
inline void aligned_delete (T *aligned) {
    if(!aligned)return;
    delete [] *((T**)aligned - 1);
}







template<typename T>
inline void printOutput2(T *a, const char *cname, int h, int w) {
    if (w == 1) {
	w = h;
	h = 1;
    }
    string name(cname);
    string fileName("out_");
    fileName += name;

    ofstream out(fileName.c_str());
    out.precision(5);

    for (int i = 0; i < h; i++) {
	for (int j = 0; j < w; j++) {
	    out << a[i * w + j] << ' ';
	}
	out << endl;
    }
    out << endl;

    out.close();
}



template<typename T>
inline void printOutput(T *a, const char *name, int w) {
    cerr << "printOutput() Type T not supported" << endl;
    exit(1);
}

template<typename T>
inline void printOutput(T *a, const char *name, int h, int w) {
    cerr << "printOutput() Type T not supported" << endl;
    exit(1);
}

template<>
inline void printOutput<double>(double *a, const char *cname, int w) {
    printOutput2<double>(a, cname, 1, w);
}


template<>
inline void printOutput<float>(float *a, const char *cname, int w) {
    printOutput2<float>(a, cname, 1, w);
}

template<>
inline void printOutput<double>(double *a, const char *cname, int h, int w) {
    printOutput2<double>(a, cname, h, w);
}


template<>
inline void printOutput<float>(float *a, const char *cname, int h, int w) {
    printOutput2<float>(a, cname, h, w);
}

template<>
inline void printOutput<int>(int *a, const char *cname, int w) {
    printOutput2<int>(a, cname, 1, w);
}

template<>
inline void printOutput<int>(int *a, const char *cname, int h, int w) {
    printOutput2<int>(a, cname, h, w);
}


template<typename T>
inline int equals(float *error, T f1, T f2) {
    cerr << "equals() Type T not supported" << endl;
    exit(1);
}


template<>
inline int equals<double>(float *error, double f1, double f2) {
    double absf1 = fabs(f1);
    double absf2 = fabs(f2);
    double e = fabs(f1 - f2);
    if (f1 == f2) {
	return 1;
    }
    if (f1 == 0 || f2 == 0) {
	if (e >= allowed_float_error) {
	    *error = e;
	    return 0;
	}
	return 1;
    }
    if (e < DBL_MIN) {
	return 1;
    }
    e /= (absf1 + absf2);
    if (e < allowed_float_error) {
	return 1;
    } else {
	*error = e;
	return 0;
    }
}


template<>
inline int equals<float>(float *error, float f1, float f2) {
    float absf1 = fabs(f1);
    float absf2 = fabs(f2);
    float e = fabs(f1 - f2);
    if (f1 == f2) {
	return 1;
    }
    if (f1 == 0 || f2 == 0) {
	if (e >= allowed_float_error) {
	    *error = e;
	    return 0;
	}
	return 1;
    }
    if (e < FLT_MIN) {
	return 1;
    }
    e /= (absf1 + absf2);
    if (e < allowed_float_error) {
	return 1;
    } else {
	*error = e;
	return 0;
    }
}


template<>
inline int equals<int>(float *error, int f1, int f2) {
    return f1 == f2;
}


template<typename T>
inline int equals(float *error, T *f1, T *f2, int size) {
    float maxerror = 0;
    for (int i = 0; i < size; i++) {
	if (!equals(error, f1[i], f2[i])) {
	    if (*error > maxerror) {
		cerr << "error = " << *error << ", f1 = " << f1[i] << ", f2 = " << f2[i] << endl;
		maxerror = *error;
	    }
	}
    }

    if (maxerror != 0) {
	*error = maxerror;
	return 0;
    }

    return 1;
}


template<typename T>
inline void copy(T *dest, T *src, int size) {
    for (int i = 0; i < size; i++) {
	dest[i] = src[i];
    }
}


template<typename T>
inline void checkOutput(T *output, const char *name, int h = 1, int w = 1) {
    T *cmpOutput = Output<T>::cmpOutput;

    if (doPrintOutput) {
	printOutput<T>(output, name, h, w);
    }

    if (!strcmp(name, "cpu") || cmpOutput == NULL) {
	cmpOutput = aligned_new<T>(h * w, 128);
	copy(cmpOutput, output, h * w);
	Output<T>::cmpOutput = cmpOutput;
	return;
    }

    if (cmpOutput != NULL) {
	float error = 0.0;
	if (!equals(&error, output, cmpOutput, h * w)) {
	    cout << name << " has an error of " << error << endl;
	    if (!doPrintOutput) {
		printOutput<T>(cmpOutput, "cpu", h, w);
		printOutput<T>(output, name, h, w);
	    }
	}
    }
}




template<typename T>
inline void cleanArray2(T *a, int size) {
    cerr << "cleanArray() Type T not supported" << endl;
    exit(1);
}


template<>
inline void cleanArray2<double>(double *a, int size) {
    for (int i = 0; i < size; i++) {
	a[i] = 0.0;
    }
}


template<>
inline void cleanArray2<float>(float *a, int size) {
    for (int i = 0; i < size; i++) {
	a[i] = 0.0;
    }
}


template<>
inline void cleanArray2<int>(int *a, int size) {
    for (int i = 0; i < size; i++) {
	a[i] = 0;
    }
}


template<typename T>
inline void cleanArray(T *a, int h = 1, int w = 1) {
    cleanArray2(a, h * w);
}







/* Creating inputs */
template<typename T>
inline void fillArrayRandom(T *array, int size) {
    cerr << "fillArrayRandom not supported" << endl;
    exit(1);
}

template<>
inline void fillArrayRandom<double>(double *array, int size) {
    for (int i = 0; i < size; i++) {
	array[i] = 10 * (rand()/(RAND_MAX+1.0));
    }
}


template<>
inline void fillArrayRandom<float>(float *array, int size) {
    for (int i = 0; i < size; i++) {
	array[i] = 10 * (rand()/(RAND_MAX+1.0));
    }
}

template<>
inline void fillArrayRandom<int>(int *array, int size) {
    for (int i = 0; i < size; i++) {
	array[i] = (int)(256 * (rand()/(RAND_MAX+1.0)));
    }
}



template<typename T>
inline void fillArrayRegular(T *array, int size) {
    cerr << "fillArrayRegular not supported" << endl;
    exit(1);
}


template<>
inline void fillArrayRegular<float>(float *array, int size) {
    for (int i = 0; i < size; i++) {
	array[i] = 1;
    }
}


template<typename T>
inline void fillArray(T *array, int size) {
    if (RANDOM) {
	fillArrayRandom(array, size);
    }
    else {
	fillArrayRegular(array, size);
    }
}

template<typename T>
inline T *createInput(int h = 1, int w = 1) {
    T *input = aligned_new<T>((h * w), 128);
    fillArray(input, h * w);
    return input;
}

void turnOnPrintingOutput();
void setError(float e);
void setFloatError(float e);

#endif
