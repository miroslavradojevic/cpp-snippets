#include <iostream>
#include <chrono>

int main(int argc, char **argv)
{
    // measure execution time
    std::chrono::time_point<std::chrono::system_clock> t0, t1;
    
    t0 = std::chrono::system_clock::now();
    
    int N = 1000000;
    
    for (int i = 0; i < N; i++) {
            std::cout << i << "/" << N << std::endl;
    }
	
    t1 = std::chrono::system_clock::now();
    
    std::chrono::duration<float> elapsed_seconds = t1 - t0;
    
    float elapsed_millis = elapsed_seconds.count() * 1000;
    
    std::cout << "Elapsed " << elapsed_seconds.count() << "s, " << elapsed_millis << "ms" << std::endl;
    
	return 0;
}
