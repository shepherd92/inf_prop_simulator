#include <cassert>
#include <cstdint>
#include <iomanip>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "types.h"

#include "config.h"
#include "main.h"

#include "event_queue.h"
#include "logger.h"
#include "network_builder.h"


int main(int argc, char **argv) {

  using namespace simulator;

  // the only argument should be the config file name
  assert(2 == argc);
  assert(nullptr != argv[1]);

  const std::string configFilename(argv[1]);

  config configuration{};
  configuration.parse(configFilename);

  const auto logFilename(configuration.get_value("SIMULATION", "log_file"));
  const level logLevel = logger::string_to_level(
      configuration.get_value("SIMULATION", "log_level"));

  initialize_logger(logFilename, logLevel);

  std::pair<uint32_t, std::mutex> numOfRemainingSimulations;
  numOfRemainingSimulations.first =
      std::stoi(configuration.get_value("SIMULATION", "num_of_simulations"));
  result_list resultList{};

  // determine number of threads
#ifndef DBG
  const uint32_t numOfThreads = std::thread::hardware_concurrency();
#else
  const uint32_t numOfThreads = 1U;
#endif
  LOG(INFO, "Number of worker threads: ", numOfThreads);

  if (numOfThreads > 1) {
    // start threads
    LOG(INFO, "Starting threads...");
    std::vector<std::thread> workers;
    for (uint32_t threadId = 0U; threadId < numOfThreads; ++threadId) {
      LOG(DEBUG, "Starting thread (ID: ", threadId, ")");
      std::thread worker(
          job, std::ref(configuration), std::ref(numOfRemainingSimulations),
          std::ref(resultList));
      workers.push_back(std::move(worker));
    }
    LOG(INFO, "Threads started.");

    // join threads
    for (auto &worker : workers) {
      worker.join();
    }
    LOG(INFO, "Threads finished.");
  } else {
    job(std::ref(configuration), std::ref(numOfRemainingSimulations),
        std::ref(resultList));
  }

  save(resultList, configuration.get_value("SIMULATION", "results_file"));

  LOG(INFO, "Simulator finished.");

  logger::close();
  return 0;
}

namespace simulator {

void initialize_logger(const std::string &logFilename, const level logLevel) {
  logger::set_log_file(logFilename);
  logger::set_log_level(logLevel);
}

void job(const config &configuration,
         std::pair<uint32_t, std::mutex> &numOfRemainingSimulations,
         result_list &resultList) {

  while (true) {

    {
      // handling number of remaining jobs
      const std::lock_guard<std::mutex> lock(numOfRemainingSimulations.second);
      if (0U == numOfRemainingSimulations.first) {
        return;
      } else {
        --numOfRemainingSimulations.first;
        LOG(INFO, "Number of remaining simulations: ",
            numOfRemainingSimulations.first);
      }
    }

    // simulate
    const auto network = build_network(configuration);
    const uint32_t numOfInitiallyInformedNodes = std::stoi(configuration.get_value("NETWORK","initially_informed"));
    LOG(DEBUG, "Number of initially informed nodes is ", numOfInitiallyInformedNodes);
    assert(numOfInitiallyInformedNodes < std::stoul(configuration.get_value("NETWORK", "num_of_nodes")));
    const auto eventQueue = initialize_propagation(network, numOfInitiallyInformedNodes);
    run_simulation(network, eventQueue);

    {
      // save result
      const std::lock_guard<std::mutex> lock(resultList.second);
      resultList.first.push_back(network->get_result());
    }
  }
}

void save(const result_list &resultList, const std::string &resultFilename) {
  LOG(INFO, "Saving results...");

  std::ofstream outputStream(resultFilename, std::fstream::out);
  outputStream << std::fixed;
  outputStream << std::setprecision(4);

  assert(outputStream.is_open());

  // create header
  outputStream << "#SimID" << '\t' << "Time" << '\t' << "Degree";

  // iterate over simulations, a simulation is a row
  for (uint32_t simulationID = 0U; simulationID < resultList.first.size();
       ++simulationID) {
    LOG(INFO, "Saving results of simulation with ID ", simulationID);
    // iterate over records in result
    for (const auto &record : *resultList.first[simulationID]) {
      outputStream << std::endl << simulationID << '\t' << record;
    }
  }

  outputStream.flush();
  outputStream.close();
  LOG(INFO, "Saving finished...");
}

network_properties create_network_properties(const config &configuration) {
  network_properties networkProperties;

  networkProperties.mNumOfNodes =
      std::stoi(configuration.get_value("NETWORK", "num_of_nodes"));
  networkProperties.mTransmissibility = std::stod(configuration.get_value("NETWORK", "transmissibility"));

  if ("power_law" == configuration.get_value("DEGREE_DISTRIBUTION", "type")) {
    networkProperties.mDegreeDistributionType =
        degree_distribution_type::power_law;
    networkProperties.mDegreeDistributionPower =
        std::stod(configuration.get_value("DEGREE_DISTRIBUTION", "power"));
  } else if ("uniform" ==
             configuration.get_value("DEGREE_DISTRIBUTION", "type")) {
    networkProperties.mDegreeDistributionType =
        degree_distribution_type::uniform;
  } else {
    // unknown degree distribution type
    assert(false);
    networkProperties.mDegreeDistributionType =
        degree_distribution_type::invalid;
  }

  networkProperties.mDegreeDistributionRange = degree_distribution_range{
      static_cast<degree>(std::stoul(
          configuration.get_value("DEGREE_DISTRIBUTION", "range_min"))),
      static_cast<degree>(std::stoul(
          configuration.get_value("DEGREE_DISTRIBUTION", "range_max")))};

  return networkProperties;
}

std::unique_ptr<int_network> build_network(const config &configuration) {
  const auto networkProperties = create_network_properties(configuration);
  LOG(DEBUG, "Network builder is initialized with ",
      networkProperties.mNumOfNodes, " nodes.");
  std::unique_ptr<int_network_builder> networkBuilder =
      std::make_unique<network_builder>();
  auto network = networkBuilder->construct(networkProperties);
  return network;
}

std::unique_ptr<int_event_queue>
initialize_propagation(const std::unique_ptr<int_network> &network, const uint32_t numOfInitiallyInformedNodes) {

  constexpr time now{0.0F};

  std::unique_ptr<int_event_queue> eventQueue =
      std::make_unique<event_queue>(now);

  for(uint32_t i = 0U; i < numOfInitiallyInformedNodes; ++i) {
     event_list initialEvents = network->inject_information_to_random_node(
         eventQueue->get_current_time());
     eventQueue->update(std::move(initialEvents));
  }

  return eventQueue;
}

void run_simulation(const std::unique_ptr<int_network> &network,
               const std::unique_ptr<int_event_queue> &eventQueue) {

  while (!eventQueue->is_empty()) {
    event nextEvent = eventQueue->next();
    if (!network->is_informed(nextEvent.mToNodeId)) {
      LOG(DEBUG, "Next event is from ", nextEvent.mFromNodeId, " to ",
          nextEvent.mToNodeId, " at time ", eventQueue->get_current_time());
      event_list newEvents = network->inform_node(
          nextEvent.mToNodeId, eventQueue->get_current_time());
      eventQueue->update(std::move(newEvents));
    }
  }
}

std::ostream &operator<<(std::ostream &output,
                         const result_record &resultRecord) {
  output << resultRecord.mInformationTime << '\t' << resultRecord.mDegree;
  return output;
}

} // namespace simulator
