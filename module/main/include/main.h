#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "types.h"

int main(int argc, char **argv);

namespace simulator {

class int_network;
class int_event_queue;

void initialize_logger(const std::string &logFilename, const level logLevel);
void job(const config &configuration,
         std::pair<uint32_t, std::mutex> &numOfRemainingSimulations,
         result_list &resultList);
void save(const result_list &resultList, const std::string &resultFilename);

network_properties create_network_properties(const config &configuration);
std::unique_ptr<int_network> build_network(const config &configuration);
std::unique_ptr<int_event_queue>
initialize_propagation(const std::unique_ptr<int_network> &network, const uint32_t numOfInitiallyInformedNodes);
void run_simulation(const std::unique_ptr<int_network> &network,
                    const std::unique_ptr<int_event_queue> &eventQueue);

std::ostream &operator<<(std::ostream &output,
                         const result_record &resultRecord);

} // namespace simulator

#endif

