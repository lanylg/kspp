#include <assert.h>
#include <memory>
#include <functional>
#include <sstream>
#include <kspp/kspp.h>
#include <kspp/impl/serdes/text_serdes.h>
#pragma once

namespace kspp {

  template<class K, class V>
  class stream_sink : public partition_sink<K, V>
  {
  public:
    enum { MAX_KEY_SIZE = 1000 };

    stream_sink(topology_base& topology, std::shared_ptr<partition_source<K, V>> source, std::ostream* os)
      : partition_sink<K, V>(source->partition())
      , _os(*os)
      , _codec(std::make_shared<kspp::text_serdes>()) {
      source->add_sink(this);
    }

    virtual ~stream_sink() {
      //_source->remove_sink()
    }

    virtual std::string name() const {
      return "stream_sink";
    }

    virtual std::string processor_name() const {
      return "stream_sink";
    }

    virtual bool process_one(int64_t tick) {
      return false;
      //return _upstream ? _upstream->process_one(tick) : false;
    }

    virtual size_t queue_len() {
      return 0;
    }

    virtual void commit(bool flush) {
      // noop
    }

  protected:
    virtual int _produce(std::shared_ptr<ktransaction<K, V>> transaction) {
      _os << "ts: " << transaction->event_time() << "  ";
      _codec->encode(transaction->record()->key, _os);
      _os << ":";
      if (transaction->record()->value)
        _codec->encode(*transaction->record()->value, _os);
      else
        _os << "<nullptr>";
      _os << std::endl;
      return 0;
    }

  private:
    std::ostream&                      _os;
    std::shared_ptr<kspp::text_serdes> _codec;
  };

  //<null, VALUE>
  template<class V>
  class stream_sink<void, V> : public partition_sink<void, V>
  {
  public:
    stream_sink(topology_base& topology, std::shared_ptr<partition_source<void, V>> source, std::ostream* os)
      : partition_sink<void, V>(source.partition())
      , _os(*os)
      , _codec(std::make_shared<kspp::text_serdes>()) {
      source->add_sink(this);
    }

    virtual ~stream_sink() {
      //_source->remove_sink()
    }

    virtual std::string name() const {
      return "stream_sink";
    }

    virtual std::string processor_name() const {
      return "stream_sink";
    }

    virtual bool process_one(int64_t tick) {
      return false;
      //return _upstream ? _upstream->process_one(tick) : false;
    }


    virtual size_t queue_len() {
      return 0;
    }

    virtual void commit(bool flush) {
      // noop
    }

  protected:
    virtual int _produce(std::shared_ptr<ktransaction<void, V>> transaction) {
      _os << "ts: " << transaction->event_time() << "  ";
      _codec->encode(*transaction->record()->value, _os);
      _os << std::endl;
      return 0;
    }

  private:
    std::ostream&                      _os;
    std::shared_ptr<kspp::text_serdes> _codec;
  };

  // <key, nullptr>
  template<class K>
  class stream_sink<K, void> : public partition_sink<K, void>
  {
  public:
    stream_sink(topology_base& topology, std::shared_ptr<partition_source<K, void>> source, std::ostream* os)
      : partition_sink<K, void>(source->partition())
      , _os(*os)
      , _codec(std::make_shared<kspp::text_serdes>()) {
      source->add_sink(this);
    }

    virtual ~stream_sink() {
      //_source->remove_sink()
    }

    virtual std::string name() const {
      return "stream_sink";
    }

    virtual std::string processor_name() const {
      return "stream_sink";
    }

    virtual bool process_one(int64_t tick) {
      return false;
      //return _upstream ? _upstream->process_one(tick) : false;
    }

    virtual size_t queue_len() {
      return 0;
    }

    virtual void commit(bool flush) {
      // noop
    }

  protected:
    virtual int _produce(std::shared_ptr<ktransaction<K, void>> transaction) {
      _os << "ts: " << transaction->event_time() << "  ";
      _codec->encode(transaction->record()->key, _os);
      _os << std::endl;
      return 0;
    }

  private:
    std::ostream&                      _os;
    std::shared_ptr<kspp::text_serdes> _codec;
  };

};