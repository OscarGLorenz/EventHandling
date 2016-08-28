/***********************************************************************
* FILENAME :        Vector.h            
*
* DESCRIPTION :
*       A dinamic array template class for C++11
* 		No std namespace
* 		Compatible with Arduino
*
* AUTHOR :    Óscar García       START DATE :    31 jul 16
***********************************************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <string.h>

namespace dyn {

template<class T> class Vector {
public:

	Vector<T>() {
		/** Constructor of Empty Vector, asigns nullptr to pointer*/
		_lenght = 0;
		_values = nullptr;
	}

	int size() {
		/** Returns the actual size of the Vector*/
		return _lenght;
	}

	Vector<T>(T * vector, int vectorSize) {
		/** Constructor of Vector, with array's values*/
		_lenght = vectorSize;
		_values = new T[vectorSize];
		memcpy(_values, vector, vectorSize);
	}

	Vector<T>(int size, T value) {
		/** Constructor of Vector, reserves memory and assigns given
		 * value*/
		 _values = new T[size];
		 for (int i = 0; i < size; i++) {
			 _values[i] = value;
		 }
		 _lenght = size;
	}

	T get(int index) {
		/** Returns the object in the given index*/
		return _values[index];
	}

	T * toArray(int * arraySize = nullptr) {
		/** Returns a pointer of a dynamic array make with 
		 * the values of the Vector
		 * Optional: Pass by reference an int
		 * to get the size of the Array*/
		 if(arraySize != nullptr) *arraySize = _lenght;
		 
		 T * aux = new T[*arraySize];
		 memcpy(aux, _values,  *arraySize);
		 return aux;
	}

	T * toArray(int begin, int * arraySize = nullptr) {
		/** Returns a pointer of a dynamic array make with 
		 * the values of the Vector from (begin) to (size - 1)
		 * Optional: Pass by reference an int
		 * to get the size of the Array*/
		 if(arraySize != nullptr) *arraySize = _lenght-begin;
		 
		 T * aux = new T[*arraySize];
		 memcpy(aux+begin, _values,  *arraySize);
		 return aux;
	}

	T * toArray(int begin, int end, int * arraySize = nullptr) {
		/** Returns a pointer of a dynamic array make with 
		 * the values of the Vector from (begin) to (end - 1)
		 * Optional: Pass by reference an int
		 * to get the size of the Array*/
		 if(arraySize != nullptr) *arraySize = end-begin;
		 
		 T * aux = new T[*arraySize];
		 memcpy(aux+begin, _values,  *arraySize);
		 return aux;
	}

	Vector<T> get(int begin, int end) {
		/**Returns a Vector with the values 
		 * between (i) and (j - 1)*/
		 T * aux = toArray(begin, end);
		 Vector<T> vec = Vector<T>(aux, _lenght);
		 delete[] aux;
		 return vec;
	}
	
	void set(int index, T value) {
		/**Updates the value of the given index*/
		_values[index] = value;
	}

	void set(int begin, int end, T value) {
		/**Updates the values from (begin) to (end - 1)*/
		for (int i = begin; i < end; i++) {
			_values[i] = value;
		}
	}

	void set(int begin, T * values, int arraySize) {
		/**Updates the values of the Vector
		 * with the values in the array starting at (begin)*/
		memcpy(_values+begin, values, arraySize);
	}

	void set(int begin, Vector<T> values) {
		/**Updates the values of the Vectorsize()
		 * with the values in the given Vector starting at (begin)*/
		memcpy(_values+begin, values.toArray(), values.size());
	}

	void add(T value) {
		/**Append at the end of the vector the given value*/
		T * aux = new T[_lenght+1];
		memcpy(aux, _values, _lenght);
		aux[_lenght] = value;
		delete[] _values;
		_values = aux;
		_lenght++;
	}

	void erase(int index) {
		/**Deletes the value in the given index
		 * and moves back other values, and deallocate free memory*/
		 T * aux = new T[_lenght-1];
		 for (int i = 0, j = 0; i < _lenght; i++, j++) {
			if (index == i) {
				j--;
				continue;
			}
			aux[j] = _values[i]; 
		}
		delete[] _values;
		_values = aux;
		_lenght--;
	}

	void erase(int begin, int end) {
		/**Deletes values between (begin) to (end - 1) and moves back
		 * other values, and deallocate free memory*/
		 T * aux = new T[_lenght-(end-begin)];
		 for (int i = 0, j = 0; i < _lenght; i++, j++) {
			if (i >= begin && i < end) {
				j--;
				continue;
			}
			aux[j] = _values[i]; 
		}
		delete[] _values;
		_values = aux;
		_lenght-=(end-begin);
	}

	bool erase(T value) {
		/**Search the given value and deletes first ocurrence,
		 * If the Vector has the given value returns true
		 * Deallocates free memory*/
		bool found = false;
		int index = 0;
		for (int i = 0; i < _lenght; i++) {
			if (_values[i] == value) {
				found = true;
				index = i;
				break;
			}
		}
		 
		if (found) {
			T * aux = new T[_lenght-1];
			for (int i = 0, j = 0; i < _lenght; i++, j++) {
			if (index == i) {
				j--;
				continue;
			}
			aux[j] = _values[i]; 
			}
			delete[] _values;
			_values = aux;
			_lenght--;
			return true;
		}
		return false;
	}

	void trim(int begin, int end) {
		/**Deletes values outside the range (begin) to (end-1).
		 * Deallocates free memory*/
		T * aux = new T[end-begin];
		memcpy(aux, _values+begin, end-begin);
		delete[] _values;
		_values = aux;
		_lenght = (end-begin);
	}

	void trim(int begin) {
		/**Deletes values outside the range (begin) to (size-1).
		 * Deallocates free memory*/
		 trim (begin, _lenght);
	}

	void addAll(Vector<T> vector) {
		/**Appends at end of the vector the given vector*/
		T * aux = new T[_lenght+vector.size()-1];
		int i;
		memcpy(aux, _values, _lenght);
		memcpy(aux+_lenght, vector.toArray(), vector.size());
		delete[] _values;
		_values = aux;
		_lenght = _lenght+vector.size()-1;
	}

	~Vector() {
		/**Free allocated memory and deletes the Array*/
		clear();
	}

	void clear() {
		/**Free allocated memory*/
		delete[] _values;
		_lenght = 0;
	}

private:
	/**Pointer of the Vector*/
	T * _values;
	/**Actual allocated memory*/
	int _lenght;
};
}
#endif

