/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    auto_array.h
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   Version of std::auto_ptr for dynamic arrays.
 *
 */

#ifndef AUTO_ARRAY_H
#define AUTO_ARRAY_H

/**
 * @brief Limited version of std::auto_ptr for dynamic arrays.
 *
 * As you know, one cannot use std::auto_ptr for storing dynamic arrays
 * because std::auto_ptr calls delete instead of delete[] in its destructor.
 * This version provides the same functinality for dynamic arrays.
 *
 * @todo Only a limited support is currently available, so
 *       add the rest of the functionality that is provided by the std::auto_ptr
 *       template class.
 */
template<typename T>
class auto_array {
	public:
		/**
		 * @brief Constructs an auto_array object from an array pointer.
		 *
		 * @param p Pointer to a dynamically allocated array. If this parameter
		 *         is 0 the auto_array is a null pointer (points to nowhere).
		 */
		explicit auto_array(T *p = 0) throw(): _p(p) { }

		/**
		 * @brief Deallocates the block of memory the object "points to" using
		 *        delete[] and destructs the object.
		 */
		~auto_array() throw() { delete[] _p; }

		/**
		 * @brief Returns the stored pointer.
		 *
		 * @return Pointer to the array pointed by the auto_array object,
		 *        if any, or zero if it does not point to any array.
		 */
		T *get() const throw() { return _p; }

		/**
		 * @brief Returns the stored pointer and sets the internal pointer to 0.
		 *
		 * @return Pointer to the array pointed by the auto_array object,
		 *        if any, or zero if it does not point to any array.
		 *
		 * Sets the auto_array internal pointer to null pointer (which indicates
		 * it points to no array) without destructing the array currently pointed
		 * by the auto_array.
		 */
		T *release() throw() {
			T *tmp = _p;
			_p = 0;
			return tmp;
		}

		/**
		 * @brief Deallocates the array that the auto_array points to and sets
		 *        a new value.
		 *
		 * Destructs the array pointed by the auto_array object, if any, and
		 * deallocates its memory (by calling operator delete[]). If a value
		 * for p is specified, the internal pointer is initialized to that value
		 * (otherwise it is set to the null pointer).
		 */
		void reset(T *p = 0) throw() {
			delete[] _p;
			_p = p;
		}

	private:
		/// Stored pointer.
		T *_p;

	private:
		// TODO - implement these:
		// http://www.cplusplus.com/reference/std/memory/auto_ptr/auto_ptr.html
		auto_array(auto_array& a) throw();
		template<class Y> auto_array(auto_array<Y> &a) throw();
		// auto_array(auto_array_ref<T> r) throw();
		T &operator*() const throw();
		T *operator->() const throw();
		auto_array &operator=(auto_array &a) throw();
		template <class Y> auto_array &operator= (auto_array<Y> &a) throw();
		//auto_array &operator=(auto_array_ref<T> r) throw();
		//template <class Y> operator auto_array_ref<Y>() throw();
		template <class Y> operator auto_array<Y>() throw();
};

#endif // #ifndef AUTO_ARRAY_H

// End of file auto_array.h
