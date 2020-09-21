#include <cassert>
#include <random>

#include "node.h"
#include "types.h"

// undefine LOG macro to not do anything
#undef LOG
#define LOG(level, ...) ;

namespace simulator
{

bool essentially_equal(const double a, const double b)
{
   return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) *
                          std::numeric_limits<double>::epsilon());
}

void test_constructor()
{
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   node testObj(node_id {3}, 1.5, randomNumberGenerator);
   assert(testObj.get_degree() == 0);
   assert(testObj.get_node_id() == 3);
   assert(!testObj.is_informed());
}

void test_add_connection()
{
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   node testObj(node_id {1U}, 1.5, randomNumberGenerator);

   testObj.add_connection(
      connection {connection::type{connection::type::outgoing}, node_id{2U}});
   testObj.add_connection(
      connection {connection::type{connection::type::incoming}, node_id{6U}});
   testObj.add_connection(
      connection {connection::type{connection::type::incoming}, node_id{7U}});

   assert(testObj.get_degree() == 3);
}

void test_is_connected_to()
{
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   node testObj(node_id {1}, 1.5, randomNumberGenerator);

   testObj.add_connection(
      connection {connection::type{connection::type::outgoing}, node_id{2U}});

   assert(!testObj.is_connected_to(node_id {1U}));
   assert(testObj.is_connected_to(node_id {2U}));
}

void test_inform()
{
   // create test object
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   node testObj(node_id {1}, 1.5, randomNumberGenerator);
   testObj.add_connection(
      connection {connection::type{connection::type::outgoing}, node_id{2U}});
   testObj.add_connection(
      connection {connection::type{connection::type::incoming}, node_id{6U}});
   testObj.add_connection(
      connection {connection::type{connection::type::incoming}, node_id{7U}});

   // call tested method
   assert(!testObj.is_informed());
   event_list newEventList = testObj.inform(time {4.7});
   assert(testObj.is_informed());

   // test information time
   const auto informationTime = testObj.get_information_time();
   assert(essentially_equal(informationTime, 4.7));

   // test new events
   assert(newEventList->size() == 3);
   assert(newEventList->at(0).mTime > 4.7);
   assert(newEventList->at(1).mTime > 4.7);
   assert(newEventList->at(2).mTime > 4.7);
}

void test_calc_sending_time()
{
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   node testObj(node_id {1U}, 1.0, randomNumberGenerator);

   testObj.inform(time {0.0});  // set mInformationTime to 0

   double sumOfMinimumSendingTimes {0.0};
   for(uint32_t j {0U}; j < 100000U; ++j)
   {
      double minimumSendingTime {std::numeric_limits<double>::infinity()};
      for(uint32_t i {0U}; i < 5U; ++i)
      {
         minimumSendingTime = std::min(minimumSendingTime, testObj.calc_sending_time());
      }
      sumOfMinimumSendingTimes += minimumSendingTime;
   }

   const double averageOfMinimumTimes {sumOfMinimumSendingTimes / 100000.0};
   constexpr double epsilon {0.01};

   assert(averageOfMinimumTimes > 0.2 - epsilon && "Generating too low sending times!");
   assert(averageOfMinimumTimes < 0.2 + epsilon && "Generating too high sending times!");
}

} // namespace simulator

int main()
{
   using namespace simulator;

   test_constructor();
   test_add_connection();
   test_is_connected_to();
   test_inform();
   test_calc_sending_time();

   return 0;
}
