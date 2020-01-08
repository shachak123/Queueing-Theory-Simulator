#include <iostream>
#include <random>
#include <queue>
#include <chrono>
using namespace std;

double generateRandomNumber() {
    long long seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator (seed);
    std::uniform_real_distribution<double> distribution(0, 1);
    return distribution(generator);
}

double min(double x, double y){
    return x > y ? y : x;
}

int max(int x, int y){
    return x > y ? x : y;
}

class Customer{
public:
    double arrival_time = 0; // time Customer entered the queue
    double service_start = 0; // time customer started getting served
    double service_end = 0; // time customer finished getting served, exiting the queue
    Customer(double a_t){
        arrival_time = a_t;
    }
};

int main(int argc, char** argv) {
    double T = stod(argv[1]);
    double lambda = stod(argv[2]);
    double mu = stod(argv[3]);
    int probabilities_size = argc - 4;
    std::vector<double> probabilities(probabilities_size);
    for (int i = 0; i < probabilities_size; i++) {
        probabilities[i] = (stod(argv[i + 4]));
    }
    
    int Y = 0, X = 0;
    double T_tag = -1, total_waiting_time = 0, total_service_time = 0;
    std::vector<double> Ti(probabilities_size), Zi(probabilities_size);
    double Tw_roof = 0, Ts_roof = 0, lambdaA_roof = 0;
    std::deque<Customer> arrival_list;
    std::deque<double> service_list; // event list for both types of events
    int current_waiting = 0;
    
    double time = (-log(generateRandomNumber()) / (lambda));
    arrival_list.emplace_back(Customer(time));
    Ti[0] += arrival_list.front().arrival_time;
    
    /* when to randomize arrival? at the very beginning and every time we take care of arrival
       when to randomize service? every time we take care of arrival and there is no service in
       the future or every time we take care of a service and there is another arrival except
       for the one the last service took care of. */
    while (time < T) {
        if (arrival_list[arrival_list.size() - 1].arrival_time == time) { // in case of arrival event
            if (probabilities[arrival_list.size() - 1] >= generateRandomNumber()) { // may actually enter
                if (service_list.empty()) {
                    service_list.emplace_back(time + (-log(generateRandomNumber()) / mu));
                    T_tag = service_list.front();
                    arrival_list[0].service_start = time;
                }
                arrival_list.emplace_back(Customer(time + (-log(generateRandomNumber()) / (lambda))));
            } 
            else { // should not enter, erase last from list
                if (service_list.empty()) {
                    service_list.emplace_back(time + (-log(generateRandomNumber()) / mu));
                    T_tag = service_list.front();
                    arrival_list[0].service_start = time;
                }
                arrival_list.erase(arrival_list.begin() + arrival_list.size() - 1);
                X++;
                arrival_list.emplace_back(Customer(time + (-log(generateRandomNumber()) / (lambda))));
                // add a new arrival to last place
            }
            current_waiting = arrival_list.size() - 1;
            Ti[current_waiting] += min(service_list.front(), arrival_list[arrival_list.size() - 1].arrival_time) - time;
        }
        else { // in case of service event
            service_list.pop_front(); // remove first element in queue
            total_waiting_time += arrival_list.front().service_start - arrival_list.front().arrival_time;
            total_service_time += time - arrival_list.front().service_start;
            arrival_list.pop_front(); // remove first element in queue
            Y++;
            current_waiting = max(arrival_list.size() - 1, 0);
            if (arrival_list.size() > 1) {
                service_list.emplace_back(time + (-log(generateRandomNumber()) / mu));
                T_tag = service_list.front();
                arrival_list[0].service_start = time;
                Ti[current_waiting] += min(service_list.front(), arrival_list[arrival_list.size() - 1].arrival_time)
                                       - time;
            }
            else {
                Ti[current_waiting] += arrival_list[arrival_list.size() - 1].arrival_time - time;
                time = arrival_list[arrival_list.size() - 1].arrival_time;
                continue;
            }
        }
        
        time = min(service_list.front(), arrival_list[arrival_list.size() - 1].arrival_time);
    }
    
    // time over, take care of elements students - if element arrival is greater than T, dismiss
    if (!arrival_list.empty()) {
        if (arrival_list[arrival_list.size() - 1].arrival_time >= T) {
            if (arrival_list.size() == 1) {
                cout << Y << " " << X << " " << T_tag << " ";
                for (int j = 0; j < probabilities_size; ++j) {
                    cout << Ti[j] << " ";
                }
                for (int k = 0; k < probabilities_size; ++k) {
                    cout << Ti[k] / T_tag << " ";
                }
                
                Tw_roof = total_waiting_time / Y;
                Ts_roof = total_service_time / Y;
                lambdaA_roof = Y / T;
                
                cout << Tw_roof << " ";
                cout << Ts_roof << " ";
                cout << lambdaA_roof << " ";
                
                return 0;
            }
            else
                arrival_list.erase(arrival_list.begin() + arrival_list.size() - 1);
        }
        current_waiting = arrival_list.size();
        if (service_list.empty()) {
            service_list.emplace_back(time + (-log(generateRandomNumber()) / mu));
            T_tag = service_list.front();
            arrival_list[0].service_start = service_list.front();
        }
        Ti[current_waiting] += service_list.front() - time;
        
        time = service_list.front();
        while (!arrival_list.empty()) {
            service_list.pop_front();
            total_waiting_time += arrival_list.front().service_start - arrival_list.front().arrival_time;
            total_service_time += time - arrival_list.front().service_start;
            arrival_list.pop_front();
            Y++;
            if (arrival_list.empty()) {
                break;
            }
            current_waiting = arrival_list.size();
            service_list.emplace_back(time + (-log(generateRandomNumber()) / mu));
            T_tag = service_list.front();
            arrival_list[0].service_start = service_list.front();
            Ti[current_waiting] += service_list.front() - time;
            
            time = service_list.front();
        }
    }
    cout << Y << " " << X << " " << T_tag << " ";
    for (int j = 0; j < probabilities_size; ++j) {
        cout << Ti[j] << " ";
    }
    for (int k = 0; k < probabilities_size; ++k) {
        cout << Ti[k] / T_tag << " ";
    }
    
    Tw_roof = total_waiting_time / Y;
    Ts_roof = total_service_time / Y;
    lambdaA_roof = Y / T;
    
    cout << Tw_roof << " ";
    cout << Ts_roof << " ";
    cout << lambdaA_roof << " ";
    return 0;
}
