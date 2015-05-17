#include <stdlib.h>
#include "qh_vector.h"

typedef qh::vector<int>::iterator qh_itr;
typedef qh::vector<int>::const_iterator qh_const_itr;

//赋值函数
template<class T>
void setValue(size_t size,qh::vector<T> &data)
{
	for(size_t i=1;i<=size;++i)
		data.push_back(i);
}

//测试构造函数，赋值操作符，拷贝构造函数，析构函数，操作符[],以及push_back，size成员函数
void test1()
{
	//测试构造函数
	qh::vector<int> num_vect,another_vect;
	qh::vector<int> third_vect(3,10);

	num_vect.push_back(1);
	setValue(3,another_vect);
	assert(num_vect[0] == 1);
	for(size_t i = 0;i < third_vect.size(); ++i)
		assert(third_vect[i] == 10);

	//检验赋值运算符	
	num_vect = another_vect;
	for(size_t i=0; i < num_vect.size(); ++i)
		assert(num_vect[i] == another_vect[i]);

	//拷贝构造函数,添加下面这句话时编译不通过，所以拷贝构造函数正常
//	qh::vector<int> copy_vect = another_vect;

	//这个函数正常退出，析构函数正常
	std::cout << "test1函数正常" << std::endl;
}

//测试一下迭代器,begin,end函数族
void test2()
{
	qh::vector<int> num_vect;
	setValue(3,num_vect);

	//begin函数
	qh_itr itr = num_vect.begin();
	assert(*itr++ == 1);
	assert(*itr == 2);

	//end函数
	qh_itr itr_end = num_vect.end();
	assert(*(itr_end-1) == 3);

	//const begin函数
	qh_const_itr itr_con_begin = num_vect.begin();
	assert(*itr_con_begin == 1);

	//正常的迭代
	qh_itr itr_begin = num_vect.begin();
	for(int i=0;itr_begin != num_vect.end();++itr_begin)
		assert(*itr_begin == num_vect[i++]);

	std::cout << "test2函数正常" << std::endl;
}

//测试back，front，pop_back，capacity等函数
void test3()
{
	qh::vector<int> num_vect;
	setValue(3,num_vect);

	num_vect.pop_back();
	assert(num_vect.size() == 2);
	assert(num_vect.back() == 2);
	assert(num_vect.size() <= num_vect.capacity());

	//front函数
	assert(num_vect.front() == 1);
	num_vect.front() += 4;
	assert(num_vect.front() == 5);
}

void test4()
{

}

int main(int argc, char* argv[])
{
    //TODO 在这里添加单元测试，越多越好，代码路径覆盖率越全越好
    //TODO 单元测试写法请参考INIParser那个项目，不要写一堆printf，要用assert进行断言判断。

    test1();
	test2();
	test3();
	test4();
	
#ifdef WIN32
    system("pause");
#endif

	return 0;
}

