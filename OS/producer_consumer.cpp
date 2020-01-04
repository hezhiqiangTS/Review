#include <iostream>
#include <array>
#include <unistd.h>
#include <random>
#include <pthread.h>
#include <sys/time.h>

#define BUFFER_SIZE 10

static std::array<int, BUFFER_SIZE> buf;
static size_t in, out, counter;

void* producer(void *){
    while(1){
        sleep(1);
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        std::default_random_engine r(tv.tv_sec);
        int product = r() % 51;
        std::cout << "[producer thread] product: "<< product << std::endl;
        while(counter == BUFFER_SIZE) sleep(2);
        buf[in] = product;
        in = (in + 1) % BUFFER_SIZE;
        counter += 1;
    }
}

void* consumer(void *){
    while(1){
        while(counter == 0) sleep(1);
        int product = buf[out];
        std::cout << "[consumer thread] product: " << product << std::endl;
        out = (out + 1) % BUFFER_SIZE;
        counter -= 1;
        //sleep(2);
    }
}
int main(){
    pthread_t pro, con;
    pthread_create(&pro, nullptr, &producer, nullptr);
    pthread_create(&con, nullptr, &consumer, nullptr);
    pthread_detach(pro);
    pthread_detach(con);
    while(1);
}
