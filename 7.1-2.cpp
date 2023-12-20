// 7.1-2.cpp 


#include <iostream>
#include<thread>
#include<chrono>
#include<vector>
#include <random>
#include <algorithm>
#include <execution>

std::mt19937 gen;
std::uniform_int_distribution<int> dist(0, 10000);

std::once_flag flag;

void show_cores() {
	std::cout << "Count of cores: " << std::thread::hardware_concurrency() << std::endl;
}

void fill_vector(std::vector<int>& v) {
	std::generate(std::execution::par, v.begin(), v.end(), [&]() {
		return dist(gen);
		});

}

void sep_sort(std::vector<int> v, int b, int e) {
	std::sort(v.begin() + b, v.end() - e);
}

auto sort1core(std::vector<int> v) {
	std::call_once(flag, show_cores);

	auto start = std::chrono::steady_clock::now();
	std::sort(v.begin(), v.end());
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

auto sort2core(std::vector<int> v) {
	auto start = std::chrono::steady_clock::now();

	std::thread t1(sep_sort, v, 0, v.size() / 2);
	std::thread t2(sep_sort, v, v.size() / 2, 0);
	t1.join();
	t2.join();

	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

auto sort4core(std::vector<int> v) {
	auto start = std::chrono::steady_clock::now();

	std::thread t1(sep_sort, v, 0, 3 * (v.size() / 4));
	std::thread t2(sep_sort, v, 1 * (v.size() / 4), 2 * (v.size() / 4));
	std::thread t3(sep_sort, v, 2 * (v.size() / 4), 1 * (v.size() / 4));
	std::thread t4(sep_sort, v, 3 * (v.size() / 4), 0);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

auto sort8core(std::vector<int> v) {
	auto start = std::chrono::steady_clock::now();

	std::thread t1(sep_sort, v, 0, 7 * (v.size() / 8));
	std::thread t2(sep_sort, v, 1 * (v.size() / 8), 6 * (v.size() / 8));
	std::thread t3(sep_sort, v, 2 * (v.size() / 8), 5 * (v.size() / 8));
	std::thread t4(sep_sort, v, 3 * (v.size() / 8), 4 * (v.size() / 8));
	std::thread t5(sep_sort, v, 4 * (v.size() / 8), 3 * (v.size() / 8));
	std::thread t6(sep_sort, v, 5 * (v.size() / 8), 2 * (v.size() / 8));
	std::thread t7(sep_sort, v, 6 * (v.size() / 8), 1 * (v.size() / 8));
	std::thread t8(sep_sort, v, 7 * (v.size() / 8), 0);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();

	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

auto sort16core(std::vector<int> v) {
	auto start = std::chrono::steady_clock::now();

	int s = 16;
	std::thread* t = new std::thread[s];
	for (int i = 0; i < s; i++)
	{
		t[i] = std::thread(sep_sort, v, i * (v.size() / 16), (15 - i) * (v.size() / 16));

	}
	for (int i = 0; i < s; i++)
	{
		t[i].join();
	}

	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

int main()
{
	std::vector<int> v1(1000);
	std::vector<int> v2(10000);
	std::vector<int> v3(100000);
	std::vector<int> v4(1000000);

	fill_vector(v1);
	fill_vector(v2);
	fill_vector(v3);
	fill_vector(v4);

	sort1core(v1).count();

	std::cout << "\t\t 1000 \t 10000 \t 100000 \t 1000000\n";

	std::cout << "1 thread:\t" << sort1core(v1).count() << "\t" << sort1core(v2).count() << "\t" << sort1core(v3).count() << "\t\t" << sort1core(v4).count() << "\n";
	std::cout << "2 threads:\t" << sort2core(v1).count() << "\t" << sort2core(v2).count() << "\t" << sort2core(v3).count() << "\t\t" << sort2core(v4).count() << "\n";
	std::cout << "4 threads:\t" << sort4core(v1).count() << "\t" << sort4core(v2).count() << "\t" << sort4core(v3).count() << "\t\t" << sort4core(v4).count() << "\n";
	std::cout << "8 threads:\t" << sort8core(v1).count() << "\t" << sort8core(v2).count() << "\t" << sort8core(v3).count() << "\t\t" << sort8core(v4).count() << "\n";
	std::cout << "16 threads:\t" << sort16core(v1).count() << "\t" << sort16core(v2).count() << "\t" << sort16core(v3).count() << "\t\t" << sort16core(v4).count() << "\n";


	return 0;
}
