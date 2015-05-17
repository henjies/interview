#include <stdio.h>
#include <assert.h>
#include <cstdlib>

#define H_ARRAYSIZE(a) \
	((sizeof(a)/sizeof(*(a)))/ \
	static_cast<size_t>(!(sizeof(a)%sizeof(*(a)))))

int initParam(const char* input,int step[],int flag[])
{
	int length = 0;
	int line = *input++ - '0';
	input++;

	for(int i=0;i<line;++i)
	{
		int start=0,end=0,height=0;
		while(*input != '\n' && *input != '\0')
		{
			start = *input++ - '0';
			input++;
			end = *input++ - '0';
			input++;
			height = *input++ - '0';
			//input++;
		}
		length = length > end ? length : end;
		++input;
		for(int j=start;j<=end;++j)
		{
			step[j] = step[j] > height ? step[j] : height;
			if(j > start)//标识和后面的点是否相连接
				flag[j-1] = 1;
		}
	}
	
	//printf("length:%d\n",length);
	return length;
}

int resolve(const char* input)
{
	if(input == NULL)
		return 0;

	int sum = 0;                 //函数返回值
	int maxLength = 0;           //指示路的长度
	int step[200] = {0};         //存储对应点的最大值
	int flag[200] = {0};         //标记是否和前面一个连起来
	
	maxLength = initParam(input,step,flag);

	for(int i=1;i<=maxLength;++i)
	{
		if (flag[i-1] == 1)
			sum += abs(step[i] - step[i-1]) + 1;
		else
			sum += abs(step[i] + step[i-1]) + 1;
	}
	sum += step[0];
	sum += step[maxLength] != 0 ? step[maxLength] : 0;

	//printf("sum:%d\n",sum);
	return sum;
}

int main(int argc,char* argv[])
{
	const char* input[] = {
		"3\n1,3,2\n2,4,4\n6,7,5\n",
		"1\n1,2,1\n",
		"2\n1,2,1\n2,3,2",
		"3\n1,2,1\n2,3,2\n3,6,1\n",
		"4\n1,2,1\n2,3,2\n3,6,1\n5,8,2",
		"5\n1,2,1\n2,3,2\n3,6,1\n5,8,2\n7,9,1",
		"1\n0,1,1",
		"2\n0,1,1\n2,4,3",
		"3\n0,1,1\n2,4,3\n3,5,1",
		"4\n0,1,1\n2,4,3\n3,5,1\n5,6,1",
		"5\n0,1,1\n2,4,3\n3,5,1\n5,6,1\n6,8,3",
		"2\n0,3,1\n2,5,1",
		"3\n1,2,1\n3,4,1\n5,6,1",
		"3\n1,2,1\n2,3,2\n3,4,3",
	};

	int expectedSteps[] = {25,4,7,10,14,15,3,12,13,14,20,7,12,10};

	for(size_t i=0;i<H_ARRAYSIZE(input);++i)
	{
		assert(resolve(input[i]) == expectedSteps[i]);
	}

	return 0;
}
