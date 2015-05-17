#include <stdlib.h>
#include "qh_vector.h"

typedef qh::vector<int>::iterator qh_itr;
typedef qh::vector<int>::const_iterator qh_const_itr;

//��ֵ����
template<class T>
void setValue(size_t size,qh::vector<T> &data)
{
	for(size_t i=1;i<=size;++i)
		data.push_back(i);
}

//���Թ��캯������ֵ���������������캯��������������������[],�Լ�push_back��size��Ա����
void test1()
{
	//���Թ��캯��
	qh::vector<int> num_vect,another_vect;
	qh::vector<int> third_vect(3,10);

	num_vect.push_back(1);
	setValue(3,another_vect);
	assert(num_vect[0] == 1);
	for(size_t i = 0;i < third_vect.size(); ++i)
		assert(third_vect[i] == 10);

	//���鸳ֵ�����	
	num_vect = another_vect;
	for(size_t i=0; i < num_vect.size(); ++i)
		assert(num_vect[i] == another_vect[i]);

	//�������캯��,���������仰ʱ���벻ͨ�������Կ������캯������
//	qh::vector<int> copy_vect = another_vect;

	//������������˳���������������
	std::cout << "test1��������" << std::endl;
}

//����һ�µ�����,begin,end������
void test2()
{
	qh::vector<int> num_vect;
	setValue(3,num_vect);

	//begin����
	qh_itr itr = num_vect.begin();
	assert(*itr++ == 1);
	assert(*itr == 2);

	//end����
	qh_itr itr_end = num_vect.end();
	assert(*(itr_end-1) == 3);

	//const begin����
	qh_const_itr itr_con_begin = num_vect.begin();
	assert(*itr_con_begin == 1);

	//�����ĵ���
	qh_itr itr_begin = num_vect.begin();
	for(int i=0;itr_begin != num_vect.end();++itr_begin)
		assert(*itr_begin == num_vect[i++]);

	std::cout << "test2��������" << std::endl;
}

//����back��front��pop_back��capacity�Ⱥ���
void test3()
{
	qh::vector<int> num_vect;
	setValue(3,num_vect);

	num_vect.pop_back();
	assert(num_vect.size() == 2);
	assert(num_vect.back() == 2);
	assert(num_vect.size() <= num_vect.capacity());

	//front����
	assert(num_vect.front() == 1);
	num_vect.front() += 4;
	assert(num_vect.front() == 5);
}

void test4()
{

}

int main(int argc, char* argv[])
{
    //TODO ��������ӵ�Ԫ���ԣ�Խ��Խ�ã�����·��������ԽȫԽ��
    //TODO ��Ԫ����д����ο�INIParser�Ǹ���Ŀ����Ҫдһ��printf��Ҫ��assert���ж����жϡ�

    test1();
	test2();
	test3();
	test4();
	
#ifdef WIN32
    system("pause");
#endif

	return 0;
}

