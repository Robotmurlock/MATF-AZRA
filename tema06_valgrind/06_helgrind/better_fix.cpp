#include <iostream>
#include <vector>
#include <thread>
#include <utility>
#include <algorithm>
#include <mutex>

#define NUM_OF_THREADS 10

std::mutex mutex;

void vsum(int id, const std::vector<int>& vec, int start, int end, int *result)
{
    mutex.lock();
    std::cout << "[Thread " << id << "] Started!" << std::endl;
    mutex.unlock();

    int sum = 0;
    for(int i=start; i<end; i++)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        sum += vec[i];
    }

    mutex.lock();
    *result += sum;
    std::cout << "[Thread " << id << "] Finished!" << std::endl;
    mutex.unlock();
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<int> vec(n);
    for(int i=0; i<n; i++)
        std::cin >> vec[i];

    int batch_size = n/NUM_OF_THREADS;

    std::vector<std::pair<int, int> > batches(NUM_OF_THREADS);
    for(int i=0; i<NUM_OF_THREADS; i++)
        batches[i] = std::make_pair(i*batch_size, std::min(n, (i+1)*batch_size));

    int result = 0;
    std::vector<std::thread> threads;
    for(int i=0; i<NUM_OF_THREADS; i++)
        threads.push_back(std::thread(vsum, i, vec, batches[i].first, batches[i].second, &result));

    for(int i=0; i<NUM_OF_THREADS; i++)
        threads[i].join();

    std::cout << "Result is: " << result << std::endl;

    return 0;
}