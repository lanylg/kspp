#include <iostream>
#include <string>
#include <chrono>
#include <regex>
#include <kspp/codecs/text_codec.h>
#include <kspp/topology_builder.h>
#include <kspp/processors/transform.h>
#include <kspp/processors/count.h>

int main(int argc, char **argv) {
  auto builder = csi::topology_builder<csi::text_codec>("localhost", "C:\\tmp");
  {
    auto sink = builder.create_kafka_sink<int, std::string>("kspp_example5_usernames");
    csi::produce(*sink, 1, "user_1");
    csi::produce(*sink, 2, "user_2");
    csi::produce(*sink, 3, "user_3");
    csi::produce(*sink, 4, "user_4");
    csi::produce(*sink, 5, "user_5");
    csi::produce(*sink, 6, "user_6");
    csi::produce(*sink, 7, "user_7");
    csi::produce(*sink, 8, "user_8");
    csi::produce(*sink, 9, "user_9");
    csi::produce(*sink, 10,"user_10");
  }
 
  {
    auto sink = builder.create_kafka_sink<int, int>("kspp_example5_user_channel"); // <user_id, channel_id>
    csi::produce(*sink, 1, 1);
    csi::produce(*sink, 2, 1);
    csi::produce(*sink, 3, 1);
    csi::produce(*sink, 4, 1);
    csi::produce(*sink, 5, 2);
    csi::produce(*sink, 6, 2);
    csi::produce(*sink, 7, 2);
    csi::produce(*sink, 8, 2);
    csi::produce(*sink, 9, 2);
    csi::produce(*sink, 10, 2);
  }

  {
    auto sink = builder.create_kafka_sink<int, std::string>("kspp_example5_channel_names");
    csi::produce(*sink, 1, "channel1");
    csi::produce(*sink, 2, "channel2");
  }
 
  {
    auto sources = builder.create_kafka_sources<int, std::string>("kspp_example5_usernames", 8);
    auto sink = builder.create_stream_sinks<int, std::string>(sources, std::cerr);
    for (auto s : sources) {
      std::cerr << s->name() << std::endl;
      s->start(-2);
      while (!s->eof()) {
        s->process_one();
      }
    }
  }


  auto topic_sink = builder.create_kafka_sink<int, std::string>("kspp_example5_usernames.per-channel");
  for (int i=0; i!=8; ++i)
  {
    auto partition_source = builder.create_kafka_source<int, std::string>("kspp_example5_usernames", i);
    auto partition_routing_table = builder.create_ktable<int, int>("example5", "kspp_example5_user_channel", i);
    auto partition_repartition = std::make_shared<csi::repartition_by_table<int, std::string, csi::text_codec>>(partition_source, partition_routing_table, topic_sink);
    partition_repartition->start(-2);
    while (!partition_repartition->eof()) {
      partition_repartition->process_one();
    }
  }


  {
    auto sources = builder.create_kafka_sources<int, std::string>("kspp_example5_usernames.per-channel", 8);
    auto sink = builder.create_stream_sinks<int, std::string>(sources, std::cerr);
    for (auto s : sources) {
      std::cerr << s->name() << std::endl;
      s->start(-2);
      while (!s->eof()) {
        s->process_one();
      }
    }
  }
}
