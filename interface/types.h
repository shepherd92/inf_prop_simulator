#ifndef __TYPES_H__
#define __TYPES_H__

#include <cstdint>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

namespace simulator
{

class int_node;

typedef double time;

typedef uint32_t node_id;
typedef uint32_t degree;
typedef std::vector<std::unique_ptr<int_node>> int_node_list;

enum level : uint32_t
{
   EMERG,
   ALERT,
   CRIT,
   ERR,
   WARNING,
   NOTICE,
   INFO,
   DEBUG,
   INVALID
};

struct connection
{
   enum class type
   {
      incoming, outgoing
   };

   connection(const type t, const node_id n)
      : mType(t)
      , mToNodeId(n)
   {
   }

   type mType {type::outgoing};
   node_id mToNodeId {0U};
};

enum class degree_distribution_type : uint8_t
{
   constant,
   uniform,
   poisson,
   power_law,
   invalid = 255U
};

enum class connectivity_result : uint8_t
{
   everything_ok,
   dangling_connections
};

struct degree_distribution_range
{
   degree mMinimum;
   degree mMaximum;
};

// specifies network properties for the network builder
struct network_properties
{
   uint32_t mNumOfNodes {0U};
   double mCharacteristicTime {1.0};
   degree_distribution_type mDegreeDistributionType {
       degree_distribution_type::invalid};
   degree mKMin {1};
   double mDegreeDistributionParameter1 {0.0};
   double mDegreeDistributionParameter2 {0.0};
   bool mDanglingConnectionsOk {false};
   bool mLoopsOk {false};
};

struct event {
   event(const node_id from = 0U, const node_id to = 0U, const time t = 0.0)
      : mFromNodeId(from)
      , mToNodeId(to)
      , mTime(t)
   {
   }

   node_id mFromNodeId {0U};
   node_id mToNodeId {0U};
   time mTime {0.0};
};

typedef std::unique_ptr<std::vector<event>> event_list;

struct result_record
{
   result_record(const time t, const degree d)
      : mInformationTime(t)
      , mDegree(d)
   {
   }

   time mInformationTime {0.0};
   degree mDegree {0U};
};

typedef std::vector<result_record> result;

typedef std::pair<std::vector<std::unique_ptr<result>>, std::mutex> result_list;

} // namespace simulator

#endif
