#ifndef QIHOO_VECTOR_H_
#define QIHOO_VECTOR_H_

#include <cassert>
#include <iostream>

namespace qh
{
    template<class T>
    class vector {
	private:
		//把拷贝构造函数声明为私有
		vector(const vector<T> &);

    public:
//#define private public  //仅仅为了测试用

		//迭代器的一些类型定义
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;

        //ctor
        vector(): data_(NULL), size_(0),CapacityEnd(0)
        {
        }

        explicit vector( size_t n, const T& value = T())
        {
			assert(n > 0);
            data_ = new T[2*n];
            for (size_t i = 0; i < n; i++)
            {
                data_[i] = value;
            }
			size_ = n;
			CapacityEnd = 2*n;
        }

        //dtor
        ~vector()
        {
            if (data_)
            {
                delete [] data_;
            }
			data_ = NULL;
			size_ = CapacityEnd = 0;
        }

        //get
        size_t size() const
        {
            return size_;
        }
		
		size_t capacity() const
		{
			return CapacityEnd;
		}
        // set & get
        T& operator[](size_t index);
		const T& operator[](size_t index) const;
        vector<T>& operator=(const vector<T>& rhs);

		const T& front() const;
		T& front();
		T back();

        // set
        void push_back(const T& element);
        void pop_back();
        void resize(size_t num,const T& element=T());
        void reserve(size_t num);
        void clear();
        bool empty()const;

		//insert & erase
		iterator insert(iterator position, const T &val)
		{
			size_t diff = position-data_;
			if (diff > size_ + 1 || diff < 0)
			{
				std::cout << "insert下标无效！" << std::endl;
				assert(false);
			}
			return insert(diff+1,val);
		}

		iterator insert(size_t position,const T &val)
		{
			if (position > size_ + 1 || position < 0)
			{
				std::cout << "insert下标无效！" << std::endl;
				assert(false);
			}
			if (CapacityEnd == 0)
				reAllocateMemory(2);
			if (size_ >= CapacityEnd)
				reAllocateMemory(2*CapacityEnd);
			BackCopyMemory(position);
			data_[position-1] = val;
			++size_;

			return data_ + position;
		}

		iterator erase(size_t position)
		{
			if (position < 0 || position > size_ || size_ <= 0)
			{
				std::cout << "erase 下标无效！" << std::endl;
				assert(false);
			}
			ForwardCopyMemory(position);
			--size_;

			return data_ + position - 1;
		}

		iterator erase(iterator position)
		{
			size_t diff = position - data_ + 1;

			return erase(diff);
		}

		//iterator
		iterator begin() 
		{
			return data_;
		};
		const_iterator begin() const
		{
			return data_;
		}
		iterator end() 
		{
			return data_+size_;
		}
		const_iterator end() const
		{
			return data_+size_;
		}

	private:
		//已经分配的内存不足容纳现有的元素时，重新分配空间
		void reAllocateMemory(size_t num)
		{
			T* tempDataPtr = NULL;
			tempDataPtr = new T[num];
			for(size_t i=0; i < size_; ++i)
			{
				tempDataPtr[i] = data_[i];
			}
			if (data_ != NULL)
				delete [] data_;
			data_ = tempDataPtr;
			CapacityEnd = num;
		}

		//插入的时候，从插入点往后移一位
		void BackCopyMemory(size_t start)
		{
			for(size_t i=size_;i >= start;--i)
				data_[i] = data_[i-1];
		}
		//删除的时候，从删除点往前移一位
		void ForwardCopyMemory(size_t start)
		{
			for(size_t i=start; i < size_;++i)
				data_[i-1] = data_[i];
		}
			
    private:
        T*      data_;			//存储的内存的地址，也是首元素指针
        size_t  size_;			//当前已经使用分配的内存
		size_t  CapacityEnd;	//分配时，分配的最大内存，容量和实际元素个数是2倍关系
    };

	//一些实现函数
	template<class T>
	T& vector<T>::operator[](size_t index)
	{
		if(index < 0 || index > size_)
		{
			std::cout << "index 越界！" << std::endl;
			assert(false);
		}
		
		return data_[index];
	}

	template<class T>
	const T& vector<T>::operator[](size_t index) const
	{
		if(index < 0 || index > size_)
		{
			std::cout << "index 越界！" << std::endl;
			assert(false);
		}

		return data_[index];
	}
	
	template<class T>
	vector<T>& vector<T>::operator=(const vector<T>& rhs)
    {
         if (this == &rhs)//自己给自己赋值，直接跳过
			return *this;

		 if (data_ != NULL)
		 {
			delete [] data_;
			data_ = NULL;
		 }
		 size_ = rhs.size();
		 CapacityEnd = size_;
		 if (size_ > 0)
			data_ = new T[size_];//进行深度复制，防止内存泄露
		for(size_t i=0; i < size_; ++i)
			data_[i] = rhs[i];

		return *this;
    }

	template<class T>
	const T& vector<T>::front() const
	{
		if (size_ < 1)
		{
			std::cout << "vector容器为空！" << std::endl;
			assert(false);
		}
		return data_[0];
	}

	template<class T>
	T& vector<T>::front()
	{
		return const_cast<T&>(static_cast< const vector<T>& >(*this)[0]);
	}

	template<class T>
	T vector<T>::back() 
	{
		if (size_ < 1)
		{
			std::cout << "vector容器为空！" << std::endl;
			assert(false);
		}
		return data_[size_-1];
	}

	template<class T>
	void vector<T>::push_back(const T& element)
	{
		if (CapacityEnd == 0)
		{
			reAllocateMemory(2);
		}
		if (size_ >= CapacityEnd)
		{
			reAllocateMemory(2*CapacityEnd);
		}
		data_[size_] = element;
		++size_;
	}
	
	template<class T>
	void vector<T>::pop_back()
	{
		if (size_ <= 0)
		{
			std::cout << "vector容器为空！" << std::endl;
			assert(false);
		}
		size_--;
	}

	template<class T>
	void vector<T>::resize(size_t num,const T& element)
	{
		if(num < 0)
		{
			std::cout << "num参数输入错误！num：" << num << std::endl;
			assert(false);
		}
		if (num <= size_)
			return;
		if (CapacityEnd == 0)
			reAllocateMemory(2);
		while (num >= CapacityEnd)//判断是否需要重新分配内存
		{
			reAllocateMemory(2*CapacityEnd);
		}
		for(size_t i=size_; i < num; ++i)
		{
			data_[i] = element;
		}
		size_ = num;
	}

	template<class T>
	void vector<T>::reserve(size_t num )
	{
		if (num <= CapacityEnd)
			return;
		else
			reAllocateMemory(num);
	}

	template<class T>
	void vector<T>::clear()
	{
		delete data_;
		data_ = NULL;
		size_ = 0;
		CapacityEnd = 0;
	}

	template<class T>
	bool vector<T>::empty() const
	{
		return size_ == 0;
	}

}

#endif
